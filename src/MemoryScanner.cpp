#include "MemoryScanner.hpp"
#include <unistd.h>
#include <sys/mman.h>

bool MemoryScanner::isReadable(uintptr_t address) {
    if (address == 0) return false;
    // Check if memory is accessible using mincore or msync
    unsigned char vec;
    return mincore((void*)address, 1, &vec) != -1;
}

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
