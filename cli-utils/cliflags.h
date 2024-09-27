#pragma once

#include <string>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, std::vector<std::string>> flag_options = {
    {"format", {"raw", "json"}},
    {"bus", {"session", "system"}}
};