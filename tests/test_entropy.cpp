#include "../include/EntropyCalculator.hpp"
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

using namespace ApkAnalyzer;

void test_basic_entropy() {
    // 0 entropy: all bytes are the same
    std::vector<uint8_t> data1(100, 0xAA);
    double e1 = EntropyCalculator::calculate(data1);
    std::cout << "Entropy of uniform data: " << e1 << std::endl;
    assert(std::abs(e1 - 0.0) < 0.001);

    // Max entropy (approx): all bytes are different
    std::vector<uint8_t> data2;
    for (int i = 0; i < 256; ++i) {
        data2.push_back(static_cast<uint8_t>(i));
    }
    double e2 = EntropyCalculator::calculate(data2);
    std::cout << "Entropy of 0-255 data: " << e2 << std::endl;
    assert(std::abs(e2 - 8.0) < 0.001);
}

int main() {
    try {
        test_basic_entropy();
        std::cout << "All entropy tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
