#pragma once

#include <unordered_map>
#include <string>
using std::string;

inline std::unordered_map<string, string> cmd_defaults = {
    {"format", "raw"},
    {"bus", "session"},
    {"handler", "echo"}
};