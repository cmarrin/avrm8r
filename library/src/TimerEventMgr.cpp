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

void
TimerEventMgrBase::stop(TimerEvent* event)
{
    TimerEvent* prev = NULL;
    for (TimerEvent* cur = _head; cur; cur = cur->_next) {
        if (cur == event) {
            if (!prev) {
                _head = cur->_next;
            } else {
                prev->_next = cur->_next;
            }
            cur->_next = NULL;
            break;
        }
        prev = cur;
    }
}

void
TimerEventMgrBase::fireISR(EventType type, EventParam param)
{
    TimerEventMgrBase* mgr = (TimerEventMgrBase*) param;
    
    TimerEvent* prev = NULL;
    for (TimerEvent* cur = mgr->_head; cur; ) {
        TimerEvent* next = cur->_next;
        if (cur->_currentCount == 0 || --cur->_currentCount == 0) {
            System::fireISR(EV_EVENT_TIMER, reinterpret_cast<EventParam>(cur));
            if (cur->_resetCount == 0) {
                if (!prev) {
                    mgr->_head = next;
                } else {
                    prev->_next = next;
                }
            } else {
                cur->_currentCount = cur->_resetCount;
            }
        }
        prev = cur;
        cur = next;
    }
}
