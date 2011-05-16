//
//  Application.cpp
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

#include <string.h>

#include "m8r/Application.h"

using namespace m8r;

Application Application::m_application;

Application& Application::application()
{
    return m_application;
}

void
Application::addEvent(EventType type)
{
    cli();
    m_eventQueue[m_eventQueueTail++] = type;
    if (m_eventQueueTail >= EVENT_QUEUE_SIZE)
        m_eventQueueTail = 0;
        
    // If we have filled the queue, don't reenable interrupts
    // But set a flag and signal an error. The flag will get
    // cleared and we will reenable interrupts when we remove
    // an item from the queue
    if (m_eventQueueHead == m_eventQueueTail) {
        setErrorCondition(ERROR_EVENT_OVERRUN, true);
        m_eventOverrunError = true;
    } else
        sei();
}

void
Application::run()
{
    while (1) {
        cli();
        if (m_eventQueueHead != m_eventQueueTail || m_eventOverrunError) {
            EventType event = m_eventQueue[m_eventQueueHead++];
            if (m_eventQueueHead >= EVENT_QUEUE_SIZE)
                m_eventQueueHead = 0;
            if (m_eventOverrunError) {
                setErrorCondition(ERROR_EVENT_OVERRUN, false);
                m_eventOverrunError = false;
            }
            sei();
            
            processEvent(event);
        }
        else {
            sei();
            processEvent(EV_IDLE);
            wait();
        }
    }
}

void * operator new(size_t size) 
{ 
    return malloc(size); 
} 

void operator delete(void * ptr) 
{ 
  free(ptr); 
}

extern "C" {
void _main() __attribute__((noreturn));
void _main()
{
    Application::application().run();
}
}

