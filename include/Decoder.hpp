#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace Decoder {
    // Simple ARM64 instruction decoder
    struct Instruction {
        uint32_t raw;
        uint32_t op;
        uint32_t rd;
        uint32_t rn;
        uint32_t imm;
    };

    // Scans for a decryption loop (XOR + Add)
    bool isDecryptionLoop(const uint8_t* code, size_t size, uint64_t& out_offset);
}
