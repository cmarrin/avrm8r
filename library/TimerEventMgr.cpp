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

#include "TimerEventMgr.h"

#include <string.h>

using namespace m8r;

TimerEventMgrBase::TimerEventMgrBase()
{
    memset(m_timerCount, 0, 8 * sizeof(uint16_t));
}

TimerID
TimerEventMgrBase::start(uint16_t intervals)
{
    if (!intervals)
        intervals = 1;
    for (uint8_t i = 0; i < 8; ++i) {
        if (m_timerCount[i] == 0) {
            m_timerCount[i] = intervals;
            return i;
        }
    }
    
    FATAL(AssertNoEventTimers);
    return 0;
}

void
TimerEventMgrBase::stop(TimerID id)
{
    m_timerCount[id] = 0;
}

void
TimerEventMgrBase::fireISR(EventType type, EventParam param)
{
    TimerEventMgrBase* mgr = (TimerEventMgrBase*) param;
    
    for (uint8_t i = 0; i < 8; ++i) {
        if (mgr->m_timerCount[i]) {
            if (--(mgr->m_timerCount[i]) == 0)
                Application::fireISR(EV_EVENT_TIMER, (EventParam) i);
        }
    }
}
