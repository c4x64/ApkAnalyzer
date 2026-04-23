#include "ElfParser.hpp"
#include <cstring>
#include <algorithm>

// Minimal ELF structures
struct Elf64_Ehdr {
    unsigned char e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct Elf64_Shdr {
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
};

struct Elf64_Sym {
    uint32_t st_name;
    unsigned char st_info;
    unsigned char st_other;
    uint16_t st_shndx;
    uint64_t st_value;
    uint64_t st_size;
};

ElfParser::ElfParser(const std::vector<uint8_t>& data) : data(data) {
    if (data.size() < 16) return;
    if (data[0] != 0x7F || data[1] != 'E' || data[2] != 'L' || data[3] != 'F') return;
    
    _is64Bit = (data[4] == 2);
    valid = true;
    
    if (_is64Bit) parse64();
    else parse32();
}

bool ElfParser::isValid() const { return valid; }
bool ElfParser::is64Bit() const { return _is64Bit; }

void ElfParser::parse64() {
    if (data.size() < sizeof(Elf64_Ehdr)) return;
    const Elf64_Ehdr* ehdr = reinterpret_cast<const Elf64_Ehdr*>(data.data());
    
    if (ehdr->e_shoff + ehdr->e_shnum * ehdr->e_shentsize > data.size()) return;
    
    const Elf64_Shdr* shdr = reinterpret_cast<const Elf64_Shdr*>(data.data() + ehdr->e_shoff);
    
    for (int i = 0; i < ehdr->e_shnum; ++i) {
        if (shdr[i].sh_type == 2 || shdr[i].sh_type == 11) { // SHT_SYMTAB or SHT_DYNSYM
            const Elf64_Sym* syms = reinterpret_cast<const Elf64_Sym*>(data.data() + shdr[i].sh_offset);
            int num_syms = shdr[i].sh_size / sizeof(Elf64_Sym);
            
            uint32_t strtab_idx = shdr[i].sh_link;
            if (strtab_idx >= ehdr->e_shnum) continue;
            const char* strtab = reinterpret_cast<const char*>(data.data() + shdr[strtab_idx].sh_offset);
            
            for (int j = 0; j < num_syms; ++j) {
                ElfSymbol s;
                s.name = strtab + syms[j].st_name;
                s.address = syms[j].st_value;
                s.size = (uint32_t)syms[j].st_size;
                s.info = syms[j].st_info;
                if (!s.name.empty()) {
                    symbols.push_back(s);
                }
            }
        }
    }
}

// 32-bit structures (minimal)
struct Elf32_Sym {
    uint32_t st_name;
    uint32_t st_value;
    uint32_t st_size;
    unsigned char st_info;
    unsigned char st_other;
    uint16_t st_shndx;
};

struct Elf32_Ehdr {
    unsigned char e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct Elf32_Shdr {
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
};

void ElfParser::parse32() {
    if (data.size() < sizeof(Elf32_Ehdr)) return;
    const Elf32_Ehdr* ehdr = reinterpret_cast<const Elf32_Ehdr*>(data.data());
    
    if (ehdr->e_shoff + ehdr->e_shnum * ehdr->e_shentsize > data.size()) return;
    
    const Elf32_Shdr* shdr = reinterpret_cast<const Elf32_Shdr*>(data.data() + ehdr->e_shoff);
    
    for (int i = 0; i < ehdr->e_shnum; ++i) {
        if (shdr[i].sh_type == 2 || shdr[i].sh_type == 11) {
            const Elf32_Sym* syms = reinterpret_cast<const Elf32_Sym*>(data.data() + shdr[i].sh_offset);
            int num_syms = shdr[i].sh_size / sizeof(Elf32_Sym);
            
            uint32_t strtab_idx = shdr[i].sh_link;
            const char* strtab = reinterpret_cast<const char*>(data.data() + shdr[strtab_idx].sh_offset);
            
            for (int j = 0; j < num_syms; ++j) {
                ElfSymbol s;
                s.name = strtab + syms[j].st_name;
                s.address = syms[j].st_value;
                s.size = syms[j].st_size;
                s.info = syms[j].st_info;
                if (!s.name.empty()) {
                    symbols.push_back(s);
                }
            }
        }
    }
}

std::vector<ElfSymbol> ElfParser::getSymbols() const { return symbols; }

uint64_t ElfParser::findSymbolAddress(const std::string& name) const {
    auto it = std::find_if(symbols.begin(), symbols.end(), [&](const ElfSymbol& s) {
        return s.name == name;
    });
    return (it != symbols.end()) ? it->address : 0;
}
