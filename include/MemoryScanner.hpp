#pragma once
#include <vector>
#include <cstdint>
#include <string>

class MemoryScanner {
public:
    static uintptr_t findSectionBase(const std::string& sectionName);
    static bool isReadable(uintptr_t address);
};
