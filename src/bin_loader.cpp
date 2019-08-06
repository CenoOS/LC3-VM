//
// Created by XingfengYang on 2019-08-04.
//

#include "../include/bin_loader.h"
#include <fstream>
#include <iterator>
#include <bitset>

using namespace lc3;

void BinLoader::loadBinFile(const char *fileName) {
    std::ifstream input(fileName, std::ios::binary);
    // copies all data into buffer
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

    for (int i = 0; i < buffer.size(); i += 2) {
        uint16_t bigEdition = (uint16_t) buffer.at(i);
        uint16_t smallEdition = (uint16_t) buffer.at(i + 1);
        uint16_t result = (bigEdition | smallEdition << 8);
        this->binBuffer.push_back(result);
    }
}

std::vector<uint16_t> BinLoader::getBinaryBuffer() {
    return this->binBuffer;
}