//
//  m8r.h
//
//  Created by Chris Marrin on 3/19/2011.
//
//

/*
Copyright (c) 2009-2011 Chris Marrin (chris@marrin.com)
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    - Redistributions of source code must retain the above copyright notice, this 
      list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright notice, 
      this list of conditions and the following disclaimer in the documentation 
      and/or other materials provided with the distribution.

    - Neither the name of Video Monkey nor the names of its contributors may be 
      used to endorse or promote products derived from this software without 
      specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
DAMAGE.
*/

#pragma once

/*  This is the minimum file to use AVR
    This file should be included by all files that will use 
    or implement the interfaces
*/

#include <stdint.h>
#include <avr/io.h>

#ifdef DEBUG
extern "C" {
    void _assert(uint8_t code);
}

#define ASSERT(expr, code) if (!(expr)) _assert(code)
#else
#define ASSERT(expr, code)
#endif

#define _INLINE_ __attribute__ ((always_inline)) 
#define _NO_INLINE_ __attribute__ ((noinline))

namespace m8r {

enum ErrorType {
    ErrorUser = 0x10,
    AssertTimerEventAlloc = 0x32,
    AssertEventAlloc = 0x33,
    AssertNoTimerEventMgr = 0x34,
    AssertSglTimerEventMgr = 0x35,
    AssertNoApp = 0x36,
    AssertSglApp = 0x37,
    AssertNoTimer0 = 0x38,
    AssertSglTimer0 = 0x39,
    AssertNoTimer1 = 0x3a,
    AssertSglTimer1 = 0x3b,
    AssertNoTimer2 = 0x3c,
    AssertSglTimer2 = 0x3d,
};

enum Ports {
#ifdef PORTA
        A = 0x00,
#endif
#ifdef PORTB
        B = 0x03,
#endif
#ifdef PORTC
        C = 0x06,
#endif
#ifdef PORTD
        D = 0x09,
#endif
#ifdef PORTE
        E = 0x0c,
#endif
#ifdef PORTF
        F = 0x0f,
#endif
#ifdef PORTG
        G = 0x12,
#endif
        _NOPORT
    };
    
    enum Registers {
#ifdef TIFR0
        _TIFR0 = 0x15,
#endif
#ifdef TIFR1
        _TIFR1 = 0x16,
#endif
#ifdef TIFR1
        _TIFR2 = 0x17,
#endif
#ifdef GTCCR
        _GTCCR = 0x23,
#endif
#ifdef TCCR0A
        _TCCR0A = 0x24,
#endif
#ifdef TCCR0B
        _TCCR0B = 0x25,
#endif
#ifdef TCNT0
        _TCNT0 = 0x26,
#endif
#ifdef OCR0A
        _OCR0A = 0x27,
#endif
#ifdef OCR0B
        _OCR0B = 0x28,
#endif
#ifdef TIMSK0
        _TIMSK0 = 0x6e,
#endif
#ifdef TIMSK1
        _TIMSK1 = 0x6f,
#endif
#ifdef TIMSK2
        _TIMSK2 = 0x70,
#endif
#ifdef TCCR1A
        _TCCR1A = 0x80,
#endif
#ifdef TCCR1B
        _TCCR1B = 0x81,
#endif
#ifdef TCCR1C
        _TCCR1C = 0x82,
#endif
#ifdef TCNT1
        _TCNT1 = 0x84,
#endif
#ifdef ICR1
        _ICR1 = 0x86,
#endif
#ifdef OCR1A
        _OCR1A = 0x88,
#endif
#ifdef OCR1B
        _OCR1B = 0x8A,
#endif
#ifdef TCCR2A
        _TCCR2A = 0xB0,
#endif
#ifdef TCCR2B
        _TCCR2B = 0xB1,
#endif
#ifdef TCNT2
        _TCNT2 = 0xB2,
#endif
#ifdef OCR2A
        _OCR2A = 0xB3,
#endif
#ifdef OCR2B
        _OCR2B = 0xB4,
#endif
#ifdef ASSR
        _ASSR = 0xB6,
#endif
        _NOREG
    };
    
#define REG8(reg) _SFR_MEM8(reg)
#define REG16(reg) _SFR_MEM16(reg)
#define PIN(port) _SFR_IO8(port)
#define DDR(port) _SFR_IO8(port + 1)
#define PORT(port) _SFR_IO8(port + 2)

// Template for Port classes
template <uint8_t port>
class Port {
public:
    uint8_t getDDR() const _INLINE_ { return DDR(port); }
    void setDDR(uint8_t v) _INLINE_ { DDR(port) = v; }
    void setBitOutput(uint8_t i) _INLINE_ { DDR(port) |= _BV(i); }
    void setBitInput(uint8_t i) _INLINE_ { DDR(port) &= ~_BV(i); }
    bool isBitOutput(uint8_t i) const _INLINE_ { return DDR(port) & _BV(i); }
    volatile uint8_t& getDDRAddress() _INLINE_ { return DDR(port); }
    
    uint8_t getPort() const _INLINE_ { return PORT(port); }
    void setPort(uint8_t v) _INLINE_ { PORT(port) = v; }
    bool isPortBit(uint8_t i) const _INLINE_ { return PORT(port) & _BV(i); }
    void setPortBit(uint8_t i) _INLINE_ { PORT(port) |= _BV(i); }
    void clearPortBit(uint8_t i) _INLINE_ { PORT(port) &= ~_BV(i); }
    volatile uint8_t& getPortAddress() _INLINE_ { return PORT(port); }

    uint8_t getPin() const _INLINE_ { return PIN(port); }
    bool isPinBit(uint8_t i) const _INLINE_ { return PIN(port) & _BV(i); }
    volatile uint8_t& getPinAddress() _INLINE_ { return PIN(port); }
};
    
// Template for 8 bit registers
template <uint8_t reg>
class Reg8 {
public:
    uint8_t get() const _INLINE_ { return REG8(reg); }
    void set(uint8_t v) _INLINE_ { REG8(reg) = v; }
    void setBit(uint8_t i, bool b) _INLINE_
    {
        if (b)
            setBit(i);
        else
            clearBit(i);
    }
    void setBit(uint8_t i) _INLINE_ { REG8(reg) |= _BV(i); }
    void clearBit(uint8_t i) _INLINE_ { REG8(reg) &= ~_BV(i); }
    bool isBitSet(uint8_t i) const _INLINE_ { return (REG8(reg) & _BV(i)) != 0; }
    void setBitMask(uint8_t m, bool b) _INLINE_
    {
        if (b)
            setBitMask(m);
        else
            clearBitMask(m);
    }
    void setBitMask(uint8_t m) _INLINE_ { REG8(reg) |= m; }
    void clearBitMask(uint8_t m) _INLINE_ { REG8(reg) &= ~m; }
    bool isBitMaskSet(uint8_t m) const _INLINE_ { return (REG8(reg) & m) != 0; }
    void setMaskedBits(uint8_t v, uint8_t m) { REG8(reg) &= ~m; REG8(reg) |= v & m; }
    volatile uint8_t& getAddress() _INLINE_ { return REG8(reg); }
};

template <uint8_t reg>
class Reg16 {
public:
    uint16_t get() const _INLINE_ { return REG16(reg); }
    void set(uint16_t v) _INLINE_ { REG16(reg) = v; }
    volatile uint8_t& getAddress() _INLINE_ { return REG16(reg); }
};
}
