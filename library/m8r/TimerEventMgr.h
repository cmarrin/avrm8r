//
//  TimerEventMgr.h
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

#include "m8r/EventListener.h"
#include "m8r/Timer.h"

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: TimerEventMgr
//
//  Manage list of events which fire at given intervals
//
//////////////////////////////////////////////////////////////////////////////

class TimerEvent;

class TimerEventMgrBase : public EventListener {
public:
    static TimerEventMgrBase* shared()
    {
        ASSERT(m_shared, AssertNoTimerEventMgr);
        return m_shared;
    }

    void add(TimerEvent*);
    void remove(TimerEvent*);

    uint16_t intervalsFromMilliseconds(uint16_t) const;
    
protected:
	TimerEventMgrBase(uint16_t usPerInterval);
    
    // EventListener override
    virtual void handleEvent(EventType, uint8_t identifier);

private:
    TimerEvent* m_head;
    TimerEvent* m_free;
    uint16_t m_usPerInterval;
    uint32_t m_currentInterval;
    
    static TimerEventMgrBase* m_shared;
};

template <class Timer>
class TimerEventMgr : public TimerEventMgrBase {
public:
	TimerEventMgr(TimerClockMode prescaler, uint16_t count, uint16_t usPerInterval)
        : TimerEventMgrBase(usPerInterval)
        , m_timer(this)
    {
        m_timer.setTimerClockMode(prescaler);
        m_timer.setOutputCompareA(count);
        m_timer.setWaveGenMode(TimerWaveGenCTC);
        m_timer.setIrptEnabled(TimerOutputCmpMatchAIrpt, true);
    }
    
private:
    Timer m_timer;
};

}