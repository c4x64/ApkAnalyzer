#include "Il2CppScanner.hpp"
#include <vector>

void Il2CppScanner::decryptStringTable(std::vector<uint8_t>& stringTable, uint32_t xorKey) {
    for (auto& byte : stringTable) {
        byte ^= (xorKey & 0xFF);
        // Add optional rotation or more complex de-obfuscation here
        byte = (byte << 3) | (byte >> 5);
    }
}
