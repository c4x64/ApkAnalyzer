#include "MemoryScanner.hpp"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#endif

bool MemoryScanner::isReadable(uintptr_t address) {
    if (address == 0) return false;
#ifdef _WIN32
    MEMORY_BASIC_INFORMATION mbi;
    return VirtualQuery((LPCVOID)address, &mbi, sizeof(mbi)) && (mbi.Protect & PAGE_READONLY || mbi.Protect & PAGE_READWRITE || mbi.Protect & PAGE_EXECUTE_READ);
#else
    unsigned char vec;
    return mincore((void*)address, 1, &vec) != -1;
#endif
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
