#include <string>

#include "flags.h"
#include "../defaults.h"

using namespace CliUtils;

//flags with a set of specified values don't have to check for empty values

ARG_PROCESSING_STATE FlagProcessor::busFlagProcessor(std::string passedValue)
{
    /*if(passedValue.empty()) 
        return ARG_PROCESSING_STATE::TOOFEWARGS;*/

    cmd_defaults["bus"] = passedValue;

    return ARG_PROCESSING_STATE::SUCCESS;
}

ARG_PROCESSING_STATE FlagProcessor::formatFlagProcessor(std::string passedValue)
{
    /*if(passedValue.empty()) 
        return ARG_PROCESSING_STATE::TOOFEWARGS;*/

    cmd_defaults["format"] = passedValue;

    return ARG_PROCESSING_STATE::SUCCESS;
}
