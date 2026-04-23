#pragma once
#include <string>
#include <vector>
#include <cstdint>

class ZipUtility {
public:
    static std::vector<uint8_t> readFile(const std::string& zipPath, const std::string& fileName);
    static std::vector<std::string> listFiles(const std::string& zipPath);
    static std::string findMetadataMagic(const std::string& zipPath);
};
