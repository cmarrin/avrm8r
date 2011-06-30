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

#include "m8r/Timer.h"

#include "m8r/Event.h"
#include <avr/interrupt.h>

using namespace m8r;


Event* Timer0::m_outputCmpMatchAEvent = 0;
Event* Timer0::m_outputCmpMatchBEvent = 0;
Event* Timer0::m_overflowEvent = 0;

Event* Timer1::m_outputCmpMatchAEvent = 0;
Event* Timer1::m_outputCmpMatchBEvent = 0;
Event* Timer1::m_overflowEvent = 0;
Event* Timer1::m_inputCapEvent = 0;

Event* Timer2::m_outputCmpMatchAEvent = 0;
Event* Timer2::m_outputCmpMatchBEvent = 0;
Event* Timer2::m_overflowEvent = 0;

Timer0::Timer0(EventListener* listener)
{
    ASSERT(!m_outputCmpMatchAEvent, AssertSglTimer0Event);
    m_outputCmpMatchAEvent = new Event(listener, EV_TIMER0_COMPA);
    
    ASSERT(!m_outputCmpMatchBEvent, AssertSglTimer0Event);
    m_outputCmpMatchBEvent = new Event(listener, EV_TIMER0_COMPB);
    
    ASSERT(!m_overflowEvent, AssertSglTimer0Event);
    m_overflowEvent = new Event(listener, EV_TIMER0_OVF);
}

Timer1::Timer1(EventListener* listener)
{
    ASSERT(!m_outputCmpMatchAEvent, AssertSglTimer1Event);
    m_outputCmpMatchAEvent = new Event(listener, EV_TIMER1_COMPA);
    
    ASSERT(!m_outputCmpMatchBEvent, AssertSglTimer1Event);
    m_outputCmpMatchBEvent = new Event(listener, EV_TIMER1_COMPB);
    
    ASSERT(!m_overflowEvent, AssertSglTimer1Event);
    m_overflowEvent = new Event(listener, EV_TIMER1_OVF);
    
    ASSERT(!m_inputCapEvent, AssertSglTimer1Event);
    m_inputCapEvent = new Event(listener, EV_TIMER1_CAPT);
}

Timer2::Timer2(EventListener* listener)
{
    ASSERT(!m_outputCmpMatchAEvent, AssertSglTimer2Event);
    m_outputCmpMatchAEvent = new Event(listener, EV_TIMER2_COMPA);
    
    ASSERT(!m_outputCmpMatchBEvent, AssertSglTimer2Event);
    m_outputCmpMatchBEvent = new Event(listener, EV_TIMER2_COMPB);
    
    ASSERT(!m_overflowEvent, AssertSglTimer2Event);
    m_overflowEvent = new Event(listener, EV_TIMER2_OVF);
}

// Interrupt handlers
ISR(TIMER0_OVF_vect)
{
    ASSERT(Timer0::m_overflowEvent, AssertNoTimer0Event);
    Application::addEvent(Timer0::m_overflowEvent);
}

ISR(TIMER0_COMPA_vect)
{
    ASSERT(Timer0::m_outputCmpMatchAEvent, AssertNoTimer0Event);
    Application::addEvent(Timer0::m_outputCmpMatchAEvent);
}

ISR(TIMER0_COMPB_vect)
{
    ASSERT(Timer0::m_outputCmpMatchBEvent, AssertNoTimer0Event);
    Application::addEvent(Timer0::m_outputCmpMatchBEvent);
}

ISR(TIMER1_OVF_vect)
{
    ASSERT(Timer1::m_overflowEvent, AssertNoTimer1Event);
    Application::addEvent(Timer1::m_overflowEvent);
}

ISR(TIMER1_COMPA_vect)
{
    ASSERT(Timer1::m_outputCmpMatchAEvent, AssertNoTimer1Event);
    Application::addEvent(Timer1::m_outputCmpMatchAEvent);
}

ISR(TIMER1_COMPB_vect)
{
    ASSERT(Timer1::m_outputCmpMatchBEvent, AssertNoTimer1Event);
    Application::addEvent(Timer1::m_outputCmpMatchBEvent);
}

ISR(TIMER1_CAPT_vect)
{
    ASSERT(Timer1::m_inputCapEvent, AssertNoTimer1Event);
    Application::addEvent(Timer1::m_inputCapEvent);
}

ISR(TIMER2_OVF_vect)
{
    ASSERT(Timer2::m_overflowEvent, AssertNoTimer2Event);
    Application::addEvent(Timer2::m_overflowEvent);
}

ISR(TIMER2_COMPA_vect)
{
    ASSERT(Timer2::m_outputCmpMatchAEvent, AssertNoTimer2Event);
    Application::addEvent(Timer2::m_outputCmpMatchAEvent);
}

ISR(TIMER2_COMPB_vect)
{
    ASSERT(Timer2::m_outputCmpMatchBEvent, AssertNoTimer2Event);
    Application::addEvent(Timer2::m_outputCmpMatchBEvent);
}

