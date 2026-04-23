#pragma once
#include <vector>
#include <string>
#include <cstdint>

struct ElfSymbol {
    std::string name;
    uint64_t address;
    uint32_t size;
    uint8_t info;
    uint16_t section_idx; // Added section index for better identification
};

class ElfParser {
public:
    ElfParser(const std::vector<uint8_t>& data);
    
    bool isValid() const;
    bool is64Bit() const;
    
    std::vector<ElfSymbol> getSymbols() const;
    uint64_t findSymbolAddress(const std::string& name) const;
    
    // New feature: Section extraction
    struct Section {
        std::string name;
        uint64_t address;
        uint64_t offset;
        uint64_t size;
        uint32_t type;
    };
    std::vector<Section> getSections() const;

private:
    std::vector<uint8_t> data;
    bool valid = false;
    bool _is64Bit = false;
    
    void parse32();
    void parse64();
    
    std::vector<ElfSymbol> symbols;
    std::vector<Section> sections;
};
