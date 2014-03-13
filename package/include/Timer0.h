//
//  Timer0.h
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

#include "TimerBase.h"

#include "System.h"

namespace m8r {

//////////////////////////////////////////////////////////////////////////////
//
//  Class: Timer0
//
//  8 Bit timer
//
//////////////////////////////////////////////////////////////////////////////

class Timer0 : public TimerBase<uint8_t,
                    Reg8<_TCCR0A>,
                    Reg8<_TCCR0B>,
                    Reg8<_TCNT0>,
                    Reg8<_OCR0A>,
                    Reg8<_OCR0B>,
                    Reg8<_TIMSK0>,
                    Reg8<_TIFR0>,
                    Reg8<_GTCCR> >
{
public:
    Timer0(EventCallback isrCallback = 0, EventParam param = EventParam())
    {
        ASSERT(!m_isrCallback, AssertSingleTimer0);
        m_isrCallback = isrCallback ? isrCallback : &System::fireISR;
        m_param = param;
    }
    
    void setWaveGenMode(TimerWaveGenMode mode)
    {
        m_controlAPort.setMaskedBits(mode, TimerWaveGenMaskA);
        m_controlBPort.setMaskedBits(mode << TimerWaveGenShiftB, TimerWaveGenMaskB);
    }

    void setPrescaleReset(bool e) { m_genTCCtrlPort.setBitMask(TimerPrescalerReset, e); }

    static EventCallback m_isrCallback;
    static EventParam m_param;
};

}