//
//  USART.h
//  marrinator
//
//  Created by Chris Marrin on 3/4/14.
//
//

#pragma once

#include "m8r.h"

const uint8_t RX_BUFFER_SIZE = 128;

namespace m8r {

class USARTBase
{
public:
	USARTBase()
        : _rxBufferHead(0)
        , _rxBufferTail(0)
    { }
    
    void receiveByte(uint8_t b)
    {
        uint8_t i = _rxBufferHead + 1 % RX_BUFFER_SIZE;
        if (i != _rxBufferTail) {
            _rxBuffer[_rxBufferHead] = b;
            _rxBufferHead = i;
        }
    }

    static USARTBase* g_USART0;
    
protected:
    uint8_t _rxBuffer[RX_BUFFER_SIZE];
    uint8_t _rxBufferHead, _rxBufferTail;
};

template <
    uint16_t BAUD,
    class UDRPort,
    class UCSRAPort,
    class UCSRBPort,
    class UCSRCPort,
    class UBRRPort>
class USARTCommon : public USARTBase
{
public:
	USARTCommon()
    {
        _UCSRAPort = 0;
        _UCSRBPort = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
        _UCSRCPort = (1 << UCSZ00) | (1 << UCSZ01); // 8 bit
        _UBRRPort = (((F_CPU / (BAUD * 16UL))) - 1);
    }

    void write(uint8_t b)
    {
        while (!(_UCSRAPort & (1 << UDRE0))) ;
        _UDRPort = b;
    }
    
    int16_t read()
    {
        // if the head isn't ahead of the tail, we don't have any characters
        if (_rxBufferHead == _rxBufferTail) {
            return -1;
        }
        uint8_t b = _rxBuffer[_rxBufferTail];
        _rxBufferTail = (_rxBufferTail + 1) % RX_BUFFER_SIZE;
        return b;
    }
    
    uint8_t bytesAvailable() const { return (RX_BUFFER_SIZE + _rxBufferHead - _rxBufferTail) % RX_BUFFER_SIZE; }

private:
    UDRPort _UDRPort;
    UCSRAPort _UCSRAPort;
    UCSRBPort _UCSRBPort;
    UCSRCPort _UCSRCPort;
    UBRRPort _UBRRPort;
};

template<uint16_t BAUD>
class USART0 : public USARTCommon<
                    BAUD,
                    Reg8<_UDR0>,
                    Reg8<_UCSR0A>,
                    Reg8<_UCSR0B>,
                    Reg8<_UCSR0C>,
                    Reg16<_UBRR0> >
{
public:
    USART0()
    {
        ASSERT(!USARTBase::g_USART0, AssertSingleUSART0);
        USARTBase::g_USART0 = this;
    }
};

}





