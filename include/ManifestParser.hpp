#pragma once
#include <vector>
#include <cstdint>
#include <string>

class ManifestParser {
public:
    ManifestParser(const std::vector<uint8_t>& data);
    bool isValid() const;
    std::string getPackageName();

private:
    std::vector<uint8_t> data;
    bool valid = false;
};
