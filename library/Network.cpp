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
#include "Socket.h"
#include "net.h"
#include <string.h>

using namespace m8r;

const char arpreqhdr[] = { 0, 1, 8, 0, 6, 4, 0, 1 };
const char iphdr[] = { 0x45, 0, 0, 0x82, 0, 0, 0x40, 0, 0x40 };

NetworkBase::NetworkBase(const uint8_t macaddr[6], const uint8_t ipaddr[4], const uint8_t gwaddr[4])
    : m_ipID(2)
    , m_next(0)
    , m_socketHead(0)
    , m_state(StateNeedToRequestGWMacAddr)
{
    memcpy(m_macAddress, macaddr, 6);
    memcpy(m_ipAddress, ipaddr, 4);
    memcpy(m_gatewayIPAddress, gwaddr, 4);
    memset(m_gatewayMACAddress, 0, 6);

    Application::addNetwork(this);
}

void
NetworkBase::setGatewayIPAddress(const uint8_t* gwaddr)
{
    memcpy(m_gatewayIPAddress, gwaddr, 4);
    sendArp(m_gatewayIPAddress);
    m_state = StateWaitForGWMacAddr;
}

void
NetworkBase::addSocket(Socket* socket)
{
    socket->setNext(m_socketHead);
    m_socketHead = socket;
}
    
void
NetworkBase::removeSocket(Socket* socket)
{
    for (Socket *prev = 0, *current = m_socketHead; current; prev = current, current = current->next())
        if (current == socket) {
            if (prev)
                prev->setNext(current->next());
            else
                m_socketHead = current->next();
        }
}

// The Ip checksum is calculated over the ip header only starting
// with the header length field and a total length of 20 bytes
// unitl ip.dst
// You must set the IP checksum field to zero before you start
// the calculation.
// len for ip is 20.
//
// For UDP/TCP we do not make up the required pseudo header. Instead we 
// use the ip.src and ip.dst fields of the real packet:
// The udp checksum calculation starts with the ip.src field
// Ip.src=4bytes,Ip.dst=4 bytes,Udp header=8bytes + data length=16+len
// You must set the upd checksum field to zero before you start
// the calculation.
// len for udp is: 8 + 8 + data length
//
// For more information on how this algorithm works see:
// http://www.netfor2.com/checksum.html
// http://www.msc.uky.edu/ken/cs471/notes/chap3.htm
// The RFC has also a C code example: http://www.faqs.org/rfcs/rfc1071.html
void
NetworkBase::setChecksum(ChecksumType type, uint16_t len)
{
    const uint16_t extraChecksumLength = (ETH_HEADER_LEN + IP_HEADER_LEN) - IP_SRC_P;

    uint32_t sum = 0;
    uint16_t checksumLocation;
    uint8_t* buf;
    
    switch(type) {
        case CHECKSUM_IP:
            len += IP_HEADER_LEN;
            buf = &m_packetBuffer[IP_P];
            checksumLocation = IP_CHECKSUM_P;
            break;
        case CHECKSUM_UDP:
            // For UDP checksum is from IP_SRC_P
            len += UDP_HEADER_LEN;
            sum += IP_PROTO_UDP_V + len;
            len += extraChecksumLength;
            buf = &m_packetBuffer[IP_SRC_P];
            checksumLocation = UDP_CHECKSUM_P;
            break;
        case CHECKSUM_TCP:
            len += extraChecksumLength + TCP_HEADER_LEN_PLAIN;
            sum += IP_PROTO_TCP_V + len - extraChecksumLength;
            buf = &m_packetBuffer[IP_SRC_P];
            checksumLocation = TCP_CHECKSUM_P;
            break;
        case CHECKSUM_ICMP:
            len += ICMP_DATA_P - ICMP_TYPE_P;
            
            // FIXME: This is probably wrong
            buf = &m_packetBuffer[ICMP_DATA_P];
            checksumLocation = ICMP_CHECKSUM_P;
            break;
        default:
            return;
    }

    // Clear the checksum before computing a new one
    m_packetBuffer[checksumLocation]=0;
    m_packetBuffer[checksumLocation + 1]=0;

    while (len > 1) {
        sum += 0xFFFF & (*buf << 8 | *(buf + 1));
        buf += 2;
        len -= 2;
    }
    
    // if there is a byte left then add it (padded with zero)
    if (len)
        sum += (0xFF & *buf) << 8;

    // now calculate the sum over the bytes in the sum
    // until the result is only 16bit long
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    // build 1's complement:
    uint16_t ck = (uint16_t) sum ^ 0xFFFF;
    m_packetBuffer[checksumLocation] = ck >> 8;
    m_packetBuffer[checksumLocation + 1] = ck & 0xff;
}

bool
NetworkBase::isMyArpPacket() const
{
    if (m_packetLength < ETH_HEADER_LEN + ETH_ARP_HEADER_LEN)
        return false;

    if (m_packetBuffer[ETH_TYPE_P] != (ETHTYPE_ARP_V >> 8) || m_packetBuffer[ETH_TYPE_P + 1] != (ETHTYPE_ARP_V & 0xff))
        return false;
    
    return memcmp(&m_packetBuffer[ETH_ARP_DST_IP_P], m_ipAddress, 4) == 0;
}

bool
NetworkBase::isMyIpPacket() const
{
    if (m_packetLength < ETH_HEADER_LEN + IP_HEADER_LEN + UDP_HEADER_LEN)
        return false;

    if (m_packetBuffer[ETH_TYPE_P] != (ETHTYPE_IP_V >> 8) || m_packetBuffer[ETH_TYPE_P + 1] != (ETHTYPE_IP_V & 0xff))
        return false;
    
    return memcmp(&m_packetBuffer[IP_DST_P], m_ipAddress, 4) == 0;
}

void
NetworkBase::setEthernetMacAddresses(const uint8_t* destMacAddr)
{
    if (!destMacAddr)
        destMacAddr = m_gatewayMACAddress;
        
    memcpy(&m_packetBuffer[ETH_DST_MAC], destMacAddr, 6);
    memcpy(&m_packetBuffer[ETH_SRC_MAC], m_macAddress, 6);
}

void 
NetworkBase::setIPHeader(uint8_t ipType, const uint8_t* destIPAddr, uint16_t length)
{
    uint16_t swappedLength = htons(length);
    
    setEthernetMacAddresses();
    
    memcpy(&m_packetBuffer[IP_DST_P], destIPAddr, 4);
    memcpy(&m_packetBuffer[IP_SRC_P], m_ipAddress, 4);
    memcpy(&m_packetBuffer[IP_P], iphdr, sizeof(iphdr));
    
    m_packetBuffer[IP_ID_P + 1] = m_ipID++;
    
    uint16_t headerLength = length + IP_HEADER_LEN + UDP_HEADER_LEN;
    m_packetBuffer[IP_TOTLEN_P] = headerLength >> 8;
    m_packetBuffer[IP_TOTLEN_P + 1] = headerLength & 0xff;

    m_packetBuffer[IP_PROTO_P] = ipType;
    
    setChecksum(CHECKSUM_IP, length);
}

void 
NetworkBase::setIPResponseHeader()
{
    // Assume packet buffer contains a packet we want to reply to
    setEthernetMacAddresses(&m_packetBuffer[ETH_SRC_MAC]);

    memcpy(&m_packetBuffer[IP_DST_P], &m_packetBuffer[IP_SRC_P], 4);
    memcpy(&m_packetBuffer[IP_SRC_P], m_ipAddress, 4);
    
    m_packetBuffer[IP_FLAGS_P] = 0x40; // don't fragment
    m_packetBuffer[IP_FLAGS_P+1] = 0;  // fragement offset
    m_packetBuffer[IP_TTL_P] = 64;
    
    setChecksum(CHECKSUM_IP);
}

void
NetworkBase::sendArp(const uint8_t destIPAddr[4])
{
    memset(&m_packetBuffer[ETH_DST_MAC], 0xff, 6);
    memcpy(&m_packetBuffer[ETH_SRC_MAC], m_macAddress, 6);

    m_packetBuffer[ETH_TYPE_P] = ETHTYPE_ARP_V >> 8;
    m_packetBuffer[ETH_TYPE_P + 1] = ETHTYPE_ARP_V & 0xff;
    
    memcpy(&m_packetBuffer[ETH_ARP_P], arpreqhdr, sizeof(arpreqhdr));
    
    memcpy(&m_packetBuffer[ETH_ARP_SRC_MAC_P], m_macAddress, 6);
    memset(&m_packetBuffer[ETH_ARP_DST_MAC_P], 0, 6);
    
    memcpy(&m_packetBuffer[ETH_ARP_DST_IP_P], destIPAddr, 4);
    memcpy(&m_packetBuffer[ETH_ARP_SRC_IP_P], m_ipAddress, 4);
    
    sendPacket(ETH_HEADER_LEN + ETH_ARP_HEADER_LEN, m_packetBuffer);
}

void
NetworkBase::respondToArp()
{
    uint8_t i=0;
    //
    setEthernetMacAddresses(&m_packetBuffer[ETH_SRC_MAC]);
    m_packetBuffer[ETH_ARP_OPCODE_P] = ETH_ARP_OPCODE_REPLY_V >> 8;
    m_packetBuffer[ETH_ARP_OPCODE_P + 1] = ETH_ARP_OPCODE_REPLY_V & 0xff;
    
    for (uint8_t i = 0; i < 6; ++i) {
        m_packetBuffer[ETH_ARP_DST_MAC_P + i] = m_packetBuffer[ETH_ARP_SRC_MAC_P + i];
        m_packetBuffer[ETH_ARP_SRC_MAC_P + i] = m_macAddress[i];
    }
    i=0;
    for (uint8_t i = 0; i < 4; ++i) {
        m_packetBuffer[ETH_ARP_DST_IP_P+i] = m_packetBuffer[ETH_ARP_SRC_IP_P+i];
        m_packetBuffer[ETH_ARP_SRC_IP_P+i] = m_ipAddress[i];
    }
    
    sendPacket(ETH_HEADER_LEN + ETH_ARP_HEADER_LEN, m_packetBuffer); 
}

void
NetworkBase::respondToPing()
{
    // Assume incoming packet is an echo we are responding to, so we just have to fix it up
    setIPResponseHeader();
    m_packetBuffer[ICMP_TYPE_P] = ICMP_TYPE_ECHOREPLY_V;
    
    // We changed only the icmp.type field from request(=8) to reply(=0). We can therefore easily correct the checksum:
    if (m_packetBuffer[ICMP_CHECKSUM_P] > (0xff-0x08))
        m_packetBuffer[ICMP_CHECKSUM_P+1]++;

    m_packetBuffer[ICMP_CHECKSUM_P] += 0x08;

    sendPacket(m_packetLength, m_packetBuffer);
}

void
NetworkBase::notifyReady()
{
    m_state = StateReady;
    for (Socket* socket = m_socketHead; socket; socket = socket->next())
        socket->handlePacket(Socket::EventConnectionReady, 0);
}

void
NetworkBase::handlePackets()
{
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
}


