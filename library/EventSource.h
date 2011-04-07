//
//  EventSource.h
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

namespace m8r {

// Event sources
// timer parameters
#define TIMER_COMPAREA		0
#define TIMER_COMPAREB		1
#define TIMER_COMPAREC		2
#define TIMER_OVERFLOW		3
#define TIMER_INPUT_CAPTURE	4
#define TIMER_NUM_PARAMS    5

// USART parameters
#define USART_RECV			0
#define USART_DATA			1
#define USART_TRANS			2
#define USART_NUM_PARAMS    3

// Number of PIN_CHANGE interrupts
#define PIN_CHANGE_NUM_PARAMS 4

// Number of external interrupts
#define INTERRUPT_NUM_PARAMS 8

enum EventType {
    EV_NO_EVENT		= 0,
    
    // To send these events add the EventType to the USART_* value.
    // For instance EV_USART2 + USART_RECV means a byte was receieved on USART2
    EV_USART0		= EV_NO_EVENT + 1,
    EV_USART1		= EV_USART0 + USART_NUM_PARAMS,
    EV_USART2		= 3,
    EV_USART3		= 4,
    
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
    
    // This event is generated once per loop through event bits
    EV_IDLE         = 28,
    
    // User generated events
    EV_USER0		= 32,
    EV_USER1		= 33,
    EV_USER2		= 34,
    EV_USER3		= 35,
    EV_USER4		= 36,
    EV_USER5		= 37,
    EV_USER6		= 38,
    EV_USER7		= 39,
    
    EV_LAST         = 40
};

const uint8_t eventArraySize = EV_LAST + 4 / 8;

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
