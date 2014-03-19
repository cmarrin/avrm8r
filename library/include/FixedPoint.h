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
        static const char* toString(uint32_t v, bool isSigned, char* s, uint8_t size)
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
        const char* toString(uint8_t integer, uint8_t fraction, bool isSigned, uint8_t maxFractionDigits, char* buf, uint8_t size)
        {
            if (fraction) {
                // Output fraction
                uint32_t multiplier = 10;
                char* p = &buf[4];
                *p++ = '.';
                uint32_t f = fraction;
                while (f && maxFractionDigits--) {
                    uint32_t digit = f * multiplier / 256;
                    *p++ = digit + '0';
                    f -= digit * 256 / multiplier;
                    multiplier *= 10;
                }
                *p = '\0';
                
                // Output integer
                return FixedPoint::toString(integer, isSigned, buf, 4);
            } else {
                buf[7] = '\0';
                return FixedPoint::toString(integer, isSigned, buf, 7);
            }
        }

        void convert(uint16_t value, uint16_t radix, bool isSigned, uint8_t& integer, uint8_t& fraction)
        {
            if (isSigned && value & 0x8000) {
                value = -value;
            } else {
                isSigned = false;
            }
            integer = value / radix;
            value -= integer * radix;
            if (isSigned) {
                integer = -integer;
            }
            fraction = (static_cast<uint32_t>(value) * 256 + 128) / radix;
        }
    };
    
    class FixedPointU8_8 : public FixedPoint
    {
    public:
        FixedPointU8_8(uint16_t value, uint8_t radix) { FixedPoint::convert(value, radix, false, _integer, _fraction); }

        // Buffer must be at least 9 characters
        const char* toString(char* buf) { return FixedPoint::toString(_integer, _fraction, false, 2, buf, 9); }
        
        uint8_t integer() const { return _integer; }
        uint8_t fraction() const { return _fraction; }

    private:
        uint8_t _integer, _fraction;
    };
    
    class FixedPointS8_8 : public FixedPoint
    {
    public:
        FixedPointS8_8(int16_t value, uint16_t radix) { convert(value, radix, true, _integer, _fraction); }

        // Buffer must be at least 9 characters
        const char* toString(char* buf) { return FixedPoint::toString(_integer, _fraction, true, 2, buf, 9); }

        uint8_t integer() const { return _integer; }
        uint8_t fraction() const { return _fraction; }

    private:
        uint8_t _integer, _fraction;
    };
}