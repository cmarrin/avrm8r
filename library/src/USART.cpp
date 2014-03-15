//
//  USART.cpp
//  marrinator
//
//  Created by Chris Marrin on 3/4/14.
//
//

#include "USART.h"

#include <stdlib.h>
#include <avr/interrupt.h>

using namespace m8r;

USARTBase* USARTBase::g_USART0 = NULL;
    
ISR(USART_RX_vect)
{
    USARTBase::g_USART0->receiveByte(UDR0);
}

ISR(USART_TX_vect)
{
}