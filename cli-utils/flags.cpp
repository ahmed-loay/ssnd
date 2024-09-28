#include <string>

#include "flags.h"
#include "../defaults.h"

using namespace CliUtils;

ARG_PROCESSING_STATE FlagProcessor::bugFlagProcessor(std::string passedValue)
{
    if(passedValue.empty()) 
        return ARG_PROCESSING_STATE::TOOFEWARGS;

    cmd_defaults["bus"] = passedValue;

    return ARG_PROCESSING_STATE::SUCCESS;
}

ARG_PROCESSING_STATE FlagProcessor::formatFlagProcessor(std::string passedValue)
{
    if(passedValue.empty()) 
        return ARG_PROCESSING_STATE::TOOFEWARGS;

    cmd_defaults["format"] = passedValue;

    return ARG_PROCESSING_STATE::SUCCESS;
}
