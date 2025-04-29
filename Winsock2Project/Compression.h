#pragma once
#include <windows.h>
#include <compressapi.h>
#include "Logger.h"
#pragma comment(lib, "Cabinet.lib")  // Required for compression functions

// define compressedSize

#define COMPRESSED_SIZE 1024
#define ORIGINAL_SIZE 2048
#define DECOMPRESSED_SIZE 2048
#define COMPRESS_ALGORITHM_LZNT1 2  // LZNT1 Compression (Windows-defined constant)

void decompressData(const std::vector<char>& compressedData, std::vector<char>& decompressedData) {

    // define compress_agorithm
    DECOMPRESSOR_HANDLE decompressor;
    if (!CreateDecompressor(COMPRESS_ALGORITHM_LZNT1, NULL, &decompressor)) {
        spdlog::error("Failed to create decompressor: " + GetLastError());
        return;
    }

    SIZE_T decompressedSize = decompressedData.size();
    if (!Decompress(decompressor, compressedData.data(), compressedData.size(),
        decompressedData.data(), decompressedSize, &decompressedSize)) {
        spdlog::error("Decompression failed: " + GetLastError());
    }

    CloseDecompressor(decompressor);  // Proper cleanup
}
