#pragma once
#include <vector>
#include <cstdint>
#include <string>

namespace ApkAnalyzer {

class EntropyCalculator {
public:
    static double calculate(const std::vector<uint8_t>& data);
    static double calculate(const uint8_t* data, size_t size);
    static double calculateFromFile(const std::string& filePath);
};

} // namespace ApkAnalyzer
