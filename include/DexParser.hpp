#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct DexMethod {
    std::string className;
    std::string methodName;
    std::string signature;
    uint32_t methodIdx;
};

class DexParser {
public:
    DexParser(const std::vector<uint8_t>& data);
    
    bool isValid() const;
    std::vector<DexMethod> getMethods() const;

private:
    std::vector<uint8_t> data;
    bool valid = false;
    std::vector<DexMethod> methods;
    
    void parse();
};
