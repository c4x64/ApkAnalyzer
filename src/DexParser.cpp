#include "DexParser.hpp"
#include <cstring>

struct DexHeader {
    uint8_t magic[8];
    uint32_t checksum;
    uint8_t signature[20];
    uint32_t file_size;
    uint32_t header_size;
    uint32_t endian_tag;
    uint32_t link_size;
    uint32_t link_off;
    uint32_t map_off;
    uint32_t string_ids_size;
    uint32_t string_ids_off;
    uint32_t type_ids_size;
    uint32_t type_ids_off;
    uint32_t proto_ids_size;
    uint32_t proto_ids_off;
    uint32_t field_ids_size;
    uint32_t field_ids_off;
    uint32_t method_ids_size;
    uint32_t method_ids_off;
    uint32_t class_defs_size;
    uint32_t class_defs_off;
    uint32_t data_size;
    uint32_t data_off;
};

struct DexMethodId {
    uint16_t class_idx;
    uint16_t proto_idx;
    uint32_t name_idx;
};

struct DexTypeId {
    uint32_t descriptor_idx;
};

DexParser::DexParser(const std::vector<uint8_t>& data) : data(data) {
    if (data.size() < sizeof(DexHeader)) return;
    if (memcmp(data.data(), "dex\n", 4) != 0) return;
    valid = true;
    parse();
}

bool DexParser::isValid() const { return valid; }

// Helper to read LEB128 (used for string lengths in DEX)
uint32_t readULeb128(const uint8_t*& ptr) {
    uint32_t result = 0;
    int shift = 0;
    while (true) {
        uint8_t byte = *ptr++;
        result |= (byte & 0x7f) << shift;
        if ((byte & 0x80) == 0) break;
        shift += 7;
    }
    return result;
}

void DexParser::parse() {
    const DexHeader* header = reinterpret_cast<const DexHeader*>(data.data());
    
    auto getString = [&](uint32_t idx) -> std::string {
        if (idx >= header->string_ids_size) return "";
        uint32_t off = *reinterpret_cast<const uint32_t*>(data.data() + header->string_ids_off + idx * 4);
        const uint8_t* ptr = data.data() + off;
        readULeb128(ptr); // Skip length
        return std::string(reinterpret_cast<const char*>(ptr));
    };
    
    auto getClassName = [&](uint32_t typeIdx) -> std::string {
        if (typeIdx >= header->type_ids_size) return "";
        uint32_t nameIdx = *reinterpret_cast<const uint32_t*>(data.data() + header->type_ids_off + typeIdx * 4);
        return getString(nameIdx);
    };
    
    const DexMethodId* methodIds = reinterpret_cast<const DexMethodId*>(data.data() + header->method_ids_off);
    for (uint32_t i = 0; i < header->method_ids_size; ++i) {
        DexMethod m;
        m.methodIdx = i;
        m.className = getClassName(methodIds[i].class_idx);
        m.methodName = getString(methodIds[i].name_idx);
        m.signature = ""; // Minimal: skip proto for now
        methods.push_back(m);
    }
}

std::vector<DexMethod> DexParser::getMethods() const { return methods; }
