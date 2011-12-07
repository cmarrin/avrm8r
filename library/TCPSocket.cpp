//
//  TCPSocket.cpp
//  etherclock
//
//  Created by Chris Marrin on 11/24/11.
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

#include "TCPSocket.h"

#include "net.h"

using namespace m8r;

TCPSocket::TCPSocket(NetworkBase* network, SocketPacketCallback callback, void* data)
    : Socket(network, callback, data)
{
}

void
TCPSocket::send(const uint8_t* data, uint16_t length)
{
    // FIXME: Implement
}

bool
TCPSocket::handlePacket(SocketEventType type, const uint8_t* data)
{
    // FIXME: This needs to be completely implemented
    
    // FIXME: Need to set the length. Do we handle split packets?
    uint16_t length = 0;
    length |= 0;

    uint16_t port = ((uint16_t) data[UDP_TCP_DST_PORT_P]) << 8;
    port |= data[UDP_TCP_DST_PORT_P + 1];
            

    if (data[IP_PROTO_P] != IP_PROTO_TCP_V || !m_port || port != m_port)
        return false;
    
    // FIXME: Where is the real payload?
    if (m_callback)
        m_callback(this, type, &data[TCP_OPTIONS_P], length, m_data);
        
    return true;
}
