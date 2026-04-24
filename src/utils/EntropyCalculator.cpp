#include "../../include/EntropyCalculator.hpp"
#include <cmath>
#include <array>
#include <algorithm>

namespace ApkAnalyzer {

double EntropyCalculator::calculate(const uint8_t* data, size_t size) {
    if (size == 0) return 0.0;

    std::array<size_t, 256> counts = {0};
    for (size_t i = 0; i < size; ++i) {
        counts[data[i]]++;
    }

    double entropy = 0.0;
    for (size_t count : counts) {
        if (count > 0) {
            double p = static_cast<double>(count) / size;
            entropy -= p * std::log2(p);
        }
    }

    return entropy;
}

double EntropyCalculator::calculate(const std::vector<uint8_t>& data) {
    return calculate(data.data(), data.size());
}

} // namespace ApkAnalyzer
