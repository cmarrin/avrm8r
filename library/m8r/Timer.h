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

// Presccale values for all timers
enum TimerClockMode {
    TimerClockStop = 0x00,
    TimerClockDIV1 = 0x01,
    TimerClockDIV8 = 0x02,
    TimerClockDIV64 = 0x03,
    TimerClockDIV256 = 0x04,
    TimerClockDIV1024 = 0x05,
    TimerExtClockFall = 0x06,
    TimerExtClockRise = 0x07
};

const TimerPrescaleMask = 0x07;

// Compare output mode
enum TimerCompOutputMode {
    TimerCompOutputNormal = 0x00,
    TimerCompOutputToggle = 0x01,
    TimerCompOutputClear = 0x02,
    TimerCompOutputSet = 0x03
};

const TimerCompOutputMaskA = 0xc0;
const TimerCompOutputMaskB = 0x30;
const TimerCompOutputShiftA = 6;
const TimerCompOutputShiftB = 4;

// Waveform generation mode (for Timer0 and Timer2)
enum TimerWaveGenMode {
    TimerWaveGenNormal = 0x00,
    TimerWaveGenPWMPC = 0x01,
    TimerWaveGenCTC = 0x02,
    TimerWaveGenFastPWM = 0x03,
    TimerWaveGenFastPWM_OCRA = 0x05,
    TimerWaveGenFastPWM_OCRARev = 0x07
};

const TimerWaveGenMaskA = 0x03;
const TimerWaveGenMaskB = 0x08;
const TimerWaveGenShiftB = 1;

// Waveform generation mode (for Timer1)
enum Timer1WaveGenMode {
    Timer1WaveGenNormal = 0x00,
    Timer1WaveGenPWM_PC8 = 0x01,
    Timer1WaveGenPWM_PC9 = 0x02,
    Timer1WaveGenPWM_PC10 = 0x03,
    Timer1WaveGenCTC_OCRA = 0x04,
    Timer1WaveGenFastPWM8 = 0x05,
    Timer1WaveGenFastPWM9 = 0x06,
    Timer1WaveGenFastPWM10 = 0x07,
    Timer1WaveGenPWM_PFC_ICR = 0x08,
    Timer1WaveGenPWM_PFC_OCRA = 0x09,
    Timer1WaveGenPWM_PC_ICR = 0x0a,
    Timer1WaveGenPWM_PC_OCRA = 0x0b,
    Timer1WaveGenCTC_ICR = 0x0c,
    Timer1WaveGenFastPWM_ICR = 0x0e,
    Timer1WaveGenFastPWM_OCRA = 0x0f
};

const Timer1WaveGenMaskA = 0x03;
const Timer1WaveGenMaskB = 0x18;
const Timer1WaveGenShiftB = 1;

// Timer interrupts
enum TimerIrptType {
    TimerOutputCmpMatchAIrpt = 0x02,
    TimerOutputCmpMatchBIrpt = 0x04,
    TimerOverflowIrpt = 0x01,
    Timer1InputCapIrpt = 0x20
};

// Other flags
const TimerForceOutputCompareA = 0x80;
const TimerForceOutputCompareB = 0x40;
const Timer1InputCapNoiseCncl = 0x80;
const Timer1InputCapEdgeSel = 0x40;

const TimerTSM = 0x80;
const TimerPrescalerReset = 0x01;
const Timer2PrescalerReset = 0x02;

// Timer2 ASSR flags
const Timer2TCCRB2Busy = 0x01;
const Timer2TCCRA2Busy = 0x02;
const Timer2OCRB2Busy = 0x04;
const Timer2OCRA2Busy = 0x08;
const Timer2TCNT2Busy = 0x10;
const Timer2Async = 0x20;
const Timer2ExtClk = 0x40;

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
    class IrptFlagPort,
    class GenTCCtrlPort>
class Timer {
public:
	Timer()
    {
        setPrescaler(TIMER_CLK_STOP);
        setCount(0);
    }
    
    void setOutputCompareA(CountSize v) { m_compareAPort.set(v); }
    CountSize outputCompareA() const { return m_compareAPort.get(); }
    
    void setOutputCompareB(CountSize v) { m_compareBPort.set(v); }
    CountSize outputCompareB() const { return m_compareBPort.get(); }
    
    void setCounter(CountSize v) { m_counterPort.set(v); }
    CountSize counter() const { return m_counterPort.get(); }
    
    void setTimerClockMode(TimerClockMode v) { m_controlBPort.setMaskedBits(v, TIMER_PRESCALE_MASK); }
    void setCompOutputAMode(TimerCompOutputMode mode) { m_controlAPort.setMaskedBits(mode << TimerCompOutputShiftA, TimerCompOutputMaskA); }
    void setCompOutputBMode(TimerCompOutputMode mode) { m_controlAPort.setMaskedBits(mode << TimerCompOutputShiftB, TimerCompOutputMaskB); }
    void setWaveGenMode(TimerWaveGenMode mode)
    {
        m_controlAPort.setMaskedBits(mode, TimerWaveGenMaskA);
        m_controlBPort.setMaskedBits(mode << TimerWaveGenShiftB, TimerWaveGenMaskB);
    }

    void setForceOutputCompareA(bool e) { m_controlBPort.setBitMask(TimerForceOutputCompareA, e); }
    void setForceOutputCompareB(bool e) { m_controlBPort.setBitMask(TimerForceOutputCompareB, e); }
    
    void setIrptEnabled(TimerIrptType type, bool enabled) { m_irptMaskPort.setBitMask(type, enabled); }
    bool isIrptTriggered(TimerIrptType type) const { return m_irptFlagPort.isBitMaskSet(type); }
    
    void setTimerTSM(bool e) { m_genTCCtrlPort.setBitMask(TimerTSM, e); }
    void setPrescaleReset(bool e) { m_genTCCtrlPort.setBitMask(TimerPrescalerReset, e); }
    
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

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Timer0
//
//  8 Bit timer
//
//////////////////////////////////////////////////////////////////////////////

class Timer0 : public<uint8_t,
                    Reg8<_TCCR0A>,
                    Reg8<_TCCR0B>,
                    Reg8<_TCNT0>,
                    Reg8<_OCR0A>,
                    Reg8<_OCR0B>,
                    Reg8<_TIMSK0>,
                    Reg8<_TIFR0>,
                    Reg8<_GTCCR> > { };

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Timer1
//
//  16 Bit timer
//
//////////////////////////////////////////////////////////////////////////////

class Timer1 : public<uint16_t,
                    Reg8<_TCCR1A>,
                    Reg8<_TCCR1B>,
                    Reg16<_TCNT1>,
                    Reg16<_OCR1A>,
                    Reg16<_OCR1B>,
                    Reg8<_TIMSK1>,
                    Reg8<_TIFR1>,
                    Reg8<_GTCCR> >
{
    void setWaveGenMode(Timer1WaveGenMode mode)
    {
        m_controlAPort.setMaskedBits(mode, Timer1WaveGenMaskA);
        m_controlBPort.setMaskedBits(mode << Timer1WaveGenShiftB, Timer1WaveGenMaskB);
    }
    
    void setInputCap(uint16_t v) { m_inputCapPort.set(v); }

private:
    Reg8<_TCCR1C> m_controlPortC;
    Reg16<_ICR1> m_inputCapPort;
};

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Timer2
//
//  8 Bit timer with async mode
//
//////////////////////////////////////////////////////////////////////////////

class Timer2 : public<uint8_t,
                    Reg8<_TCCR0A>,
                    Reg8<_TCCR0B>,
                    Reg8<_TCNT0>,
                    Reg8<_OCR0A>,
                    Reg8<_OCR0B>,
                    Reg8<_TIMSK0>,
                    Reg8<_TIFR0>,
                    Reg8<_GTCCR> >
{
    void setPrescaleReset(bool e) { m_genTCCtrlPort.setBitMask(Timer2PrescalerReset, e); }
    void setExtClk(bool e) { m_asyncStatusPort.setMaskedBits(Timer2ExtClk, e); }
    void setAsync(bool e) { m_asyncStatusPort.setMaskedBits(Timer2Async, e); }
    
    bool isTCCRABusy() const { return m_asyncStatusPort.isBitMaskSet(Timer2TCCRA2Busy); }
    bool isTCCRBBusy() const { return m_asyncStatusPort.isBitMaskSet(Timer2TCCRB2Busy); }
    bool isOCRABusy() const { return m_asyncStatusPort.isBitMaskSet(Timer2OCRA2Busy); }
    bool isOCRBBusy() const { return m_asyncStatusPort.isBitMaskSet(Timer2OCRB2Busy); }
    bool isTCNTBusy() const { return m_asyncStatusPort.isBitMaskSet(Timer2TCNT2Busy); }
    
private:
    Reg8<_ASSR> m_asyncStatusPort;
};
