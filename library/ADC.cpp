//
//  ADC.cpp
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

#include "m8r/ADC.h"
#include "m8r/Application.h"

using namespace m8r;

// FIXME: Add handling of ADC channel

ADC::ADC(uint8_t prescale, uint8_t reference)
{
    ADCSRA |= _BV(ADEN);             // enable ADC (turn on ADC power)
    
#ifdef ADFR
    ADCSRA &= ~_BV(ADFR);            // default to single sample convert mode
#else
    ADCSRA &= ~_BV(ADATE);          // default to no auto trigger
#endif
    setPrescaler(prescale);         // set default prescaler
    setReference(reference);        // set default reference
    
    //ADCSR |= _BV(ADIE);               // enable ADC interrupts
    setChannel(0);
}

uint16_t
ADC::convert10Bit()
{
    ADCSRA |= _BV(ADIF);                     // clear hardware "conversion complete" flag 
    ADCSRA |= _BV(ADSC);                     // start conversion
    
    // wait for completion
    while(bit_is_set(ADCSRA, ADSC))
    ;

    return ADCW;
}

// Interrupt handler for ADC complete interrupt.
ISR(SIG_ADC)
{
    ADCSRA &= ~_BV(ADIE);    // disable ADC interrupts
    Application::application()->addEvent(EV_ADC);
}


