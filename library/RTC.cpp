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

#include "RTC.h"

#include "Application.h"
#include <avr/pgmspace.h>

const uint32_t minutesPerDay = 24UL * 60UL;
const uint16_t epochYear = 1970;
const uint8_t epochDayOfWeek = 6; // Saturday

extern const uint8_t mygMonthDayTable[] PROGMEM;
const uint8_t mygMonthDayTable[] = {31,28,31,30,31,30,31,31,30,31,30,31};

static inline uint8_t 
daysInMonth(uint8_t month)
{
    return pgm_read_byte(&mygMonthDayTable[month]);
}

using namespace m8r;

static inline bool
isLeapYear(uint16_t year)
{
    return (((year % 4) == 0 && (year % 100) != 0) || (year % 400) == 0);
}

void
RTCBase::currentTime(RTCTime& rtc)
{
    uint32_t minutesInDay = m_minutes % minutesPerDay;
	uint16_t days = m_minutes / minutesPerDay;

	rtc.seconds = m_seconds;
	rtc.minutes = minutesInDay % 60UL;
	rtc.hours = minutesInDay / 60UL;
	rtc.day = (days + epochDayOfWeek) % 7;
    
    bool leap;
    uint16_t year;
    
    for (year = epochYear; ; ++year) {
        leap = isLeapYear(year);
        uint16_t yearLength = leap ? 366 : 365;
        if (days > yearLength)
            break;
        days -= yearLength;
    }

    rtc.year = year;
    
    uint8_t month;
    
    for (month = 0; ; ++month) {
        uint8_t monthLength = daysInMonth(month);
        if (days > monthLength)
            break;
        days -= monthLength;
    }

    rtc.month = month;
    rtc.date = days;
}

void
RTCBase::handleISR(EventType, void* data)
{
    RTCBase* rtc = (RTCBase*) data;
    
    if (++rtc->m_intervalCount < rtc->m_intervalsPerSecond)
        return;
        
    rtc->m_intervalCount = 0;
    
    if (++rtc->m_seconds >= 60) {
        rtc->m_seconds = 0;
        rtc->m_minutes++;
        Application::handleISR(EV_RTC_MINUTES_EVENT);
    }
    
    Application::handleISR(EV_RTC_SECONDS_EVENT);
}

