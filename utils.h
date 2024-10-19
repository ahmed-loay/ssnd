#pragma once

#include <vector>
#include <string>

std::vector<std::string> split(const std::string &s, const std::string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

std::vector<std::string> split(std::vector<std::string> input, const std::string &delimiter){
    std::vector<std::string> output;
    
    for(std::string str: input){
        std::vector<std::string> splitStr = split(str, delimiter);
        if(splitStr.empty()) continue;

        output.insert(output.end(), splitStr.begin(), splitStr.end());
    }

    return output;
}

std::unique_ptr<char**> vecStrToCStrArray(std::vector<std::string> &vec){
    auto cstrarr = std::make_unique<char**>(new char*[vec.size() + 1]);

    for(int i = 0; i < vec.size(); i++){
        //char* cstr = new char[vec[i].size()];
        //vec[i].copy(cstr, vec[i].size());
        (*cstrarr.get())[i] = vec[i].data();
    }

    (*cstrarr.get())[vec.size()] = NULL;

    return cstrarr;
}

std::vector<std::string> rawOutputToEnvs(const std::string &raw) {
    std::vector<std::string> envs = split(raw, "\n");

    std::vector<std::string> formattedEnvs;
    for(auto env: envs){
        auto pos = env.find(' ', 0);
        if(pos != std::string::npos){
            env[pos] = '=';
            formattedEnvs.push_back(env);
        }
    }

    return formattedEnvs;
}