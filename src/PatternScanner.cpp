#include "PatternScanner.hpp"
#include "MemoryUtils.hpp"

uintptr_t PatternScanner::scanForCodeRegistration(uintptr_t base, size_t size) {
    // IL2CPP Version-specific patterns (common for obfuscated titles)
    // Pattern 1: x86/arm64 LEA or ADRP signatures for CodeRegistration
    std::vector<uint8_t> pattern = { 0x48, 0x8d, 0x05 };
    std::string mask = "xxx";
    
    return MemoryUtils::findPattern(base, base + size, pattern, mask);
}
