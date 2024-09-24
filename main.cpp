#include <thread>
#include <chrono>
#include <string>
#include <iostream>

#include <unistd.h>

//signatures MUST come first
#include "serverinfo_signature.h"
#include <dbus-cxx.h>

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
NotiServerInfo GetServerInformation() { 
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
uint32_t Notify(string app_name, uint32_t replaces_id, string app_icon, string summary, string body, std::vector<string> actions, std::vector<std::tuple<string, DBus::Variant>> hints, int32_t timeout){
    std::cout << summary << std::endl;
    return nextNotiId++;
}

int main()
{
    std::shared_ptr<DBus::Dispatcher> dispatcher = DBus::StandaloneDispatcher::create();
    std::shared_ptr<DBus::Connection> conn = dispatcher->create_connection(DBus::BusType::SESSION);
 
    if(conn->request_name("org.freedesktop.Notifications", DBUSCXX_NAME_FLAG_REPLACE_EXISTING) != DBus::RequestNameResponse::PrimaryOwner)
        std::cout << "Warn: org.freedesktop.Notifications is already registered! (Maybe a notification daemon is already running?)" << std::endl;
    
    std::shared_ptr<DBus::Object> object = conn->create_object("/org/freedesktop/Notifications", DBus::ThreadForCalling::DispatcherThread);

    object->create_method<NotiServerInfo()>("org.freedesktop.Notifications", "GetServerInformation", sigc::ptr_fun(GetServerInformation));
    object->create_method<uint32_t(string, uint32_t, string, string, string, std::vector<string>, std::vector<std::tuple<string, DBus::Variant>>, int32_t)>("org.freedesktop.Notifications", "Notify", sigc::ptr_fun(Notify));

    pause();
    return 0;
}