#pragma once

#include <string>
using std::string;

class JsonBuilder {

    public:
        void pushPair(string key, string value);
        void pushPair(string key, int value);

        string finalize();

    private:
        string inProgressStr;

};