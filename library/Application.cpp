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

#include <string.h>

#include "m8r/Application.h"

#include "m8r/EventListener.h"

using namespace m8r;

ErrorConditionHandler* Application::m_errorConditionHandler = 0;
bool Application::m_eventOnIdle = 0;
IdleEventListener* Application::m_idleEventListeners = 0;
Event* Application::m_eventHead = 0;

void
Application::removeIdleEventListener(IdleEventListener* listener)
{
    if (listener == m_idleEventListeners) {
        m_idleEventListeners = listener->m_next;
        return;
    }
    
    for (IdleEventListener* currentListener = m_idleEventListeners; ; currentListener = currentListener->m_next) {
        if (!currentListener->m_next)
            return;
        if (currentListener->m_next == listener) {
            currentListener->m_next = listener->m_next;
            return;
        }
    }
}

void
Application::run()
{
    while (1) {
        cli();
        
        if (m_eventHead) {
            Event* event = m_eventHead;
            m_eventHead = 0;
            sei();
            
            for ( ; event; event = event->m_next) {
                event->m_listener->handleEvent(event->m_type, event->m_identifier);
                event->m_active = false;
            }
        }
        else
            sei();

        for (IdleEventListener* listener = m_idleEventListeners; listener; listener = listener->m_next)
            listener->handleIdleEvent();
            
        wait();
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
void __cxa_pure_virtual() { while (1); }

#ifdef DEBUG
void _showErrorCondition(uint8_t code, ErrorConditionType condition)
{
    Application::handleErrorCondition((ErrorType) code, condition);
}
#endif

void _main() __attribute__((noreturn));
void _main()
{
    Application::run();
}
}

