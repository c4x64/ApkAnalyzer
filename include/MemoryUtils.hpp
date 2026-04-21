#pragma once
#include <string>
#include <cstdint>
#include <vector>

struct MemoryRange {
    uintptr_t start;
    uintptr_t end;
    std::string pathname;
};

class MemoryUtils {
public:
    static uintptr_t getBaseAddress(const std::string& libName);
    static std::vector<MemoryRange> getProcessMaps();
    static uintptr_t findPattern(uintptr_t start, uintptr_t end, const char* pattern, const char* mask);
};
