//
//  TimerEventMgr.cpp
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

#include "m8r/TimerEventMgr.h"

#include "m8r/Event.h"
#include "m8r/TimerEvent.h"
#include <avr/interrupt.h>

using namespace m8r;

TimerEventMgrBase* TimerEventMgrBase::m_shared = 0;

TimerEventMgrBase::TimerEventMgrBase(uint16_t usPerInterval)
    : m_head(0)
    , m_free(0)
    , m_usPerInterval(usPerInterval)
    , m_currentInterval(0)
{
    ASSERT(!m_shared, AssertSglTimerEventMgr);
    m_shared = this;
}

uint16_t
TimerEventMgrBase::intervalsFromMilliseconds(uint16_t ms) const
{
    uint32_t us = ((uint32_t) ms) * 1000;
    return (us + (m_usPerInterval >> 1)) / (uint32_t) m_usPerInterval;
}

void
TimerEventMgrBase::add(TimerEvent* event)
{
    uint32_t endInterval = (int32_t) event->m_intervals + m_currentInterval;
    event->m_endInterval = endInterval;
    
    if (!m_head || m_head->m_endInterval >= endInterval) {
        event->m_next = m_head;
        m_head = event;
        return;
    }
    
    for (TimerEvent* currentEvent = m_head; ; currentEvent = currentEvent->m_next) {
        if (currentEvent->m_next->m_endInterval >= endInterval) {
            event->m_next = currentEvent;
            currentEvent->m_next = event;
            return;
        }
        
        if (!currentEvent->m_next) {
            event->m_next = 0;
            currentEvent->m_next = event;
            return;
        }
    }
}

void
TimerEventMgrBase::remove(TimerEvent* event)
{
    if (event == m_head) {
        m_head = event->m_next;
        return;
    }
    
    for (TimerEvent* currentEvent = m_head; currentEvent; currentEvent = currentEvent->m_next) {
        if (currentEvent->m_next == event) {
            currentEvent->m_next = event->m_next;
            return;
        }
    }
}

// FIXME: There will be a lot of churn for events with an interval count of 1.
// It will fire every time, get removed from the front of the list and then
// readded to the front of the list. This can be optimized.
void
TimerEventMgrBase::handleEvent(EventType, uint8_t identifier)
{
    ++m_currentInterval;
    
    TimerEvent* newEvents = 0;
    
    while (m_head) {
        if (m_head->m_endInterval <= m_currentInterval) {
            m_head->m_eventListener->handleEvent(m_head->m_type, m_head->m_identifier);
            TimerEvent* nextEvent = m_head->m_next;
            
            if (m_head->m_mode == TimerEventRepeating) {
                m_head->m_next = newEvents;
                newEvents = m_head;
            }
            
            m_head = nextEvent;
        }
        else
            break;
    }
    
    while (newEvents) {
        TimerEvent* event = newEvents;
        newEvents = newEvents->m_next;
        add(event);
    }
}
