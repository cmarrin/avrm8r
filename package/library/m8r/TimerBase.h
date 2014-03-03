//
//  TimerBase.h
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

#include "m8r.h"
#include "EventSourceEnums.h"

/*
 Timers share some common functionality (e.g., prescalers, compare, interrupts).
 This class represents that functionality for all timers. A template class
 is used to allow the specific registers to be passed in. The table below
 summarizes the timer functionality:
 
Template Name   Description             Timer0         Timer1          Timer2
==============+===================+===============+===============+===============
ControlAPort   Control bits         TCCR0A          TCCR1A          TCCR2A
ControlBPort   Control bits         TCCR0B          TCCR1B          TCCR2B
CounterPort    Current ctr value    TCNT0           TCNT1H/TCNT1L   TCNT2
CompareAPort   Compare value        OCR0A           OCR1AH/OCR1AL   OCR2A
CompareBPort   Compare value        OCR0B           OCR1BH/OCR1BL   OCR2B
IrptMaskPort   Irpt mask bits       TIMSK0          TIMSK1          TIMSK2
IrptFlagPort   Irpt flag bits       TIFR0           TIFR1           TIFR2
GenTCCtrlPort  Gen timer/ctr ctrl   ---             ---             GTCCR
               Control bits         ---             TCCR1C          ---
               Input capture        ---             ICR1H/ICR1L     ---
               Async ccontrol       ---             ---             ASSR

Functions in the control and interrupt registers are as follows:

Register   Description             Timer0            Timer1            Timer2
=========+=======================+=================+=================+=================
TCCRxA    Compare match output A   COM0A1/COM0A0     COM1A1/COM1A0     COM2A1/COM2A0
TCCRxA    Compare match output B   COM0B1/COM0B0     COM1B1/COM1B0     COM2B1/COM2B0
TCCRxA    Waveform gen mode        WGM01/WGM00       WGM11/WGM10       WGM21/WGM20

TCCRxB    Waveform gen mode        WGM02             WGM13/WGM12       WGM22
TCCRxB    Force output compare A   FOC0A             --- (1)           FOC2A
TCCRxB    Force output compare B   FOC0B             --- (1)           FOC2B
TCCRxB    Clock select             CS02/CS01/CS00    CS12/CS11/CS10    CS22/CS21/CS20
TCCRxB    Input cap noise cancel   ---               ICNC1             ---
TCCRxB    Input cap edge select    ---               ICES1             ---

TCCRxC    Force output compare A   ---               FOC1A (1)         ---
TCCRxC    Force output compare B   ---               FOC1B (1)         ---

TIMSKx    Output cmp match A irpt OCIE0A             OCIE1A            OCIE2A
TIMSKx    Output cmp match B irpt OCIE0B             OCIE1B            OCIE2B
TIMSKx    Overflow irpt           TOIE0              TOIE1             TOIE2
TIMSKx    Input capture irpt      ---                ICIE1             ---

TIFRx     Output cmp match A flag OCF0A              OCF1A             OCF2A
TIFRx     Output cmp match B flag OCF0B              OCF1B             OCF2B
TIFRx     Overflow flag           TOV0               TOV1              TOV2
TIFRx     Input capture flag      ---                ICF1              ---

GTCCR     Sync mode               TSM                TSM               TSM (2)
GTCCR     Prescaler reset         PSRSYNC            PSRSYNC           --- (3)
GTCCR     Prescaler reset         ---                ---               PSRASY

ASSR      Enable ext clk input    ---                ---               EXCLK
ASSR      Async timer/ctr         ---                ---               AS2
ASSR      TCNT2 update busy       ---                ---               TCN2UB
ASSR      OCR2A update busy       ---                ---               OCR2AUB
ASSR      OCR2B update busy       ---                ---               OCR2BUB
ASSR      TCCR2A update busy      ---                ---               TCR2AUB
ASSR      TCCR2B update busy      ---                ---               TCR2BUB

Notes:
    (1) FOC1A and FOC1B are in TCCR1C for Timer 1, but in the same bit positions
    (2) The TSM bit is shared by all 3 Timers
        This bit can be set and tested in all 3 timer classes. It is up to the
        programmer to avoid conflicts
    (3) The PSRSYNC bit is shared by Timer 0 and Timer 1
        This bit can be set and tested in the Timer0 and Timer1 classes. It is up 
        to the programmer to avoid conflicts
    
Functional Differences:
    - Timer1 is a 16 bit timer, the others are 8 bit.
    - Timer1 has an Input Capture register and associated control bits.
    - Timer1 has an extra bit for the waveform generation mode, giving it
      twice the modes.
    - Timer0 and Timer1 share the prescaler and associated bits. But each
      can be set to independent prescaler values.
    - Timer2 has a separate prescaler, which can be asynchronous.
    - Timer2 has bits associated with accessing its registers asynchronously
    
Note that this timer functionality is common to most if not all megaAVR chips.
*/

// Presccale values for all timers. Lower 3 bits are the same as the bits sent
// to the TCCRnB register. The upper 4 bits are the exponent of the divider,
// or 0xf if there is no divider.
enum TimerClockMode {
    TimerClockStop = 0xf0,
    TimerClockDIV1 = 0x01,
    TimerClockDIV8 = 0x32,
    TimerClockDIV64 = 0x63,
    TimerClockDIV256 = 0x84,
    TimerClockDIV1024 = 0xa5,
    TimerExtClockFall = 0xf6,
    TimerExtClockRise = 0xf7
};

const uint8_t TimerPrescaleMask = 0x07;

#define TimerPrescaleDivisor(prescale) (((prescale >> 4) == 0x0f) ? 0 : (1 << (prescale >> 4)))

// Compare output mode
enum TimerCompOutputMode {
    TimerCompOutputNormal = 0x00,
    TimerCompOutputToggle = 0x01,
    TimerCompOutputClear = 0x02,
    TimerCompOutputSet = 0x03
};

const uint8_t TimerCompOutputMaskA = 0xc0;
const uint8_t TimerCompOutputMaskB = 0x30;
const uint8_t TimerCompOutputShiftA = 6;
const uint8_t TimerCompOutputShiftB = 4;

// Waveform generation mode (for Timer0 and Timer2)
enum TimerWaveGenMode {
    TimerWaveGenNormal = 0x00,
    TimerWaveGenPWM_PC = 0x01, // This is PWM_PC8 for Timer1
    TimerWaveGenCTC = 0x02,
    TimerWaveGenFastPWM = 0x03,
    TimerWaveGenPWM_PC_OCRA = 0x05,
    TimerWaveGenFastPWM_OCRA = 0x07,
    
    // These are mapped values, mask off high bit for actual bit value
    Timer1WaveGenPWM_PC9 = 0x82,
    Timer1WaveGenPWM_PC10 = 0x83,
    Timer1WaveGenCTC = 0x84, // TimerWaveGenCTC maps here
    Timer1WaveGenFastPWM8 = 0x85, // TimerWaveGenFastPWM maps here
    Timer1WaveGenFastPWM9 = 0x86,
    Timer1WaveGenFastPWM10 = 0x87,
    Timer1WaveGenPWM_PFC_ICR = 0x88,
    Timer1WaveGenPWM_PFC_OCRA = 0x89,
    Timer1WaveGenPWM_PC_ICR = 0x8a,
    Timer1WaveGenPWM_PC_OCRA = 0x8b, // TimerWaveGenPWM_PC_OCRA maps here
    Timer1WaveGenCTC_ICR = 0x8c,
    Timer1WaveGenFastPWM_ICR = 0x8e,
    Timer1WaveGenFastPWM_OCRA = 0x0f // TimerWaveGenFastPWM_OCRA maps here
};

const uint8_t TimerWaveGenMaskA = 0x03;
const uint8_t TimerWaveGenMaskB = 0x08;
const uint8_t TimerWaveGenShiftB = 1;
const uint8_t Timer1WaveGenMaskB = 0x18;

// Timer interrupts
enum TimerIrptType {
    TimerOutputCmpMatchAIrpt = 0x02,
    TimerOutputCmpMatchBIrpt = 0x04,
    TimerOverflowIrpt = 0x01,
    Timer1InputCapIrpt = 0x20
};

// Other flags
const uint8_t TimerForceOutputCompareA = 0x80;
const uint8_t TimerForceOutputCompareB = 0x40;
const uint8_t Timer1InputCapNoiseCncl = 0x80;
const uint8_t Timer1InputCapEdgeSel = 0x40;

const uint8_t TimerTSM = 0x80;
const uint8_t TimerPrescalerReset = 0x01;
const uint8_t Timer2PrescalerReset = 0x02;

// Timer2 ASSR flags
const uint8_t Timer2TCCRB2Busy = 0x01;
const uint8_t Timer2TCCRA2Busy = 0x02;
const uint8_t Timer2OCRB2Busy = 0x04;
const uint8_t Timer2OCRA2Busy = 0x08;
const uint8_t Timer2TCNT2Busy = 0x10;
const uint8_t Timer2Async = 0x20;
const uint8_t Timer2ExtClk = 0x40;

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: TimerBase
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
    class IrptFlagPort,
    class GenTCCtrlPort>
class TimerBase {
    typedef TimerBase<CountSize, ControlAPort, ControlBPort, CounterPort, 
                                    CompareAPort, CompareBPort, IrptMaskPort, IrptFlagPort, 
                                    GenTCCtrlPort> TimerBaseType;

public:
	TimerBase()
    {
    }
    
    void setOutputCompareA(CountSize v) { m_compareAPort.set(v); }
    CountSize outputCompareA() const { return m_compareAPort.get(); }
    
    void setOutputCompareB(CountSize v) { m_compareBPort.set(v); }
    CountSize outputCompareB() const { return m_compareBPort.get(); }
    
    void setCounter(CountSize v) { m_counterPort.set(v); }
    CountSize counter() const { return m_counterPort.get(); }
    
    void setTimerClockMode(TimerClockMode v) { m_controlBPort.setMaskedBits(v, TimerPrescaleMask); }
    void setCompOutputAMode(TimerCompOutputMode mode) { m_controlAPort.setMaskedBits(mode << TimerCompOutputShiftA, TimerCompOutputMaskA); }
    void setCompOutputBMode(TimerCompOutputMode mode) { m_controlAPort.setMaskedBits(mode << TimerCompOutputShiftB, TimerCompOutputMaskB); }

    void setForceOutputCompareA(bool e) { m_controlBPort.setBitMask(TimerForceOutputCompareA, e); }
    void setForceOutputCompareB(bool e) { m_controlBPort.setBitMask(TimerForceOutputCompareB, e); }
    
    void setIrptEnabled(TimerIrptType type, bool enabled) { m_irptMaskPort.setBitMask(type, enabled); }
    bool isIrptTriggered(TimerIrptType type) const { return m_irptFlagPort.isBitMaskSet(type); }
    
    void setTimerTSM(bool e) { m_genTCCtrlPort.setBitMask(TimerTSM, e); }
    
protected:
    ControlAPort m_controlAPort;
    ControlBPort m_controlBPort;
    CounterPort m_counterPort;
    CompareAPort m_compareAPort;
    CompareBPort m_compareBPort;
    IrptMaskPort m_irptMaskPort;
    IrptFlagPort m_irptFlagPort;
    GenTCCtrlPort m_genTCCtrlPort;
};

}