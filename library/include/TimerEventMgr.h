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

#include "TimerBase.h"
#include "TimerEvent.h"

#include "System.h"

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: TimerEventMgr
//
// Manage events which fire at given intervals. The timer needs to be
// about 1ms so system clients can give interval counts that will give
// approximately the desired timeout. For instance the Button debounce
// timer needs to have sufficient time to get accurate button values.
// The prescale value is given to the timer and the compareCount is computed
// from that and F_CPU. If the prescale value is insufficient to do a 1ms
// timeout with the given timer, the closest approximation is used. For
// instance, if an 8 bit timer is used and a prescale value of 1 is
// selected with an 8MHz clock, a 30us would be the result. This is
// probably a bad thing if you're using the Button or Network components.
// But selecting a prescale of 64 the 8 bit timer can use a compareCount
// of 125 to achieve a 1ms timeout. A compareCount can be provided to
// override this behavior, as long as the consequences are known. 
//
//////////////////////////////////////////////////////////////////////////////

class TimerEventMgrBase
{
    friend class System;
    
public:
protected:
	TimerEventMgrBase()
        : _head(NULL)

    {
        System::setTimerEventMgr(this);
    }

    void start(TimerEvent* event)
    {
        event->_next = _head;
        _head = event;
    }
    
    void stop(TimerEvent*);

    static void fireISR(EventType, EventParam);
    
private:
    TimerEvent* _head;
};

// Default is to create a 1ms timer
template <class Timer, TimerClockMode prescaler, uint16_t compareCount = 0>
class TimerEventMgr : public TimerEventMgrBase {
public:
	TimerEventMgr()
        : TimerEventMgrBase()
        , m_timer(&fireISR, this)
    {
        m_timer.setTimerClockMode(prescaler);
        uint16_t count = compareCount;
        if (count == 0) {
            count = F_CPU / TimerPrescaleDivisor(prescaler) / 1000;
            if (count < 2) {
                count = 1;
            } else {
                count -= 1;
            }
            // FIXME: Add a static const to timers to give the max compare value
            // and use that to make sure we don't go over.
        }
        m_timer.setOutputCompareA(count);
        m_timer.setWaveGenMode(TimerWaveGenCTC);
        m_timer.setIrptEnabled(TimerOutputCmpMatchAIrpt, true);
    }
    
    void start(TimerEvent* event)
    {
        
    }
    
private:
    Timer m_timer;
};

}