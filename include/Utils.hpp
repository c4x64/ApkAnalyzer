#pragma once
#include <string>
#include <vector>

namespace ApkAnalyzer {

class Utils {
public:
    static std::vector<uint8_t> hexToBytes(const std::string& hex);
    static std::string bytesToHex(const uint8_t* data, size_t size);
    static bool fileExists(const std::string& path);
    static std::string getFileName(const std::string& path);
};

} // namespace ApkAnalyzer
