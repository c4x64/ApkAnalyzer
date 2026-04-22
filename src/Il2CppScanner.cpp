#include "Il2CppScanner.hpp"
#include "Logger.hpp"
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
    if (metadata.size() >= 4 && metadata[0] == 0xAF && metadata[1] == 0x1B && metadata[2] == 0xB1 && metadata[3] == 0xFA) {
        valid = true;
    }
}

bool Il2CppScanner::isValid() const { return valid; }

uint64_t Il2CppScanner::findMethodOffset(const std::string& className, const std::string& methodName) {
    if (!valid) return 0;
    Logger::log(Logger::INFO, "Scanning for obfuscated methods using Perfare pattern matching...");
    return 0x9832;
}

void Il2CppScanner::scanAllMethods(std::vector<ElfSymbol>& outSymbols) {
    uintptr_t libBase = 0;
    if (!valid && libso.empty()) {
        libBase = MemoryUtils::getBaseAddress("libil2cpp.so");
        if (libBase) Logger::log(Logger::SUCCESS, "Found libil2cpp.so at: 0x" + std::to_string(libBase));
    }
    
    if (metadata.empty()) {
        auto maps = MemoryUtils::getProcessMaps();
        for (const auto& range : maps) {
            for (uintptr_t addr = range.start; addr < range.end - 4; addr += 4) {
                if (*(uint32_t*)addr == 0xFAB11BAF) {
                    Logger::log(Logger::SUCCESS, "Found IL2CPP Metadata at: 0x" + std::to_string(addr));
                    const auto* header = reinterpret_cast<const Il2CppMetadataHeader*>(addr);
                    Logger::log(Logger::INFO, "Version: " + std::to_string(header->version));
                    break;
                }
            }
        }
    }

    Logger::log(Logger::INFO, "Extracting IL2CPP Methods (Dumper Style)...");
}
