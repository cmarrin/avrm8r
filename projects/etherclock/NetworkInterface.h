//
//  NetworkInterface.h
//  etherclock
//
//  Created by Chris Marrin on 11/24/11.
//  Copyright (c) 2011 Apple. All rights reserved.
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

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: NetworkInterface
//
//  Abstract interface for network hardware
//
//////////////////////////////////////////////////////////////////////////////

class NetworkInterface {
public:
    NetworkInterface(const uint8_t macaddr[6])
    {
        m_macaddr[0] = macaddr[0];
        m_macaddr[1] = macaddr[1];
        m_macaddr[2] = macaddr[2];
        m_macaddr[3] = macaddr[3];
        m_macaddr[4] = macaddr[4];
        m_macaddr[5] = macaddr[5];
    }
    
    virtual ~NetworkInterface() { }
    
    virtual void sendPacket(uint16_t len, uint8_t* packet) = 0;
    virtual uint16_t receivePacket(uint16_t maxlen, uint8_t* packet) = 0;
    virtual bool hasRxPkt(void) = 0;
    virtual bool linkup() = 0;
    
    virtual void phyWrite(uint8_t address, uint16_t data) = 0;
    
    const uint8_t* macaddr() const { return m_macaddr; }
    
private:
    uint8_t m_macaddr[6];
};

}
