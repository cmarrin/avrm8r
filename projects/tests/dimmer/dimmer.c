//
//  dimmer.c
//  dimmer
//
//  Created by Chris Marrin on 4/1/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/pgmspace.h>
//#include <avr/sleep.h>
//#include <inttypes.h>

// Interrupts 

#define MAX_BRIGHT 250
#define MIN_BRIGHT 8

int num2bit(int x)
{
	return (1 << x) - 1;
}

int main (void) 
{
	DDRD = 0xFF;      // port B output
 
	int x = 0;
  
	// Activate ADC with Prescaler 16 --> 1Mhz/16 = 62.5kHz
	ADCSR = _BV(ADEN) | _BV(ADPS2);
  
	for (;;) {  
		// Select pin ADC0 using MUX
		ADMUX = 0;
    
		//Start conversion
		ADCSR |= _BV(ADSC);
    
		// wait until converstion completed
		while (ADCSR & _BV(ADSC) ) {}
    
		// get converted value
		x = ADCW;
		
		// scale
		x -= MIN_BRIGHT;
		if (x < 0)
			x = 0;
		else if (x > MAX_BRIGHT)
			x = MAX_BRIGHT;
		
		x = x * 8 / MAX_BRIGHT;
    
		// output
		PORTD = ~num2bit(x);
	}
}

