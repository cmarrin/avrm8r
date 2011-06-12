//
//  Timer.h
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

#include "m8r/EventSource.h"

/*
 Timers share some common functionality (e.g., prescalers, compare, interrupts).
 This class represents that functionality for all timers. A template class
 is used to allow the specific registers to be passed in. The table below
 summarizes the timer functionality:
 
Template Name   Description             Timer1         Timer2          Timer3
==============+===================+===============+===============+===============
ControlAPort   Control bits         TCCR0A          TCCR1A          TCCR1A
ControlBPort   Control bits         TCCR0B          TCCR1B          TCCR2B
               Control bits         ---             TCCR1C          ---
CounterPort    Current ctr value    TCNT0           TCNT1H/TCNT1L   TCNT2
CompareAPort   Compare value        OCR0A           OCR1AH/OCR1AL   OCR2A
CompareBPort   Compare value        OCR0B           OCR1BH/OCR1BL   OCR2B
               Input capture        ---             ICR1H/ICR1L     ---
IrptMaskPort   Irpt mask bits       TIMSK0          TIMSK1          TIMSK2
IrptFlagPort   Irpt flag bits       TIFR0           TIFR1           TIFR2
               Async ccontrol       ---             ---             ASSR
               Gen timer/ctr ctrl   ---             ---             GTCCR


 Note that this timer functionality is common to most if not all megaAVR chips.
*/

// presccale values for all timers
#define TIMER_CLK_STOP          0x00    ///< Timer Stopped
#define TIMER_CLK_DIV1          0x01    ///< Timer clocked at F_CPU
#define TIMER_CLK_DIV8          0x02    ///< Timer clocked at F_CPU/8
#define TIMER_CLK_DIV64         0x03    ///< Timer clocked at F_CPU/64
#define TIMER_CLK_DIV256        0x04    ///< Timer clocked at F_CPU/256
#define TIMER_CLK_DIV1024       0x05    ///< Timer clocked at F_CPU/1024
#define TIMER_CLK_T_FALL        0x06    ///< Timer clocked at T falling edge
#define TIMER_CLK_T_RISE        0x07    ///< Timer clocked at T rising edge
#define TIMER_PRESCALE_MASK     0x07    ///< Timer Prescaler Bit-Mask

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Timer
//
//  Template base class for timers
//
//////////////////////////////////////////////////////////////////////////////

template <
    class CountSize, // uint8_t or uint16_t
    class ControlAPort, 
    class ControlBPort, 
    class CounterPort, 
    class CompareAPort, 
    class CompareBPort, 
    class IrptMaskPort, 
    class IrptFlagPort>
class Timer {
public:
	Timer()
    {
        setPrescaler(TIMER_CLK_STOP);
        setCount(0);
    }
    
    void setInterruptEnabled(uint8_t irpt, bool b);
    bool isInterruptEnabled(uint8_t irpt) const;
        
    void setPrescaler(uint8_t v) { m_controlBPort.setMaskedBits(v, TIMER_PRESCALE_MASK); }
    uint8_t getPrescaler() const { return m_controlBPort.get() & TIMER_PRESCALE_MASK; }

    void setOutputCompareA(CountSize v) { CompareAPort.set(v); }
    CountSize getOutputCompareA() const { return CompareAPort.get(); }
    
    void setOutputCompareB(CountSize v) { CompareBPort.set(v); }
    CountSize getOutputCompareB() const { return CompareBPort.get(); }
    
    void setValue(CountSize v) { CounterPort.set(v); }
    CountSize getValue() const { return CounterPort.get(); }
    
    void setClearOnOutputCompare(bool b)
    {
        // FIXME: This is different on modern controllers
        if (b)
            TCCR1B |= _BV(WGM10);
        else
            TCCR1B &= ~_BV(WGM10);
    }
    
    // FIXME: This is different on modern controllers
    bool isClearOnOutputCompare() const  { return (TCCR1B & _BV(WGM10)) != 0; }

private:
    ControlAPort m_controlAPort;
    ControlBPort m_controlBPort;
    CounterPort m_counterPort;
    CompareAPort m_compareAPort;
    CompareBPort m_compareBPort;
    IrptMaskPort m_irptMaskPort;
    IrptFlagPort m_irptFlagPort;
};

}
