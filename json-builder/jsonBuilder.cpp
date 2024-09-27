#include "jsonBuilder.h"

#include <format>
#include <string>

void JsonBuilder::pushPair(string key, string value)
{
    inProgressStr += std::format("\"{}\": \"{}\",", key, value);
}

void JsonBuilder::pushPair(string key, int value)
{
    inProgressStr += std::format("\"{}\": {},", key, value);
}

string JsonBuilder::finalize()
{
    //removes the last appended comma
    inProgressStr.pop_back();

    return string("{" + inProgressStr + "}");
}
