//
//  Timer1.h
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

#include "EventSource.h"

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Timer1
//
//  Interface to 16 bit Timer/Counter Timer 1
//
//////////////////////////////////////////////////////////////////////////////
	
class Timer1 : public EventSource {
public:
	Timer1(uint8_t prescaler, uint16_t initial)
    {
        // initialize timer 1
        setPrescaler(prescaler);	// set prescaler
        TCNT1 = initial;            // reset TCNT1
        mygTimer1 = this;
    }
    
	~Timer1()                       { }
    
    static Timer1* getTimer1()      { return mygTimer1; }
    
    void setInterruptEnable(uint8_t irpt, bool b);
    bool getInterruptEnable(uint8_t irpt) const;
        
    void setPrescaler(uint8_t v)
    {
        if ((v & ~TIMER_PRESCALE_MASK) != 0)
            v = 0;
        TCCR1B = (TCCR1B & ~TIMER_PRESCALE_MASK) | v;
    }
    
    uint8_t getPrescaler() const        { return TCCR1B & TIMER_PRESCALE_MASK; }

    void setOutputCompare(uint16_t v)   { OCR1A = v; }
    uint16_t getOutputCompare() const   { return OCR1A; }
    
    void setValue(uint16_t v)           { TCNT1 = v; }
    uint16_t getValue() const           { return TCNT1; }
    
    uint16_t getInputCapture() const    { return ICR1; }
    
    void setClearOnOutputCompare(bool b)
    {
        // FIXME: This is different on modern controllers
        if (b)
            TCCR1B |= _BV(WGM10);
        else
            TCCR1B &= ~_BV(WGM10);
    }
    
    // FIXME: This is different on modern controllers
    bool isClearOnOutputCompare() const  { return (TCCR1B & _BV(WGM10)) != 0; }

private:
    static Timer1* mygTimer1;
};

}
