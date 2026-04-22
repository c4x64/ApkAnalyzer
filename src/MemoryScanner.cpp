#include "MemoryScanner.hpp"
#include <fstream>
#include <sstream>

uintptr_t MemoryScanner::findSectionBase(const std::string& sectionName) {
    std::ifstream maps("/proc/self/maps");
    std::string line;
    while (std::getline(maps, line)) {
        if (line.find(sectionName) != std::string::npos) {
            std::stringstream ss(line);
            std::string addrRange;
            ss >> addrRange;
            return std::stoull(addrRange.substr(0, addrRange.find('-')), nullptr, 16);
        }
    }
    return 0;
}
