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

#include <avr/interrupt.h>
#include "Timer1.h"

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Timer1RTC
//
//  Interface to 16 bit Timer/Counter, used as a real-time clock
//
//  On creation, the prescaler and output match values are set to the
//  passed values. The timer will interrupt on output match, at which
//  point the RTC values will be updated and the counter reset. The 
//  interrupts are assumed to occur at 'period' times per second.
//
//////////////////////////////////////////////////////////////////////////////

class RTCTime {
public:
    RTCTime() 
    : myTicks(0)
    , mySeconds(0)
    , myMinutes(0)
    , myHours(0)
    , myDay(0)
    , myMonth(0)
    , myYear(0)
    { }
    
    RTCTime(const RTCTime& t)
    : myTicks(t.myTicks)
    , mySeconds(t.mySeconds)
    , myMinutes(t.myMinutes)
    , myHours(t.myHours)
    , myDay(t.myDay)
    , myMonth(t.myMonth)
    , myYear(t.myYear)
    { }
    
    uint16_t    myTicks;
    uint8_t     mySeconds, myMinutes, myHours;
    uint8_t     myDay, myMonth;
    uint16_t    myYear;    
};
    
class Timer1RTC : public Timer1 {
public:
	Timer1RTC(uint8_t prescaler, uint16_t match, uint16_t period)
    : Timer1(prescaler, 0)
    , myPeriod(period)
    {
        setOutputCompare(match);
        setInterruptEnable(TIMER_COMPAREA, true);
        setClearOnOutputCompare(true);
    }
    
	~Timer1RTC() { }
    
    void onOutputCompare();
    
    void getTime(RTCTime& t) const { cli(); t = myTime; sei(); }

private:
    RTCTime     myTime;
    uint16_t    myPeriod;
};

}
