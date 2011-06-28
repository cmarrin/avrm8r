//
//  Application.h
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

#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>
#include "m8r/Event.h"
#include "m8r/EventListener.h"

// Setup for C++ operation
void * operator new(size_t size); 
void operator delete(void * ptr); 

namespace m8r {

const uint32_t innerDelayCount = (uint32_t) 1539;
const uint32_t delayCountMultiplier = (uint32_t) F_CPU / innerDelayCount;
const uint16_t delayCount100ms = (delayCountMultiplier * (uint32_t) 100) / (uint32_t) 1000;
const uint16_t delayCount250ms = (delayCountMultiplier * (uint32_t) 250) / (uint32_t) 1000;
const uint16_t delayCount1000ms = (delayCountMultiplier * (uint32_t) 1000) / (uint32_t) 1000;
const uint16_t delayCount2000ms = (delayCountMultiplier * (uint32_t) 2000) / (uint32_t) 1000;

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Application
//
//  Main application for AVR apps. Static class
//
//////////////////////////////////////////////////////////////////////////////

class EventListener;

class ErrorConditionHandler {
public:
    virtual void handleErrorCondition(ErrorType, ErrorConditionType) = 0;
};
    
class Application {
public:
	static void _NO_INLINE_ addEvent(Event* event)
    {
        if (event->m_active)
            return;
        event->m_next = m_eventHead;
        m_eventHead = event;
        event->m_active = true;
    }
    
    static void _INLINE_ addIdleEventListener(IdleEventListener* listener)
    {
        listener->m_next = m_idleEventListeners;
        m_idleEventListeners = listener;
    }

    static void removeIdleEventListener(IdleEventListener* listener);

    static void handleErrorCondition(ErrorType type, ErrorConditionType condition)
    {
        if (m_errorConditionHandler)
            m_errorConditionHandler->handleErrorCondition(type, condition);
    }
    
    static void run() __attribute__((noreturn));
    
    static void setEventOnIdle(bool b) { m_eventOnIdle = b; }
    static void setErrorConditionHandler(ErrorConditionHandler* handler) { m_errorConditionHandler = handler; }
    
    // delay count = ((6 * 256 - 1) + 4) * count = 1539 * count
    // Can handle up to 5000ms delay at 20MHz
    static void _INLINE_ delay(uint16_t count)
    {
        __asm__ volatile (
            "mov __tmp_reg__, __zero_reg__ \n\t"
            "1: nop \n\t"
            "nop \n\t"
            "nop \n\t"
            "inc __tmp_reg__ \n\t"
            "brne 1b \n\t"
            "sbiw %0, 1 \n\t"
            "brne 1b \n\t"
            : "=w" (count)
            : "0" (count)
        );
    }
    
private:
    static void wait()
    {
    }

    static ErrorConditionHandler* m_errorConditionHandler;
    
    static bool m_eventOnIdle;
    static IdleEventListener* m_idleEventListeners;
    
    static Event* m_eventHead;
};

}
