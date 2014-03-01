//
//  MyApplication.cpp
//
//  Created by Chris Marrin on 2/27/2014.
//

#include "m8r.h"
#include "Application.h"
#include "EventListener.h"
#include "RTC.h"
#include "Timer1.h"
#include "TimerEventMgr.h"

#include "BlinkErrorReporter.h"

//
//  Sample Blinking Light Application
//
// LED is on port PB5, Arduino pin 13
// 

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
//    TimerEventMgr<Timer1> m_timerEventMgr;
    RTC<Timer1> m_clock;
    OutputBit<LEDPort, LEDBit> m_LEDPort;
};

MyApp g_app;

MyApp::MyApp()
//    : m_timerEventMgr(TimerClockDIV1024, F_CPU / 1024) // ~1s timer
    : m_clock(TimerClockDIV1, 15999, 1000) // 1s timer

{
    sei();
}

void
MyApp::handleEvent(EventType type, EventParam param)
{
    switch(type)
    {
        case EV_IDLE:
        break;
        case EV_EVENT_TIMER:
            m_LEDPort = !m_LEDPort;
            break;
        case EV_RTC_SECONDS:
            m_LEDPort = !m_LEDPort;
            break;
        default:
            break;
    }
}
