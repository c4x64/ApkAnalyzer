#include "Decoder.hpp"
#include <vector>

namespace Decoder {
    // Decrypts an encrypted IL2CPP string table using the static pattern scanner
    void decryptStringTable(std::vector<uint8_t>& table, int32_t key) {
        // Find decryption loop in the table data itself if obfuscated
        uint64_t loopOffset = 0;
        if (isDecryptionLoop(table.data(), table.size(), loopOffset)) {
            // Apply simple XOR based on the key
            for (size_t i = 0; i < table.size(); i++) {
                table[i] ^= (uint8_t)(key & 0xFF);
            }
        }
    }
}
