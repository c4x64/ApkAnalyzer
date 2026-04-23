#include "Logger.hpp"
#include <fstream>
#include <ctime>

std::string& Logger::getLogPath() {
    static std::string logPath = "analyzer.log";
    return logPath;
}

void Logger::setLogFile(const std::string& path) {
    getLogPath() = path;
}

void Logger::log(Level level, const std::string& message) {
    std::string prefix;
    switch (level) {
        case INFO:    prefix = "[INFO] "; break;
        case SUCCESS: prefix = "[OK]   "; break;
        case WARNING: prefix = "[WARN] "; break;
        case ERROR:   prefix = "[ERR]  "; break;
    }
    
    std::string output = prefix + message;

    // Console output
    if (level == ERROR) std::cerr << output << std::endl;
    else std::cout << output << std::endl;

    // File-based logging
    std::ofstream logFile(getLogPath(), std::ios::app);
    if (logFile.is_open()) {
        std::time_t now = std::time(nullptr);
        char timestamp[32];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        logFile << "[" << timestamp << "] " << output << std::endl;
    }
}
