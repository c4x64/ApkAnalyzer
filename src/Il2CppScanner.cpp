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
                    
                    // Locate CodeRegistration (scanning for typical registration patterns in libil2cpp)
                    std::vector<uint8_t> pattern = { 0x48, 0x8d, 0x05 }; // Example: LEA instruction
                    std::string mask = "xxx";
                    uintptr_t codeRegistration = MemoryUtils::findPattern(libBase, libBase + 0x500000, pattern, mask);

                    if (codeRegistration) {
                        Logger::log(Logger::INFO, "Found potential CodeRegistration at: 0x" + std::to_string(codeRegistration));
                        const auto* reg = reinterpret_cast<const Il2CppCodeRegistration*>(codeRegistration);
                        // ... mapping logic remains the same
                    }
                    break;
                }
            }
        }
    }
}
