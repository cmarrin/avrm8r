//
//  MyApplication.cpp
//
//  Created by Chris Marrin on 3/19/2011.
//
//  Etherclock - Ethernet-connected clock, using Tuxgraphics Ethernet board

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

#include <m8r.h>
#include <m8r/Animator.h>
#include <m8r/Application.h>
#include <m8r/ADC.h>
#include <m8r/BlinkErrorReporter.h>
#include <m8r/ENC28J60.h>
#include <m8r/MAX6969.h>
#include <m8r/RTC.h>
#include <m8r/TimerEventMgr.h>

//
// Etherclock
//
// Connections to display board:
//
//   1 - Gnd
//   2 - Colon 0
//   3 - Colon 1
//   4 - /OE
//   5 - LE
//   6 - Data
//   7 - Clk
//   8 - Light sensor
//   9 - 5v
//  10 - 3.3v
//
// AVR Ports
//  Port C
//      0 - Light sensor
//      1 - Clk
//      2 - Data
//      3 - Latch
//      4 - Enable
//
//  Port D
//      0 - Colon 0
//      1 - Colon 1

using namespace m8r;

#define ErrorPort Port<B>
#define ErrorBit 1

const uint8_t NumLightSensorValuesToAccumulate = 250;
const uint8_t MacAddr[6] = {'m', 't', 'e', 't', 'h', 0x01};

class MyApp {    
public:
    MyApp();
    
    uint8_t brightness() const { return m_currentBrightness; }

    void accumulateBrightnessValue(uint8_t value)
    {
        m_accumulatedLightSensorValues += value;
        if (++m_numAccumulatedLightSensorValues >= NumLightSensorValuesToAccumulate) {
            m_currentBrightness = g_brightnessTable[m_accumulatedLightSensorValues / NumLightSensorValuesToAccumulate / 32];
            m_accumulatedLightSensorValues = 0;
            m_numAccumulatedLightSensorValues = 0;
        }
    }
    
    BlinkErrorReporter<Port<B>, 1> m_errorReporter;
    ADC m_adc;
    MAX6969<Port<C>, 1, Port<C>, 2, Port<C>, 3, Port<C>, 4> m_shiftReg;
    RTC<Timer1> m_clock;
    ENC28J60 m_ethernet;
    Port<D> m_colonPort;
    
    uint16_t m_accumulatedLightSensorValues;
    uint8_t m_numAccumulatedLightSensorValues;
    uint8_t m_currentBrightness;
    uint8_t m_brightnessCount;
    uint8_t m_animationValue;
    
    static uint8_t g_brightnessTable[8];
};

uint8_t MyApp::g_brightnessTable[] = { 30, 60, 90, 120, 150, 180, 210, 255 };

MyApp g_app;

MyApp::MyApp()
    : m_adc(0, ADC_PS_DIV128, ADC_REF_AVCC)
    , m_clock(TimerClockDIV1, 12499, 1000) // 1ms timer
    , m_ethernet(MacAddr, ClockOutDiv2, _BV(MSTR), _BV(SPI2X))
    , m_accumulatedLightSensorValues(0)
    , m_numAccumulatedLightSensorValues(0)
    , m_currentBrightness(0)
    , m_brightnessCount(0)
    , m_animationValue(0)
{
    // Testing
    m_shiftReg.setChar('1', true);
    m_shiftReg.setChar('2', true);
    m_shiftReg.setChar('3', true);
    m_shiftReg.setChar('4', false);
    m_shiftReg.latch();
    m_shiftReg.setOutputEnable(true);
    
    m_colonPort.setBitOutput(0);
    m_colonPort.setBitOutput(1);
    m_colonPort.setPortBit(0);
    m_colonPort.setPortBit(1);
    
    m_adc.setEnabled(true);
    
    sei();
    m_adc.startConversion();
}
    
void
Application::handleISR(EventType type, void*)
{
    switch(type)
    {
        case EV_ADC:
            g_app.accumulateBrightnessValue(g_app.m_adc.lastConversion8Bit());
            g_app.m_adc.startConversion();
            break;
        case EV_ANIMATOR_EVENT:
            //m_animationValue = Animator::sineValue(m_colonAnimator.currentValue());
            break;            
        case EV_TIMER_EVENT:
            break;
        case EV_RTC_SECONDS_EVENT: {
            RTCTime t;
            g_app.m_clock.currentTime(t);
            
            // FIXME: This is bogus, just for testing
            uint8_t c = (t.seconds % 10) + '0';
            g_app.m_shiftReg.setChar(c, false);
            g_app.m_shiftReg.setChar(c, false);
            g_app.m_shiftReg.setChar(c, false);
            g_app.m_shiftReg.setChar(c, false);
            g_app.m_shiftReg.latch();
            break;
        }
        default:
            break;
    }
}

void
Application::handleIdle()
{
    if (g_app.m_brightnessCount++ == g_app.brightness()) {
        g_app.m_shiftReg.setOutputEnable(false);
        g_app.m_colonPort.setPortBit(0);
        g_app.m_colonPort.setPortBit(1);
    } else if (g_app.m_brightnessCount == 0) {
        g_app.m_shiftReg.setOutputEnable(true);
        g_app.m_colonPort.clearPortBit(0);
        g_app.m_colonPort.clearPortBit(1);
    }
}

void
Application::handleErrorCondition(ErrorType type, ErrorConditionType condition)
{
    g_app.m_errorReporter.reportError(type, condition);
}


