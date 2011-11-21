//
//  TimerEvent.h
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
#include "m8r/EventListener.h"

namespace m8r {

enum TimerEventMode { TimerEventRepeating, TimerEventOneShot };

//////////////////////////////////////////////////////////////////////////////
//
//  Class: TimerEvent
//
//  Timer events managed by TimerEventMgr
//
//////////////////////////////////////////////////////////////////////////////
    

class TimerEvent {
    friend class TimerEventMgrBase;
    
public:
    TimerEvent(EventListener* listener, uint16_t ms, TimerEventMode mode, EventType type = EV_TIMER_EVENT);
    ~TimerEvent() { stop(); }
    
    void start();
    void stop();
    
    uint8_t identifier() const { return m_identifier; }

private:
    uint16_t m_intervals;
    uint16_t m_endInterval;
    TimerEventMode m_mode;
    EventListener* m_eventListener;
    TimerEvent* m_next;
    uint8_t m_identifier;
    EventType m_type;
    
    static uint8_t m_nextIdentifier;
};

}