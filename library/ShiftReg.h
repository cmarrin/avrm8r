/*-------------------------------------------------------------------------
This source file is a part of Fusion

For the latest info, see http://www.emma3d.org/

Copyright (c) 2006, Murat Aktihanoglu, Chris Marrin, Rob Myers
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

    - Redistributions of source code must retain the above copyright notice, 
	  this list of conditions and the following disclaimer.
	  
    - Redistributions in binary form must reproduce the above copyright 
	  notice, this list of conditions and the following disclaimer in the 
	  documentation and/or other materials provided with the distribution.
	  
    - Neither the name of the <ORGANIZATION> nor the names of its 
	  contributors may be used to endorse or promote products derived from 
	  this software without specific prior written permission.
	  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------*/

#pragma once

#include "marrinator.h"

namespace marrinator {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: ShiftReg
//
//  Generic shift register interface. Has variable bit length, and is 
//  controlled by a clock and data-in bit. Clock can use leading or
//  trailing edge.
//
//////////////////////////////////////////////////////////////////////////////
	
class ShiftReg {
public:
	ShiftReg(uint8_t clkPort, uint8_t clkBit, uint8_t dataPort, uint8_t dataBit, uint8_t bits, bool rising, bool msbFirst)
    : myClkPort(getPort(clkPort))
    , myClkDDR(getDDR(clkPort))
    , myClkMask(1<<clkBit)
    , myDataPort(getPort(dataPort))
    , myDataDDR(getDDR(dataPort))
    , myDataMask(1<<dataBit)
    , myBits(bits)
    , myRising(rising)
    , myMSBFirst(msbFirst)
    {
        *myClkDDR |= myClkMask;
        *myDataDDR |= myDataMask;
        if (myRising)
            *myClkPort &= ~myClkMask;
        else
            *myClkPort |= myClkMask;
    }
    
	~ShiftReg()		{ }
    
    void clear()    { send(0); }
    
    void send(uint8_t value, uint8_t n = 0);
    	
private:
    volatile uint8_t*   myClkPort;
    volatile uint8_t*   myClkDDR;
    uint8_t             myClkMask;
    volatile uint8_t*   myDataPort;
    volatile uint8_t*   myDataDDR;
    uint8_t             myDataMask;
    uint8_t             myBits;
	bool                myRising, myMSBFirst;
};

}
