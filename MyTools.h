#pragma once

#include <vector>
#include <string>

class StringTools{
        public:
        static std::vector<std::string> Split(std::string, char);
        static std::string FileToString(std::string);
};