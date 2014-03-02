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

    - Neither the name of Marrinator nor the names of its contributors may be 
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

#define _INLINE_ __attribute__ ((always_inline)) 
#define _NO_INLINE_ __attribute__ ((noinline))

enum ErrorConditionType { ErrorConditionNote, ErrorConditionWarning, ErrorConditionFatal };

#ifdef __cplusplus
namespace m8r {
#endif

enum ErrorType {
    AssertOutOfMem = 0x01,
    AssertPureVirtual = 0x02,
    AssertDeleteNotSupported = 0x03,
    AssertSingleApp = 0x04,
    AssertNoApp = 0x05,
    AssertSingleADC = 0x10,
    AssertSingleTimer0 = 0x20,
    AssertSingleTimer1 = 0x21,
    AssertSingleTimer2 = 0x22,
    AssertSingleTimerEventMgr = 0x23,
    AssertNoTimerEventMgr = 0x24,
    AssertNoEventTimers = 0x25,
    AssertEthernetBadLength = 0x30,
    AssertEthernetNotInHandler = 0x31,
    AssertEthernetCannotSendData = 0x32,
    AssertEthernetNotWaitingToSendData = 0x33,
    AssertEthernetTransmitError = 0x34,
    AssertEthernetReceiveError = 0x35,
    ErrorUser = 0x80,
};

typedef uint8_t TimerID;
const uint8_t NoTimer = 0xff;

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
        _TIFR0 = 0x35,
#endif
#ifdef TIFR1
        _TIFR1 = 0x36,
#endif
#ifdef TIFR1
        _TIFR2 = 0x37,
#endif
#ifdef GTCCR
        _GTCCR = 0x43,
#endif
#ifdef TCCR0A
        _TCCR0A = 0x44,
#endif
#ifdef TCCR0B
        _TCCR0B = 0x45,
#endif
#ifdef TCNT0
        _TCNT0 = 0x46,
#endif
#ifdef OCR0A
        _OCR0A = 0x47,
#endif
#ifdef OCR0B
        _OCR0B = 0x48,
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

#ifdef __cplusplus
#define REG8(reg) _SFR_MEM8(reg)
#define REG16(reg) _SFR_MEM16(reg)
#define PIN(port) _SFR_IO8(port)
#define DDR(port) _SFR_IO8(port + 1)
#define PORT(port) _SFR_IO8(port + 2)

// Template for Port classes
template <uint8_t p>
class Port {
public:
    uint8_t getDDR() const _INLINE_ { return DDR(p); }
    void setDDR(uint8_t v) _INLINE_ { DDR(p) = v; }
    void setBitOutput(uint8_t i) _INLINE_ { DDR(p) |= _BV(i); }
    void setBitInput(uint8_t i) _INLINE_ { DDR(p) &= ~_BV(i); }
    bool isBitOutput(uint8_t i) const _INLINE_ { return DDR(p) & _BV(i); }
    volatile uint8_t& getDDRAddress() _INLINE_ { return DDR(p); }
    
    uint8_t port() const _INLINE_ { return PORT(p); }
    void setPort(uint8_t v) _INLINE_ { PORT(p) = v; }
    bool isPortBit(uint8_t i) const _INLINE_ { return PORT(p) & _BV(i); }
    void setPortBit(uint8_t i) _INLINE_ { PORT(p) |= _BV(i); }
    void clearPortBit(uint8_t i) _INLINE_ { PORT(p) &= ~_BV(i); }
    volatile uint8_t& getPortAddress() _INLINE_ { return PORT(p); }

    uint8_t pin() const _INLINE_ { return PIN(p); }
    bool isPinBit(uint8_t i) const _INLINE_ { return PIN(p) & _BV(i); }
    volatile uint8_t& getPinAddress() _INLINE_ { return PIN(p); }
};

// Template for 1 bit output port value
template <uint8_t p, uint8_t bit>
class OutputBit : Port<p> {
public:
    OutputBit() _INLINE_ { Port<p>::setBitOutput(bit); }
    
    OutputBit& operator=(bool b) _INLINE_ { if (b) Port<p>::setPortBit(bit); else Port<p>::clearPortBit(bit); return *this; }
    operator bool() _INLINE_ { return Port<p>::isPortBit(bit); }
};
    
// Template for 1 bit input port value
template <uint8_t p, uint8_t bit>
class InputBit : Port<p> {
public:
    InputBit() _INLINE_ { Port<p>::setBitInput<p>(bit); }
    
    operator bool() _INLINE_ { return Port<p>::isPinBit<p>(bit); }
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
    void setMaskedBits(uint8_t v, uint8_t m) _INLINE_ { REG8(reg) &= ~m; REG8(reg) |= v & m; }
    volatile uint8_t& getAddress() _INLINE_ { return REG8(reg); }
};

template <uint8_t reg>
class Reg16 {
public:
    uint16_t get() const _INLINE_ { return REG16(reg); }
    void set(uint16_t v) _INLINE_ { REG16(reg) = v; }
    volatile uint8_t& getAddress() _INLINE_ { return REG16(reg); }
};

inline uint16_t makeUInt(uint8_t c1, uint8_t c2)
{
    union {
        uint8_t a[2];
        uint16_t uint;
    } u;
    
    u.a[0] = c2;
    u.a[1] = c1;
    return u.uint;
}
#endif

#ifdef DEBUG
#ifdef __cplusplus
extern "C" {
#endif
    void _showErrorCondition(char c, uint16_t code, enum ErrorConditionType type);
#ifdef __cplusplus
}
#endif

#define ASSERT(expr, code) if (!(expr)) FATAL(code)
#define FATAL(code) _showErrorCondition(0, code, ErrorConditionFatal)
#define WARNING(code) _showErrorCondition(0, code, ErrorConditionWarning)
#ifdef __cplusplus
inline void NOTE(uint16_t code) { _showErrorCondition(0, code, ErrorConditionNote); }
inline void NOTE(uint8_t code1, uint8_t code2) { _showErrorCondition(0, makeUInt(code1, code2), ErrorConditionNote); }
inline void CNOTE(char c, uint8_t code) { _showErrorCondition(c, code, ErrorConditionNote); }
#else
#define NOTE(code) _showErrorCondition(0, code, ErrorConditionNote)
#define CNOTE(c, code) _showErrorCondition(c, code, ErrorConditionNote)
#endif
#else
#define ASSERT(expr, code)
#define FATAL(code) while(1)
#define WARNING(code)
#ifdef __cplusplus
inline void NOTE(uint16_t code) { }
inline void NOTE(uint8_t code1, uint8_t code2) { }
inline void CNOTE(char c, uint8_t code) { }
#else
#define NOTE(code)
#define CNOTE(c, code)
#endif
#endif

#ifdef __cplusplus
}
#endif
