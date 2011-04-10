//
//  STP08CDC596.h
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

#include "m8r.h"
#include "ShiftReg.h"

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: STP08CDC596
//
//  8 bit constant current LED driver.
//
//////////////////////////////////////////////////////////////////////////////
	
class STP08CDC596 : public ShiftReg {
public:
	STP08CDC596(uint8_t clkPort, uint8_t clkBit, uint8_t dataPort, uint8_t dataBit, 
                uint8_t latchPort, uint8_t latchBit, uint8_t enaPort, uint8_t enaBit)
    : ShiftReg(clkPort, clkBit, dataPort, dataBit, 8, true, true)
    , myLatchPort(getPort(latchPort))
    , myLatchDDR(getDDR(latchPort))
    , myLatchMask(1<<latchBit)
    , myEnaPort(getPort(enaPort))
    , myEnaDDR(getDDR(enaPort))
    , myEnaMask(1<<enaBit)
    {
        *myLatchDDR |= myLatchMask;
        *myEnaDDR |= myEnaMask;
        *myLatchPort &= ~myLatchMask;   // latch is active on the rising edge
        *myEnaPort |= myEnaMask;        // enable is active low
    }
    
	~STP08CDC596()		{ }
    
    void setOutputEnable(bool e)
    {
        if (e) {
            *myLatchPort |= myLatchMask;
            *myLatchPort &= ~myLatchMask;
            *myEnaPort &= ~myEnaMask;
        }
        else
            *myEnaPort |= myEnaMask;
    }
    
    void setChar(uint8_t c);
    
private:
    volatile uint8_t*   myLatchPort;
    volatile uint8_t*   myLatchDDR;
    uint8_t             myLatchMask;
    volatile uint8_t*   myEnaPort;
    volatile uint8_t*   myEnaDDR;
    uint8_t             myEnaMask;
};

}
