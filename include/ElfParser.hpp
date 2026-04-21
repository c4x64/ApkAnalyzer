#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <map>

struct ElfSymbol {
    std::string name;
    uint64_t address;
    uint32_t size;
    uint8_t info;
};

class ElfParser {
public:
    ElfParser(const std::vector<uint8_t>& data);
    
    bool isValid() const;
    bool is64Bit() const;
    
    std::vector<ElfSymbol> getSymbols() const;
    uint64_t findSymbolAddress(const std::string& name) const;

private:
    std::vector<uint8_t> data;
    bool valid = false;
    bool _is64Bit = false;
    
    void parse32();
    void parse64();
    
    std::vector<ElfSymbol> symbols;
};
