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

// FIXME: Implement Shared RTC

#define WaitLoop
//#define TimerEvent
//#define DedicatedRTC
//#define SharedRTC

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
#if defined(WaitLoop)
#elif defined(TimerEvent)
    TimerEventMgr<Timer0, TimerClockDIV64> m_timerEventMgr;
    RepeatingTimerEvent _timerEvent;
#elif defined(DedicatedRTC)
    RTC<Timer0> m_clock;
#elif defined(SharedRTC)
#endif
    OutputBit<LEDPort, LEDBit> m_LEDPort;
};

MyApp g_app;

MyApp::MyApp()
#if defined(WaitLoop)
#elif defined(TimerEvent)
    : _timerEvent(1000)
#elif defined(DedicatedRTC)
    : m_clock(TimerClockDIV64, 249, 1000) // 1s timer
#elif defined(SharedRTC)
#endif

{
    sei();
#if defined(WaitLoop)
#elif defined(TimerEvent)
    System::startEventTimer(&_timerEvent);
#elif defined(DedicatedRTC)
#elif defined(SharedRTC)
#endif
}

void
MyApp::handleEvent(EventType type, EventParam param)
{
    switch(type)
    {
#if defined(WaitLoop)
        case EV_IDLE:
            System::msDelay<1000>();
            m_LEDPort = !m_LEDPort;
        break;
#elif defined(TimerEvent)
        case EV_EVENT_TIMER:
            m_LEDPort = !m_LEDPort;
            break;
#elif defined(DedicatedRTC)
        case EV_RTC_SECONDS:
            m_LEDPort = !m_LEDPort;
            break;
#elif defined(SharedRTC)
#endif
        default:
            break;
    }
}
