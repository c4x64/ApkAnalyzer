#pragma once
#include <string>
#include <iostream>

#define LOG_TAG "ApkAnalyzer"

class Logger {
public:
    enum Level { INFO, SUCCESS, WARNING, ERROR };
    static void log(Level level, const std::string& message);
    static void setLogFile(const std::string& path);
private:
    static std::string& getLogPath();
};
