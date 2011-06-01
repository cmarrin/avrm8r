//
//  MAX6969.h
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
#include "m8r/ShiftReg.h"

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: MAX6969
//
//  16 bit constant current LED driver.
//
//////////////////////////////////////////////////////////////////////////////

class MAX6969Base {
protected:
    uint8_t patternFromChar(uint8_t c) const;
};

template <class ClockPort, uint8_t ClockBit, class DataPort, uint8_t DataBit,
          class LatchPort, uint8_t LatchBit, class EnablePort, uint8_t EnableBit>
class MAX6969 : public ShiftReg<ClockPort, ClockBit, DataPort, DataBit>, public MAX6969Base {
public:
    MAX6969()
    : ShiftReg<ClockPort, ClockBit, DataPort, DataBit>(true, true)
    {
        // latch is active on the rising edge
        m_latchPort.clearPortBit(LatchBit);
        m_latchPort.setBitOutput(LatchBit);

        // enable is active low
        m_enablePort.setPortBit(EnableBit);
        m_enablePort.setBitOutput(EnableBit);
        
        ShiftReg<ClockPort, ClockBit, DataPort, DataBit>::send(0, 8);
        ShiftReg<ClockPort, ClockBit, DataPort, DataBit>::send(0, 8);
    }

    void setOutputEnable(bool e)
    {
        if (e)
            m_enablePort.clearPortBit(EnableBit);
        else
            m_enablePort.setPortBit(EnableBit);
    }
    
    void latch()
    {
        m_latchPort.setPortBit(LatchBit);
        m_latchPort.clearPortBit(LatchBit);
    }
    
    void setChar(uint8_t c)
    {
        send(patternFromChar(c), 8);
    }
    
private:
    LatchPort m_latchPort;
    EnablePort m_enablePort;
};

}
