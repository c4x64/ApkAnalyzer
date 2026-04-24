#include "../../include/dex_header.hpp"
#include <cstring>

extern "C" {
    bool validate_dex_header(const uint8_t* data, size_t size) {
        if (size < sizeof(native::dex_header)) return false;
        
        const native::dex_header* header = reinterpret_cast<const native::dex_header*>(data);
        return std::memcmp(header->magic, "dex\n", 4) == 0;
    }

    uint32_t get_dex_class_count(const uint8_t* data, size_t size) {
        if (!validate_dex_header(data, size)) return 0;
        
        const native::dex_header* header = reinterpret_cast<const native::dex_header*>(data);
        return header->class_defs_size;
    }
}
