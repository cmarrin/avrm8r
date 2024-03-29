//
//  Button.cpp
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

#include "Button.h"

#include "System.h"

using namespace m8r;

void ButtonBase::_handleEvent(bool value, uint8_t numDebounceTests)
{
    if (value == m_value) {
        m_debounceValue = value;
        return;
    }
    
    if (m_debounceValue == value) {
        if (--m_debounceTestsRemaining == 0) {
            m_value = value;
            System::handleEvent(value ? EV_BUTTON_DOWN : EV_BUTTON_UP, this);
        }
        return;
    }

    m_debounceTestsRemaining = numDebounceTests;
    m_debounceValue = value;
}

bool ButtonSetBase::_handleEvent(bool newValue, bool& currentValue, bool& debounceValue, uint8_t& debounceTestsRemaining, uint8_t numDebounceTests)
{
    if (newValue == currentValue) {
        debounceValue = newValue;
        return false;
    }
    
    if (debounceValue == newValue) {
        if (--debounceTestsRemaining == 0) {
            currentValue = newValue;
            return true;
        }
        return false;
    }

    debounceTestsRemaining = numDebounceTests;
    debounceValue = newValue;
    return false;
}
