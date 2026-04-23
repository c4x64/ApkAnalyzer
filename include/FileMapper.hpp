#pragma once
#include <vector>
#include <string>
#include <cstdint>

// Simple Windows Memory Mapping wrapper to optimize I/O on older systems
class FileMapper {
public:
    FileMapper(const std::string& path);
    ~FileMapper();

    bool isValid() const;
    const uint8_t* getData() const;
    size_t getSize() const;

private:
    uint8_t* data = nullptr;
    size_t size = 0;
#ifdef _WIN32
    void* hFile = nullptr;
    void* hMap = nullptr;
#endif
};
