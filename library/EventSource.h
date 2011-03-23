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

#include "avr.h"

namespace avr {
	// Event sources
	enum EventType {
		EV_NO_EVENT		= 0,
		EV_USART0		= 1,	// with parameter USART_*
		EV_USART1		= 2,	// with parameter USART_*
		EV_USART2		= 3,	// with parameter USART_*
		EV_USART3		= 4,	// with parameter USART_*
		EV_SPI			= 5,
		EV_TWI			= 6,
		EV_WATCHDOG		= 7,
		EV_TIMER0		= 8,	// with parameter TIMER_*
		EV_TIMER1		= 9,	// with parameter TIMER_*
		EV_TIMER2		= 10,	// with parameter TIMER_*
		EV_TIMER3		= 11,	// with parameter TIMER_*
		EV_TIMER4		= 12,	// with parameter TIMER_*
		EV_TIMER5		= 13,	// with parameter TIMER_*
		EV_COMPARATOR0	= 14,
		EV_COMPARATOR1	= 15,
		EV_ADC			= 16,
		EV_PIN_CHANGE	= 17,	// with parameter 0-3
		EV_INTERRUPT	= 18,	// with parameter 0-7
		EV_EEPROM		= 19,
		EV_SPM			= 20,

		// There are special and probably exclusive (any given chip supports at most one)
		// So they can probably share a bit
		EV_PSC			= 21,
		EV_USB			= 22,
		EV_DMA			= 23,
		EV_MAC			= 24,
		EV_CAN			= 25,
		EV_LCD			= 26,
		EV_FPGA			= 27,
		
		EV_USER0		= 28,
		EV_USER1		= 29,
		EV_USER2		= 30,
		EV_USER3		= 31
	};

// timer parameters
#define TIMER_COMPAREA		0
#define TIMER_COMPAREB		1
#define TIMER_COMPAREC		2
#define TIMER_OVERFLOW		3
#define TIMER_INPUT_CAPTURE	4

// USART parameters
#define USART_RECV			0
#define USART_DATA			1
#define USART_TRANS			2

//////////////////////////////////////////////////////////////////////////////
//
//  Class: EventSource
//
//  Base class for event sources in run loop
//
//////////////////////////////////////////////////////////////////////////////

class EventSource {
	friend class Application;
	
public:
	EventSource()	{ }
	~EventSource()	{ }
	
protected:
	void addEvent(EventType type);
	
private:
};

}
