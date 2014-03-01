//
//  Animator.h
//
//  Created by Chris Marrin on 6/30/2011.
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

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Animator
//
//  generate a value that animates from 0 to 255 over time
//
//////////////////////////////////////////////////////////////////////////////

class EventListener;

class AnimatorBase {
public:
    AnimatorBase(uint8_t startValue, uint8_t endValue);
    
    void start(uint16_t rate);
    void pause() { m_paused = true; }
    void resume() { m_paused = false; }
    
    uint8_t currentRawValue() const { return m_currentValue; }
    uint8_t currentSineValue();

protected:
    static void fireISR(EventType, EventParam);
    
private:
    bool m_paused;
    uint8_t m_startValue, m_endValue, m_currentValue;
    uint16_t m_rate, m_count;
};

template <class Timer>
class Animator : public AnimatorBase {
public:
	Animator(TimerClockMode prescaler, uint16_t count, uint8_t startValue = 0, uint8_t endValue = 255)
        : AnimatorBase(startValue, endValue)
        , m_timer(&fireISR, this)
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