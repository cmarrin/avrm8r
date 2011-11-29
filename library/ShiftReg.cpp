//
//  ShiftReg.cpp
//
//  Created by Chris Marrin on 3/19/2011.

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

#include "ShiftReg.h"

#include <avr/pgmspace.h>

extern const uint8_t g_charTable[] PROGMEM;
const uint8_t g_charTable[] =
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
extern const uint8_t g_numTable[] PROGMEM;
const uint8_t g_numTable[] = 
    { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f };

using namespace m8r;

uint8_t
ShiftRegBase::patternFromChar(uint8_t c) const
{
    if (c < '0')
        return 0;
    if (c <= '9')
        return pgm_read_byte_near(&(g_numTable[c-'0']));
    if (c < 'A')
        return 0;
    if (c <= 'Z')
        return pgm_read_byte_near(&(g_charTable[c-'A']));
    if (c < 'a')
        return 0;
    if (c <= 'z')
        return pgm_read_byte_near(&(g_charTable[c-'a']));
    return 0;
}

void
ShiftRegBase::send(uint8_t v, uint8_t n)
{        
    for (uint8_t mask = m_msbFirst ? 0x80 : 1; n > 0; --n) {
        // set data bit
        uint8_t tmp = ((uint8_t) v) & ((uint8_t) mask);
        if (tmp)
            setDataBit();
        else
            clearDataBit();

        // clock in data
        if (m_rising) {
            setClockBit();
            clearClockBit();
        }
        else {
            clearClockBit();
            setClockBit();
        }

        mask = m_msbFirst ? (mask >> 1) : (mask << 1);
    }
}
    	
