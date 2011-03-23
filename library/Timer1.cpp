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


#include "avrTimer1.h"
#include "avrApplication.h"
#include <avr/interrupt.h>

using namespace avr;

Timer1* Timer1::mygTimer1 = 0;

void
Timer1::setInterruptEnable(uint8_t irpt, bool b)
{
    uint8_t bit = 0;
    if (irpt == TIMER_COMPAREA)
        bit = _BV(OCIE1);
    else if (irpt == TIMER_OVERFLOW)
        bit = _BV(TOIE1);
    else if (irpt == TIMER_INPUT_CAPTURE)
        bit = _BV(TICIE1);
    TIMSK = b ? (TIMSK | bit) : (TIMSK & ~bit);
}

bool
Timer1::getInterruptEnable(uint8_t irpt) const
{
    uint8_t bit = 0;
    if (irpt == TIMER_COMPAREA)
        bit = _BV(OCIE1);
    else if (irpt == TIMER_OVERFLOW)
        bit = _BV(TOIE1);
    else if (irpt == TIMER_INPUT_CAPTURE)
        bit = _BV(TICIE1);
    return (TIMSK & bit) != 0;
}

// Interrupt handlers
SIGNAL(SIG_OVERFLOW1)
{
	avr::Application::getApplication()->addEvent(EV_TIMER1);
}

SIGNAL(SIG_OUTPUT_COMPARE1A)
{
	avr::Application::getApplication()->addEvent(EV_TIMER1);
}

SIGNAL(SIG_INPUT_CAPTURE1)
{
	avr::Application::getApplication()->addEvent(EV_TIMER1);
}

