//
//  EventSourceEnums.h
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

#pragma once
/*
    This enum assigns numeric values to all possible 
    Contents of this file are generated by:

    for i in `curl http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html`
    do
        if [[ $i == '<td>'*_vect ]]
        then
            i=${i#'<td>'}
            i=${i%_vect}
            echo '#ifdef' ${i}_vect
            echo '    EV_'$i' = __COUNTER__,'
            echo '#endif'
        fi
    done
*/

// HACK until we get a version of gcc with __COUNTER__
#define __COUNTER__ 0

enum EventType {
    EV_NO_EVENT		= 0,

#ifdef ADC_vect
    EV_ADC = __COUNTER__,
#endif
#ifdef ANALOG_COMP_0_vect
    EV_ANALOG_COMP_0 = __COUNTER__,
#endif
#ifdef ANALOG_COMP_1_vect
    EV_ANALOG_COMP_1 = __COUNTER__,
#endif
#ifdef ANALOG_COMP_2_vect
    EV_ANALOG_COMP_2 = __COUNTER__,
#endif
#ifdef ANALOG_COMP_vect
    EV_ANALOG_COMP = __COUNTER__,
#endif
#ifdef ANA_COMP_vect
    EV_ANA_COMP = __COUNTER__,
#endif
#ifdef CANIT_vect
    EV_CANIT = __COUNTER__,
#endif
#ifdef EEPROM_READY_vect
    EV_EEPROM_READY = __COUNTER__,
#endif
#ifdef EE_RDY_vect
    EV_EE_RDY = __COUNTER__,
#endif
#ifdef EE_READY_vect
    EV_EE_READY = __COUNTER__,
#endif
#ifdef EXT_INT0_vect
    EV_EXT_INT0 = __COUNTER__,
#endif
#ifdef INT0_vect
    EV_INT0 = __COUNTER__,
#endif
#ifdef INT1_vect
    EV_INT1 = __COUNTER__,
#endif
#ifdef INT2_vect
    EV_INT2 = __COUNTER__,
#endif
#ifdef INT3_vect
    EV_INT3 = __COUNTER__,
#endif
#ifdef INT4_vect
    EV_INT4 = __COUNTER__,
#endif
#ifdef INT5_vect
    EV_INT5 = __COUNTER__,
#endif
#ifdef INT6_vect
    EV_INT6 = __COUNTER__,
#endif
#ifdef INT7_vect
    EV_INT7 = __COUNTER__,
#endif
#ifdef IO_PINS_vect
    EV_IO_PINS = __COUNTER__,
#endif
#ifdef LCD_vect
    EV_LCD = __COUNTER__,
#endif
#ifdef LOWLEVEL_IO_PINS_vect
    EV_LOWLEVEL_IO_PINS = __COUNTER__,
#endif
#ifdef OVRIT_vect
    EV_OVRIT = __COUNTER__,
#endif
#ifdef PCINT0_vect
    EV_PCINT0 = __COUNTER__,
#endif
#ifdef PCINT1_vect
    EV_PCINT1 = __COUNTER__,
#endif
#ifdef PCINT2_vect
    EV_PCINT2 = __COUNTER__,
#endif
#ifdef PCINT3_vect
    EV_PCINT3 = __COUNTER__,
#endif
#ifdef PCINT_vect
    EV_PCINT = __COUNTER__,
#endif
#ifdef PSC0_CAPT_vect
    EV_PSC0_CAPT = __COUNTER__,
#endif
#ifdef PSC0_EC_vect
    EV_PSC0_EC = __COUNTER__,
#endif
#ifdef PSC1_CAPT_vect
    EV_PSC1_CAPT = __COUNTER__,
#endif
#ifdef PSC1_EC_vect
    EV_PSC1_EC = __COUNTER__,
#endif
#ifdef PSC2_CAPT_vect
    EV_PSC2_CAPT = __COUNTER__,
#endif
#ifdef PSC2_EC_vect
    EV_PSC2_EC = __COUNTER__,
#endif
#ifdef SPI_STC_vect
    EV_SPI_STC = __COUNTER__,
#endif
#ifdef SPM_RDY_vect
    EV_SPM_RDY = __COUNTER__,
#endif
#ifdef SPM_READY_vect
    EV_SPM_READY = __COUNTER__,
#endif
#ifdef TIM0_COMPA_vect
    EV_TIM0_COMPA = __COUNTER__,
#endif
#ifdef TIM0_COMPB_vect
    EV_TIM0_COMPB = __COUNTER__,
#endif
#ifdef TIM0_OVF_vect
    EV_TIM0_OVF = __COUNTER__,
#endif
#ifdef TIM1_CAPT_vect
    EV_TIM1_CAPT = __COUNTER__,
#endif
#ifdef TIM1_COMPA_vect
    EV_TIM1_COMPA = __COUNTER__,
#endif
#ifdef TIM1_COMPB_vect
    EV_TIM1_COMPB = __COUNTER__,
#endif
#ifdef TIM1_OVF_vect
    EV_TIM1_OVF = __COUNTER__,
#endif
#ifdef TIMER0_CAPT_vect
    EV_TIMER0_CAPT = __COUNTER__,
#endif
#ifdef TIMER0_COMPA_vect
    EV_TIMER0_COMPA = __COUNTER__,
#endif
#ifdef TIMER0_COMPB_vect
    EV_TIMER0_COMPB = __COUNTER__,
#endif
#ifdef TIMER0_COMP_A_vect
    EV_TIMER0_COMP_A = __COUNTER__,
#endif
#ifdef TIMER0_COMP_vect
    EV_TIMER0_COMP = __COUNTER__,
#endif
#ifdef TIMER0_OVF0_vect
    EV_TIMER0_OVF0 = __COUNTER__,
#endif
#ifdef TIMER0_OVF_vect
    EV_TIMER0_OVF = __COUNTER__,
#endif
#ifdef TIMER1_CAPT1_vect
    EV_TIMER1_CAPT1 = __COUNTER__,
#endif
#ifdef TIMER1_CAPT_vect
    EV_TIMER1_CAPT = __COUNTER__,
#endif
#ifdef TIMER1_CMPA_vect
    EV_TIMER1_CMPA = __COUNTER__,
#endif
#ifdef TIMER1_CMPB_vect
    EV_TIMER1_CMPB = __COUNTER__,
#endif
#ifdef TIMER1_COMP1_vect
    EV_TIMER1_COMP1 = __COUNTER__,
#endif
#ifdef TIMER1_COMPA_vect
    EV_TIMER1_COMPA = __COUNTER__,
#endif
#ifdef TIMER1_COMPB_vect
    EV_TIMER1_COMPB = __COUNTER__,
#endif
#ifdef TIMER1_COMPC_vect
    EV_TIMER1_COMPC = __COUNTER__,
#endif
#ifdef TIMER1_COMPD_vect
    EV_TIMER1_COMPD = __COUNTER__,
#endif
#ifdef TIMER1_COMP_vect
    EV_TIMER1_COMP = __COUNTER__,
#endif
#ifdef TIMER1_OVF1_vect
    EV_TIMER1_OVF1 = __COUNTER__,
#endif
#ifdef TIMER1_OVF_vect
    EV_TIMER1_OVF = __COUNTER__,
#endif
#ifdef TIMER2_COMPA_vect
    EV_TIMER2_COMPA = __COUNTER__,
#endif
#ifdef TIMER2_COMPB_vect
    EV_TIMER2_COMPB = __COUNTER__,
#endif
#ifdef TIMER2_COMP_vect
    EV_TIMER2_COMP = __COUNTER__,
#endif
#ifdef TIMER2_OVF_vect
    EV_TIMER2_OVF = __COUNTER__,
#endif
#ifdef TIMER3_CAPT_vect
    EV_TIMER3_CAPT = __COUNTER__,
#endif
#ifdef TIMER3_COMPA_vect
    EV_TIMER3_COMPA = __COUNTER__,
#endif
#ifdef TIMER3_COMPB_vect
    EV_TIMER3_COMPB = __COUNTER__,
#endif
#ifdef TIMER3_COMPC_vect
    EV_TIMER3_COMPC = __COUNTER__,
#endif
#ifdef TIMER3_OVF_vect
    EV_TIMER3_OVF = __COUNTER__,
#endif
#ifdef TIMER4_CAPT_vect
    EV_TIMER4_CAPT = __COUNTER__,
#endif
#ifdef TIMER4_COMPA_vect
    EV_TIMER4_COMPA = __COUNTER__,
#endif
#ifdef TIMER4_COMPB_vect
    EV_TIMER4_COMPB = __COUNTER__,
#endif
#ifdef TIMER4_COMPC_vect
    EV_TIMER4_COMPC = __COUNTER__,
#endif
#ifdef TIMER4_OVF_vect
    EV_TIMER4_OVF = __COUNTER__,
#endif
#ifdef TIMER5_CAPT_vect
    EV_TIMER5_CAPT = __COUNTER__,
#endif
#ifdef TIMER5_COMPA_vect
    EV_TIMER5_COMPA = __COUNTER__,
#endif
#ifdef TIMER5_COMPB_vect
    EV_TIMER5_COMPB = __COUNTER__,
#endif
#ifdef TIMER5_COMPC_vect
    EV_TIMER5_COMPC = __COUNTER__,
#endif
#ifdef TIMER5_OVF_vect
    EV_TIMER5_OVF = __COUNTER__,
#endif
#ifdef TWI_vect
    EV_TWI = __COUNTER__,
#endif
#ifdef TXDONE_vect
    EV_TXDONE = __COUNTER__,
#endif
#ifdef TXEMPTY_vect
    EV_TXEMPTY = __COUNTER__,
#endif
#ifdef UART0_RX_vect
    EV_UART0_RX = __COUNTER__,
#endif
#ifdef UART0_TX_vect
    EV_UART0_TX = __COUNTER__,
#endif
#ifdef UART0_UDRE_vect
    EV_UART0_UDRE = __COUNTER__,
#endif
#ifdef UART1_RX_vect
    EV_UART1_RX = __COUNTER__,
#endif
#ifdef UART1_TX_vect
    EV_UART1_TX = __COUNTER__,
#endif
#ifdef UART1_UDRE_vect
    EV_UART1_UDRE = __COUNTER__,
#endif
#ifdef UART_RX_vect
    EV_UART_RX = __COUNTER__,
#endif
#ifdef UART_TX_vect
    EV_UART_TX = __COUNTER__,
#endif
#ifdef UART_UDRE_vect
    EV_UART_UDRE = __COUNTER__,
#endif
#ifdef USART0_RXC_vect
    EV_USART0_RXC = __COUNTER__,
#endif
#ifdef USART0_RX_vect
    EV_USART0_RX = __COUNTER__,
#endif
#ifdef USART0_TXC_vect
    EV_USART0_TXC = __COUNTER__,
#endif
#ifdef USART0_TX_vect
    EV_USART0_TX = __COUNTER__,
#endif
#ifdef USART0_UDRE_vect
    EV_USART0_UDRE = __COUNTER__,
#endif
#ifdef USART1_RXC_vect
    EV_USART1_RXC = __COUNTER__,
#endif
#ifdef USART1_RX_vect
    EV_USART1_RX = __COUNTER__,
#endif
#ifdef USART1_TXC_vect
    EV_USART1_TXC = __COUNTER__,
#endif
#ifdef USART1_TX_vect
    EV_USART1_TX = __COUNTER__,
#endif
#ifdef USART1_UDRE_vect
    EV_USART1_UDRE = __COUNTER__,
#endif
#ifdef USART2_RX_vect
    EV_USART2_RX = __COUNTER__,
#endif
#ifdef USART2_TX_vect
    EV_USART2_TX = __COUNTER__,
#endif
#ifdef USART2_UDRE_vect
    EV_USART2_UDRE = __COUNTER__,
#endif
#ifdef USART3_RX_vect
    EV_USART3_RX = __COUNTER__,
#endif
#ifdef USART3_TX_vect
    EV_USART3_TX = __COUNTER__,
#endif
#ifdef USART3_UDRE_vect
    EV_USART3_UDRE = __COUNTER__,
#endif
#ifdef USART_RXC_vect
    EV_USART_RXC = __COUNTER__,
#endif
#ifdef USART_RX_vect
    EV_USART_RX = __COUNTER__,
#endif
#ifdef USART_TXC_vect
    EV_USART_TXC = __COUNTER__,
#endif
#ifdef USART_TX_vect
    EV_USART_TX = __COUNTER__,
#endif
#ifdef USART_UDRE_vect
    EV_USART_UDRE = __COUNTER__,
#endif
#ifdef USI_OVERFLOW_vect
    EV_USI_OVERFLOW = __COUNTER__,
#endif
#ifdef USI_OVF_vect
    EV_USI_OVF = __COUNTER__,
#endif
#ifdef USI_START_vect
    EV_USI_START = __COUNTER__,
#endif
#ifdef USI_STRT_vect
    EV_USI_STRT = __COUNTER__,
#endif
#ifdef USI_STR_vect
    EV_USI_STR = __COUNTER__,
#endif
#ifdef WATCHDOG_vect
    EV_WATCHDOG = __COUNTER__,
#endif
#ifdef WDT_OVERFLOW_vect
    EV_WDT_OVERFLOW = __COUNTER__,
#endif
#ifdef WDT_vect
    EV_WDT = __COUNTER__,
#endif

    // User generated events
    EV_USER0		= __COUNTER__,
    EV_USER1		= __COUNTER__,
    EV_USER2		= __COUNTER__,
    EV_USER3		= __COUNTER__,
    EV_USER4		= __COUNTER__,
    EV_USER5		= __COUNTER__,
    EV_USER6		= __COUNTER__,
    EV_USER7		= __COUNTER__,
    
    EV_LAST         = __COUNTER__
};