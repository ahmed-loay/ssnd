#include "cliutils.h"

#include <iostream>
#include <string>

#include "cliflags.h"
#include "../defaults.h"

using std::string;

namespace CliUtils {
    void printUsage() {
        std::cout << "Usage: ssnd [--format json|raw] [--bus system|session] COMMAND" << std::endl;
    }

    bool flagValueValid(string flag, string value){
        //if flag doesn't exist in flag_options then, any value is valid
        if(flag_options.count(flag) == 0){
            return true;
        }

        for(string validValue: flag_options[flag]){
            return value == validValue;
        }
    }

    //TODO: factor out err printing from processArgs
    int processArgs(int argc, const char* argv[]) {
        bool foundCmd = false;

        for(int i = 1; i < argc; i++) {
            string arg = string(argv[i]);

            if(arg.starts_with("--")){
                string flag = arg.substr(2);

                if(flag == "help"){
                    printUsage();
                    exit(0);
                }

                if(cmd_defaults.count(flag) > 0){
                    //check if arguments are enough before skipping forward
                    if(i + 1 > argc - 1){
                        std::cout << "ERR: Too few arguments!" << std::endl;
                        printUsage();
                        return -1;
                    }

                    if(flagValueValid(flag, argv[i + 1])){
                        //inc i, taking the flag's value and skipping iterating over it
                        cmd_defaults[flag] = argv[++i];
                    }
                    else {
                        std::cout << "ERR: Invalid value for --" << flag << " flag!" << std::endl;
                        printUsage();
                        return -1;
                    }
                }
                else {
                    std::cout << "ERR: Invalid flag!" << std::endl;
                    printUsage();
                    return -1;
                }
            }
            else {
                if(foundCmd) {
                    std::cout << "ERR: Too many arguments!" << std::endl;
                    printUsage();
                    return -1;
                }

                cmd_defaults["handler"] = arg;
                foundCmd = true;
            }
        }

        return 0;
    }
}