#include "serverinfo_signature.h"

DBus::MessageIterator& operator>> (DBus::MessageIterator& iter, struct NotiServerInfo& info)
{
    info.name = iter.get_string();
    iter.next();

    info.vendor = iter.get_string();
    iter.next();

    info.version = iter.get_string();
    iter.next();

    info.spec_version = iter.get_string();

    return iter;
}

DBus::MessageAppendIterator& operator<< (DBus::MessageAppendIterator& appendIter, const struct NotiServerInfo& info)
{
    appendIter << (string)info.name;
    appendIter << (string)info.vendor;
    appendIter << (string)info.version;
    appendIter << (string)info.spec_version;

    return appendIter;
}
