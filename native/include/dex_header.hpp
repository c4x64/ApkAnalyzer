#pragma once
#include <cstdint>

namespace native {
    struct dex_header {
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
}
