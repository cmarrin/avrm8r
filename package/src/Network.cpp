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

#include "System.h"
extern "C" {
#include "enc28j60.h"
#include "ip_arp_udp_tcp.h"
}
#include "Socket.h"
#include <string.h>

using namespace m8r;

Network::Network(const uint8_t macaddr[6], const uint8_t ipaddr[4], const uint8_t gwaddr[4], bool doubleClockRate)
    : m_gwArpState(0)
    , m_dnsState(0)
    , m_dnsLookupCounter(0)
    , m_next(0)
{
    memcpy(m_gwip, gwaddr, 4);
    enc28j60Init(const_cast<uint8_t*>(macaddr));
    
    if (doubleClockRate)
        enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
    
    System::usDelay<60>();

    // Magjack leds configuration, see enc28j60 datasheet, page 11
    // LEDB=yellow LEDA=green
    //
    // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
    // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
    enc28j60PhyWrite(PHLCON,0x476);

    init_udp_or_www_server(const_cast<uint8_t*>(macaddr), const_cast<uint8_t*>(ipaddr));
    
    System::startEventTimer(&_timerEvent);
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
    uint8_t allxff[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    send_udp(m_packetBuffer, (char*) const_cast<uint8_t*>(data), length, srcPort, const_cast<uint8_t*>(dstIP), dstPort, allxff);
}

void Network::arpResolverResultCallback(uint8_t *ip, void* userdata, uint8_t* mac)
{
    memcpy(static_cast<Network*>(userdata)->m_gwmac, mac, 6);
}

void
Network::handleEvent(EventType type, EventParam param)
{
    if (type != EV_EVENT_TIMER || &_timerEvent != (TimerEvent*) param)
        return;
        
    System::startEventTimer(&_timerEvent);

    m_packetLength = enc28j60PacketReceive(PacketBufferSize, m_packetBuffer);
    uint16_t dataStartOffset = packetloop_arp_icmp_tcp(m_packetBuffer, m_packetLength);

    if (!m_packetLength) {
        if (m_gwArpState == 0) {
            get_mac_with_arp(m_gwip, this, &arpResolverResultCallback);
            m_gwArpState = 1;
        }
        if (get_mac_with_arp_wait() == 0 && m_gwArpState == 1)
            m_gwArpState = 2;

        for (Socket* socket = m_socketHead; socket; socket = socket->next())
            if (socket->waitingForSendData())
                socket->handlePacket(Socket::EventSendDataReady, 0);
        return;
    }
    
    for (Socket* socket = m_socketHead; socket; socket = socket->next())
        if (socket->handlePacket(Socket::EventDataReceived, &m_packetBuffer[dataStartOffset]))
            break;
}


