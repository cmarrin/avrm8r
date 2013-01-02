/* vim: set sw=8 ts=8 si et : */
/*********************************************
* Author: Guido Socher
* Copyright: GPL V2
**********************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../timeout.h"

// 1.1V Vref with capacitor on AREF pin:
#if defined(__AVR_ATmega644__)||defined(__AVR_ATmega644P__)
#define ADMUX_REFSBITS (1<<REFS1)
#else
// atmega168, atmega328, 1.1V:
#define ADMUX_REFSBITS (1<<REFS1)|(1<<REFS0)
#endif
// Pre-scaling is needed to stay in the recommended range of 50-200kHz 
// Clock freq for the adc.
//pre-scale by 128:
#define ADCSRA_PSBITS (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)
// pre-scale by 64 (at 12MHz this will be 187kHz ADC clock):
//#define ADCSRA_PSBITS (1<<ADPS2)|(1<<ADPS1)

// return analog value of a given channel. Works without interrupt 
// Resolution = 10bit
uint16_t convertanalog(unsigned char channel) 
{
        unsigned char adlow;
        uint16_t result;
        //  enable analog to digital conversion in single run mode
        //  without noise canceler function. See datasheet of atmega88 page 250
        // ADEN: Analog Digital Converter Enable
        // ADIE: ADC Interrupt Enable  (0=no interrupt)
        // ADIF: ADC Interrupt Flag
        // ADCSR: ADC Control and Status Register
        // ADPS2..ADPS0: ADC Prescaler Select Bits
        // REFS: Reference Selection Bits (page 203)
        // 
        // write only the lower 3 bit for channel selection
        ADMUX=ADMUX_REFSBITS |(channel & 0x7);
        // 
        // switch off digital input line:
        if (channel <6){// 6 and 7 have no digital input
                DIDR0=(1<<channel)& 0x3f;
        }
        ADCSRA=(1<<ADEN)|ADCSRA_PSBITS;
        //  start conversion 
        ADCSRA|= (1<<ADSC);
        while(bit_is_set(ADCSRA,ADSC)); // wait for result 
        adlow=ADCL; // read low first
        // use only 10 bit resolution:
        result=(ADCH<<8)|adlow;
        return(result);
}

