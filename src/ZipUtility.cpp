#include "ZipUtility.hpp"
#define MINIZ_HEADER_FILE_ONLY
#include "miniz.c"
#include <stdexcept>

std::vector<uint8_t> ZipUtility::readFile(const std::string& zipPath, const std::string& fileName) {
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    if (!mz_zip_reader_init_file(&zip_archive, zipPath.c_str(), 0)) {
        return {};
    }

    size_t uncompressed_size;
    void* p = mz_zip_reader_extract_file_to_heap(&zip_archive, fileName.c_str(), &uncompressed_size, 0);
    
    if (!p) {
        mz_zip_reader_end(&zip_archive);
        return {};
    }

    std::vector<uint8_t> data(reinterpret_cast<uint8_t*>(p), reinterpret_cast<uint8_t*>(p) + uncompressed_size);
    mz_free(p);
    mz_zip_reader_end(&zip_archive);
    
    return data;
}

std::vector<std::string> ZipUtility::listFiles(const std::string& zipPath) {
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    if (!mz_zip_reader_init_file(&zip_archive, zipPath.c_str(), 0)) {
        return {};
    }

    std::vector<std::string> files;
    mz_uint num_files = mz_zip_reader_get_num_files(&zip_archive);
    for (mz_uint i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat file_stat;
        if (mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
            files.push_back(file_stat.m_filename);
        }
    }

    mz_zip_reader_end(&zip_archive);
    return files;
}

std::string ZipUtility::findMetadataMagic(const std::string& zipPath) {
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    if (!mz_zip_reader_init_file(&zip_archive, zipPath.c_str(), 0)) {
        return "";
    }

    mz_uint num_files = mz_zip_reader_get_num_files(&zip_archive);
    for (mz_uint i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat file_stat;
        if (mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
            // Read first 4 bytes to check magic
            uint32_t magic = 0;
            size_t bytes_read = 0;
            void* p = mz_zip_reader_extract_file_to_heap(&zip_archive, file_stat.m_filename, &bytes_read, 0);
            if (p && bytes_read >= 4) {
                magic = *(uint32_t*)p;
                mz_free(p);
                if (magic == 0xFAB11BAF) {
                    mz_zip_reader_end(&zip_archive);
                    return file_stat.m_filename;
                }
            } else if (p) {
                mz_free(p);
            }
        }
    }

    mz_zip_reader_end(&zip_archive);
    return "";
}
