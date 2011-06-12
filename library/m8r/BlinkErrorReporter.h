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

#pragma once

#include "m8r.h"
#include "m8r/Application.h"

namespace m8r {


//////////////////////////////////////////////////////////////////////////////
//
//  Class: BlinkErrorReporter
//
//  Blinks out error codes. One long dash for every 5 and 1 short dot for
//  every 1. So a code of 12 would be long, long, short, short. Delay
//  1s after each set of blinks and 2 seconds after the last.
//
//////////////////////////////////////////////////////////////////////////////

template <class ErrorPort, uint8_t ErrorBit>
class BlinkErrorReporter {
public:
	BlinkErrorReporter()
        : m_hang(true)
    {
        m_errorPort.setPortBit(ErrorBit);
        m_errorPort.setBitOutput(ErrorBit);
        setError(false);
    }
    
    void hangOnError(bool hang) { m_hang = hang; }
	
    // Blink out the code 3 times then hang if 'hang' is true, otherwise return
    void reportError(uint8_t code)
    {
        for (uint8_t i = 0; m_hang || i < 3; ++i) {
            uint8_t j = code;
            for ( ; j >= 5; j -= 5)
                blink(1000);
            for ( ; j > 0; --j)
                blink(250);
            Application::application().msDelay(2000);
        }
        Application::application().msDelay(2000);
    }
    
private:
    void blink(uint16_t duration)
    {
        setError(true);
        Application::application().msDelay(duration);
        setError(false);
        Application::application().msDelay(250);
    }
    
    void setError(bool error)
    {
        if (error)
            m_errorPort.clearPortBit(ErrorBit);
        else
            m_errorPort.setPortBit(ErrorBit);
    }
    
    ErrorPort m_errorPort;
    bool m_hang;
};

}
