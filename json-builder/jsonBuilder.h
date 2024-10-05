#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>

#include <dbus-cxx.h>

namespace JsonUtils {
    enum DBusTypes {
        string,
        boolean,
        byte,
        int64
    };

    struct NativeDBusNotification {
        std::string app_name;
        std::string app_icon;
        std::string summary;
        std::string body;
        uint32_t replaces_id;
        std::vector<std::string> actions;
        std::vector<std::tuple<std::string, DBus::Variant>> hints;
        int32_t timeout;
    };

    enum DBusParsedHints {
        CATEGORY,
        IMAGE_PATH,
        DESKTOP_ENTRY,
        URGENCY,
        SENDER_PID
    };

    inline static std::unordered_map<std::string, DBusParsedHints> parsedHintsTable = {
        {"category", DBusParsedHints::CATEGORY},
        {"image-path", DBusParsedHints::IMAGE_PATH},
        {"image_path", DBusParsedHints::IMAGE_PATH}, //deprecated, for spec version 1.1 only
        {"desktop-entry", DBusParsedHints::DESKTOP_ENTRY},
        {"urgency", DBusParsedHints::URGENCY},
        {"sender-pid", DBusParsedHints::SENDER_PID}
    };

    inline static std::unordered_map<std::string, DBusTypes> hintValueTypes = {
        {"category", DBusTypes::string},
        {"image-path", DBusTypes::string},
        {"image_path", DBusTypes::string}, //deprecated, for spec version 1.1 only
        {"desktop-entry", DBusTypes::string},
        {"urgency", DBusTypes::byte},
        {"sender-pid", DBusTypes::int64}
    };

    class DBusNotification {
        std::string app_name;
        std::string app_icon;
        std::string summary;
        std::string body;
        std::string category;
        std::string image_path;
        std::string desktop_entry;
        uint8_t urgency;
        int sender_pid;

        public:
        DBusNotification(NativeDBusNotification native);
        std::string toJson();
        friend std::ostream & operator << (std::ostream &out, const DBusNotification &c);

        private:
            DBusNotification();
    };
}
class JsonBuilder {
    public:
        void pushPair(std::string key, std::string value);
        void pushPair(std::string key, int value);

        std::string finalize();

    private:
        std::string inProgressStr;
};