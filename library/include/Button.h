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
#include "TimerEvent.h"

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

template <class Bit, uint8_t debounceTimerCount = 10, uint8_t numDebounceTests = 5>
class Button : public ButtonBase, public EventListener {
public:
	Button()
    {
        _bit = true; // Activate pullup
        System::startEventTimer(&_event);
    }
    
    // EventListener override
    virtual void handleEvent(EventType type, EventParam param)
    {
        if (type != EV_EVENT_TIMER || &_event != (TimerEvent*) param)
            return;
            
        _handleEvent(type, param, !_bit, numDebounceTests);
    }
    
private:
    Bit _bit;
    RepeatingTimerEvent<debounceTimerCount> _event;
};

class ButtonSetBase
{
public:
    virtual void handleEvent(EventType, EventParam) = 0;
    virtual bool state(uint8_t b) const = 0;
};

template<uint8_t ButtonCount>
class ButtonSet : public ButtonSetBase
{
public:
    ButtonSet(ButtonBase* button, ...)
    {
        memset(_state, 0, (ButtonCount + 7) / 8);
        va_list ap;
        va_start(ap, button);
        while (button) {
            addButton(button);
            button = va_arg(ap, ButtonBase*);
        }
        va_end(ap);
    }

    virtual void handleEvent(EventType type, EventParam param)
    {
        switch(type) {
            case EV_BUTTON_UP:
            case EV_BUTTON_DOWN: {
                ButtonBase* button = reinterpret_cast<ButtonBase*>(param);
                for (uint8_t i = 0; i < _count; ++i) {
                    if (_buttons[i] == button) {
                        setState(i, type == EV_BUTTON_DOWN);
                        break;
                    }
                }
                break;
            }
            default: break;
        }
    }
    
    virtual bool state(uint8_t b) const
    {
        ASSERT(b < _count, AssertButtonOutOfRange);
        return _state[b / 8] & (1 << (b % 8));
    }
    
private:
    void addButton(ButtonBase* button)
    {
        ASSERT(_count >= ButtonCount, AssertButtonTooMany);
        _buttons[_count++] = button;
    }
    
    void setState(uint8_t i, bool state)
    {
        if (state) {
            _state[i / 8] |= 1 << (i % 8);
        } else {
            _state[i / 8] &= ~(1 << (i % 8));
        }
    }
    
    ButtonBase* _buttons[ButtonCount];
    uint8_t _state[(ButtonCount + 7) / 8];
    uint8_t _count;
};

template<uint8_t rows, uint8_t cols>
class ButtonMatrix : public ButtonSetBase
{
};

}
