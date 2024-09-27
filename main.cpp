#include <thread>
#include <chrono>
#include <string>
#include <iostream>

#include <unistd.h>

//signatures MUST come first
#include "serverinfo_signature.h"
#include <dbus-cxx.h>

#include "jsonBuilder.h"
#include "defaults.h"

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
    JsonBuilder outputStr;
    
    outputStr.pushPair("app_name", app_name);
    outputStr.pushPair("app_icon", app_icon);
    outputStr.pushPair("summary", summary);
    outputStr.pushPair("body", body);
    
    std::cout << outputStr.finalize() << std::endl;
    return nextNotiId++;
}

void printUsage() {
    std::cout << "Usage: ssnd [--format json|raw] [--bus system|session] COMMAND" << std::endl;
}

//TODO: factor out err printing from processArgs

int processArgs(int argc, const char* argv[]) {
    bool foundCmd = false;

    for(int i = 1; i < argc; i++) {
        string arg = string(argv[i]);

        if(arg.starts_with("--")){
            string flag = arg.substr(2);

            if(flag == "help"){
                printUsage();
                exit(0);
            }

            if(cmd_defaults.count(flag) > 0){
                cmd_defaults[flag] = argv[++i];
            }
            else {
                std::cout << "ERR: Invalid flag!" << std::endl;
                printUsage();
                return -1;
            }
        }
        else {
            if(foundCmd) {
                std::cout << "ERR: Too many arguments!" << std::endl;
                printUsage();
                return -1;
            }

            cmd_defaults["handler"] = arg;
            foundCmd = true;
        }
    }

    return 0;
}

int main(int argc, const char* argv[])
{
    if(processArgs(argc, argv)){
        return 1;
    }

    std::shared_ptr<DBus::Dispatcher> dbusDispatcher = DBus::StandaloneDispatcher::create();
    std::shared_ptr<DBus::Connection> dbusConn = dbusDispatcher->create_connection(DBus::BusType::SESSION);
 
    if(dbusConn->request_name("org.freedesktop.Notifications", DBUSCXX_NAME_FLAG_REPLACE_EXISTING) != DBus::RequestNameResponse::PrimaryOwner)
        std::cout << "Warn: org.freedesktop.Notifications is already registered! (Maybe a notification daemon is already running?)" << std::endl;
    
    std::shared_ptr<DBus::Object> dbusObj = dbusConn->create_object("/org/freedesktop/Notifications", DBus::ThreadForCalling::DispatcherThread);

    dbusObj->create_method<NotiServerInfo()>("org.freedesktop.Notifications", "GetServerInformation", sigc::ptr_fun(GetServerInformationCallback));
    dbusObj->create_method<uint32_t(string, uint32_t, string, string, string, std::vector<string>, std::vector<std::tuple<string, DBus::Variant>>, int32_t)>("org.freedesktop.Notifications", "Notify", sigc::ptr_fun(NotifyCallback));

    pause();
    return 0;
}