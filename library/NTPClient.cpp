//
//  Socket.cpp
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

#include "NTPClient.h"

#include <string.h>

using namespace m8r;

uint32_t NTPClient::m_unixTime = 0;

const uint8_t ntpipaddr[4] = { 96, 44, 157, 90 };
const char ntpreqhdr[] = { 0xe3, 0, 4, 0xfa, 0, 1, 0, 0, 0, 1};

static void
ntpCallback(Socket* socket, Socket::EventType type, const uint8_t* buffer, uint16_t length, void* data)
{
    if (type == Socket::EventSendDataReady) {
        uint8_t buf[48];
        memset(buf, 0, 48);
        buf[0] = 0xe3;
        buf[2] = 4;
        buf[3] = 0xfa;
        buf[5] = 1;
        socket->send(buf, 48);
        return;
    }
    
    // Assume this is the returned NTP data
    // FIXME: Implement
    NOTE(length);
}

NTPClient::NTPClient(NetworkBase* network)
    : m_socket(network, ntpCallback, this)
{
}

void
NTPClient::request()
{
    m_socket.requestSend(ntpipaddr, 123);
}
