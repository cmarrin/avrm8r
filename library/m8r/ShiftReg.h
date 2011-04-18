//
//  ShiftReg.h
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

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: ShiftReg
//
//  Generic shift register interface. Has variable bit length, and is 
//  controlled by a clock and data-in bit. Clock can use leading or
//  trailing edge.
//
//////////////////////////////////////////////////////////////////////////////

class ShiftRegBase {
protected:
    void send(volatile uint8_t& clk, uint8_t clkBit, volatile uint8_t& data, uint8_t dataBit, 
              uint8_t v, uint8_t n, bool rising, bool msbFirst);
};

template <class ClockPort, uint8_t ClockBit, class DataPort, uint8_t DataBit>
class ShiftReg : ShiftRegBase {
public:
	ShiftReg(bool rising, bool msbFirst)
    : m_rising(rising)
    , m_msbFirst(msbFirst)
    {
        m_clockPort.setDDRMask(_BV(ClockBit));
        m_dataPort.setDDRMask(_BV(DataBit));
        if (m_rising)
            m_clockPort.clearPortMask(_BV(ClockBit));
        else
            m_clockPort.setPortMask(_BV(ClockBit));
    }
    
    void clear()    { send(0); }
    
    void send(uint8_t v, uint8_t n)
    {        
        for (uint8_t mask = m_msbFirst ? 0x80 : 1; n > 0; --n) {
            // set data bit
            uint8_t tmp = ((uint8_t) v) & ((uint8_t) mask);
            if (tmp)
                m_dataPort.setPortMask(_BV(DataBit));
            else
                m_dataPort.clearPortMask(_BV(DataBit));

            // clock in data
            if (m_rising) {
                m_clockPort.setPortMask(_BV(ClockBit));
                m_clockPort.clearPortMask(_BV(ClockBit));
            }
            else {
                m_clockPort.clearPortMask(_BV(ClockBit));
                m_clockPort.setPortMask(_BV(ClockBit));
            }

            mask = m_msbFirst ? (mask >> 1) : (mask << 1);
        }
    }
    	
private:    
    ClockPort m_clockPort;
    DataPort m_dataPort;
	bool m_rising, m_msbFirst;
};

}