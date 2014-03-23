//
//  FixedPoinr.h
//  marrinator
//
//  Created by Chris Marrin on 3/18/14.
//
//

#pragma once

#include "m8r.h"

namespace m8r {

    class FixedPoint
    {
    public:
        // s must contain space for at least 3 characters
        static char* toString(uint32_t v, bool isSigned, char* s, uint8_t size)
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

    protected:
        const char* toString(int16_t value, uint8_t maxFractionDigits, char* buf, uint8_t size)
        {
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
            buf[size - 1] = '\0';
            char* p = toString(ivalue, true, buf, size - 1);
            
            // Insert decimal
            int8_t integerDigits = buf + size - p - maxFractionDigits - 1;
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

        int16_t convert(int16_t inputValue, uint16_t scale)
        {
            return (static_cast<int32_t>(inputValue) * 256 + (scale >> 1)) / scale;
        }
    };
    
    class FixedPoint8_8 : public FixedPoint
    {
    public:
        FixedPoint8_8() : _value(0) { }
        FixedPoint8_8(int16_t value, uint16_t scale) : _value(FixedPoint::convert(value, scale)) { }
        
        void setValue(int16_t value, uint16_t scale) { _value = FixedPoint::convert(value, scale); }

        // Buffer must be at least 9 characters
        const char* toString(char* buf, uint8_t fractionDigits) { return FixedPoint::toString(_value, fractionDigits, buf, 9); }
        
        int16_t value() const { return _value; }

    private:
        int16_t _value;
    };
}