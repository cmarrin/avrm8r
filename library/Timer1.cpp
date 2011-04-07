//
//  Timer1.cpp
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

#include "m8r/Timer1.h"
#include "m8r/Application.h"
#include <avr/interrupt.h>

using namespace m8r;

Timer1* Timer1::mygTimer1 = 0;

void
Timer1::setInterruptEnable(uint8_t irpt, bool b)
{
    // FIXME: This is different on modern controllers
    uint8_t bit = 0;
    if (irpt == TIMER_COMPAREA)
        bit = _BV(OCIE1A);
    else if (irpt == EV_TIMER1_OVF)
        bit = _BV(TOIE1);
    else if (irpt == TIMER_INPUT_CAPTURE)
        bit = _BV(TOIE1);
    TIMSK0 = b ? (TIMSK0 | bit) : (TIMSK0 & ~bit);
}

bool
Timer1::getInterruptEnable(uint8_t irpt) const
{
    // FIXME: This is different on modern controllers
    uint8_t bit = 0;
    if (irpt == TIMER_COMPAREA)
        bit = _BV(OCIE1A);
    else if (irpt == EV_TIMER1_OVF)
        bit = _BV(TOIE1);
    else if (irpt == TIMER_INPUT_CAPTURE)
        bit = _BV(TOIE1);
    return (TIMSK0 & bit) != 0;
}

// Interrupt handlers
ISR(SIG_OVERFLOW1)
{
	Application::application()->addEvent(EV_TIMER1);
}

ISR(SIG_OUTPUT_COMPARE1A)
{
	Application::application()->addEvent(EV_TIMER1);
}

ISR(SIG_INPUT_CAPTURE1)
{
	Application::application()->addEvent(EV_TIMER1);
}

