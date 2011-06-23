#include <m8r.h>
#include <m8r/Application.h>
#include <m8r/ADC.h>
#include <m8r/BlinkErrorReporter.h>
#include <m8r/MAX6969.h>
#include <m8r/RTC.h>
#include <m8r/TimerEventMgr.h>

using namespace m8r;

#define ErrorPort Port<B>
#define ErrorBit 1

#define NumBrightnessValuesToAccumulate 100

class MyApp : public Application, public EventListener {
public:
    MyApp()
    : m_adc(0, ADC_PS_DIV128, ADC_REF_AVCC)
    , m_timerEventMgr(TimerClockDIV1, 6249, 1000) // 1ms timer
    , m_accumulatedBrightness(0)
    , m_numAccumulatedBrightness(0)
    , m_currentBrightness(0)
    , m_brightnessCount(0)
    {
        Application::application()->setEventOnIdle(true);
        
        // Testing
        m_shiftReg.setChar('8', true);
        m_shiftReg.setChar('8', true);
        m_shiftReg.setChar('8', true);
        m_shiftReg.setChar('8', true);
        m_shiftReg.latch();
        
        m_timerEventMgr.createTimerEvent(5000, TimerEventOneShot);
        
        m_adc.setEnabled(true);
        
        sei();
        m_adc.startConversion();
    }
    
    // Application overrides
    virtual void setErrorCondition(ErrorType, ErrorConditionType);
    
    // EventListener overrides
    virtual bool handleEvent(EventType type, uint8_t identifier);
    
    uint8_t brightness() const { return m_currentBrightness; }

protected:
    void accumulateBrightnessValue(uint8_t value)
    {
        m_accumulatedBrightness += value;
        if (++m_numAccumulatedBrightness >= NumBrightnessValuesToAccumulate) {
            m_currentBrightness = g_brightnessTable[m_accumulatedBrightness / NumBrightnessValuesToAccumulate / 32];
            m_accumulatedBrightness = 0;
            m_numAccumulatedBrightness = 0;
        }
    }
    
private:
    ADC m_adc;
    MAX6969<Port<C>, 1, Port<C>, 2, Port<C>, 3, Port<C>, 4> m_shiftReg;
    BlinkErrorReporter<Port<B>, 1> m_errorReporter;
    TimerEventMgr<Timer1> m_timerEventMgr;
    RTC m_clock;
    
    uint16_t m_accumulatedBrightness;
    uint8_t m_numAccumulatedBrightness;
    uint8_t m_currentBrightness;
    
    uint8_t m_brightnessCount;
    uint8_t m_brightnessMatch;
    
    static uint8_t g_brightnessTable[8];
};

uint8_t MyApp::g_brightnessTable[] = { 30, 60, 90, 120, 150, 180, 210, 255 };

MyApp g_myApp;

void
MyApp::setErrorCondition(ErrorType error, ErrorConditionType condition)
{
    m_errorReporter.reportError(error, condition);
}

bool
MyApp::handleEvent(EventType type, uint8_t identifier)
{
    switch(type)
    {
        case EV_ADC:
            accumulateBrightnessValue(m_adc.getLastConversion8Bit());
            m_adc.startConversion();
            break;
        case EV_TIMER_EVENT:
            NOTE(0x12);
            break;
        case EV_IDLE:
            if (++m_brightnessCount == 0) {
                m_shiftReg.setOutputEnable(true);
                m_brightnessMatch = brightness();
            }
            else if (m_brightnessCount == m_brightnessMatch)
                m_shiftReg.setOutputEnable(false);
            break;
        default:
            break;
    }
    
    return false;
}
