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

#include "m8r/Timer.h"
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

struct RTCTime {
    uint8_t seconds, minutes, hours;
    uint8_t day, date, month;
    uint16_t year;    
};

class RTCBase
{
public:
    RTCBase(uint16_t intervalsPerSecond) 
        : m_intervalsPerSecond(intervalsPerSecond)
        , m_minutes(0)
        , m_seconds(0)
        , m_intervalsRemaining(intervalsPerSecond)
    {
    }
    
    void setTicks(uint32_t t)
    {
        m_minutes = t / 60;
        m_seconds = t % 60;
    }

    void currentTime(RTCTime& rtc);

protected:
    static void handleISR(EventType, void*);
    
private:
    uint16_t m_intervalsPerSecond;
    uint32_t m_minutes;
    uint8_t m_seconds;
    uint16_t m_intervalsRemaining;
};

template <class Timer>
class RTC : public RTCBase {
public:
	RTC(TimerClockMode prescaler, uint16_t count, uint16_t intervalsPerSecond)
        : RTCBase(intervalsPerSecond)
        , m_timer(&handleISR, this)
    {
NOTE(count / 10);
        m_timer.setTimerClockMode(prescaler);
        m_timer.setOutputCompareA(count);
        m_timer.setWaveGenMode(TimerWaveGenCTC);
        m_timer.setIrptEnabled(TimerOutputCmpMatchAIrpt, true);
    }
    
private:
    Timer m_timer;
};

}
