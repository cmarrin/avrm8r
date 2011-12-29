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

#include "m8r.h"
#include "Application.h"
#include "ADC.h"
#ifdef DEBUG
#include "BlinkErrorReporter.h"
#endif
#include "Button.h"
#include "ENC28J60.h"
#include "EventListener.h"
#include "MAX6969.h"
#include "Network.h"
#include "RTC.h"
#include "Timer0.h"
#include "Timer1.h"
#include "TimerEventMgr.h"
#include "UDPSocket.h"

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
//  Port B
//      1 - On board LED
//      2 - ENC28J60 /CS 
//      3 - ENC28J60 SI
//      4 - ENC28J60 SO
//      5 - ENC28J60 SCK
//
//  Port C
//      0 - Light sensor
//      1 - Clk
//      2 - Data
//      3 - Latch
//      4 - Enable
//      5 - Button
//
//  Port D
//      0 - Colon 0
//      1 - Colon 1

using namespace m8r;

//#define TEST_LOOP_TIMING

#define ErrorPort Port<B>
#define ErrorBit 1

const uint8_t MacAddr[6] = {'m', 't', 'e', 't', 'h', 0x01};
const uint8_t IPAddr[4] = { 10, 0, 1, 210 };
const uint8_t GWAddr[4] = { 10, 0, 1, 1 };
const uint8_t DestAddr[4] = { 10, 0, 1, 201 };

class MyApp;

#ifdef DEBUG
class MyErrorReporter : public ErrorReporter {
public:
    virtual void reportError(char c, uint16_t, ErrorConditionType);
};
#endif

class MyApp : public EventListener {    
public:
    MyApp();
    
    void accumulateBrightnessValue(uint8_t value)
    {
        m_accumulatedLightSensorValues += value;
        if (++m_numAccumulatedLightSensorValues >= 4) {
            m_averageLightSensorValue = m_accumulatedLightSensorValues >> 2;
            m_accumulatedLightSensorValues = 0;
            m_numAccumulatedLightSensorValues = 0;
        }
    }
    
    // EventListener override
    virtual void handleEvent(EventType type, EventParam);
    
    void updateDisplay();

    void showChars(const char chars[4], uint8_t dps, bool showLeadingZero);
    void scrollChars(const char* string);

#ifdef DEBUG
    //BlinkErrorReporter<Port<B>, 1> m_errorReporter;
    MyErrorReporter m_errorReporter;
#endif
    ADC m_adc;
    MAX6969<Port<C>, 1, Port<C>, 2, Port<C>, 3, Port<C>, 4> m_shiftReg;
    TimerEventMgr<Timer0> m_timerEventMgr;
    RTC<Timer1> m_clock;
    Network<ENC28J60<ClockOutDiv2, _BV(MSTR), _BV(SPI2X)> > m_network;
    UDPSocket m_socket;
    Port<D> m_colonPort;
    Button<Port<C>, 5, 10, 5> m_button;
    
    enum DisplayState { DisplayTime, DisplayDay, DisplayDate, DisplayCurrentTemp, DisplayHighTemp, DisplayLowTemp };
    DisplayState m_displayState;
    TimerID m_displaySequenceTimer;
    
    uint16_t m_accumulatedLightSensorValues;
    uint8_t m_numAccumulatedLightSensorValues;
    uint8_t m_averageLightSensorValue, m_lastAverageLightSensorValue;
    uint8_t m_currentBrightness;
    uint8_t m_brightnessCount;
    
    uint8_t m_currentColonBrightness;
    uint8_t m_colonBrightnessCount;
    
    static uint8_t m_brightnessTable[8];
    
    int8_t m_curTemp, m_lowTemp, m_highTemp;
};

uint8_t MyApp::m_brightnessTable[] = { 30, 60, 90, 120, 150, 180, 210, 255 };
const int8_t Hysteresis = 2;

MyApp g_app;

#ifdef DEBUG
void
MyErrorReporter::reportError(char c, uint16_t code, ErrorConditionType type)
{
    cli();
    
    bool truncateLeadingZeros = code < 256;
    char string[4];
    for (uint8_t i = 4; i > 0; --i) {
        char c = code & 0xf;
        c += ((c > 9) ? ('A' - 10) : '0');
        if ((i == 1 || i == 2) && truncateLeadingZeros)
            c = ' ';
        string[i - 1] = c;
        code >>= 4;
    }
    
    if (c)
        string[0] = c;
    
    uint8_t dps = (type == ErrorConditionNote) ? 1 : ((type == ErrorConditionWarning) ? 3 : 7);
    g_app.showChars(string, dps, true);

    for (uint8_t i = 0; i < 3; ++i) {
        g_app.m_shiftReg.setOutputEnable(true);
        Application::msDelay<900>();
        g_app.m_shiftReg.setOutputEnable(false);
        Application::msDelay<100>();
    }
    
    g_app.m_shiftReg.setOutputEnable(true);
    if (type == ErrorConditionFatal)
        while (1) ;
    
    Application::msDelay<1000>();
    sei();
}
#endif

static uint32_t parseNumber(const uint8_t* string)
{
    uint8_t c;
    uint32_t n;
    while ((c = *string++)) {
        if (c == ' ')
            continue;
        if (c < '0' || c > '9')
            break;
            
        n *= 10;
        n += c - '0';
    }
    
    return n;
}

const char startupMessage[] = "EtherClock  v1-0";

static void
networkUpdateCallback(Socket* socket, Socket::EventType type, const uint8_t* data, uint16_t length, void*)
{
    if (type == Socket::EventSendDataReady) {
        g_app.m_socket.send(0, 0);
        return;
    }
    
    if (type != Socket::EventDataReceived)
        return;
        
    if (data[0] == 'T')
        g_app.m_clock.setTicks(parseNumber(&data[1]) - 8 * 60 * 60);
}

MyApp::MyApp()
    : m_adc(0, ADC_PS_DIV128, ADC_REF_AVCC)
    , m_timerEventMgr(TimerClockDIV64, 195) // ~100us timer
    , m_clock(TimerClockDIV1, 12499, 1000) // 1ms timer
    , m_network(MacAddr, IPAddr, GWAddr)
    , m_socket(&m_network, networkUpdateCallback, this)
    , m_accumulatedLightSensorValues(0)
    , m_numAccumulatedLightSensorValues(0)
    , m_averageLightSensorValue(0xff)
    , m_lastAverageLightSensorValue(0)
    , m_currentBrightness(0xff)
    , m_brightnessCount(0)
    , m_currentColonBrightness(0xff)
    , m_colonBrightnessCount(0)
    , m_curTemp(-1)
    , m_lowTemp(-1)
    , m_highTemp(-1)
{
    m_shiftReg.setOutputEnable(true);
    
#ifdef TEST_ALL_CHARS
    // Test all chars
    for (char c = 0x20; c < 0x5f; ) {
        char string[4];
        for (uint8_t i = 0; i < 4; ++i, c++)
            string[i] = c;
        showChars(string, 0x08, true);
        Application::msDelay<2000>();
    }
#endif

    // Show startup
    scrollChars(startupMessage);
    Application::msDelay<1000>();
    
    m_colonPort.setBitOutput(0);
    m_colonPort.setBitOutput(1);
    m_colonPort.setPortBit(0);
    m_colonPort.setPortBit(1);
    
    m_adc.setEnabled(true);
    
    sei();
    m_adc.startConversion();
    
    m_socket.listen(1956);
    m_socket.requestSend(DestAddr, 1956);
}

void
MyApp::updateDisplay()
{
    switch (m_displayState) {
        case DisplayDate:
        case DisplayDay:
        case DisplayTime: {
            RTCTime t;
            g_app.m_clock.currentTime(t);

            uint8_t hours = t.hours;
            uint8_t dps = 0;
            if (hours > 12) {
                hours -= 12;
                dps = 0x08;
            }
            
            char string[4];
            if (hours < 10)
                string[0] = 0x20;
            else
                string[0] = (hours / 10) + '0';
            string[1] = (hours % 10) + '0';
            string[2] = (t.minutes / 10) + '0';
            string[3] = (t.minutes % 10) + '0';
            showChars(string, dps, false);
            break;
        }
        case DisplayDay:
            break;
        case DisplayDate:
            break;
        case DisplayCurrentTemp:
            break;
        case DisplayHighTemp:
            break;
        case DisplayLowTemp:       
            break;
        default:
            break;
    }
}

void
MyApp::showChars(const char* string, uint8_t dps, bool showLeadingZero)
{
    bool blank = false;
    for (uint8_t i = 0; i < 4; ++i, dps <<= 1) {
        if (!blank && string[i] == '\0' && !showLeadingZero)
            blank = true;
        m_shiftReg.setChar(blank ? ' ' : string[i], dps & 0x08);
    }
    m_shiftReg.latch();
}

void
MyApp::scrollChars(const char* string)
{
    showChars("    ", 0, true);
    
    for (const char* s = string; *s; s++) { 
        m_shiftReg.setChar(*s);
        m_shiftReg.latch();
        Application::msDelay<200>();
    }
}

#ifdef TEST_LOOP_TIMING
const uint8_t TestLoopSecondsToDisplay = 10;

static uint32_t g_testLoopIterationCount = 0;
static uint8_t g_testLoopIterationSecondsLeft = TestLoopSecondsToDisplay;
#endif

void
MyApp::handleEvent(EventType type, EventParam param)
{
    switch(type)
    {
        case EV_ADC:
            g_app.accumulateBrightnessValue(g_app.m_adc.lastConversion8Bit());
            break;
        case EV_IDLE:
#ifdef TEST_LOOP_TIMING
            g_testLoopIterationCount++;
#endif
            if (g_app.m_brightnessCount++ == g_app.m_currentBrightness)
                g_app.m_shiftReg.setOutputEnable(false);

            if (g_app.m_brightnessCount == 0) {
                g_app.m_shiftReg.setOutputEnable(true);
                
                // Add some hysteresis to the brightness value
                int8_t diff = (int16_t) g_app.m_lastAverageLightSensorValue - (int16_t) g_app.m_averageLightSensorValue;
                if (diff < 0)
                    diff = -diff;
                if (diff < Hysteresis)
                    return;
                    
                g_app.m_lastAverageLightSensorValue = g_app.m_averageLightSensorValue;
                g_app.m_currentBrightness = MyApp::m_brightnessTable[g_app.m_averageLightSensorValue >> 5];
                g_app.m_currentColonBrightness = g_app.m_currentBrightness;
            }
            
            if (g_app.m_colonBrightnessCount++ == g_app.m_currentColonBrightness) {
                g_app.m_colonPort.setPortBit(0);
                g_app.m_colonPort.setPortBit(1);
            }
            
            if (g_app.m_colonBrightnessCount == 0) {
                g_app.m_colonPort.clearPortBit(0);
                g_app.m_colonPort.clearPortBit(1);
            }
            break;
        case EV_RTC_SECONDS: {
#ifdef TEST_LOOP_TIMING
            if (--g_testLoopIterationSecondsLeft == 0) {
                g_testLoopIterationCount /= TestLoopSecondsToDisplay;
                if (g_testLoopIterationCount > 65536 * 256)
                    CNOTE('D', g_testLoopIterationCount >> 24);
                CNOTE('C', (g_testLoopIterationCount >> 16) & 0xff);
                NOTE(g_testLoopIterationCount & 0xffff);
                g_testLoopIterationCount = 0;
                g_testLoopIterationSecondsLeft = TestLoopSecondsToDisplay;
            }
#endif
            g_app.m_adc.startConversion();
            updateDisplay();
            break;
        }
        case EV_BUTTON_DOWN:
            m_displaySequenceTimer = Application::startEventTimer(2000);
            m_displayState = DisplayDay;
            break;
        case EV_EVENT_TIMER:
            if (m_displaySequenceTimer != MakeTimerID(param))
                break;
                
            if (m_displayState == DisplayDay)
                m_displayState = DisplayDate;
            else if (m_displayState == DisplayDate)
                m_displayState = DisplayCurrentTemp;
            else if (m_displayState == DisplayCurrentTemp)
                m_displayState = DisplayHighTemp;
            else if (m_displayState == DisplayHighTemp)
                m_displayState = DisplayLowTemp;
            else if (m_displayState == DisplayLowTemp)
                m_displayState = DisplayTime;
                
            if (m_displayState != DisplayTime)
                m_displaySequenceTimer = Application::startEventTimer(2000);
        default:
            break;
    }
}
