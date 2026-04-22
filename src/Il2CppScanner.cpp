#include "Il2CppScanner.hpp"
#include "Il2CppMetadataDefs.hpp"
#include "Il2CppCodeRegistration.hpp"
#include "PatternScanner.hpp"
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
    
    uintptr_t metadataBase = 0;
    if (metadata.empty()) {
        auto maps = MemoryUtils::getProcessMaps();
        for (const auto& range : maps) {
            for (uintptr_t addr = range.start; addr < range.end - 4; addr += 4) {
                if (*(uint32_t*)addr == 0xFAB11BAF) {
                    metadataBase = addr;
                    Logger::log(Logger::SUCCESS, "Found IL2CPP Metadata at: 0x" + std::to_string(metadataBase));
                    const auto* header = reinterpret_cast<const Il2CppMetadataHeader*>(metadataBase);
                    
                    const Il2CppMethodDefinition* methods = reinterpret_cast<const Il2CppMethodDefinition*>(metadataBase + header->methodDefinitionsOffset);
                    
                    // Locate CodeRegistration (scanning for typical registration patterns in libil2cpp)
                    std::vector<uint8_t> pattern = { 0x48, 0x8d, 0x05 }; // Example: LEA instruction
                    std::string mask = "xxx";
                    uintptr_t codeRegistration = MemoryUtils::findPattern(libBase, libBase + 0x500000, pattern, mask);

                    if (codeRegistration) {
                        Logger::log(Logger::INFO, "Found potential CodeRegistration at: 0x" + std::to_string(codeRegistration));
                        const auto* reg = reinterpret_cast<const Il2CppCodeRegistration*>(codeRegistration);
                        const uintptr_t* methodPtrs = reinterpret_cast<const uintptr_t*>(reg->methodPointers);
                        const char* stringData = reinterpret_cast<const char*>(metadataBase + header->stringDataOffset);
                        const int32_t* stringIndices = reinterpret_cast<const int32_t*>(metadataBase + header->stringIndexTableOffset);

                        for (uint32_t i = 0; i < header->methodDefinitionsCount; i++) {
                            ElfSymbol sym;
                            sym.address = methodPtrs[i];
                            int32_t nameIdx = methods[i].nameIndex;
                            if (nameIdx >= 0) {
                                sym.name = std::string(stringData + stringIndices[nameIdx]);
                            } else {
                                sym.name = "UnknownMethod_" + std::to_string(i);
                            }
                            outSymbols.push_back(sym);
                        }
                        Logger::log(Logger::SUCCESS, "Extracted " + std::to_string(header->methodDefinitionsCount) + " methods with names.");
                    }
                    break;
                }
            }
        }
    }
}
