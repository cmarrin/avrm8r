//
//  Network.cpp
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

#include "Network.h"

#include "Application.h"
extern "C" {
#include "ip_arp_udp_tcp.h"
}
#include "Socket.h"
#include <string.h>

using namespace m8r;

Network::Network(const uint8_t macaddr[6], const uint8_t ipaddr[4], const uint8_t gwaddr[4])
    : m_next(0)
    , m_timerID()
{
    init_ip_arp_udp_tcp(const_cast<uint8_t*>(macaddr), const_cast<uint8_t*>(ipaddr), 0);
    client_set_gwip(const_cast<uint8_t*>(gwaddr));
    
    m_timerID = Application::startEventTimer(NetworkTimerInterval);
}

void
Network::addSocket(Socket* socket)
{
    socket->setNext(m_socketHead);
    m_socketHead = socket;
}
    
void
Network::removeSocket(Socket* socket)
{
    for (Socket *prev = 0, *current = m_socketHead; current; prev = current, current = current->next())
        if (current == socket) {
            if (prev)
                prev->setNext(current->next());
            else
                m_socketHead = current->next();
        }
}

void
Network::respondUDP(const uint8_t* data, uint8_t length, uint16_t port)
{
    make_udp_reply_from_request(m_packetBuffer, (char*) const_cast<uint8_t*>(data), length, port);
}

void
Network::sendUDP(const uint8_t* data, uint8_t length, uint16_t srcPort, const uint8_t* dstIP, uint16_t dstPort)
{
    send_udp(m_packetBuffer, (char*) const_cast<uint8_t*>(data), length, srcPort, const_cast<uint8_t*>(dstIP), dstPort);
}

void
Network::handleEvent(::EventType type, EventParam param)
{
/*
    if (type != EV_EVENT_TIMER || m_timerID != MakeTimerID(param))
        return;
        
    m_timerID = Application::startEventTimer(NetworkTimerInterval);    

#ifdef DEBUG
    NetworkInterfaceError error = checkError();
    ASSERT(error != NetworkInterfaceTransmitError, AssertEthernetTransmitError);
    ASSERT(error != NetworkInterfaceReceiveError, AssertEthernetReceiveError);
#endif

    if (m_state == StateNeedToRequestGWMacAddr) {
        setGatewayIPAddress(m_gatewayIPAddress);
        return;
    }
    
    m_packetLength = receivePacket(PacketBufferSize, m_packetBuffer);
    if (!m_packetLength) {
        if (m_state != StateReady)
            return;
            
        for (Socket* socket = m_socketHead; socket; socket = socket->next())
            if (socket->waitingForSendData())
                socket->handlePacket(Socket::EventSendDataReady, 0);
    
        return;
    }
    
    if (isMyArpPacket()) {
        if (m_packetBuffer[ETH_ARP_OPCODE_P] == (ETH_ARP_OPCODE_REQ_V >> 8) && 
                m_packetBuffer[ETH_ARP_OPCODE_P + 1] == (ETH_ARP_OPCODE_REQ_V & 0xff))
            respondToArp();
        else if (m_state == StateWaitForGWMacAddr && memcmp(&m_packetBuffer[ETH_ARP_SRC_IP_P], m_gatewayIPAddress, 4) == 0 && 
                m_packetBuffer[ETH_ARP_OPCODE_P] == (ETH_ARP_OPCODE_REPLY_V >> 8) && 
                m_packetBuffer[ETH_ARP_OPCODE_P + 1] == (ETH_ARP_OPCODE_REPLY_V & 0xff)) {
            // We have our gw mac addr
            memcpy(m_gatewayMACAddress, &m_packetBuffer[ETH_ARP_SRC_MAC_P], 6);
            notifyReady();
        }
    } else if (isMyIpPacket()) {
        if (m_packetBuffer[IP_PROTO_P] == IP_PROTO_ICMP_V && m_packetBuffer[ICMP_TYPE_P] == ICMP_TYPE_ECHOREQUEST_V)
            respondToPing();
        else {
            for (Socket* socket = m_socketHead; socket; socket = socket->next()) {
                if (socket->handlePacket(Socket::EventDataReceived, m_packetBuffer))
                    break;
            }
        }
    }
    */
}


