#pragma once
#include <string>
#include <iostream>

class Logger {
public:
    enum Level { INFO, SUCCESS, WARNING, ERROR };
    static void log(Level level, const std::string& message);
};
