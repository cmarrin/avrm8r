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
/*
    This enum assigns numeric values to all possible event types for a given processor
    Contents of the EventType enum are generated by:

    for i in `curl http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html`
    do
        if [[ $i == '<td>'*_vect ]]
        then
            i=${i#'<td>'}
            i=${i%_vect}
            echo '#ifdef' ${i}_vect
            printf '    EV_%s,\n' $i
            echo '#endif'
        fi
    done
    
    For the purposes of printing the map of enum names to numeric values, a second
    array of strings is generated with this:
    
    for i in `curl http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html`
    do
        if [[ $i == '<td>'*_vect ]]
        then
            i=${i#'<td>'}
            i=${i%_vect}
            echo '#ifdef' ${i}_vect
            printf '    \"EV_%s\",\n' $i
            echo '#endif'
        fi
    done
*/

enum EventType {
    EV_NO_EVENT		= 0,

#ifdef ADC_vect
    EV_ADC,
#endif
#ifdef ANALOG_COMP_0_vect
    EV_ANALOG_COMP_0,
#endif
#ifdef ANALOG_COMP_1_vect
    EV_ANALOG_COMP_1,
#endif
#ifdef ANALOG_COMP_2_vect
    EV_ANALOG_COMP_2,
#endif
#ifdef ANALOG_COMP_vect
    EV_ANALOG_COMP,
#endif
#ifdef ANA_COMP_vect
    EV_ANA_COMP,
#endif
#ifdef CANIT_vect
    EV_CANIT,
#endif
#ifdef EEPROM_READY_vect
    EV_EEPROM_READY,
#endif
#ifdef EE_RDY_vect
    EV_EE_RDY,
#endif
#ifdef EE_READY_vect
    EV_EE_READY,
#endif
#ifdef EXT_INT0_vect
    EV_EXT_INT0,
#endif
#ifdef INT0_vect
    EV_INT0,
#endif
#ifdef INT1_vect
    EV_INT1,
#endif
#ifdef INT2_vect
    EV_INT2,
#endif
#ifdef INT3_vect
    EV_INT3,
#endif
#ifdef INT4_vect
    EV_INT4,
#endif
#ifdef INT5_vect
    EV_INT5,
#endif
#ifdef INT6_vect
    EV_INT6,
#endif
#ifdef INT7_vect
    EV_INT7,
#endif
#ifdef IO_PINS_vect
    EV_IO_PINS,
#endif
#ifdef LCD_vect
    EV_LCD,
#endif
#ifdef LOWLEVEL_IO_PINS_vect
    EV_LOWLEVEL_IO_PINS,
#endif
#ifdef OVRIT_vect
    EV_OVRIT,
#endif
#ifdef PCINT0_vect
    EV_PCINT0,
#endif
#ifdef PCINT1_vect
    EV_PCINT1,
#endif
#ifdef PCINT2_vect
    EV_PCINT2,
#endif
#ifdef PCINT3_vect
    EV_PCINT3,
#endif
#ifdef PCINT_vect
    EV_PCINT,
#endif
#ifdef PSC0_CAPT_vect
    EV_PSC0_CAPT,
#endif
#ifdef PSC0_EC_vect
    EV_PSC0_EC,
#endif
#ifdef PSC1_CAPT_vect
    EV_PSC1_CAPT,
#endif
#ifdef PSC1_EC_vect
    EV_PSC1_EC,
#endif
#ifdef PSC2_CAPT_vect
    EV_PSC2_CAPT,
#endif
#ifdef PSC2_EC_vect
    EV_PSC2_EC,
#endif
#ifdef SPI_STC_vect
    EV_SPI_STC,
#endif
#ifdef SPM_RDY_vect
    EV_SPM_RDY,
#endif
#ifdef SPM_READY_vect
    EV_SPM_READY,
#endif
#ifdef TIM0_COMPA_vect
    EV_TIM0_COMPA,
#endif
#ifdef TIM0_COMPB_vect
    EV_TIM0_COMPB,
#endif
#ifdef TIM0_OVF_vect
    EV_TIM0_OVF,
#endif
#ifdef TIM1_CAPT_vect
    EV_TIM1_CAPT,
#endif
#ifdef TIM1_COMPA_vect
    EV_TIM1_COMPA,
#endif
#ifdef TIM1_COMPB_vect
    EV_TIM1_COMPB,
#endif
#ifdef TIM1_OVF_vect
    EV_TIM1_OVF,
#endif
#ifdef TIMER0_CAPT_vect
    EV_TIMER0_CAPT,
#endif
#ifdef TIMER0_COMPA_vect
    EV_TIMER0_COMPA,
#endif
#ifdef TIMER0_COMPB_vect
    EV_TIMER0_COMPB,
#endif
#ifdef TIMER0_COMP_A_vect
    EV_TIMER0_COMP_A,
#endif
#ifdef TIMER0_COMP_vect
    EV_TIMER0_COMP,
#endif
#ifdef TIMER0_OVF0_vect
    EV_TIMER0_OVF0,
#endif
#ifdef TIMER0_OVF_vect
    EV_TIMER0_OVF,
#endif
#ifdef TIMER1_CAPT1_vect
    EV_TIMER1_CAPT1,
#endif
#ifdef TIMER1_CAPT_vect
    EV_TIMER1_CAPT,
#endif
#ifdef TIMER1_CMPA_vect
    EV_TIMER1_CMPA,
#endif
#ifdef TIMER1_CMPB_vect
    EV_TIMER1_CMPB,
#endif
#ifdef TIMER1_COMP1_vect
    EV_TIMER1_COMP1,
#endif
#ifdef TIMER1_COMPA_vect
    EV_TIMER1_COMPA,
#endif
#ifdef TIMER1_COMPB_vect
    EV_TIMER1_COMPB,
#endif
#ifdef TIMER1_COMPC_vect
    EV_TIMER1_COMPC,
#endif
#ifdef TIMER1_COMPD_vect
    EV_TIMER1_COMPD,
#endif
#ifdef TIMER1_COMP_vect
    EV_TIMER1_COMP,
#endif
#ifdef TIMER1_OVF1_vect
    EV_TIMER1_OVF1,
#endif
#ifdef TIMER1_OVF_vect
    EV_TIMER1_OVF,
#endif
#ifdef TIMER2_COMPA_vect
    EV_TIMER2_COMPA,
#endif
#ifdef TIMER2_COMPB_vect
    EV_TIMER2_COMPB,
#endif
#ifdef TIMER2_COMP_vect
    EV_TIMER2_COMP,
#endif
#ifdef TIMER2_OVF_vect
    EV_TIMER2_OVF,
#endif
#ifdef TIMER3_CAPT_vect
    EV_TIMER3_CAPT,
#endif
#ifdef TIMER3_COMPA_vect
    EV_TIMER3_COMPA,
#endif
#ifdef TIMER3_COMPB_vect
    EV_TIMER3_COMPB,
#endif
#ifdef TIMER3_COMPC_vect
    EV_TIMER3_COMPC,
#endif
#ifdef TIMER3_OVF_vect
    EV_TIMER3_OVF,
#endif
#ifdef TIMER4_CAPT_vect
    EV_TIMER4_CAPT,
#endif
#ifdef TIMER4_COMPA_vect
    EV_TIMER4_COMPA,
#endif
#ifdef TIMER4_COMPB_vect
    EV_TIMER4_COMPB,
#endif
#ifdef TIMER4_COMPC_vect
    EV_TIMER4_COMPC,
#endif
#ifdef TIMER4_OVF_vect
    EV_TIMER4_OVF,
#endif
#ifdef TIMER5_CAPT_vect
    EV_TIMER5_CAPT,
#endif
#ifdef TIMER5_COMPA_vect
    EV_TIMER5_COMPA,
#endif
#ifdef TIMER5_COMPB_vect
    EV_TIMER5_COMPB,
#endif
#ifdef TIMER5_COMPC_vect
    EV_TIMER5_COMPC,
#endif
#ifdef TIMER5_OVF_vect
    EV_TIMER5_OVF,
#endif
#ifdef TWI_vect
    EV_TWI,
#endif
#ifdef TXDONE_vect
    EV_TXDONE,
#endif
#ifdef TXEMPTY_vect
    EV_TXEMPTY,
#endif
#ifdef UART0_RX_vect
    EV_UART0_RX,
#endif
#ifdef UART0_TX_vect
    EV_UART0_TX,
#endif
#ifdef UART0_UDRE_vect
    EV_UART0_UDRE,
#endif
#ifdef UART1_RX_vect
    EV_UART1_RX,
#endif
#ifdef UART1_TX_vect
    EV_UART1_TX,
#endif
#ifdef UART1_UDRE_vect
    EV_UART1_UDRE,
#endif
#ifdef UART_RX_vect
    EV_UART_RX,
#endif
#ifdef UART_TX_vect
    EV_UART_TX,
#endif
#ifdef UART_UDRE_vect
    EV_UART_UDRE,
#endif
#ifdef USART0_RXC_vect
    EV_USART0_RXC,
#endif
#ifdef USART0_RX_vect
    EV_USART0_RX,
#endif
#ifdef USART0_TXC_vect
    EV_USART0_TXC,
#endif
#ifdef USART0_TX_vect
    EV_USART0_TX,
#endif
#ifdef USART0_UDRE_vect
    EV_USART0_UDRE,
#endif
#ifdef USART1_RXC_vect
    EV_USART1_RXC,
#endif
#ifdef USART1_RX_vect
    EV_USART1_RX,
#endif
#ifdef USART1_TXC_vect
    EV_USART1_TXC,
#endif
#ifdef USART1_TX_vect
    EV_USART1_TX,
#endif
#ifdef USART1_UDRE_vect
    EV_USART1_UDRE,
#endif
#ifdef USART2_RX_vect
    EV_USART2_RX,
#endif
#ifdef USART2_TX_vect
    EV_USART2_TX,
#endif
#ifdef USART2_UDRE_vect
    EV_USART2_UDRE,
#endif
#ifdef USART3_RX_vect
    EV_USART3_RX,
#endif
#ifdef USART3_TX_vect
    EV_USART3_TX,
#endif
#ifdef USART3_UDRE_vect
    EV_USART3_UDRE,
#endif
#ifdef USART_RXC_vect
    EV_USART_RXC,
#endif
#ifdef USART_RX_vect
    EV_USART_RX,
#endif
#ifdef USART_TXC_vect
    EV_USART_TXC,
#endif
#ifdef USART_TX_vect
    EV_USART_TX,
#endif
#ifdef USART_UDRE_vect
    EV_USART_UDRE,
#endif
#ifdef USI_OVERFLOW_vect
    EV_USI_OVERFLOW,
#endif
#ifdef USI_OVF_vect
    EV_USI_OVF,
#endif
#ifdef USI_START_vect
    EV_USI_START,
#endif
#ifdef USI_STRT_vect
    EV_USI_STRT,
#endif
#ifdef USI_STR_vect
    EV_USI_STR,
#endif
#ifdef WATCHDOG_vect
    EV_WATCHDOG,
#endif
#ifdef WDT_OVERFLOW_vect
    EV_WDT_OVERFLOW,
#endif
#ifdef WDT_vect
    EV_WDT,
#endif

    EV_EVENT_TIMER,
    EV_RTC_SECONDS,
    EV_RTC_MINUTES,
    EV_ANIMATOR_VALUE_CHANGED,
    EV_ANIMATOR_TICK,
    EV_BUTTON_UP,
    EV_BUTTON_DOWN,
    EV_I2C_SLAVE_RECEIVE,
    EV_USER0,
    EV_USER1,
    EV_USER2,
    EV_USER3,
    EV_USER4,
    EV_USER5,
    EV_USER6,
    EV_USER7,
    EV_USER8,
    EV_USER9,
    EV_USERA,
    EV_USERB,
    EV_USERC,
    EV_USERD,
    EV_USERE,
    EV_USERF,
    
    EV_IDLE,
    
    EV_LAST,
};

#define EV_SIZE ((EV_LAST + 7) / 8)

#ifdef PRINT_ENUMS
const char* g_eventNames[] = {

#ifdef ADC_vect
    "EV_ADC",
#endif
#ifdef ANALOG_COMP_0_vect
    "EV_ANALOG_COMP_0",
#endif
#ifdef ANALOG_COMP_1_vect
    "EV_ANALOG_COMP_1",
#endif
#ifdef ANALOG_COMP_2_vect
    "EV_ANALOG_COMP_2",
#endif
#ifdef ANALOG_COMP_vect
    "EV_ANALOG_COMP",
#endif
#ifdef ANA_COMP_vect
    "EV_ANA_COMP",
#endif
#ifdef CANIT_vect
    "EV_CANIT",
#endif
#ifdef EEPROM_READY_vect
    "EV_EEPROM_READY",
#endif
#ifdef EE_RDY_vect
    "EV_EE_RDY",
#endif
#ifdef EE_READY_vect
    "EV_EE_READY",
#endif
#ifdef EXT_INT0_vect
    "EV_EXT_INT0",
#endif
#ifdef INT0_vect
    "EV_INT0",
#endif
#ifdef INT1_vect
    "EV_INT1",
#endif
#ifdef INT2_vect
    "EV_INT2",
#endif
#ifdef INT3_vect
    "EV_INT3",
#endif
#ifdef INT4_vect
    "EV_INT4",
#endif
#ifdef INT5_vect
    "EV_INT5",
#endif
#ifdef INT6_vect
    "EV_INT6",
#endif
#ifdef INT7_vect
    "EV_INT7",
#endif
#ifdef IO_PINS_vect
    "EV_IO_PINS",
#endif
#ifdef LCD_vect
    "EV_LCD",
#endif
#ifdef LOWLEVEL_IO_PINS_vect
    "EV_LOWLEVEL_IO_PINS",
#endif
#ifdef OVRIT_vect
    "EV_OVRIT",
#endif
#ifdef PCINT0_vect
    "EV_PCINT0",
#endif
#ifdef PCINT1_vect
    "EV_PCINT1",
#endif
#ifdef PCINT2_vect
    "EV_PCINT2",
#endif
#ifdef PCINT3_vect
    "EV_PCINT3",
#endif
#ifdef PCINT_vect
    "EV_PCINT",
#endif
#ifdef PSC0_CAPT_vect
    "EV_PSC0_CAPT",
#endif
#ifdef PSC0_EC_vect
    "EV_PSC0_EC",
#endif
#ifdef PSC1_CAPT_vect
    "EV_PSC1_CAPT",
#endif
#ifdef PSC1_EC_vect
    "EV_PSC1_EC",
#endif
#ifdef PSC2_CAPT_vect
    "EV_PSC2_CAPT",
#endif
#ifdef PSC2_EC_vect
    "EV_PSC2_EC",
#endif
#ifdef SPI_STC_vect
    "EV_SPI_STC",
#endif
#ifdef SPM_RDY_vect
    "EV_SPM_RDY",
#endif
#ifdef SPM_READY_vect
    "EV_SPM_READY",
#endif
#ifdef TIM0_COMPA_vect
    "EV_TIM0_COMPA",
#endif
#ifdef TIM0_COMPB_vect
    "EV_TIM0_COMPB",
#endif
#ifdef TIM0_OVF_vect
    "EV_TIM0_OVF",
#endif
#ifdef TIM1_CAPT_vect
    "EV_TIM1_CAPT",
#endif
#ifdef TIM1_COMPA_vect
    "EV_TIM1_COMPA",
#endif
#ifdef TIM1_COMPB_vect
    "EV_TIM1_COMPB",
#endif
#ifdef TIM1_OVF_vect
    "EV_TIM1_OVF",
#endif
#ifdef TIMER0_CAPT_vect
    "EV_TIMER0_CAPT",
#endif
#ifdef TIMER0_COMPA_vect
    "EV_TIMER0_COMPA",
#endif
#ifdef TIMER0_COMPB_vect
    "EV_TIMER0_COMPB",
#endif
#ifdef TIMER0_COMP_A_vect
    "EV_TIMER0_COMP_A",
#endif
#ifdef TIMER0_COMP_vect
    "EV_TIMER0_COMP",
#endif
#ifdef TIMER0_OVF0_vect
    "EV_TIMER0_OVF0",
#endif
#ifdef TIMER0_OVF_vect
    "EV_TIMER0_OVF",
#endif
#ifdef TIMER1_CAPT1_vect
    "EV_TIMER1_CAPT1",
#endif
#ifdef TIMER1_CAPT_vect
    "EV_TIMER1_CAPT",
#endif
#ifdef TIMER1_CMPA_vect
    "EV_TIMER1_CMPA",
#endif
#ifdef TIMER1_CMPB_vect
    "EV_TIMER1_CMPB",
#endif
#ifdef TIMER1_COMP1_vect
    "EV_TIMER1_COMP1",
#endif
#ifdef TIMER1_COMPA_vect
    "EV_TIMER1_COMPA",
#endif
#ifdef TIMER1_COMPB_vect
    "EV_TIMER1_COMPB",
#endif
#ifdef TIMER1_COMPC_vect
    "EV_TIMER1_COMPC",
#endif
#ifdef TIMER1_COMPD_vect
    "EV_TIMER1_COMPD",
#endif
#ifdef TIMER1_COMP_vect
    "EV_TIMER1_COMP",
#endif
#ifdef TIMER1_OVF1_vect
    "EV_TIMER1_OVF1",
#endif
#ifdef TIMER1_OVF_vect
    "EV_TIMER1_OVF",
#endif
#ifdef TIMER2_COMPA_vect
    "EV_TIMER2_COMPA",
#endif
#ifdef TIMER2_COMPB_vect
    "EV_TIMER2_COMPB",
#endif
#ifdef TIMER2_COMP_vect
    "EV_TIMER2_COMP",
#endif
#ifdef TIMER2_OVF_vect
    "EV_TIMER2_OVF",
#endif
#ifdef TIMER3_CAPT_vect
    "EV_TIMER3_CAPT",
#endif
#ifdef TIMER3_COMPA_vect
    "EV_TIMER3_COMPA",
#endif
#ifdef TIMER3_COMPB_vect
    "EV_TIMER3_COMPB",
#endif
#ifdef TIMER3_COMPC_vect
    "EV_TIMER3_COMPC",
#endif
#ifdef TIMER3_OVF_vect
    "EV_TIMER3_OVF",
#endif
#ifdef TIMER4_CAPT_vect
    "EV_TIMER4_CAPT",
#endif
#ifdef TIMER4_COMPA_vect
    "EV_TIMER4_COMPA",
#endif
#ifdef TIMER4_COMPB_vect
    "EV_TIMER4_COMPB",
#endif
#ifdef TIMER4_COMPC_vect
    "EV_TIMER4_COMPC",
#endif
#ifdef TIMER4_OVF_vect
    "EV_TIMER4_OVF",
#endif
#ifdef TIMER5_CAPT_vect
    "EV_TIMER5_CAPT",
#endif
#ifdef TIMER5_COMPA_vect
    "EV_TIMER5_COMPA",
#endif
#ifdef TIMER5_COMPB_vect
    "EV_TIMER5_COMPB",
#endif
#ifdef TIMER5_COMPC_vect
    "EV_TIMER5_COMPC",
#endif
#ifdef TIMER5_OVF_vect
    "EV_TIMER5_OVF",
#endif
#ifdef TWI_vect
    "EV_TWI",
#endif
#ifdef TXDONE_vect
    "EV_TXDONE",
#endif
#ifdef TXEMPTY_vect
    "EV_TXEMPTY",
#endif
#ifdef UART0_RX_vect
    "EV_UART0_RX",
#endif
#ifdef UART0_TX_vect
    "EV_UART0_TX",
#endif
#ifdef UART0_UDRE_vect
    "EV_UART0_UDRE",
#endif
#ifdef UART1_RX_vect
    "EV_UART1_RX",
#endif
#ifdef UART1_TX_vect
    "EV_UART1_TX",
#endif
#ifdef UART1_UDRE_vect
    "EV_UART1_UDRE",
#endif
#ifdef UART_RX_vect
    "EV_UART_RX",
#endif
#ifdef UART_TX_vect
    "EV_UART_TX",
#endif
#ifdef UART_UDRE_vect
    "EV_UART_UDRE",
#endif
#ifdef USART0_RXC_vect
    "EV_USART0_RXC",
#endif
#ifdef USART0_RX_vect
    "EV_USART0_RX",
#endif
#ifdef USART0_TXC_vect
    "EV_USART0_TXC",
#endif
#ifdef USART0_TX_vect
    "EV_USART0_TX",
#endif
#ifdef USART0_UDRE_vect
    "EV_USART0_UDRE",
#endif
#ifdef USART1_RXC_vect
    "EV_USART1_RXC",
#endif
#ifdef USART1_RX_vect
    "EV_USART1_RX",
#endif
#ifdef USART1_TXC_vect
    "EV_USART1_TXC",
#endif
#ifdef USART1_TX_vect
    "EV_USART1_TX",
#endif
#ifdef USART1_UDRE_vect
    "EV_USART1_UDRE",
#endif
#ifdef USART2_RX_vect
    "EV_USART2_RX",
#endif
#ifdef USART2_TX_vect
    "EV_USART2_TX",
#endif
#ifdef USART2_UDRE_vect
    "EV_USART2_UDRE",
#endif
#ifdef USART3_RX_vect
    "EV_USART3_RX",
#endif
#ifdef USART3_TX_vect
    "EV_USART3_TX",
#endif
#ifdef USART3_UDRE_vect
    "EV_USART3_UDRE",
#endif
#ifdef USART_RXC_vect
    "EV_USART_RXC",
#endif
#ifdef USART_RX_vect
    "EV_USART_RX",
#endif
#ifdef USART_TXC_vect
    "EV_USART_TXC",
#endif
#ifdef USART_TX_vect
    "EV_USART_TX",
#endif
#ifdef USART_UDRE_vect
    "EV_USART_UDRE",
#endif
#ifdef USI_OVERFLOW_vect
    "EV_USI_OVERFLOW",
#endif
#ifdef USI_OVF_vect
    "EV_USI_OVF",
#endif
#ifdef USI_START_vect
    "EV_USI_START",
#endif
#ifdef USI_STRT_vect
    "EV_USI_STRT",
#endif
#ifdef USI_STR_vect
    "EV_USI_STR",
#endif
#ifdef WATCHDOG_vect
    "EV_WATCHDOG",
#endif
#ifdef WDT_OVERFLOW_vect
    "EV_WDT_OVERFLOW",
#endif
#ifdef WDT_vect
    "EV_WDT",
#endif

    "EV_EVENT_TIMER",
    "EV_RTC_SECONDS",
    "EV_RTC_MINUTES",
    "EV_ANIMATOR_VALUE_CHANGED",
    "EV_ANIMATOR_TICK",
    "EV_USER0",
    "EV_USER1",
    "EV_USER2",
    "EV_USER3",
    "EV_USER4",
    "EV_USER5",
    "EV_USER6",
    "EV_USER7",
    "EV_USER8",
    "EV_USER9",
    "EV_USERA",
    "EV_USERB",
    "EV_USERC",
    "EV_USERD",
    "EV_USERE",
    "EV_USERF",
    "EV_IDLE_EVENT",
};

#endif
