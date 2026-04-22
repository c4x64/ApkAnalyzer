#include "Il2CppScanner.hpp"
#include "Il2CppMetadataDefs.hpp"
#include "Il2CppCodeRegistration.hpp"
#include "Logger.hpp"
#include <iostream>
#include <algorithm>
#include <cstring>

// ... (existing Il2CppMetadataHeader)

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
                    const auto* header = reinterpret_cast<const Il2CppMetadataHeader*>(metadataBase);
                    
                    const Il2CppMethodDefinition* methods = reinterpret_cast<const Il2CppMethodDefinition*>(metadataBase + header->methodDefinitionsOffset);
                    
                    // Locate CodeRegistration (often found by scanning for patterns in libil2cpp)
                    uintptr_t codeRegistration = MemoryUtils::findPattern(libBase, libBase + 0x1000000, 
                        "\x00\x00\x00\x00", "???"); // Pattern would be specific to IL2CPP version

                    if (codeRegistration) {
                        const auto* reg = reinterpret_cast<const Il2CppCodeRegistration*>(codeRegistration);
                        const uintptr_t* methodPtrs = reinterpret_cast<const uintptr_t*>(reg->methodPointers);

                        for (uint32_t i = 0; i < header->methodDefinitionsCount; i++) {
                            ElfSymbol sym;
                            sym.address = methodPtrs[i];
                            // Resolve string name using methods[i].nameIndex
                            outSymbols.push_back(sym);
                        }
                        Logger::log(Logger::SUCCESS, "Extracted " + std::to_string(header->methodDefinitionsCount) + " methods.");
                    }
                    break;
                }
            }
        }
    }
}
