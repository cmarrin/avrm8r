//
//  Timer1RTC.h
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
#include "m8r/TimerEventMgr.h"
#include <avr/interrupt.h>

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: RTC
//
//  Real time clock
//
//  Sets up a 1 second timer and updates time and date on each event firing
//
//////////////////////////////////////////////////////////////////////////////

class RTCTime {
public:
    RTCTime()
        : m_seconds(0)
        , m_minutes(0)
        , m_hours(12)
        , m_day(1)
        , m_month(1)
        , m_year(2000)
    { }
    
    RTCTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hours, uint8_t minutes, uint8_t seconds)
        : m_seconds(seconds)
        , m_minutes(minutes)
        , m_hours(hours)
        , m_day(day)
        , m_month(month)
        , m_year(year)
    { }
    
    RTCTime(const RTCTime& t)
    : m_ticks(t.m_ticks)
    , m_seconds(t.m_seconds)
    , m_minutes(t.m_minutes)
    , m_hours(t.m_hours)
    , m_day(t.m_day)
    , m_month(t.m_month)
    , m_year(t.m_year)
    { }
    
    uint16_t    m_ticks;
    uint8_t     m_seconds, m_minutes, m_hours;
    uint8_t     m_day, m_month;
    uint16_t    m_year;    
};
    
class RTC : public EventListener {
public:
	RTC()
        : m_ticks(0)
        , m_timerId(TimerEventMgrBase::shared()->createTimerEvent(1000, TimerEventRepeating))
    {
    }
    
    void setTime(const RTCTime& time) { m_time = time; }
    const RTCTime& time() const { return m_time; }

    // EventListener overrides
    virtual bool handleEvent(EventType type, uint8_t identifier);
    
private:
    uint32_t m_ticks;
    RTCTime m_time;
    uint8_t m_timerId;
};

}
