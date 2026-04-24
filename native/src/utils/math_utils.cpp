#include <cmath>
#include <vector>
#include <cstdint>

extern "C" {
    double calculate_shannon_entropy(const uint8_t* data, size_t size) {
        if (size == 0) return 0.0;
        size_t counts[256] = {0};
        for (size_t i = 0; i < size; ++i) counts[data[i]]++;
        
        double entropy = 0.0;
        for (size_t i = 0; i < 256; ++i) {
            if (counts[i] > 0) {
                double p = (double)counts[i] / size;
                entropy -= p * log2(p);
            }
        }
        return entropy;
    }
}
