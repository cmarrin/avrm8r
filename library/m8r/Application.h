//
//  Application.h
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

#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>
#include "m8r.h"
#include "m8r/Event.h"

// Setup for C++ operation
void * operator new(size_t size); 
void operator delete(void * ptr); 

namespace m8r {

#define EVENT_QUEUE_SIZE 10

enum ErrorType {
    ERROR_USER = 10
};

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Application
//
//  Main application for AVR apps. Singleton
//
//////////////////////////////////////////////////////////////////////////////

    
class Application {
public:
    Application()
    : m_eventOnIdle(false)
    {
        m_application = this;
    }
    
    static Application& application() { return *m_application; }
    
    virtual void processEvent(EventType, uint8_t identifier = 0) { }
    virtual void setErrorCondition(ErrorType, bool raise) { }
    
    void run() __attribute__((noreturn));
    
    
    void setEventOnIdle(bool b) { m_eventOnIdle = b; }
    
    // Delays of 16ms are possible down to F_CPU of 1MHz
    static void usDelay(uint16_t us)
    {
        _delay_loop_2((uint32_t) us * 4000000 / F_CPU);
    }
    
    static void msDelay(uint16_t ms)
    {
        for ( ; ms > 0; --ms)
            usDelay(1000);
    }
    
private:
    void wait()
    {
    }
    
    static Application* m_application;
    
    bool m_eventOnIdle;
};

}
