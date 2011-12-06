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

#include "Application.h"

#include "EventListener.h"
#include "Network.h"
#include "TimerEventMgr.h"
#include <string.h>

using namespace m8r;

NetworkBase* Application::m_networkHead = 0;
EventListener* Application::m_eventListenerHead = 0;
ErrorReporter* Application::m_errorReporter = 0;
TimerEventMgrBase* Application::m_timerEventMgr = 0;

void
Application::run()
{
    while (1) {
        handleEvent(EV_IDLE);
        
        for (NetworkBase* network = m_networkHead; network; network = network->next())
            network->handlePackets();
        wait();
    }
}

void
Application::addEventListener(EventListener* listener)
{
    listener->setNext(m_eventListenerHead);
    m_eventListenerHead = listener;
}

void
Application::removeEventListener(EventListener* listener)
{
    for (EventListener *prev = 0, *current = m_eventListenerHead; current; prev = current, current = current->next())
        if (current == listener) {
            if (prev)
                prev->setNext(current->next());
            else
                m_eventListenerHead = current->next();
        }
}

void
Application::handleEvent(EventType type, EventParam param)
{
    for (EventListener* listener = m_eventListenerHead; listener; listener = listener->next())
        listener->handleEvent(type, param);
}

void
Application::addNetwork(NetworkBase* network)
{
    network->setNext(m_networkHead);
    m_networkHead = network;
}
    
void
Application::removeNetwork(NetworkBase* network)
{
    for (NetworkBase *prev = 0, *current = m_networkHead; current; prev = current, current = current->next())
        if (current == network) {
            if (prev)
                prev->setNext(current->next());
            else
                m_networkHead = current->next();
        }
}

void
Application::startEventTimer(uint16_t count)
{
    ASSERT(m_timerEventMgr, AssertNoTimerEventMgr);
    m_timerEventMgr->start(count);
}

void
Application::stopEventTimer(TimerID id)
{
    ASSERT(m_timerEventMgr, AssertNoTimerEventMgr);
    m_timerEventMgr->stop(id);
}

void operator delete(void * ptr) 
{ 
  FATAL(AssertDeleteNotSupported);
}

extern "C" {
void __cxa_pure_virtual()
{
    FATAL(AssertPureVirtual);
}

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

