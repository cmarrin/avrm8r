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
        static char* toString(uint32_t v, bool isSigned, char* s, uint8_t size);

    protected:
        static const char* toString(int16_t value, uint8_t maxFractionDigits);\
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
        const char* toString(uint8_t fractionDigits) { return FixedPoint::toString(_value, fractionDigits); }
        
        int16_t value() const { return _value; }

    private:
        int16_t _value;
    };
}