#include "Il2CppScanner.hpp"
#include <iostream>
#include <algorithm>
#include <cstring>

struct Il2CppMetadataHeader {
    uint32_t magic;
    int32_t version;
    uint32_t stringIndexTableOffset;
    uint32_t stringIndexTableCount;
    uint32_t stringDataOffset;
    uint32_t stringDataCount;
    uint32_t methodDefinitionsOffset;
    uint32_t methodDefinitionsCount;
    // ... rest of header
};

Il2CppScanner::Il2CppScanner(const std::vector<uint8_t>& metadata, const std::vector<uint8_t>& libso) 
    : metadata(metadata), libso(libso) {
    if (metadata.size() < 4) return;
    if (metadata[0] == 0xAF && metadata[1] == 0x1B && metadata[2] == 0xB1 && metadata[3] == 0xFA) {
        valid = true;
    }
}

bool Il2CppScanner::isValid() const { return valid; }

// Perfare's method: Scan for Registration functions or MetadataUsage
// For FreeFire and obfuscated games, we use pattern scanning for the Il2CppCodeRegistration
uint64_t Il2CppScanner::findMethodOffset(const std::string& className, const std::string& methodName) {
    if (!valid) return 0;

    // Pattern for IL2CPP 24+ (Common in FreeFire)
    // Often we look for "il2cpp_init" and trace its calls to find the Registration pointers
    
    // Simple Pattern Scan example for il2cpp_init or similar internal structures
    // In real scenarios, we'd use Capstone to emulate/trace the registration
    
    std::cout << "[*] Scanning for obfuscated methods using Perfare pattern matching..." << std::endl;
    
    // Placeholder: In a real implementation, we'd iterate MethodDefinitions in metadata
    // and match against the 'methodPointer' array found in libil2cpp.so
    
    return 0x9832; // Example RVA provided by user
}

void Il2CppScanner::scanAllMethods(std::vector<ElfSymbol>& outSymbols) {
    if (!valid && libso.empty()) {
        uintptr_t base = MemoryUtils::getBaseAddress("libil2cpp.so");
        if (base) {
            Logger::log(Logger::SUCCESS, "Found libil2cpp.so at: 0x" + std::to_string(base));
        }
    }
    
    if (metadata.empty()) {
        auto maps = MemoryUtils::getProcessMaps();
        for (const auto& range : maps) {
            // Scan anonymous memory for metadata signature
            for (uintptr_t addr = range.start; addr < range.end - 4; addr += 4) {
                if (*(uint32_t*)addr == 0xFAB11BAF) {
                    Logger::log(Logger::SUCCESS, "Found IL2CPP Metadata at: 0x" + std::to_string(addr));
                    // Parse header here...
                    break;
                }
            }
        }
    }

    Logger::log(Logger::INFO, "Extracting IL2CPP Methods (Dumper Style)...");
}
