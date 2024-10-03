#pragma once

#include "flags.h"

namespace CliUtils {
    void printUsage();
    ARG_PROCESSING_STATE processArgs(int argc, const char* argv[]);
}