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

const uint8_t NoButton = 0xff;

class ButtonBase
{
public:
    ButtonBase() 
        : m_debounceTestsRemaining(0)
        , m_debounceValue(false)
        , m_value(false)
    {
    }
    
    void _handleEvent(bool value, uint8_t numDebounceTests);
    
private:
    uint8_t m_debounceTestsRemaining;
    bool m_debounceValue;
    bool m_value;
};

template <class Bit, uint8_t debounceTimerCount = 10, uint8_t numDebounceTests = 5>
class Button : public ButtonBase, public EventListener {
public:
	Button() : _event(debounceTimerCount)
    {
        _bit = true; // Activate pullup
        System::startEventTimer(&_event);
    }
    
    // EventListener override
    virtual void handleEvent(EventType type, EventParam param)
    {
        if (type != EV_EVENT_TIMER || &_event != (TimerEvent*) param)
            return;
            
        _handleEvent(!_bit, numDebounceTests);
    }
    
private:
    Bit _bit;
    RepeatingTimerEvent _event;
};

class ButtonSetBase
{
public:
    virtual void handleEvent(EventType, EventParam) = 0;
    virtual bool state(uint8_t b) const = 0;
    virtual uint8_t readButton(bool peek = false) = 0;

protected:
    static bool _handleEvent(bool newValue, bool& currentValue, bool& debounceValue, uint8_t& debounceTestsRemaining, uint8_t numDebounceTests);
};

template<class... Bits>
class ButtonSet : public ButtonSetBase, public EventListener
{
    static const uint8_t _numButtons = sizeof...(Bits);
    static const uint8_t _debounceTimerCount = 10;
    static const uint8_t _numDebounceTests = 5;
    
public:
    ButtonSet() : _event(_debounceTimerCount)
    {
        memset(_state, 0, (_numButtons + 7) / 8);
        addButton<0, Bits...>();
        System::startEventTimer(&_event);
    }
    
    uint8_t numButtons() const { return _numButtons; }
    uint8_t port(uint8_t i) const
    {
        ASSERT(i < _numButtons, AssertButtonOutOfRange);
        return _bits[i].port();
    }
    uint8_t pin(uint8_t i) const
    {
        ASSERT(i < _numButtons, AssertButtonOutOfRange);
        return _bits[i].pin();
    }

    virtual void handleEvent(EventType type, EventParam param) override
    {
        switch(type) {
            case EV_EVENT_TIMER:
                if (&_event != (TimerEvent*) param) {
                    return;
                }
                for (uint8_t i = 0; i < _numButtons; ++i) {
                    if (_handleEvent(!_bits[i], _value[i], _debounceValue[i], _debounceTestsRemaining[i], _numDebounceTests)) {
                        System::handleEvent(_value[i] ? EV_BUTTON_DOWN : EV_BUTTON_UP, this);
                        setState(i, _value[i]);
                        _lastButton = i;
                    }
                }
                break;
            default: break;
        }
    }
    
    virtual bool state(uint8_t b) const override
    {
        ASSERT(b < _numButtons, AssertButtonOutOfRange);
        return _state[b / 8] & (1 << (b % 8));
    }
    
    virtual uint8_t readButton(bool peek = false) override
    {
        uint8_t retval = _lastButton;
        if (!peek) {
            _lastButton = NoButton;
        }
        return retval;
    }

private:
    template<uint8_t n, class AddBit, class... AddBits>
    void addButton()
    {
        ASSERT(n < _numButtons, AssertButtonTooMany);
        _bits[n] = AddBit();
        _bits[n] = true; // Activate pullup
        _debounceTestsRemaining[n] = 0;
        _debounceValue[n] = false;
        _value[n] = false;
        addButton<n + 1, AddBits...>();
    }
    
    void setState(uint8_t i, bool state)
    {
        if (state) {
            _state[i / 8] |= 1 << (i % 8);
        } else {
            _state[i / 8] &= ~(1 << (i % 8));
        }
    }
    
    template<uint8_t n> void addButton() { }
    
    RepeatingTimerEvent _event;

    uint8_t _state[(_numButtons + 7) / 8];
    DynamicInputBitBase _bits[_numButtons];
    uint8_t _debounceTestsRemaining[_numButtons];
    bool _debounceValue[_numButtons];
    bool _value[_numButtons];

    uint8_t _lastButton = NoButton;
};

template<uint8_t rows, uint8_t cols>
class ButtonMatrix : public ButtonSetBase
{
};

}
