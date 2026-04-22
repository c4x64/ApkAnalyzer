#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include "ElfParser.hpp"
#include "MemoryUtils.hpp"

class Il2CppScanner {
public:
    Il2CppScanner(const std::vector<uint8_t>& metadata, const std::vector<uint8_t>& libso);
    
    bool isValid() const;
    uint64_t findMethodOffset(const std::string& className, const std::string& methodName);
    void scanAllMethods(std::vector<ElfSymbol>& outSymbols);
    void decryptStringTable(std::vector<uint8_t>& stringTable, uint32_t xorKey);

private:
    std::vector<uint8_t> metadata;
    std::vector<uint8_t> libso;
    bool valid = false;
};
