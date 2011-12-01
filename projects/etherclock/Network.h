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

namespace m8r {

const uint16_t PacketBufferSize = 250;

enum ChecksumType { CHECKSUM_IP = 0, CHECKSUM_UDP, CHECKSUM_TCP, CHECKSUM_ICMP };

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Network
//
//  Core networking. ARP, IP, TCP and UDP server
//
//////////////////////////////////////////////////////////////////////////////

class NetworkBase {
public:
    NetworkBase(const uint8_t ipaddr[4]);
    
    virtual const uint8_t* macaddr() const = 0;
    
    void sendUdpResponse(uint8_t* data, uint16_t length, uint16_t port);
    
protected:
    virtual void sendPacket(uint16_t len, uint8_t* packet) = 0;
    virtual uint16_t receivePacket(uint16_t maxlen, uint8_t* packet) = 0;
    
    void handlePackets();

private:
    void setChecksum(uint8_t *buf, ChecksumType type, uint16_t len = 0);
    
    bool isMyArpPacket() const;
    bool isMyIpPacket() const;
    
    void setEthernetResponseHeader();
    void setIPResponseHeader();
    
    void respondToArp();
    void respondToPing();
    
    uint8_t m_ipaddr[4];
    uint8_t m_packetBuffer[PacketBufferSize + 1];
    uint16_t m_packetLength;
};

template <class NetworkInterface>
class Network : public NetworkBase {
public:
    Network(NetworkInterface* interface, const uint8_t ipaddr[4])
        : NetworkBase(ipaddr)
        , m_interface(interface)
    { }
        
protected:
    virtual void sendPacket(uint16_t len, uint8_t* packet) { m_interface->sendPacket(len, packet); }
    virtual uint16_t receivePacket(uint16_t maxlen, uint8_t* packet) { return m_interface->receivePacket(maxlen, packet); }
    virtual const uint8_t* macaddr() const { return m_interface->macaddr(); }
private:
    NetworkInterface* m_interface;
};

}
