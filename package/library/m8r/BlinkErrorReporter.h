//
//  BlinkErrorReporter.h
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

#include "m8r.h"

#include "Application.h"

namespace m8r {


//////////////////////////////////////////////////////////////////////////////
//
//  Class: BlinkErrorReporter
//
//  Blinks out error codes. One long dash for every 4 and 1 short dot for
//  every 1. So a code of 10 would be long, long, short, short. Delay
//  1s after each set of blinks and 2 seconds after the last. Zero is a
//  400ms burst of quick pulses.
//
//  ErrorConditionType codes:
//
//      ErrorConditionNote      - Prefix blink code with 1 sec burst of quick 
//                                pulses, and repeat code 3 times then return.
//
//      ErrorConditionWarning   - Repeat blink code 3 times then return.
//
//      ErrorConditionFatal     - Repeat blink code forever.
//
//////////////////////////////////////////////////////////////////////////////

template <class ErrorPort, uint8_t ErrorBit, bool invert>
class BlinkErrorReporter : public ErrorReporter {
public:
	_NO_INLINE_ BlinkErrorReporter()
    {
        m_errorPort.setPortBit(ErrorBit);
        m_errorPort.setBitOutput(ErrorBit);
        setError(false);
    }
        
    void _NO_INLINE_ setError(bool error)
    {
        if (error != invert)
            m_errorPort.setPortBit(ErrorBit);
        else
            m_errorPort.clearPortBit(ErrorBit);
    }
    
    bool isErrorSet() const { return m_errorPort.isPortBit(ErrorBit); }

    // Blink out the code 3 times then repeat if condition == ErrorConditionFatal, otherwise return
    virtual void reportError(char, uint16_t code, ErrorConditionType condition)
    {
        setError(false);
        Application::msDelay<500>();
        for (uint8_t i = 0; condition == ErrorConditionFatal || i < 3; ++i) {
            if (condition == ErrorConditionNote) {
                if (code > 0xff) {
                    flicker(3);
                    Application::msDelay<25>();
                    flicker(3);
                    Application::msDelay<25>();
                    flicker(3);
                }
                else
                    flicker(10);
                    
                Application::msDelay<1000>();
            }
            if (code > 0xff)
                blinkCode(code >> 8);
            blinkCode(code);
            Application::msDelay<1000>();
        }
    }
private:
    void _NO_INLINE_ flicker(uint8_t num)
    {
        while (--num > 0)
            blink<50, 50>();
        Application::msDelay<100>();
    }
    
    template <uint16_t duration, uint16_t delay> void blink()
    {
        setError(true);
            Application::msDelay<duration>();
        setError(false);
            Application::msDelay<delay>();
    }
    
    void _NO_INLINE_ blinkDigit(uint8_t digit)
    {
        if (digit == 0)
            flicker(4);
        
        for ( ; digit >= 4; digit -= 4)
            blink<500, 100>();
        for ( ; digit > 0; --digit)
            blink<100, 100>();
        Application::msDelay<1000>();
    }
    
    void _NO_INLINE_ blinkCode(uint8_t code)
    {
        blinkDigit(code >> 4);
        blinkDigit(code & 0x0f);
    }
    
    ErrorPort m_errorPort;
};

}
