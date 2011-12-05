//
//  Socket.h
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

#pragma once

#include "m8r.h"

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Socket
//
//  Network socket for IP based comm.
//
//////////////////////////////////////////////////////////////////////////////

enum SocketEventType {
    SocketEventDataReceived,
    SocketEventDataDelivered,
    SocketEventNewConnection,
    SocketEventRetransmit
};

class Socket;

typedef void (*SocketPacketCallback)(Socket*, SocketEventType, const uint8_t* data, uint16_t length);

class NetworkBase;

class Socket {
public:
    Socket(NetworkBase*, SocketPacketCallback);

    void listen(uint16_t port) { m_port = port; }
    
    // Send can only be called from a handlePacket function and uses the current source address and port
    virtual void send(const uint8_t* data, uint16_t length) = 0;
    virtual void sendto(const uint8_t ipaddr[4], uint16_t port, const uint8_t* data, uint16_t length) = 0;
    
    virtual bool handlePacket(SocketEventType, const uint8_t* data) = 0;

    void setNext(Socket* next) { m_next = next; }
    Socket* next() const { return m_next; }
    
protected:
    uint16_t m_port;
    SocketPacketCallback m_callback;
    NetworkBase* m_network;
    
private:
    Socket* m_next;
};

}
