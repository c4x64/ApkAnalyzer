#include "Decoder.hpp"
#include <cstring>

namespace Decoder {
    // Very simple ARM64 pattern matcher for typical XOR loop
    // ARM64: EOR Wd, Wn, Wm / ADD Xn, Xn, #imm
    bool isDecryptionLoop(const uint8_t* code, size_t size, uint64_t& out_offset) {
        if (size < 8) return false;
        
        for (size_t i = 0; i < size - 8; i += 4) {
            uint32_t instr1 = *reinterpret_cast<const uint32_t*>(code + i);
            uint32_t instr2 = *reinterpret_cast<const uint32_t*>(code + i + 4);
            
            // Check for EOR (0x4A000000) followed by ADD (0x91000000)
            // This is a naive heuristic
            if ((instr1 & 0x7E000000) == 0x4A000000 && (instr2 & 0x9F000000) == 0x91000000) {
                out_offset = i;
                return true;
            }
        }
        return false;
    }
}
