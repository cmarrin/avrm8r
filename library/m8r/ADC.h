//
//  ADC.h
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
#include <avr/interrupt.h>

#include "m8r/EventSource.h"

#undef ADC

namespace m8r {

// defines - borrowed from avrlib by Pascal Stang - Copyright (C) 2002

// A2D clock prescaler select
// This assumes ADPS{0,1,2} are 0,1,2, which is true for all chips that have them
#define ADC_PS_DIV2         0x00	///< 0x01,0x00 -> CPU clk/2
#define ADC_PS_DIV4         0x02	///< 0x02 -> CPU clk/4
#define ADC_PS_DIV8         0x03	///< 0x03 -> CPU clk/8
#define ADC_PS_DIV16		0x04	///< 0x04 -> CPU clk/16
#define ADC_PS_DIV32		0x05	///< 0x05 -> CPU clk/32
#define ADC_PS_DIV64		0x06	///< 0x06 -> CPU clk/64
#define ADC_PS_DIV128		0x07	///< 0x07 -> CPU clk/128
#define ADC_PS_MASK         0x07
	
// A2D voltage reference select
// This code assumes REFS0 and REFS1 are 6 and 7, which is true for all chips that have 2 ref bits
#define ADC_REF_AREF		0x00	///< 0x00 -> AREF pin, internal VREF turned off
#define ADC_REF_AVCC		0x40	///< 0x01 -> AVCC pin, internal VREF turned off
#define ADC_REF_256V		0xC0	///< 0x03 -> Internal 2.56V VREF
#define ADC_REF_MASK        0xC0
	
// bit mask for A2D channel multiplexer
#define ADC_MUX_MASK		0x1F
	
// channel defines
// This code assumes the bits are the low order 4 bits, which is usually true
#define ADC_CH_ADC0			0x00
#define ADC_CH_ADC1			0x01
#define ADC_CH_ADC2			0x02
#define ADC_CH_ADC3			0x03
#define ADC_CH_ADC4			0x04
#define ADC_CH_ADC5			0x05
#define ADC_CH_ADC6			0x06
#define ADC_CH_ADC7			0x07
#define ADC_CH_122V			0x0e	///< 1.22V voltage reference
#define ADC_CH_AGND			0x0f	///< AGND
#define ADC_CH_MASK         0x0f
	
// these channels supported only in ATmega128
// differential with gain
#define ADC_CH_0_0_DIFF10X		0x08
#define ADC_CH_1_0_DIFF10X		0x09
#define ADC_CH_0_0_DIFF200X		0x0A
#define ADC_CH_1_0_DIFF200X		0x0B
#define ADC_CH_2_2_DIFF10X		0x0C
#define ADC_CH_3_2_DIFF10X		0x0D
#define ADC_CH_2_2_DIFF200X		0x0E
#define ADC_CH_3_2_DIFF200X		0x0F
	
// differential
#define ADC_CH_0_1_DIFF1X		0x10
#define ADC_CH_1_1_DIFF1X		0x11
#define ADC_CH_2_1_DIFF1X		0x12
#define ADC_CH_3_1_DIFF1X		0x13
#define ADC_CH_4_1_DIFF1X		0x14
#define ADC_CH_5_1_DIFF1X		0x15
#define ADC_CH_6_1_DIFF1X		0x16
#define ADC_CH_7_1_DIFF1X		0x17
	
#define ADC_CH_0_2_DIFF1X		0x18
#define ADC_CH_1_2_DIFF1X		0x19
#define ADC_CH_2_2_DIFF1X		0x1A
#define ADC_CH_3_2_DIFF1X		0x1B
#define ADC_CH_4_2_DIFF1X		0x1C
#define ADC_CH_5_2_DIFF1X		0x1D
	
//////////////////////////////////////////////////////////////////////////////
//
//  Class: ADC
//
//  Interface to the A/2 hardware on the chip
//
//////////////////////////////////////////////////////////////////////////////
	
class ADC : public EventSource {
public:
	ADC(uint8_t channel, uint8_t prescale, uint8_t reference);
	~ADC() { }
	
	void setEnabled(bool e);
	bool isEnabled() const _INLINE_ { return ADCSRA | _BV(ADEN); }
	
	void setPrescaler(uint8_t prescale);
	uint8_t	getPrescaler() const
    {
        uint8_t psval = ADCSRA;
        return (((psval & (1 << ADPS0)) != 0) | (((psval & (1 << ADPS1)) != 0) << 1) | (((psval & (1 << ADPS2)) != 0) << 2));
    }
	
	void setReference(uint8_t ref);
	uint8_t getReference() const { return (ADMUX & 0x03) >> 6; }
	
	void setChannel(uint8_t channel);
	uint8_t getChannel() const { return ADMUX & ADC_MUX_MASK; }
	
	void startConversion();    
    uint16_t getLastConversion10Bit();
    uint8_t getLastConversion8Bit() _INLINE_ { return getLastConversion10Bit() >> 2; }
	
    uint16_t convert10Bit();
    uint16_t convert8Bit() _INLINE_ { return convert10Bit() >> 2; }
    
	bool isConversionComplete() const _INLINE_ { return (ADCSRA & (1 << ADSC)) == 0; }

private:
};

}
