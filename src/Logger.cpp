#include "Logger.hpp"
#include <fstream>
#include <ctime>

void Logger::log(Level level, const std::string& message) {
    std::string prefix;
    switch (level) {
        case INFO:    prefix = "[*] "; break;
        case SUCCESS: prefix = "[+] "; break;
        case WARNING: prefix = "[!] "; break;
        case ERROR:   prefix = "[-] ERROR: "; break;
    }
    
    // Console output
    if (level == ERROR) std::cerr << prefix << message << std::endl;
    else std::cout << prefix << message << std::endl;

    // File-based logging for Android/Linux analysis persistence
    std::ofstream logFile("analyzer.log", std::ios::app);
    if (logFile.is_open()) {
        std::time_t now = std::time(nullptr);
        logFile << std::ctime(&now) << " " << prefix << message << std::endl;
    }
}
