#pragma once
#include <vector>
#include <cstdint>

class PatternScanner {
public:
    static uintptr_t scanForCodeRegistration(uintptr_t base, size_t size);
};
