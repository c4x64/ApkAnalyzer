#include "ManifestParser.hpp"

ManifestParser::ManifestParser(const std::vector<uint8_t>& data) : data(data) {
    if (data.size() < 8) return;
    // Android binary XML magic 0x00080003
    if (data[0] == 0x03 && data[1] == 0x00 && data[2] == 0x08 && data[3] == 0x00) {
        valid = true;
    }
}

bool ManifestParser::isValid() const { return valid; }

std::string ManifestParser::getPackageName() {
    // Requires parsing the string pool in the binary XML
    return "unknown.package";
}
