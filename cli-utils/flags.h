#pragma once

#include <vector>
#include <string>
#include <unordered_map>

namespace CliUtils {
    enum ARG_PROCESSING_STATE {
        SUCCESS,
        TOOFEWARGS,
        TOOMANYARGS,
        INVALIDFLAG,
        INVALIDVALUE
    };

    namespace FlagProcessor { 
        ARG_PROCESSING_STATE bugFlagProcessor(std::string passedValue);
        ARG_PROCESSING_STATE formatFlagProcessor(std::string passedValue);
    }

    struct FlagOptions {
        std::vector<std::string> allowedValues;
        ARG_PROCESSING_STATE (*flagProcessor)(std::string value);
    };

    inline static std::unordered_map<std::string, FlagOptions> flags = {
        {"format", FlagOptions{.allowedValues={"raw", "json"}, .flagProcessor=FlagProcessor::formatFlagProcessor}},
        {"bus", FlagOptions{.allowedValues={"session", "system"}, .flagProcessor=FlagProcessor::bugFlagProcessor}}
    };
}