//
//  Button.h
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

#include "EventListener.h"

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Button
//
//  Debounced button on a port pin
//
//////////////////////////////////////////////////////////////////////////////

class ButtonBase
{
public:
    ButtonBase() 
        : m_debounceTestsRemaining(0)
        , m_debounceValue(false)
        , m_value(false)
    {
    }
    
    void _handleEvent(EventType, EventParam, bool value, uint8_t numDebounceTests);
private:
    uint8_t m_debounceTestsRemaining;
    bool m_debounceValue;
    bool m_value;
};

template <class Port, uint8_t Bit, uint8_t debounceTimerCount = 10, uint8_t numDebounceTests = 5>
class Button : public ButtonBase {
public:
	Button()
        : ButtonBase()
    {
        m_port.setBitInput(Bit);
        m_timerID = Application::startEventTimer(debounceTimerCount);
    }
    
    // EventListener override
    virtual void handleEvent(EventType type, EventParam param)
    {
        if (type != EV_EVENT_TIMER || m_timerID != MakeTimerID(param))
            return;
            
        _handleEvent(type, param, m_port.isPortBit(Bit), numDebounceTests);
        m_timerID = Application::startEventTimer(debounceTimerCount);
    }
    
private:
    Port m_port;
    TimerID m_timerID;
};

}
