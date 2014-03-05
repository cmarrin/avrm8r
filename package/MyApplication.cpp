//
//  MyApplication.cpp
//
//  Created by Chris Marrin on 2/27/2014.
//

#include "m8r.h"
#include "System.h"
#include "EventListener.h"
#include "RTC.h"
#include "Serial.h"
#include "Timer0.h"
#include "TimerEventMgr.h"
#include "USART.h"

#include "BlinkErrorReporter.h"

//
//  Sample Blinking Light Application
//
// LED is on port PB5, Arduino pin 13
//
// This sample uses 4 different ways of timing 1 second:
//
//  Wait loop:      AVR spins for 1 second between changes in LED
//  Timer event:    1ms TimerEventMgr is created and a TimerEvent counts 1000 cycles and then fires
//  Dedicated RTC:  Dedicated RTC is created which gives event every second
//  Shared RTC:     A RTC which shares the 1ms TimerEventMgr is created which gives event event
//                  every second (assuming TimerEventMgr is precisely 1ms)

// Runtime sizes in bytes (Release)
//
//      Code   Data     BSS
//      ----   ----     ---
//       390    12       8
//      1116    12      30
//       946    12      29
//      1240    18      39

//#define WAIT_LOOP
//#define TIMER_EVENT
//#define DEDICATED_RTC
#define SHARED_RTC

using namespace m8r;

#define LEDPort B
#define LEDBit 5

class MyApp;

class MyApp : public EventListener {    
public:
    MyApp();
    
    // EventListener override
    virtual void handleEvent(EventType type, EventParam);
    
    BlinkErrorReporter<Port<LEDPort>, LEDBit, false> m_errorReporter;
    Serial<USART0<19200> > _serial;

#if defined(WAIT_LOOP)
#elif defined(TIMER_EVENT)
    TimerEventMgr<Timer0, TimerClockDIV64> m_timerEventMgr;
    RepeatingTimerEvent<1000> _timerEvent;
#elif defined(DEDICATED_RTC)
    DedicatedRTC<Timer0, TimerClockDIV64, 249, 1000> m_clock; // 1s timer
#elif defined(SHARED_RTC)
    TimerEventMgr<Timer0, TimerClockDIV64> m_timerEventMgr;
    SharedRTC<> m_clock;
#endif
    OutputBit<LEDPort, LEDBit> m_LEDPort;
};

MyApp g_app;

MyApp::MyApp()
{
    sei();
#if defined(WAIT_LOOP)
#elif defined(TIMER_EVENT)
    System::startEventTimer(&_timerEvent);
#elif defined(DEDICATED_RTC)
#elif defined(SHARED_RTC)
#endif

    uint8_t n = 37;
    _serial << F("The number ") << n << F(" is the answer\n");
}

void
MyApp::handleEvent(EventType type, EventParam param)
{
    switch(type)
    {
        case EV_IDLE:
//            if (_serial.bytesAvailable()) {
//                _serial.write(_serial.read());
//            }
#if defined(WAIT_LOOP)
            System::msDelay<1000>();
            m_LEDPort = !m_LEDPort;
#endif
        break;
#if defined(TIMER_EVENT)
        case EV_EVENT_TIMER:
            m_LEDPort = !m_LEDPort;
            break;
#elif defined(DEDICATED_RTC) || defined(SHARED_RTC)
        case EV_RTC_SECONDS:
            m_LEDPort = !m_LEDPort;
            break;
#endif
        default:
            break;
    }
}
