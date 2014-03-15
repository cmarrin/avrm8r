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

#include "ADC.h"
#include "System.h"

using namespace m8r;

uint16_t ADC::m_lastConversion = 0;

ADC::ADC(uint8_t channel, uint8_t prescale, uint8_t reference)
{
    // Set prescaler
    ADCSRA &= ~ADC_PS_MASK;
    ADCSRA |= prescale;
    
    // Set reference
    ADMUX &= ~ADC_REF_MASK;
    ADMUX |= reference;

    // Set channel
    ADMUX &= ~ADC_CH_MASK;
    ADMUX |= channel;
}

void
ADC::setEnabled(bool e)
{
    if (e) {
        ADCSRA |= _BV(ADIE); // enable ADC interrupts
        ADCSRA |= _BV(ADEN); // enable ADC (turn on ADC power)
    }
    else {
        ADCSRA &= ~_BV(ADIE); // disable ADC interrupts
        ADCSRA &= ~_BV(ADEN); // disable ADC (turn off ADC power)
    }
}

void
ADC::setPrescaler(uint8_t prescale)
{
    if (prescale & ADC_PS_MASK)
        prescale = ADC_PS_DIV128;
    
    ADCSRA &= ~ADC_PS_MASK;
    ADCSRA |= prescale;
}

void
ADC::setReference(uint8_t ref)
{
    if (ref & ADC_REF_MASK)
        ref = ADC_REF_AVCC;

    ADMUX &= ~ADC_REF_MASK;
    ADMUX |= ref;
}

void
ADC::setChannel(uint8_t channel)
{
    if (channel & ADC_CH_MASK)
        channel = ADC_CH_ADC0;
    
    ADMUX &= ~ADC_CH_MASK;
    ADMUX |= channel;
}

uint16_t
ADC::convert10Bit()
{
    ADCSRA |= _BV(ADIF);                     // clear hardware "conversion complete" flag 
    ADCSRA |= _BV(ADSC);                     // start conversion
    
    // wait for completion
    while(!isConversionComplete())
        ;
    
    return ADCW;
}

// Interrupt handler for ADC complete interrupt.
ISR(ADC_vect)
{
    ADC::setLastConversion(ADCW);
    System::fireISR(EV_ADC);
}


