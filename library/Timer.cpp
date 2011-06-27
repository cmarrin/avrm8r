//
//  Timer.cpp
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

#include "m8r/Timer.h"

#include "m8r/Event.h"
#include <avr/interrupt.h>

using namespace m8r;

Timer0* Timer0::m_shared = 0;
Timer1* Timer1::m_shared = 0;
Timer2* Timer2::m_shared = 0;

Timer0::Timer0(EventListener* listener)
    : m_outputCmpMatchAEvent(listener, EV_TIMER0_COMPA)
    , m_outputCmpMatchBEvent(listener, EV_TIMER0_COMPB)
    , m_overflowEvent(listener, EV_TIMER0_OVF)
{
    ASSERT(!m_shared, AssertSglTimer0);
    m_shared = this;
}

Timer1::Timer1(EventListener* listener)
    : m_outputCmpMatchAEvent(listener, EV_TIMER1_COMPA)
    , m_outputCmpMatchBEvent(listener, EV_TIMER1_COMPB)
    , m_overflowEvent(listener, EV_TIMER1_OVF)
    , m_inputCapEvent(listener, EV_TIMER1_CAPT)
{
    ASSERT(!m_shared, AssertSglTimer1);
    m_shared = this;
}

Timer2::Timer2(EventListener* listener)
    : m_outputCmpMatchAEvent(listener, EV_TIMER2_COMPA)
    , m_outputCmpMatchBEvent(listener, EV_TIMER2_COMPB)
    , m_overflowEvent(listener, EV_TIMER2_OVF)
{
    ASSERT(!m_shared, AssertSglTimer2);
    m_shared = this;
}

// Interrupt handlers
ISR(TIMER0_OVF_vect)
{
	Timer0::shared()->handleOverflowIrpt(Timer0::shared());
}

ISR(TIMER0_COMPA_vect)
{
	Timer0::shared()->handleOutputCmpMatchAIrpt(Timer0::shared());
}

ISR(TIMER0_COMPB_vect)
{
	Timer0::shared()->handleOutputCmpMatchBIrpt(Timer0::shared());
}

ISR(TIMER1_OVF_vect)
{
	Timer1::shared()->handleOverflowIrpt(Timer1::shared());
}

ISR(TIMER1_COMPA_vect)
{
	Timer1::shared()->handleOutputCmpMatchAIrpt(Timer1::shared());
}

ISR(TIMER1_COMPB_vect)
{
	Timer1::shared()->handleOutputCmpMatchBIrpt(Timer1::shared());
}

ISR(TIMER1_CAPT_vect)
{
	Timer1::shared()->handleInputCapIrpt(Timer1::shared());
}

ISR(TIMER2_OVF_vect)
{
	Timer2::shared()->handleOverflowIrpt(Timer2::shared());
}

ISR(TIMER2_COMPA_vect)
{
	Timer2::shared()->handleOutputCmpMatchAIrpt(Timer2::shared());
}

ISR(TIMER2_COMPB_vect)
{
	Timer2::shared()->handleOutputCmpMatchBIrpt(Timer2::shared());
}

