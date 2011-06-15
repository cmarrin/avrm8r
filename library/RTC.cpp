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

void
RTC::fire(EventType, uint8_t)
{
	myTime.myTicks++;
    if(myTime.myTicks == myPeriod) {
		myTime.myTicks = 0;
		myTime.mySeconds++;
		if(myTime.mySeconds > 59) {
			myTime.mySeconds = 0;
			myTime.myMinutes++;
			if(myTime.myMinutes > 59) {
				myTime.myMinutes = 0;
				myTime.myHours++;
				if(myTime.myHours > 23) {
					myTime.myHours = 0;
					myTime.myDay++;
					// check days overflow
					if((myTime.myMonth == 2 && isLeapYear(myTime.myYear) && myTime.myDay == 29) ||
                            (myTime.myDay == pgm_read_byte(&mygMonthDayTable[myTime.myMonth-1]))) {
						myTime.myDay = 1;
						myTime.myMonth++;
						if(myTime.myMonth == 13) {
							myTime.myMonth = 1;
							myTime.myYear++;
						}
					}
				}
			}
		}
	}
    
    Event::add(EV_RTC_EVENT);
}

