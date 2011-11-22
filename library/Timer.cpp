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

#include "m8r/Application.h"
#include <avr/interrupt.h>

using namespace m8r;

ISRCallback Timer0::m_isrCallback = 0;
void* Timer0::m_data = 0;
ISRCallback Timer1::m_isrCallback = 0;
void* Timer1::m_data = 0;
ISRCallback Timer2::m_isrCallback = 0;
void* Timer2::m_data = 0;

// Interrupt handlers
ISR(TIMER0_OVF_vect)
{
    Timer0::m_isrCallback(EV_TIMER0_OVF, Timer0::m_data);
}

ISR(TIMER0_COMPA_vect)
{
    Timer0::m_isrCallback(EV_TIMER0_COMPA, Timer0::m_data);
}

ISR(TIMER0_COMPB_vect)
{
    Timer0::m_isrCallback(EV_TIMER0_COMPB, Timer0::m_data);
}

ISR(TIMER1_OVF_vect)
{
    Timer1::m_isrCallback(EV_TIMER1_OVF, Timer1::m_data);
}

ISR(TIMER1_COMPA_vect)
{
    Timer1::m_isrCallback(EV_TIMER1_COMPA, Timer1::m_data);
}

ISR(TIMER1_COMPB_vect)
{
    Timer1::m_isrCallback(EV_TIMER1_COMPB, Timer1::m_data);
}

ISR(TIMER1_CAPT_vect)
{
    Timer1::m_isrCallback(EV_TIMER1_CAPT, Timer1::m_data);
}

ISR(TIMER2_OVF_vect)
{
    Timer2::m_isrCallback(EV_TIMER2_OVF, Timer2::m_data);
}

ISR(TIMER2_COMPA_vect)
{
    Timer2::m_isrCallback(EV_TIMER2_COMPA, Timer2::m_data);
}

ISR(TIMER2_COMPB_vect)
{
    Timer2::m_isrCallback(EV_TIMER2_COMPB, Timer2::m_data);
}

