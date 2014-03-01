//
//  Network.h
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

#include "EventListener.h"

namespace m8r {

const uint16_t PacketBufferSize = 220;
const uint16_t NetworkTimerInterval = 10; // ms
const uint16_t DNSLookupTimeout = 60U * 1000U / NetworkTimerInterval; // 60 seconds with a 10ms event rate

enum NetworkInterfaceError { NetworkInterfaceNoError, NetworkInterfaceTransmitError, NetworkInterfaceReceiveError };
enum ChecksumType { CHECKSUM_IP = 0, CHECKSUM_UDP, CHECKSUM_TCP, CHECKSUM_ICMP };

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Network
//
//  Core networking. ARP, IP, TCP and UDP server
//
//////////////////////////////////////////////////////////////////////////////

class Socket;

class Network : public EventListener {
public:
    Network(const uint8_t macaddr[6], const uint8_t ipaddr[4], const uint8_t gwaddr[4], bool doubleClockRate);

    static uint16_t _INLINE_ _swap(uint16_t value)
    {
        uint16_t result;
        __asm__ volatile (
            "mov __tmp_reg__, %A0"  "\n\t"
             "mov %A0, %B0"         "\n\t"
             "mov %B0, __tmp_reg__" "\n\t"
             : "=r" (result)
             : "0" (value)
        );
        return result;
    }

    static uint16_t _INLINE_ htons(uint16_t v) { return _swap(v); }
    static uint16_t _INLINE_ ntohs(uint16_t v) { return _swap(v); }

    // EventListener override
    virtual void handleEvent(::EventType, EventParam);
    
    void setNext(Network* next) { m_next = next; }
    Network* next() const { return m_next; }
        
    void addSocket(Socket*);
    void removeSocket(Socket*);
    
    uint8_t* packetBuffer() { return m_packetBuffer; }
    uint16_t packetBufferSize() const { return PacketBufferSize; }
    
    void respondUDP(const uint8_t* data, uint8_t length, uint16_t port);
    void sendUDP(const uint8_t* data, uint8_t length, uint16_t srcPort, const uint8_t* dstIP, uint16_t dstPort);

private:
    void notifyReady();
    static void arpResolverResultCallback(uint8_t *ip, void* userdata, uint8_t* mac);
    
    uint8_t m_packetBuffer[PacketBufferSize + 1];
    uint16_t m_packetLength;
    uint8_t m_gwArpState;
    uint8_t m_dnsState;
    uint8_t m_gwip[4];
    uint8_t m_gwmac[6];
    
    uint16_t m_dnsLookupCounter;

    Network* m_next;
    Socket* m_socketHead;
    
    TimerID m_timerID;    
};

}
