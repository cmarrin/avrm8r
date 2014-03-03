//
//  MyApplication.cpp
//
//  Created by Chris Marrin on 2/27/2014.
//

#include "m8r.h"
#include "System.h"
#include "EventListener.h"
#include "RTC.h"
#include "Timer0.h"
#include "TimerEventMgr.h"

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

#define WAIT_LOOP
//#define TIMER_EVENT
//#define DEDICATED_RTC
//#define SHARED_RTC

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
#if defined(WAIT_LOOP)
#elif defined(TIMER_EVENT)
    TimerEventMgr<Timer0, TimerClockDIV64> m_timerEventMgr;
    RepeatingTimerEvent _timerEvent;
#elif defined(DEDICATED_RTC)
    DedicatedRTC<Timer0> m_clock;
#elif defined(SHARED_RTC)
    TimerEventMgr<Timer0, TimerClockDIV64> m_timerEventMgr;
    SharedRTC m_clock;
#endif
    OutputBit<LEDPort, LEDBit> m_LEDPort;
};

MyApp g_app;

MyApp::MyApp()
#if defined(WAIT_LOOP)
#elif defined(TIMER_EVENT)
    : _timerEvent(1000)
#elif defined(DEDICATED_RTC)
    : m_clock(TimerClockDIV64, 249, 1000) // 1s timer
#elif defined(SHARED_RTC)
#endif

{
    sei();
#if defined(WAIT_LOOP)
#elif defined(TIMER_EVENT)
    System::startEventTimer(&_timerEvent);
#elif defined(DEDICATED_RTC)
#elif defined(SHARED_RTC)
#endif
}

void
MyApp::handleEvent(EventType type, EventParam param)
{
    switch(type)
    {
#if defined(WAIT_LOOP)
        case EV_IDLE:
            System::msDelay<1000>();
            m_LEDPort = !m_LEDPort;
        break;
#elif defined(TIMER_EVENT)
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
