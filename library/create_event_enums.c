//
//  create_event_enums.h
//
//  Created by Chris Marrin on 3/19/2011.
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

    - Neither the name of Video Monkey nor the names of its contributors may be 
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

// Generate EventType enums

#include <avr/io.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    printf("#pragma once\n");
    printf("enum EventType {\n");
    printf("    EV_NO_EVENT = 0,\n");

    int i = 1;
    
#ifdef ADC_vect
    printf("    EV_ADC = %d,\n", i++);
#endif
#ifdef ANALOG_COMP_0_vect
    printf("    EV_ANALOG_COMP_0 = %d,\n", i++);
#endif
#ifdef ANALOG_COMP_1_vect
    printf("    EV_ANALOG_COMP_1 = %d,\n", i++);
#endif
#ifdef ANALOG_COMP_2_vect
    printf("    EV_ANALOG_COMP_2 = %d,\n", i++);
#endif
#ifdef ANALOG_COMP_vect
    printf("    EV_ANALOG_COMP = %d,\n", i++);
#endif
#ifdef ANA_COMP_vect
    printf("    EV_ANA_COMP = %d,\n", i++);
#endif
#ifdef CANIT_vect
    printf("    EV_CANIT = %d,\n", i++);
#endif
#ifdef EEPROM_READY_vect
    printf("    EV_EEPROM_READY = %d,\n", i++);
#endif
#ifdef EE_RDY_vect
    printf("    EV_EE_RDY = %d,\n", i++);
#endif
#ifdef EE_READY_vect
    printf("    EV_EE_READY = %d,\n", i++);
#endif
#ifdef EXT_INT0_vect
    printf("    EV_EXT_INT0 = %d,\n", i++);
#endif
#ifdef INT0_vect
    printf("    EV_INT0 = %d,\n", i++);
#endif
#ifdef INT1_vect
    printf("    EV_INT1 = %d,\n", i++);
#endif
#ifdef INT2_vect
    printf("    EV_INT2 = %d,\n", i++);
#endif
#ifdef INT3_vect
    printf("    EV_INT3 = %d,\n", i++);
#endif
#ifdef INT4_vect
    printf("    EV_INT4 = %d,\n", i++);
#endif
#ifdef INT5_vect
    printf("    EV_INT5 = %d,\n", i++);
#endif
#ifdef INT6_vect
    printf("    EV_INT6 = %d,\n", i++);
#endif
#ifdef INT7_vect
    printf("    EV_INT7 = %d,\n", i++);
#endif
#ifdef IO_PINS_vect
    printf("    EV_IO_PINS = %d,\n", i++);
#endif
#ifdef LCD_vect
    printf("    EV_LCD = %d,\n", i++);
#endif
#ifdef LOWLEVEL_IO_PINS_vect
    printf("    EV_LOWLEVEL_IO_PINS = %d,\n", i++);
#endif
#ifdef OVRIT_vect
    printf("    EV_OVRIT = %d,\n", i++);
#endif
#ifdef PCINT0_vect
    printf("    EV_PCINT0 = %d,\n", i++);
#endif
#ifdef PCINT1_vect
    printf("    EV_PCINT1 = %d,\n", i++);
#endif
#ifdef PCINT2_vect
    printf("    EV_PCINT2 = %d,\n", i++);
#endif
#ifdef PCINT3_vect
    printf("    EV_PCINT3 = %d,\n", i++);
#endif
#ifdef PCINT_vect
    printf("    EV_PCINT = %d,\n", i++);
#endif
#ifdef PSC0_CAPT_vect
    printf("    EV_PSC0_CAPT = %d,\n", i++);
#endif
#ifdef PSC0_EC_vect
    printf("    EV_PSC0_EC = %d,\n", i++);
#endif
#ifdef PSC1_CAPT_vect
    printf("    EV_PSC1_CAPT = %d,\n", i++);
#endif
#ifdef PSC1_EC_vect
    printf("    EV_PSC1_EC = %d,\n", i++);
#endif
#ifdef PSC2_CAPT_vect
    printf("    EV_PSC2_CAPT = %d,\n", i++);
#endif
#ifdef PSC2_EC_vect
    printf("    EV_PSC2_EC = %d,\n", i++);
#endif
#ifdef SPI_STC_vect
    printf("    EV_SPI_STC = %d,\n", i++);
#endif
#ifdef SPM_RDY_vect
    printf("    EV_SPM_RDY = %d,\n", i++);
#endif
#ifdef SPM_READY_vect
    printf("    EV_SPM_READY = %d,\n", i++);
#endif
#ifdef TIM0_COMPA_vect
    printf("    EV_TIM0_COMPA = %d,\n", i++);
#endif
#ifdef TIM0_COMPB_vect
    printf("    EV_TIM0_COMPB = %d,\n", i++);
#endif
#ifdef TIM0_OVF_vect
    printf("    EV_TIM0_OVF = %d,\n", i++);
#endif
#ifdef TIM1_CAPT_vect
    printf("    EV_TIM1_CAPT = %d,\n", i++);
#endif
#ifdef TIM1_COMPA_vect
    printf("    EV_TIM1_COMPA = %d,\n", i++);
#endif
#ifdef TIM1_COMPB_vect
    printf("    EV_TIM1_COMPB = %d,\n", i++);
#endif
#ifdef TIM1_OVF_vect
    printf("    EV_TIM1_OVF = %d,\n", i++);
#endif
#ifdef TIMER0_CAPT_vect
    printf("    EV_TIMER0_CAPT = %d,\n", i++);
#endif
#ifdef TIMER0_COMPA_vect
    printf("    EV_TIMER0_COMPA = %d,\n", i++);
#endif
#ifdef TIMER0_COMPB_vect
    printf("    EV_TIMER0_COMPB = %d,\n", i++);
#endif
#ifdef TIMER0_COMP_A_vect
    printf("    EV_TIMER0_COMP_A = %d,\n", i++);
#endif
#ifdef TIMER0_COMP_vect
    printf("    EV_TIMER0_COMP = %d,\n", i++);
#endif
#ifdef TIMER0_OVF0_vect
    printf("    EV_TIMER0_OVF0 = %d,\n", i++);
#endif
#ifdef TIMER0_OVF_vect
    printf("    EV_TIMER0_OVF = %d,\n", i++);
#endif
#ifdef TIMER1_CAPT1_vect
    printf("    EV_TIMER1_CAPT1 = %d,\n", i++);
#endif
#ifdef TIMER1_CAPT_vect
    printf("    EV_TIMER1_CAPT = %d,\n", i++);
#endif
#ifdef TIMER1_CMPA_vect
    printf("    EV_TIMER1_CMPA = %d,\n", i++);
#endif
#ifdef TIMER1_CMPB_vect
    printf("    EV_TIMER1_CMPB = %d,\n", i++);
#endif
#ifdef TIMER1_COMP1_vect
    printf("    EV_TIMER1_COMP1 = %d,\n", i++);
#endif
#ifdef TIMER1_COMPA_vect
    printf("    EV_TIMER1_COMPA = %d,\n", i++);
#endif
#ifdef TIMER1_COMPB_vect
    printf("    EV_TIMER1_COMPB = %d,\n", i++);
#endif
#ifdef TIMER1_COMPC_vect
    printf("    EV_TIMER1_COMPC = %d,\n", i++);
#endif
#ifdef TIMER1_COMPD_vect
    printf("    EV_TIMER1_COMPD = %d,\n", i++);
#endif
#ifdef TIMER1_COMP_vect
    printf("    EV_TIMER1_COMP = %d,\n", i++);
#endif
#ifdef TIMER1_OVF1_vect
    printf("    EV_TIMER1_OVF1 = %d,\n", i++);
#endif
#ifdef TIMER1_OVF_vect
    printf("    EV_TIMER1_OVF = %d,\n", i++);
#endif
#ifdef TIMER2_COMPA_vect
    printf("    EV_TIMER2_COMPA = %d,\n", i++);
#endif
#ifdef TIMER2_COMPB_vect
    printf("    EV_TIMER2_COMPB = %d,\n", i++);
#endif
#ifdef TIMER2_COMP_vect
    printf("    EV_TIMER2_COMP = %d,\n", i++);
#endif
#ifdef TIMER2_OVF_vect
    printf("    EV_TIMER2_OVF = %d,\n", i++);
#endif
#ifdef TIMER3_CAPT_vect
    printf("    EV_TIMER3_CAPT = %d,\n", i++);
#endif
#ifdef TIMER3_COMPA_vect
    printf("    EV_TIMER3_COMPA = %d,\n", i++);
#endif
#ifdef TIMER3_COMPB_vect
    printf("    EV_TIMER3_COMPB = %d,\n", i++);
#endif
#ifdef TIMER3_COMPC_vect
    printf("    EV_TIMER3_COMPC = %d,\n", i++);
#endif
#ifdef TIMER3_OVF_vect
    printf("    EV_TIMER3_OVF = %d,\n", i++);
#endif
#ifdef TIMER4_CAPT_vect
    printf("    EV_TIMER4_CAPT = %d,\n", i++);
#endif
#ifdef TIMER4_COMPA_vect
    printf("    EV_TIMER4_COMPA = %d,\n", i++);
#endif
#ifdef TIMER4_COMPB_vect
    printf("    EV_TIMER4_COMPB = %d,\n", i++);
#endif
#ifdef TIMER4_COMPC_vect
    printf("    EV_TIMER4_COMPC = %d,\n", i++);
#endif
#ifdef TIMER4_OVF_vect
    printf("    EV_TIMER4_OVF = %d,\n", i++);
#endif
#ifdef TIMER5_CAPT_vect
    printf("    EV_TIMER5_CAPT = %d,\n", i++);
#endif
#ifdef TIMER5_COMPA_vect
    printf("    EV_TIMER5_COMPA = %d,\n", i++);
#endif
#ifdef TIMER5_COMPB_vect
    printf("    EV_TIMER5_COMPB = %d,\n", i++);
#endif
#ifdef TIMER5_COMPC_vect
    printf("    EV_TIMER5_COMPC = %d,\n", i++);
#endif
#ifdef TIMER5_OVF_vect
    printf("    EV_TIMER5_OVF = %d,\n", i++);
#endif
#ifdef TWI_vect
    printf("    EV_TWI = %d,\n", i++);
#endif
#ifdef TXDONE_vect
    printf("    EV_TXDONE = %d,\n", i++);
#endif
#ifdef TXEMPTY_vect
    printf("    EV_TXEMPTY = %d,\n", i++);
#endif
#ifdef UART0_RX_vect
    printf("    EV_UART0_RX = %d,\n", i++);
#endif
#ifdef UART0_TX_vect
    printf("    EV_UART0_TX = %d,\n", i++);
#endif
#ifdef UART0_UDRE_vect
    printf("    EV_UART0_UDRE = %d,\n", i++);
#endif
#ifdef UART1_RX_vect
    printf("    EV_UART1_RX = %d,\n", i++);
#endif
#ifdef UART1_TX_vect
    printf("    EV_UART1_TX = %d,\n", i++);
#endif
#ifdef UART1_UDRE_vect
    printf("    EV_UART1_UDRE = %d,\n", i++);
#endif
#ifdef UART_RX_vect
    printf("    EV_UART_RX = %d,\n", i++);
#endif
#ifdef UART_TX_vect
    printf("    EV_UART_TX = %d,\n", i++);
#endif
#ifdef UART_UDRE_vect
    printf("    EV_UART_UDRE = %d,\n", i++);
#endif
#ifdef USART0_RXC_vect
    printf("    EV_USART0_RXC = %d,\n", i++);
#endif
#ifdef USART0_RX_vect
    printf("    EV_USART0_RX = %d,\n", i++);
#endif
#ifdef USART0_TXC_vect
    printf("    EV_USART0_TXC = %d,\n", i++);
#endif
#ifdef USART0_TX_vect
    printf("    EV_USART0_TX = %d,\n", i++);
#endif
#ifdef USART0_UDRE_vect
    printf("    EV_USART0_UDRE = %d,\n", i++);
#endif
#ifdef USART1_RXC_vect
    printf("    EV_USART1_RXC = %d,\n", i++);
#endif
#ifdef USART1_RX_vect
    printf("    EV_USART1_RX = %d,\n", i++);
#endif
#ifdef USART1_TXC_vect
    printf("    EV_USART1_TXC = %d,\n", i++);
#endif
#ifdef USART1_TX_vect
    printf("    EV_USART1_TX = %d,\n", i++);
#endif
#ifdef USART1_UDRE_vect
    printf("    EV_USART1_UDRE = %d,\n", i++);
#endif
#ifdef USART2_RX_vect
    printf("    EV_USART2_RX = %d,\n", i++);
#endif
#ifdef USART2_TX_vect
    printf("    EV_USART2_TX = %d,\n", i++);
#endif
#ifdef USART2_UDRE_vect
    printf("    EV_USART2_UDRE = %d,\n", i++);
#endif
#ifdef USART3_RX_vect
    printf("    EV_USART3_RX = %d,\n", i++);
#endif
#ifdef USART3_TX_vect
    printf("    EV_USART3_TX = %d,\n", i++);
#endif
#ifdef USART3_UDRE_vect
    printf("    EV_USART3_UDRE = %d,\n", i++);
#endif
#ifdef USART_RXC_vect
    printf("    EV_USART_RXC = %d,\n", i++);
#endif
#ifdef USART_RX_vect
    printf("    EV_USART_RX = %d,\n", i++);
#endif
#ifdef USART_TXC_vect
    printf("    EV_USART_TXC = %d,\n", i++);
#endif
#ifdef USART_TX_vect
    printf("    EV_USART_TX = %d,\n", i++);
#endif
#ifdef USART_UDRE_vect
    printf("    EV_USART_UDRE = %d,\n", i++);
#endif
#ifdef USI_OVERFLOW_vect
    printf("    EV_USI_OVERFLOW = %d,\n", i++);
#endif
#ifdef USI_OVF_vect
    printf("    EV_USI_OVF = %d,\n", i++);
#endif
#ifdef USI_START_vect
    printf("    EV_USI_START = %d,\n", i++);
#endif
#ifdef USI_STRT_vect
    printf("    EV_USI_STRT = %d,\n", i++);
#endif
#ifdef USI_STR_vect
    printf("    EV_USI_STR = %d,\n", i++);
#endif
#ifdef WATCHDOG_vect
    printf("    EV_WATCHDOG = %d,\n", i++);
#endif
#ifdef WDT_OVERFLOW_vect
    printf("    EV_WDT_OVERFLOW = %d,\n", i++);
#endif
#ifdef WDT_vect
    printf("    EV_WDT = %d,\n", i++);
#endif

    // User generated events
    printf("    EV_USER0		= %d,\n", i++);
    printf("    EV_USER1		= %d,\n", i++);
    printf("    EV_USER2		= %d,\n", i++);
    printf("    EV_USER3		= %d,\n", i++);
    printf("    EV_USER4		= %d,\n", i++);
    printf("    EV_USER5		= %d,\n", i++);
    printf("    EV_USER6		= %d,\n", i++);
    printf("    EV_USER7		= %d,\n", i++);
    printf("    EV_LAST         = %d,\n", i++);
printf("};\n");



    return 0;
}


