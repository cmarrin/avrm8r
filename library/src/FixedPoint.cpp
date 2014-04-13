//
//  FixedPoint.cpp
//  marrinator
//
//  Created by Chris Marrin on 4/13/14.
//
//

#include "FixedPoint.h"

using namespace m8r;

char* FixedPoint::toString(uint32_t v, bool isSigned, char* s, uint8_t size)
{
    ASSERT(size >= 3, AssertFixedPointBufferTooSmall);
    if (isSigned && ((int32_t) v < 0)) {
        v = -v;
    } else {
        isSigned = false;
    }
    char* p = s + size;
    if (!v) {
        *--p = '0';
    } else {
        while (v && p > s) {
            *--p = (v % 10) + '0';
            v /= 10;
        }
        if (isSigned && p > s) {
            *--p = '-';
        }
    }
    return p;
}

char _buf[12];

const char* FixedPoint::toString(int16_t value, uint8_t maxFractionDigits)
{
    ASSERT(sizeof(_buf) >= 3 && maxFractionDigits <= sizeof(_buf) - 3, AssertFixedPointBufferTooSmall);
    bool negative = false;
    if (value < 0) {
        negative = true;
        value = -value;
    }
    int32_t multiplier = 1;
    for (uint8_t i = maxFractionDigits; i; --i) {
        multiplier *= 10;
    }
    uint32_t ivalue = (static_cast<int32_t>(value) * multiplier + 128) / 256;
#ifdef DEBUG
    uint32_t _maxValue = 1;
    for (uint8_t i = sizeof(_buf) - 3; i > 0; --i) {
        _maxValue *= 10;
    }
    ASSERT(ivalue <= _maxValue, AssertFixedPointBufferTooSmall);
#endif
    _buf[sizeof(_buf) - 1] = '\0';
    char* p = toString(ivalue, true, _buf, sizeof(_buf) - 1);
    
    // Insert decimal
    int8_t integerDigits = _buf + sizeof(_buf) - p - maxFractionDigits - 1;
    if (integerDigits <= 0) {
        // No integer digits, pad and return 0.xxx
        while (integerDigits++ < 0) {
            *--p = '0';
        }
        *--p = '.';
        *--p = '0';
    } else {
        --p;
        for (uint8_t i = 0; i < integerDigits; ++i) {
            p[i] = p[i+1];
        }
        p[integerDigits] = '.';
    }
    if (negative) {
        *--p = '-';
    }
    return p;
}
