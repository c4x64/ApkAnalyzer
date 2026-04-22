#include "Logger.hpp"

void Logger::log(Level level, const std::string& message) {
    switch (level) {
        case INFO:    std::cout << "[*] " << message << std::endl; break;
        case SUCCESS: std::cout << "[+] " << message << std::endl; break;
        case WARNING: std::cout << "[!] " << message << std::endl; break;
        case ERROR:   std::cerr << "[-] ERROR: " << message << std::endl; break;
    }
}
