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

#include "m8r/TimerEventMgr.h"

#include "m8r/Event.h"
#include <avr/interrupt.h>

using namespace m8r;

TimerEventMgrBase* TimerEventMgrBase::m_shared = 0;


uint8_t
TimerEventMgrBase::createTimerEventWithIntervals(uint16_t intervals, TimerEventMode mode)
{
    TimerEvent* event = alloc(intervals, mode);
    add(event);
    return event->identifier();
}

TimerEvent*
TimerEventMgrBase::alloc(uint16_t intervals, TimerEventMode mode)
{
    if (m_free) {
        TimerEvent* event = m_free;
        m_free = event->m_next;
        return event;
    }
    
    TimerEvent* event = new TimerEvent(intervals, mode, m_nextIdentifier++);
    ASSERT(event, AssertTimerEventAlloc);
    return event;
}

void
TimerEventMgrBase::add(TimerEvent* event)
{
    TimerEvent* before = 0;
    TimerEvent* after;
    
    for (after = m_head; after; before = after, after = after->m_next)
        if (event->m_remainingIntervals < after->m_remainingIntervals)
            break;
            
    if (!before) {
        event->m_next = m_head;
        m_head = event;
    }
    else {
        event->setNext(after);
        before->m_next = event;
    }
}

// FIXME: There will be a lot of churn for events with an interval count of 1.
// It will fire every time, get removed from the front of the list and then
// readded to the front of the list. This can be optimized.
void
TimerEventMgrBase::fireInterval()
{
    TimerEvent* finishedEvents = 0;
    
    for (TimerEvent* event = m_head; event; ) {
        TimerEvent* nextEvent = event->m_next;
        
        if (--event->m_remainingIntervals == 0) {
            // FIXME: Need to make timer event ids start at EV_LAST and go up from there
            Event::add(EV_TIMER_EVENT, event->m_identifier);
            
            
            if (event->m_mode == TimerEventRepeating) {
                event->m_next = finishedEvents;
                finishedEvents = event;
            }
            else {
                event->m_next = m_free;
                m_free = event;
            }
        }

        event = nextEvent;
    }
    
    while (finishedEvents) {
        TimerEvent* event = finishedEvents;
        finishedEvents = finishedEvents->m_next;
        event->m_remainingIntervals = event->m_intervals;
        add(event);
    }
}
