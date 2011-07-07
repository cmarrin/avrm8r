//
//  Animator.cpp
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

#include "m8r/Animator.h"

#include "m8r/TimerEventMgr.h"

using namespace m8r;

static uint8_t g_sineTable[ ] = { 0, 49, 90, 117, 127 };

static uint8_t interpSine(uint8_t t)
{
    uint8_t i = t >> 4;
    return (((uint16_t) (g_sineTable[i + 1] - g_sineTable[i]) * (uint16_t) (t & 0x0F)) >> 4) + g_sineTable[i];
}

static uint8_t sine(uint8_t t)
{
    if (t < 64)
        return interpSine(t) + 128;
    if (t < 128)
        return interpSine(127 - t) + 128;
    if (t < 192)
        return 127 - interpSine(t - 128);
    return 127 - interpSine(255 - t);
}

Animator::Animator(EventListener* listener, uint8_t msRate, uint8_t startValue, uint8_t endValue)
    : m_timerEvent(listener, msRate, TimerEventRepeating, EV_ANIMATOR_EVENT)
    , m_startInterval(0)
    , m_pauseInterval(0)
    , m_intervalsPerIteration((uint16_t) msRate << 8)
    , m_rate(msRate)
    , m_startValue(startValue)
    , m_endValue(endValue)
    , m_iterations(0)
{
}

void
Animator::start(uint8_t iterations)
{
    m_startInterval = TimerEventMgrBase::shared()->currentInterval();
    m_pauseInterval = 0;
    m_timerEvent.start();
}

void
Animator::pause()
{
    if (!m_startInterval || m_pauseInterval)
        return;
    m_pauseInterval = TimerEventMgrBase::shared()->currentInterval();
    m_timerEvent.stop();
}

void
Animator::resume()
{
    if (!m_startInterval || !m_pauseInterval)
        return;
    m_startInterval += TimerEventMgrBase::shared()->currentInterval() - m_pauseInterval;
    m_pauseInterval = 0;
    m_timerEvent.start();    
}

uint8_t
Animator::currentValue() const
{
    uint32_t endInterval = m_pauseInterval ? m_pauseInterval : TimerEventMgrBase::shared()->currentInterval();
    uint16_t diff = (endInterval - m_startInterval) % (uint32_t) m_intervalsPerIteration;
    uint16_t t = diff / (uint16_t) m_rate;
    return ((uint16_t) (m_endValue - m_startValue) * t) / 255 + m_startValue;
}

uint8_t
Animator::sineValue(uint8_t t)
{
    return sine(t);
}
