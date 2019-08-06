//
// Created by XingfengYang on 2019-08-05.
//

#include "../include/common.h"
#include <iostream>

uint16_t signExtend(uint16_t source, uint16_t numberOfDigits) {
    if ((source >> (numberOfDigits - 1)) & 1) {
        source |= (0xFFFF << numberOfDigits);
    }
    return source;
}

uint16_t swapEdition(uint16_t source) {
    uint16_t smallEdition = ((source << 8) & 0xFF00);
    uint16_t bigEdition = ((source >> 8) & 0xFF);
    return smallEdition | bigEdition;
}

