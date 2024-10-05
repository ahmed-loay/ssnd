#include "jsonBuilder.h"

#include <dbus-cxx.h>
#include <iostream>
#include <format>
#include <string>

void JsonBuilder::pushPair(std::string key, std::string value)
{
    inProgressStr += std::format("\"{}\": \"{}\",", key, value);
}

void JsonBuilder::pushPair(std::string key, int value)
{
    inProgressStr += std::format("\"{}\": {},", key, value);
}

std::string JsonBuilder::finalize()
{
    //removes the last appended comma
    inProgressStr.pop_back();

    return std::string("{" + inProgressStr + "}");
}

JsonUtils::DBusNotification::DBusNotification(NativeDBusNotification native){
    app_name = native.app_name;
    app_icon = native.app_icon;
    summary = native.summary;
    body = native.body;
    
    for(auto hint: native.hints){
        std::string hintKey = std::get<string>(hint);
        DBus::Variant hintValueVariant = std::get<DBus::Variant>(hint);

        switch(parsedHintsTable[hintKey]){
            case DBusParsedHints::CATEGORY:
                app_name = hintValueVariant.to_string();
                break;

            case DBusParsedHints::IMAGE_PATH:
                image_path = hintValueVariant.to_string();
                break;

            case DBusParsedHints::DESKTOP_ENTRY:
                desktop_entry = hintValueVariant.to_string();
                break;
                
            case DBusParsedHints::URGENCY:
                urgency = hintValueVariant.to_uint8();
                break;

            case DBusParsedHints::SENDER_PID:
                urgency = hintValueVariant.to_int64();
                break;

            default:
                std::cout << "Encountered unfamiliar hint key!\n";
        }
    }
}

std::string JsonUtils::DBusNotification::toJson()
{
    JsonBuilder generatedJson;

    generatedJson.pushPair("app-name", app_name);
    generatedJson.pushPair("app-icon", app_icon);
    generatedJson.pushPair("summary", summary);
    generatedJson.pushPair("body", body);
    generatedJson.pushPair("category", category);
    generatedJson.pushPair("image-path", image_path);
    generatedJson.pushPair("desktop-entry", desktop_entry);
    generatedJson.pushPair("urgency", urgency);
    generatedJson.pushPair("sender-pid", sender_pid);

    return generatedJson.finalize();
}

std::ostream &JsonUtils::operator<<(std::ostream &out, const DBusNotification &noti)
{
     return out << "app_name \"" << noti.app_name << "\"\n"
                << "app_icon \"" << noti.app_icon << "\"\n"
                << "summary \"" << noti.summary << "\"\n"
                << "body \"" << noti.body << "\"\n"
                << "category \"" << noti.category << "\"\n"
                << "image_path \"" << noti.image_path << "\"\n"
                << "desktop_entry \"" << noti.desktop_entry << "\"\n"
                << "urgency \"" << std::to_string(noti.urgency) << "\"\n"
                << "sender_pid \"" << std::to_string(noti.sender_pid) << "\"\n";
}
