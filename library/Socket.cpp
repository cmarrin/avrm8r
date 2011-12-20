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

#include "Socket.h"

#include "Network.h"
#include "net.h"

using namespace m8r;

Socket::Socket(NetworkBase* network, PacketCallback callback, void* data)
    : m_port(0)
    , m_callback(callback)
    , m_data(data)
    , m_network(network)
    , m_state(StateIdle)
    , m_inHandler(false)
    , m_next(0)
{
    m_network->addSocket(this);
}

void
Socket::requestSend(const uint8_t ipaddr[4], uint16_t port)
{
    m_destinationAddress = ipaddr;
    m_destinationPort = port;
    m_state = StateWaitSendData;
}

void
Socket::requestSend(const char* hostname, uint16_t port)
{
    // FIXME: Implement. See above
}

void
Socket::createSendPacket(uint16_t length)
{
    // FIXME: Consolidate this and createResponsePacket
    m_network->setEthernetMacAddresses();
    
    // FIXME: This should not be UDP_HEADER_LEN here
    length += IP_HEADER_LEN + UDP_HEADER_LEN;
    
    m_network->packetBuffer()[IP_TOTLEN_P] = length >> 8;
    m_network->packetBuffer()[IP_TOTLEN_P + 1] = length;
    m_network->setIPResponseHeader();
    
    // Send to port of sender and use "port" as own source:
    m_network->packetBuffer()[UDP_TCP_DST_PORT_P] = m_network->packetBuffer()[UDP_TCP_SRC_PORT_P];
    m_network->packetBuffer()[UDP_TCP_DST_PORT_P + 1] = m_network->packetBuffer()[UDP_TCP_SRC_PORT_P + 1];
    m_network->packetBuffer()[UDP_TCP_SRC_PORT_P] = m_port >> 8;
    m_network->packetBuffer()[UDP_TCP_SRC_PORT_P + 1] = m_port & 0xff;
}

void
Socket::createResponsePacket(uint16_t length)
{
    ASSERT(m_inHandler, AssertEthernetNotInHandler);
    if (!m_inHandler)
        return;
        
    m_network->setEthernetMacAddresses(&m_network->packetBuffer()[ETH_SRC_MAC]);
    
    // FIXME: This should not be UDP_HEADER_LEN here
    length += IP_HEADER_LEN + UDP_HEADER_LEN;
    
    m_network->packetBuffer()[IP_TOTLEN_P] = length >> 8;
    m_network->packetBuffer()[IP_TOTLEN_P + 1] = length;
    m_network->setIPResponseHeader();
    
    // Send to port of sender and use "port" as own source:
    m_network->packetBuffer()[UDP_TCP_DST_PORT_P] = m_network->packetBuffer()[UDP_TCP_SRC_PORT_P];
    m_network->packetBuffer()[UDP_TCP_DST_PORT_P + 1] = m_network->packetBuffer()[UDP_TCP_SRC_PORT_P + 1];
    m_network->packetBuffer()[UDP_TCP_SRC_PORT_P] = m_port >> 8;
    m_network->packetBuffer()[UDP_TCP_SRC_PORT_P + 1] = m_port & 0xff;
    
}

bool
Socket::handlePacket(EventType type, const uint8_t* data)
{
    if (type == EventSendDataReady) {
        ASSERT(m_state == StateWaitSendData, AssertEthernetNotWaitingToSendData);
        m_state = StateCanSendData;
    }
    
    m_inHandler = true;
    bool result = _handlePacket(type, data);
    m_inHandler = false;
    
    if (type == EventSendDataReady && m_state == StateCanSendData)
        m_state = StateIdle;
        
    return result;
}

