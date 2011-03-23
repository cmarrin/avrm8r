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

#include "avrEventSource.h"

namespace avr {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Timer0
//
//  Interface to 8 bit Timer/Counter Timer 0
//
//////////////////////////////////////////////////////////////////////////////
	
class Timer0 : public EventSource {
public:
	Timer0(uint8_t prescaler, uint8_t initial)
    {
        // initialize timer 1
        setPrescaler(prescaler);	// set prescaler
        TCNT0 = initial;			// reset TCNT1
        setInterruptEnable(TIMER_OVERFLOW, true);
        mygTimer0 = this;
    }
    
	~Timer0()                       { }
    
    static Timer0* getTimer0()      { return mygTimer0; }
    
    void setInterruptEnable(uint8_t irpt, bool b);
    bool getInterruptEnable(uint8_t irpt) const;
        
    void setPrescaler(uint8_t v)
    {
        if ((v & ~TIMER_PRESCALE_MASK) != 0)
            v = 0;
        TCCR0 = (TCCR0 & ~TIMER_PRESCALE_MASK) | v;
    }
    
    uint8_t getPrescaler() const        { return TCCR0 & TIMER_PRESCALE_MASK; }

    void setValue(uint8_t v)            { TCNT0 = v; }
    uint16_t getValue() const           { return TCNT0; }
    
private:
    static Timer0* mygTimer0;
};

}
