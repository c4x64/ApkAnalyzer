#include "../../include/Utils.hpp"
#include <sstream>
#include <iomanip>
#include <sys/stat.h>
#include <algorithm>

namespace ApkAnalyzer {

std::vector<uint8_t> Utils::hexToBytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    std::string cleanHex = hex;
    cleanHex.erase(std::remove(cleanHex.begin(), cleanHex.end(), ' '), cleanHex.end());
    
    for (size_t i = 0; i < cleanHex.length(); i += 2) {
        std::string byteString = cleanHex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(strtol(byteString.c_str(), nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

std::string Utils::bytesToHex(const uint8_t* data, size_t size) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < size; ++i) {
        ss << std::setw(2) << static_cast<int>(data[i]);
    }
    return ss.str();
}

bool Utils::fileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

std::string Utils::getFileName(const std::string& path) {
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash == std::string::npos) return path;
    return path.substr(lastSlash + 1);
}

} // namespace ApkAnalyzer
