//
//  ENC28J60.cpp
//
//  Created by Chris Marrin on 6/30/2011.
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

#include "m8r/ENC28J60.h"

#define CSACTIVE SPI_PORT &= ~_BV(SS_BIT)
#define CSPASSIVE SPI_PORT |= _BV(SS_BIT)
#define waitspi() while(!(SPSR & _BV(SPIF)))

using namespace m8r;

ENC28J60Base::ENC28J60Base()
{
}

void
ENC28J60Base::writeOp(uint8_t op, uint8_t address, uint8_t data)
{
    CSACTIVE;
    SPDR = op | (address & ADDR_MASK);
    waitspi();

    SPDR = data;
    waitspi();
    CSPASSIVE;
}

void
ENC28J60Base::setBank(uint8_t address)
{
    if ((address & BANK_MASK) != m_bank) {
        writeOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_BSEL1 | ECON1_BSEL0);
        writeOp(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK) >> 5);
        m_bank = (address & BANK_MASK);
    }
}

