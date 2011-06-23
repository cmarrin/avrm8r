//
//  Timer1RTC.cpp
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

#include "m8r/RTC.h"

#include "m8r/Event.h"
#include <avr/pgmspace.h>

extern const uint8_t mygMonthDayTable[] PROGMEM;
const uint8_t mygMonthDayTable[] = {31,28,31,30,31,30,31,31,30,31,30,31};

using namespace m8r;

static inline bool
isLeapYear(uint16_t year)
{
    // leap year:
    // is divisable by 4 and not by 100
    // or is divisable by 400 
    return (((year % 4) == 0 && (year % 100) != 0) || (year % 400) == 0);
}

bool
RTC::handleEvent(EventType type, uint8_t identifier)
{
    if (identifier != m_timerId)
        return false;

    // Once per second.
    m_time.m_seconds++;
    if (m_time.m_seconds > 59) {
        m_time.m_seconds = 0;
        m_time.m_minutes++;
        if (m_time.m_minutes > 59) {
            m_time.m_minutes = 0;
            m_time.m_hours++;
            if (m_time.m_hours > 23) {
                m_time.m_hours = 0;
                m_time.m_day++;
                // check days overflow
                if ((m_time.m_month == 2 && isLeapYear(m_time.m_year) && m_time.m_day == 29) ||
                        (m_time.m_day == pgm_read_byte(&mygMonthDayTable[m_time.m_month-1]))) {
                    m_time.m_day = 1;
                    m_time.m_month++;
                    if (m_time.m_month == 13) {
                        m_time.m_month = 1;
                        m_time.m_year++;
                    }
                }
            }
        }
    }
    
    Event::add(EV_RTC_EVENT);
    return true;
}

