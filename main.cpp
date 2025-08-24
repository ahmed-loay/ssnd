#include <thread>
#include <chrono>
#include <string>
#include <cstring>
#include <iostream>
#include <format>
#include <algorithm>

#include <sys/types.h>
#include <unistd.h>

//signatures MUST come first
#include "serverinfo_signature.h"
#include <dbus-cxx.h>

#include "defaults.h"
#include "jsonBuilder.h"
#include "cliutils.h"

#include "utils.h"

using std::string;

int nextNotiId = 0;

struct NotiInfo {
    string app_name;
    uint32_t replaces_id;
    string app_icon;
    string summary;
    string body;
    DBus::MessageIterator actions;
    DBus::MessageIterator hints;
    uint32_t expire_timeout;
};

// -- https://specifications.freedesktop.org/notification-spec/latest/protocol.html#command-get-capabilities
//as org.freedesktop.Notifications.GetCapabilities (void);	 
NotiServerInfo GetServerInformationCallback() { 
    return NotiServerInfo {
        .name = "noti-server",
        .vendor = "ahmed",
        .version = "1.0", 
        .spec_version = "1.2"
    };
}

/*
-- https://specifications.freedesktop.org/notification-spec/latest/protocol.html#command-notify
u org.freedesktop.Notifications.Notify (
    in s app_name,
 	in u replaces_id,
 	in s app_icon,
 	in s summary,
 	in s body,
 	in as actions,
 	in a{sv} hints,
 	in i expire_timeout
);
*/

uint32_t NotifyCallback(string app_name, uint32_t replaces_id, string app_icon, string summary, string body, std::vector<string> actions, std::vector<std::tuple<string, DBus::Variant>> hints, int32_t timeout){
    JsonUtils::DBusNotification notification({
        .app_name = app_name,
        .app_icon = app_icon,
        .summary = summary,
        .body = body,
        .replaces_id = replaces_id,
        .actions = actions,
        .hints = hints,
        .timeout = timeout,
    });

    pid_t forkResult = fork();
    if(!cmd_defaults.empty()){
        if(forkResult < 0){
            std::cout << "Failed to fork process! Exiting!\n";
            exit(1);
        }
        else if(forkResult == 0){
            if(cmd_defaults["format"] == "json"){
                std::string notiJson = notification.toJson();
                std::cout << notiJson << std::endl;

                std::string jsonEnv = "noti_json=" + notiJson;

                static char* newargv[] = {cmd_defaults["handler"].data(), NULL};
                static char* newenvp[] = { jsonEnv.data(), NULL};

                execvpe(cmd_defaults["handler"].data(), newargv, newenvp);
                
                perror("execvpe");   /* execvpe() returns only on error */
                exit(EXIT_FAILURE);
            }
            else if(cmd_defaults["format"] == "raw"){
                std::stringstream ss;
                ss << notification;

                std::string rawOutput = ss.str();
                std::cout << rawOutput << std::endl;

                static char* newargv[] = {cmd_defaults["handler"].data(), NULL};
                auto newenvp = rawOutputToEnvs(rawOutput);
                
                auto test = vecStrToCStrArray(newenvp);

                execvpe(cmd_defaults["handler"].data(), newargv, (*test.get()));
                
                perror("execvpe");   /* execvpe() returns only on error */
                exit(EXIT_FAILURE);
            }
        }
    }
    else {
        if(cmd_defaults["format"] == "json"){
            std::cout << notification.toJson();
        }
        else if(cmd_defaults["format"] == "raw"){
            std::cout << notification;
        }
    }

    return nextNotiId++;
}

int main(int argc, const char* argv[])
{
    CliUtils::processArgs(argc, argv);

    std::shared_ptr<DBus::Dispatcher> dbusDispatcher = DBus::StandaloneDispatcher::create();

    std::shared_ptr<DBus::Connection> dbusConn;
    if(cmd_defaults["bus"] == "session"){
        dbusConn = dbusDispatcher->create_connection(DBus::BusType::SESSION);
    }
    else if(cmd_defaults["bus"] == "system"){
        dbusConn = dbusDispatcher->create_connection(DBus::BusType::SYSTEM);
    }
 
    if(dbusConn->request_name("org.freedesktop.Notifications", DBUSCXX_NAME_FLAG_REPLACE_EXISTING) != DBus::RequestNameResponse::PrimaryOwner)
        std::cout << "Warn: org.freedesktop.Notifications is already registered! (Maybe a notification daemon is already running?)" << std::endl;
    
    std::shared_ptr<DBus::Object> dbusObj = dbusConn->create_object("/org/freedesktop/Notifications", DBus::ThreadForCalling::DispatcherThread);

    dbusObj->create_method<NotiServerInfo()>("org.freedesktop.Notifications", "GetServerInformation", sigc::ptr_fun(GetServerInformationCallback));
    dbusObj->create_method<uint32_t(string, uint32_t, string, string, string, std::vector<string>, std::vector<std::tuple<string, DBus::Variant>>, int32_t)>("org.freedesktop.Notifications", "Notify", sigc::ptr_fun(NotifyCallback));

    pause();
    return 0;
}