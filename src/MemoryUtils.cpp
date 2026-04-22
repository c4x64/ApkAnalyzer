#include "MemoryUtils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

#if defined(__linux__) || defined(__ANDROID__)
#include <unistd.h>
#endif

std::vector<MemoryRange> MemoryUtils::getProcessMaps() {
    std::vector<MemoryRange> ranges;
#if defined(__linux__) || defined(__ANDROID__)
    std::ifstream maps("/proc/self/maps");
    std::string line;
    while (std::getline(maps, line)) {
        MemoryRange range;
        char r, w, x, p;
        unsigned long start, end;
        char path[256] = {0};
        if (sscanf(line.c_str(), "%lx-%lx %c%c%c%c %*s %*s %*s %s", &start, &end, &r, &w, &x, &p, path) >= 6) {
            range.start = start;
            range.end = end;
            range.pathname = path;
            ranges.push_back(range);
        }
    }
#endif
    return ranges;
}

uintptr_t MemoryUtils::getBaseAddress(const std::string& libName) {
    auto ranges = getProcessMaps();
    for (const auto& range : ranges) {
        if (range.pathname.find(libName) != std::string::npos) {
            return range.start;
        }
    }
    return 0;
}

uintptr_t MemoryUtils::findPattern(uintptr_t start, uintptr_t end, const std::vector<uint8_t>& pattern, const std::string& mask) {
    size_t patternLen = mask.length();
    for (uintptr_t i = start; i < end - patternLen; i++) {
        bool found = true;
        for (size_t j = 0; j < patternLen; j++) {
            if (mask[j] != '?' && pattern[j] != *(uint8_t*)(i + j)) {
                found = false;
                break;
            }
        }
        if (found) return i;
    }
    return 0;
}
