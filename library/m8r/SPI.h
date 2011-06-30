//
//  SPI.h
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

#pragma once

#include "m8r.h"
#include <util/delay_basic.h>

// Sadly, several of the mega parts don't define the SPI values.
// Define them here for mega88, mega168, mega328, and mega644
#ifndef MOSI_DDR
#define MOSI_PORT PORTB
#define MOSI_DDR DDRB

#if defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) 
#define SS_BIT 2
#define MISO_BIT 4
#define MOSI_BIT 3
#define SCLK_BIT 5
#endif
#if defined(__AVR_ATmega644__)||defined(__AVR_ATmega644P__)
#define SS_BIT 4
#define MISO_BIT 6
#define MOSI_BIT 5
#define SCLK_BIT 7
#endif
#endif

// This assumes all SPI bits are on the same port
#define SPI_PORT MOSI_PORT
#define SPI_DDR MOSI_DDR

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SPI
//
//  SPI interface.
//
//////////////////////////////////////////////////////////////////////////////

template <uint8_t spcr, uint8_t spsr>
class SPI {
public:
	SPI()
    {
        SPI_DDR |= _BV(SS_BIT) | _BV(SCLK_BIT) | _BV(MOSI_BIT);
        SPI_PORT |= _BV(SCLK_BIT) | _BV(MOSI_BIT);
        SPCR = _BV(SPE) | spcr;
        SPSR |= spsr;
    }
    
    void send(uint8_t v, uint8_t n)
    {        
    }
    	
private:
};

}
