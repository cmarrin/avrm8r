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

#pragma once

#include "m8r.h"

#include "EventSourceEnums.h"

#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>

namespace m8r {

const uint32_t shortInnerDelayCount = (uint32_t) 4;
const uint32_t shortDelayCountMultiplier = (uint32_t) F_CPU / shortInnerDelayCount;
const uint32_t longInnerDelayCount = (uint32_t) 1539;
const uint32_t longDelayCountMultiplier = (uint32_t) F_CPU / longInnerDelayCount;

static inline uint16_t shortCountFromUS(uint16_t us) { return (uint16_t)((shortDelayCountMultiplier * (uint32_t) us) / (uint32_t) 1000000); }
static inline uint16_t longCountFromMS(uint16_t ms) { return (uint16_t)((longDelayCountMultiplier * (uint32_t) ms) / (uint32_t) 1000); }

// Event management
typedef void* EventParam;
inline TimerID MakeTimerID(EventParam param) { return (TimerID) (uint32_t) param; }
inline uint16_t MakeUint16(EventParam param) { return (uint16_t) (uint32_t) param; }
        
typedef void (*EventCallback)(EventType, EventParam);

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Application
//
//  Main application for AVR apps. Static class
//
//////////////////////////////////////////////////////////////////////////////

class ErrorReporter;
class EventListener;
class TimerEventMgrBase;

class Application {
public:
    Application();
    
    static void fireISR(EventType type, EventParam param = EventParam()) { handleEvent(type, param); }
    
#ifdef DEBUG
    static void setErrorReporter(ErrorReporter* reporter) { m_errorReporter = reporter; }
    static void handleErrorCondition(char c, uint16_t errorType, ErrorConditionType conditionType);
#endif
    
    static void addEventListener(EventListener*);
    static void removeEventListener(EventListener*);
    static void handleEvent(EventType, EventParam = 0);
    
    static void setTimerEventMgr(TimerEventMgrBase* mgr)
    {
        ASSERT(!m_timerEventMgr, AssertSingleTimerEventMgr);
        m_timerEventMgr = mgr;
    }
    
    static TimerID startEventTimer(uint16_t count);
    static void stopEventTimer(TimerID);
        
    static void run() __attribute__((noreturn));
    
    template <uint16_t ms> static void _INLINE_ msDelay() { longDelay(longCountFromMS(ms)); }
    template <uint16_t us> static void _INLINE_ usDelay() { _delay_loop_2(shortCountFromUS(us)); }
    
    // delay count = ((6 * 256 - 1) + 4) * count = 1539 * count
    // Can handle up to 5000ms delay at 20MHz
    static void _INLINE_ longDelay(uint16_t count)
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

protected:

private:
    static void wait()
    {
    }
    
    static EventListener* m_eventListenerHead;
    static ErrorReporter* m_errorReporter;
    static TimerEventMgrBase* m_timerEventMgr;
};

#ifdef DEBUG
// Error handling
class ErrorReporter {
public:
    ErrorReporter() { Application::setErrorReporter(this); }
    virtual void reportError(char, uint16_t, ErrorConditionType) = 0;
};
#endif

}
