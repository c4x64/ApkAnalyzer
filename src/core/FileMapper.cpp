#include "FileMapper.hpp"
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

FileMapper::FileMapper(const std::string& path) {
#ifdef _WIN32
    hFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return;
    
    LARGE_INTEGER li;
    GetFileSizeEx(hFile, &li);
    size = li.QuadPart;
    
    hMap = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (!hMap) return;
    
    data = (uint8_t*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
#else
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) return;
    struct stat st;
    fstat(fd, &st);
    size = st.st_size;
    data = (uint8_t*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
#endif
}

FileMapper::~FileMapper() {
#ifdef _WIN32
    if (data) UnmapViewOfFile(data);
    if (hMap) CloseHandle(hMap);
    if (hFile) CloseHandle(hFile);
#else
    if (data) munmap(data, size);
#endif
}

bool FileMapper::isValid() const { return data != nullptr; }
const uint8_t* FileMapper::getData() const { return data; }
size_t FileMapper::getSize() const { return size; }
