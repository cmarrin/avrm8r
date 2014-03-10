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
#include <avr/pgmspace.h>

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
    
    AssertSingleUSART0 = 0x40,
    ErrorUser = 0x80,
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
#ifdef UCSR0A
        _UCSR0A = 0xC0,
#endif
#ifdef UCSR0B
        _UCSR0B = 0xC1,
#endif
#ifdef UCSR0C
        _UCSR0C = 0xC2,
#endif
#ifdef UBRR0
        _UBRR0 = 0xC4,
#endif
#ifdef UDR0
        _UDR0 = 0xC6,
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
class Port
{
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

class NullPort : Port<0>
{
public:
    uint8_t getDDR() const _INLINE_ { return 0; }
    void setDDR(uint8_t v) _INLINE_ { }
    void setBitOutput(uint8_t i) _INLINE_ { }
    void setBitInput(uint8_t i) _INLINE_ { }
    bool isBitOutput(uint8_t i) const _INLINE_ { return 0; }
    uint8_t getDDRAddress() _INLINE_ { return 0; }
    
    uint8_t port() const _INLINE_ { return 0; }
    void setPort(uint8_t v) _INLINE_ { }
    bool isPortBit(uint8_t i) const _INLINE_ { return false; }
    void setPortBit(uint8_t i) _INLINE_ { }
    void clearPortBit(uint8_t i) _INLINE_ { }
    uint8_t getPortAddress() _INLINE_ { return 0; }

    uint8_t pin() const _INLINE_ { return 0; }
    bool isPinBit(uint8_t i) const _INLINE_ { return false; }
    uint8_t getPinAddress() _INLINE_ { return 0; }
};

// Template for 1 bit output port value
template <class P, uint8_t bit>
class OutputBit
{
public:
    OutputBit() _INLINE_ { _port.setBitOutput(bit); }
    
    OutputBit& operator=(bool b) _INLINE_ { if (b) _port.setPortBit(bit); else _port.clearPortBit(bit); return *this; }
    operator bool() _INLINE_ { return _port.isPortBit(bit); }

private:
    P _port;
};
    
class NullOutputBit
{
public:
    NullOutputBit() _INLINE_ { }
    
    NullOutputBit& operator=(bool b) _INLINE_ { return *this; }
    operator bool() _INLINE_ { return false; }
    
    bool isNull() const { return true; }
};
    
// Template for 1 bit input port value
template <class port, uint8_t bit>
class InputBit
{
public:
    InputBit() _INLINE_ { port::setBitInput(bit); }
    
    operator bool() _INLINE_ { return port::isPinBit(bit); }
};
    
class NullInputBit
{
public:
    NullInputBit() _INLINE_ {  }
    
    operator bool() _INLINE_ { return false; }
};
    
// Template for 8 bit registers
template <uint8_t reg>
class Reg8
{
public:
    Reg8& operator=(uint8_t v) _INLINE_ { REG8(reg) = v; return *this; }
    Reg8& operator|=(uint8_t v) _INLINE_ { REG8(reg) |= v; return *this; }
    Reg8& operator&=(uint8_t v) _INLINE_ { REG8(reg) &= v; return *this; }
    operator uint8_t() _INLINE_ { return REG8(reg); }
        
    void setBitMask(uint8_t m, bool b) _INLINE_
    {
        if (b)
            REG8(reg) |= m;
        else
            REG8(reg) &= ~m;
    }
    bool isBitMaskSet(uint8_t m) const _INLINE_ { return (REG8(reg) & m) != 0; }
    void setMaskedBits(uint8_t v, uint8_t m) _INLINE_ { REG8(reg) &= ~m; REG8(reg) |= v & m; }
};

class NullReg8 : Reg8<0>
{
public:
    Reg8& operator=(uint8_t) _INLINE_ { return *this; }
    Reg8& operator|=(uint8_t) _INLINE_ { return *this; }
    Reg8& operator&=(uint8_t) _INLINE_ { return *this; }
    operator uint8_t() _INLINE_ { return 0; }
        
    void setBitMask(uint8_t m, bool b) _INLINE_ { }
    bool isBitMaskSet(uint8_t m) const _INLINE_ { return false; }
    void setMaskedBits(uint8_t v, uint8_t m) _INLINE_ { }
};

template <uint8_t reg>
class Reg16
{
public:
    Reg16& operator=(uint16_t v) _INLINE_ { REG16(reg) = v; return *this; }
    operator uint16_t() _INLINE_ { return REG16(reg); }
};

class NullReg16 : Reg16<0>
{
public:
    Reg16& operator=(uint16_t v) _INLINE_ { return *this; }
    operator uint16_t() _INLINE_ { return 0; }
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

class _FlashString
{
public:
    _FlashString(const char* s) : _s(s) { }
    const char* _s;
};

#define FS(str) (_FlashString(PSTR(str)))

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

