//
//  marrinator.h
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

namespace m8r {

// Fixed point floats
//
// Format is <sign>:<integer part>:<fractional part>
// 6 formats supported - 1:0:7, 1:3:4, 1:0:15, 1:7:8, 1:0:31, 1:15:16
// Formats with 0 bits of integer part have 2 special values. All fractional bits 1 is a value of 1.
typedef uint8_t fixed_8; // 1:3:4
typedef uint8_t fixed0_8; // 1:0:7
typedef uint16_t fixed_16; // 1:7:8
typedef uint16_t fixed0_16; // 1:0:15
typedef uint32_t fixed_32; // 1:15:16
typedef uint32_t fixed0_32; // 1:0:31

inline fixed_8 makeFixed8(int8_t i, uint8_t f)
{
    return (i < -7 || i > 7) ? 0 : ((i << 4) | (f & 0xf));
}

/*
// 32 bit integers representing the value multiplied by 1000,
// give 3 decimal digits of floating point precision.
#define FPF int32_t
#define FPF_ADD(a,b)    (a+b)
#define FPF_SUB(a,b)    (a-b)
#define FPF_MUL(a,b)    (a*b/1000)
#define FPF_DIV(a,b)    (a*1000/b)
inline FPF FPF_MAKE(uint32_t i, uint32_t f, uint8_t dp, uint32_t e)
{
    int32_t num = i*1000;
    while (dp < 3) {
        ++dp;
        f *= 10;
    }
    while (dp > 3) {
        --dp;
        f /= 10;
    }
    num += f;
    while (e > 0) {
        --e;
        num *= 10;
    }
    while (e < 0) {
        ++e;
        num /= 10;
    }
    return num;
}

#define FPF_TO_INT(f)   (f/1000)
#define INT_TO_FPF(i)   (i*1000)

#else

// Floats are normal IEEE floating point numbers
#define FPF float
#define FPF_ADD(a,b)    (a+b)
#define FPF_SUB(a,b)    (a-b)
#define FPF_MUL(a,b)    (a*b)
#define FPF_DIV(a,b)    (a/b)
inline FPF FPF_MAKE(uint32_t i, uint32_t f, uint8_t dp, uint32_t e)
{
    float num = (float) f;
    while (dp-- > 0)
        num /= 10;
    num += (float) i;
    while (e > 0) {
        --e;
        num *= 10;
    }
    while (e < 0) {
        ++e;
        num /= 10;
    }
    return num;
}

#define FPF_TO_INT(f)   ((int32_t) f)
#define INT_TO_FPF(i)   ((float) i)
*/

// presccale values for all timers
#define TIMER_CLK_STOP          0x00    ///< Timer Stopped
#define TIMER_CLK_DIV1          0x01    ///< Timer clocked at F_CPU
#define TIMER_CLK_DIV8          0x02    ///< Timer clocked at F_CPU/8
#define TIMER_CLK_DIV64         0x03    ///< Timer clocked at F_CPU/64
#define TIMER_CLK_DIV256        0x04    ///< Timer clocked at F_CPU/256
#define TIMER_CLK_DIV1024       0x05    ///< Timer clocked at F_CPU/1024
#define TIMER_CLK_T_FALL        0x06    ///< Timer clocked at T falling edge
#define TIMER_CLK_T_RISE        0x07    ///< Timer clocked at T rising edge
#define TIMER_PRESCALE_MASK     0x07    ///< Timer Prescaler Bit-Mask

// Macro for Port classes
#define PORT_CLASS(PortId) \
class Port##PortId { \
public: \
    uint8_t getDDR() const { return DDR##PortId; } \
    void setDDR(uint8_t v) { DDR##PortId = v; } \
    bool isDDRBit(uint8_t i) const { return DDR##PortId & _BV(i); } \
    void setDDRBit(uint8_t i, bool v) { if (v) DDR##PortId |= _BV(i); else DDR##PortId &= ~_BV(i); } \
    bool isDDRMask(uint8_t m) const { return DDR##PortId & m; } \
    void setDDRMask(uint8_t m) { DDR##PortId |= m; } \
    void clearDDRMask(uint8_t m) { DDR##PortId &= ~m; } \
    volatile uint8_t& getDDRAddress() { return DDR##PortId; } \
 \
    uint8_t getPort() const { return PORT##PortId; } \
    void setPort(uint8_t v) { PORT##PortId = v; } \
    bool isPortBit(uint8_t i) const { return PORT##PortId & _BV(i); } \
    void setPortBit(uint8_t i, bool v) { if (v) PORT##PortId |= _BV(i); else PORT##PortId &= ~_BV(i); } \
    bool isPortMask(uint8_t m) const { return PORT##PortId & m; } \
    void setPortMask(uint8_t m) { PORT##PortId |= m; } \
    void clearPortMask(uint8_t m) { PORT##PortId &= ~m; } \
    volatile uint8_t& getPortAddress() { return PORT##PortId; } \
 \
    uint8_t getPin() const { return PIN##PortId; } \
    bool isPinBit(uint8_t i) const { return PIN##PortId & _BV(i); } \
    bool isPinMask(uint8_t m) const { return PIN##PortId & m; } \
    volatile uint8_t& getPinAddress() { return PIN##PortId; } \
}; \

#ifdef PORTA
PORT_CLASS(A)
#endif
#ifdef PORTB
PORT_CLASS(B)
#endif
#ifdef PORTC
PORT_CLASS(C)
#endif
#ifdef PORTD
PORT_CLASS(D)
#endif
#ifdef PORTE
PORT_CLASS(E)
#endif
#ifdef PORTF
PORT_CLASS(F)
#endif
#ifdef PORTG
PORT_CLASS(G)
#endif
#ifdef PORTH
PORT_CLASS(H)
#endif
#ifdef PORTI
PORT_CLASS(I)
#endif
#ifdef PORTJ
PORT_CLASS(J)
#endif
#ifdef PORTK
PORT_CLASS(K)
#endif
#ifdef PORTL
PORT_CLASS(L)
#endif

}
