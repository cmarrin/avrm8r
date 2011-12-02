//
//  ENC28J60.cpp
//
//  Created by Chris Marrin on 6/30/2011.
//
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

#include "ENC28J60.h"

#include "Application.h"
#include <string.h>

#define CSACTIVE SPI_PORT &= ~_BV(SS_BIT)
#define CSPASSIVE SPI_PORT |= _BV(SS_BIT)

using namespace m8r;

ENC28J60Base::ENC28J60Base(const uint8_t* macaddr, ENC28J60ClockOutType clockOut, uint8_t spcr, uint8_t spsr)
    : m_spi(spcr, spsr)
    , m_bank(0xff)
    , m_nextPacketPtr(RXSTART_INIT)
{
	CSPASSIVE;
    
    // Set clkout to 2 (12.5MHz)
    write(ECOCON, clockOut & 0x7);
	
    writeOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
    Application::msDelay<20>();

    // Rx start
	write(ERXSTL, RXSTART_INIT & 0xFF);
	write(ERXSTH, RXSTART_INIT >> 8);
    
	// set receive pointer address
	write(ERXRDPTL, RXSTART_INIT & 0xFF);
	write(ERXRDPTH, RXSTART_INIT >> 8);
    
	// RX end
	write(ERXNDL, RXSTOP_INIT & 0xFF);
	write(ERXNDH, RXSTOP_INIT >> 8);
    
	// TX start
	write(ETXSTL, TXSTART_INIT & 0xFF);
	write(ETXSTH, TXSTART_INIT >> 8);
    
	// TX end
	write(ETXNDL, TXSTOP_INIT & 0xFF);
	write(ETXNDH, TXSTOP_INIT >> 8);
    
	// do bank 1 stuff, packet filter:
    //      For broadcast packets we allow only ARP packtets
    //      All other packets should be unicast only for our mac (MAADR)
    //
    //      The pattern to match on is therefore
    //      Type     ETH.DST
    //      ARP      BROADCAST
    //      06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
    //      in binary these poitions are:11 0000 0011 1111
    //      This is hex 303F->EPMM0=0x3f,EPMM1=0x30
	write(ERXFCON, ERXFCON_UCEN | ERXFCON_CRCEN | ERXFCON_PMEN);
	write(EPMM0, 0x3f);
	write(EPMM1, 0x30);
	write(EPMCSL, 0xf9);
	write(EPMCSH, 0xf7);

	// do bank 2 stuff
	// enable MAC receive
	write(MACON1, MACON1_MARXEN | MACON1_TXPAUS | MACON1_RXPAUS);
    
	// bring MAC out of reset
	write(MACON2, 0x00);
    
	// enable automatic padding to 60bytes and CRC operations
	writeOp(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN);
    
	// set inter-frame gap (non-back-to-back)
	write(MAIPGL, 0x12);
	write(MAIPGH, 0x0C);
    
	// set inter-frame gap (back-to-back)
	write(MABBIPG, 0x12);
    
	// Set the maximum packet size which the controller will accept
    // Do not send packets longer than MAX_FRAMELEN:
	write(MAMXFLL, MAX_FRAMELEN & 0xFF);	
	write(MAMXFLH, MAX_FRAMELEN >> 8);
    
	// do bank 3 stuff
    // write MAC address
    // NOTE: MAC address in ENC28J60 is byte-backward
    memcpy(m_macaddr, macaddr, 6);
    write(MAADR5, macaddr[0]);
    write(MAADR4, macaddr[1]);
    write(MAADR3, macaddr[2]);
    write(MAADR2, macaddr[3]);
    write(MAADR1, macaddr[4]);
    write(MAADR0, macaddr[5]);
    
	// no loopback of transmitted frames
	phyWrite(PHCON2, PHCON2_HDLDIS);
    
	// switch to bank 0
	setBank(ECON1);
    
	// enable interrutps
	writeOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE | EIE_PKTIE);
    
	// enable packet reception
	writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
}

void
ENC28J60Base::sendPacket(uint16_t len, uint8_t* packet)
{
    // Check no transmit in progress
    while (readOp(ENC28J60_READ_CTRL_REG, ECON1) & ECON1_TXRTS) {
        // Reset the transmit logic problem. See Rev. B4 Silicon Errata point 12.
        if(read(EIR) & EIR_TXERIF) {
            writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRST);
            writeOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRST);
        }
    }
    
	// Set the write pointer to start of transmit buffer area
	write(EWRPTL, TXSTART_INIT&0xFF);
	write(EWRPTH, TXSTART_INIT>>8);
	// Set the TXND pointer to correspond to the packet size given
	write(ETXNDL, (TXSTART_INIT+len)&0xFF);
	write(ETXNDH, (TXSTART_INIT+len)>>8);
	// write per-packet control byte (0x00 means use macon3 settings)
	writeOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);
	// copy the packet into the transmit buffer
	writeBuffer(len, packet);
	// send the contents of the transmit buffer onto the network
	writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
}

uint16_t
ENC28J60Base::receivePacket(uint16_t maxlen, uint8_t* packet)
{
	if(!hasRxPkt())
		return 0;

	// Set the read pointer to the start of the received packet
	write(ERDPTL, m_nextPacketPtr & 0xFF);
	write(ERDPTH, m_nextPacketPtr >> 8);
    
	// read the next packet pointer
	m_nextPacketPtr  = readOp(ENC28J60_READ_BUF_MEM, 0);
	m_nextPacketPtr |= readOp(ENC28J60_READ_BUF_MEM, 0) << 8;
    
	// read the packet length (see datasheet page 43)
	uint16_t len = readOp(ENC28J60_READ_BUF_MEM, 0);
	len |= readOp(ENC28J60_READ_BUF_MEM, 0) << 8;
    
    //remove the CRC count
    len -= 4;
    
	// read the receive status (see datasheet page 43)
	uint16_t rxstat = readOp(ENC28J60_READ_BUF_MEM, 0);
	rxstat |= ((uint16_t) readOp(ENC28J60_READ_BUF_MEM, 0)) << 8;
    
	// limit retrieve length
    if (len > maxlen - 1)
        len = maxlen - 1;

    // check CRC and symbol errors (see datasheet page 44, table 7-3):
    // The ERXFCON.CRCEN is set by default. Normally we should not
    // need to check this.
    if (!(rxstat & 0x80))
        len = 0;
    else
        readBuffer(len, packet);
    
    // Move the RX read pointer to the start of the next received packet
    // This frees the memory we just read out. 
    // However, compensate for the errata point 13, rev B4: never write an even address!
    // m_nextPacketPtr is always an even address if RXSTOP_INIT is odd.
    if (m_nextPacketPtr -1 > RXSTOP_INIT) {
        // RXSTART_INIT is zero, no test for m_nextPacketPtr less than RXSTART_INIT.
        write(ERXRDPTL, (RXSTOP_INIT) & 0xFF);
        write(ERXRDPTH, (RXSTOP_INIT) >> 8);
    } else {
        write(ERXRDPTL, (m_nextPacketPtr - 1) & 0xFF);
        write(ERXRDPTH, (m_nextPacketPtr - 1) >> 8);
    }
	// decrement the packet counter indicate we are done with this packet
	writeOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
	
    return len;
}

bool
ENC28J60Base::hasRxPkt(void)
{
	return read(EPKTCNT);
}

bool
ENC28J60Base::linkup()
{
	// Set the right address and start the register read operation
	write(MIREGADR, PHSTAT2);
	write(MICMD, MICMD_MIIRD);
    Application::usDelay<10>();

	// wait until the PHY read completes
	while(read(MISTAT) & MISTAT_BUSY);

	// reset reading bit
	write(MICMD, 0x00);
	
	return read(MIRDH) && 4;
}

uint8_t
ENC28J60Base::chipRev()
{
    uint8_t rev = read(EREVID);
    
    // microchip forgot to step the number on the silcon when they
    // released the revision B7. 6 is now rev B7. We still have
    // to see what they do when they release B8.
	return (rev > 5) ? (rev + 1) : rev;
}

uint8_t
ENC28J60Base::readOp(uint8_t op, uint8_t address)
{
    CSACTIVE;
    m_spi.write(op | (address & ADDR_MASK));
    m_spi.wait();

    m_spi.write(0x00);
    m_spi.wait();
    
    // do dummy read if needed (for mac and mii, see datasheet page 29)
    if (address & 0x80) {
        m_spi.write(0x00);
        m_spi.wait();
    }

    CSPASSIVE;
    return(SPDR);
}

void
ENC28J60Base::writeOp(uint8_t op, uint8_t address, uint8_t data)
{
    CSACTIVE;
    m_spi.write(op | (address & ADDR_MASK));
    m_spi.wait();

    m_spi.write(data);
    m_spi.wait();
    CSPASSIVE;
}

void
ENC28J60Base::readBuffer(uint16_t len, uint8_t* data)
{
    CSACTIVE;
    m_spi.write(ENC28J60_READ_BUF_MEM);
    m_spi.wait();
    
    while (len) {
        len--;
        m_spi.write(0x00);
        m_spi.wait();
        *data = SPDR;
        data++;
    }
    
    *data='\0';
    CSPASSIVE;
}

void
ENC28J60Base::writeBuffer(uint16_t len, uint8_t* data)
{
    CSACTIVE;
    m_spi.write(ENC28J60_WRITE_BUF_MEM);
    m_spi.wait();
    
    while(len) {
        len--;
        m_spi.write(*data);
        data++;
        m_spi.wait();
    }
    CSPASSIVE;
}

void
ENC28J60Base::setBank(uint8_t address)
{
    if ((address & BANK_MASK) != m_bank) {
        writeOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_BSEL1 | ECON1_BSEL0);
        writeOp(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK) >> 5);
        m_bank = (address & BANK_MASK);
    }
}

void
ENC28J60Base::phyWrite(uint8_t address, uint16_t data)
{
    write(MIREGADR, address);
    write(MIWRL, data);
    write(MIWRH, data>>8);

    while(read(MISTAT) & MISTAT_BUSY)
        Application::usDelay<10>();
}

