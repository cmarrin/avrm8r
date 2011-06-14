//
//  Event.h
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
#include "EventSourceEnums.h"

namespace m8r {


//////////////////////////////////////////////////////////////////////////////
//
//  Class: Event
//
//  Linked list of event objects
//
//////////////////////////////////////////////////////////////////////////////

class Event {
public:
	static void _INLINE_ add(EventType type, uint8_t identifier = 0)
    {
        Event* event = alloc(type, identifier);
        event->m_next = m_head;
        m_head = event;
    }
    
    static void processAllEvents();
    
private:
	Event(EventType type, uint8_t identifier) 
        : m_type(type)
        , m_identifier(identifier)
        , m_next(0)	{ }
	
    static Event* _INLINE_ alloc(EventType type, uint8_t identifier)
    {
        if (m_free) {
            Event* event = m_free;
            m_free = event->m_next;
            return event;
        }
        
        return new Event(type, identifier);
    }
    
    EventType m_type;
    uint8_t m_identifier;
    Event* m_next;
    
    static Event* m_head;
    static Event* m_free;
};

}
