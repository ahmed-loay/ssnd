#pragma once

#include <string>
using std::string;

struct NotiServerInfo {
    string name;
    string vendor;
    string version;
    string spec_version;
};

namespace DBus {
    inline string signature(NotiServerInfo info) {return "ss";}
}

#include <dbus-cxx.h>

DBus::MessageIterator& operator>> (DBus::MessageIterator& iter, NotiServerInfo& info);
DBus::MessageAppendIterator& operator<< (DBus::MessageAppendIterator& appendIter, const NotiServerInfo& info);