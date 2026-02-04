#pragma once
#include <string>

struct Log {
    static void info(const std::string& msg);
    static void error(const std::string& msg);
};
