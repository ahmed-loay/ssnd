#include "cliutils.h"

#include <iostream>
#include <string>

#include "../defaults.h"
#include "flags.h"

using std::string;

namespace CliUtils {
    void printUsage() {
        std::cout << "Usage: ssnd [--format json|raw] [--bus system|session] COMMAND" << std::endl;
    }

    bool flagValueValid(string flag, string value){
        //if flag doesn't exist
        if(flags.count(flag) == 0){
            std::cout << "Flag '" << flag << "' doesn't exist!\n";
            return false;
        }

        for(string validValue: flags[flag].allowedValues){
            if(value == validValue) return true;
        }

        std::cout << "No such value '" << value << "' for flag '" << flag << "'!";
        return false;
    }

    //processing state machine
    ARG_PROCESSING_STATE processArgs(int argc, const char* argv[]){
        string lastFlag = "";
        bool wasPrevFlag = false;
        bool commandSet = false;
        
        for(int i = 1; i < argc; i++){
            string arg = string(argv[i]);
            if(arg.starts_with("--")){
                //last flag didnt have a value
                if(wasPrevFlag){
                    //printf("flag %s didnt have a value asscoistaed with it.\n", lastFlag.c_str());
                    if(flagValueValid(lastFlag, arg)){
                        ARG_PROCESSING_STATE result = flags[lastFlag].flagProcessor(arg);
                        if(result != ARG_PROCESSING_STATE::SUCCESS) return result;
                    }
                    else {
                        return ARG_PROCESSING_STATE::INVALIDVALUE;
                    }
                }

                lastFlag = arg.substr(2);
                wasPrevFlag = true;

                if(lastFlag == "help"){
                    printUsage();
                    exit(0);
                }
            }
            else {
                if(wasPrevFlag){
                    wasPrevFlag = false;

                    //printf("flag %s with val %s processed!\n", lastFlag.c_str(), arg.c_str());

                    if(flagValueValid(lastFlag, arg)){
                        ARG_PROCESSING_STATE result = flags[lastFlag].flagProcessor(arg);
                        if(result != ARG_PROCESSING_STATE::SUCCESS) return result;
                    }
                    else {
                        return ARG_PROCESSING_STATE::INVALIDVALUE;
                    }
                }
                else {
                    if(!commandSet){
                        commandSet = true;
                        cmd_defaults["handler"] = arg;
                        //std::cout << "cmd set" << std::endl;
                    }
                    else {
                        std::cout << "Too many args!" << std::endl;
                        return ARG_PROCESSING_STATE::TOOMANYARGS;
                    }
                }
            }
        }

        return ARG_PROCESSING_STATE::SUCCESS;
    }

    //TODO: factor out err printing from processArgs
    /*
    int processArgs(int argc, const char* argv[]) {
        bool foundCmd = false;
        bool 

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
                        //cmd_defaults[flag] = argv[++i];
                        flags[flag].flagProcessor(argv[i + 1]);
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
    */
}