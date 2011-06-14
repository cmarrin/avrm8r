//
//  TimerEventManager.h
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

#include "m8r/Timer.h"

namespace m8r {

enum TimerEventMode { TimerEventRepeating, TimerEventOneShot };

//////////////////////////////////////////////////////////////////////////////
//
//  Class: TimerEvent
//
//  Timer events managed by TimerEventManager
//
//////////////////////////////////////////////////////////////////////////////
    

class TimerEvent {
    friend class TimerEventManagerBase;
    
protected:
	TimerEvent(uint16_t intervals, TimerEventMode mode, uint8_t identifier)
    : m_intervals(intervals ? intervals : 1)
    , m_remainingIntervals(intervals)
    , m_mode(mode)
    , m_identifier(identifier)
    , m_next(0) { }

    uint16_t intervals() const { return m_intervals; }
    uint16_t remainingIntervals() const { return m_remainingIntervals; }
    TimerEventMode mode() const { return m_mode; }
    uint8_t identifier() const { return m_identifier; }
    
    void setNext(TimerEvent* event) { m_next = event; }
    TimerEvent* next() { return m_next; }

private:
    uint16_t m_intervals, m_remainingIntervals;
    TimerEventMode m_mode;
    uint8_t m_identifier;
    TimerEvent* m_next;
};

//////////////////////////////////////////////////////////////////////////////
//
//  Class: TimerEventManager
//
//  Manage list of events which fire at given intervals
//
//////////////////////////////////////////////////////////////////////////////

class TimerEventManagerBase {
public:
    uint8_t createTimerEvent(uint16_t intervals, TimerEventMode mode);

    void fireInterval();

protected:
	TimerEventManagerBase(TimerClockMode prescaler, uint16_t count)
        : m_head(0)
        , m_free(0)
        , m_nextIdentifier(1)
    {
    }


private:
    TimerEvent* alloc(uint16_t intervals, TimerEventMode mode);
    void add(TimerEvent* event);

    TimerEvent* m_head;
    TimerEvent* m_free;
    uint8_t m_nextIdentifier;
};

template <class Timer>
class TimerEventManager : public TimerEventManagerBase {
    class MyTimer : public Timer {
    public:
        MyTimer(TimerEventManagerBase* base) : m_timerEventManagerBase(base) { }
    
    private:
        virtual void handleOutputCmpMatchAIrpt(Timer*) { m_timerEventManagerBase->fireInterval(); }
        
        TimerEventManagerBase* m_timerEventManagerBase;
    };
    
public:
	TimerEventManager(TimerClockMode prescaler, uint16_t count)
        : TimerEventManagerBase(prescaler, count)
        , m_timer(this)
    {
        m_timer.setTimerClockMode(prescaler);
        m_timer.setOutputCompareA(count);
        m_timer.setWaveGenMode(TimerWaveGenCTC);
        m_timer.setIrptEnabled(TimerOutputCmpMatchAIrpt, true);
    }
    
private:
    MyTimer m_timer;
};

}