#include <m8r.h>
#include <m8r/Animator.h>
#include <m8r/Application.h>
#include <m8r/ADC.h>
#include <m8r/BlinkErrorReporter.h>
#include <m8r/ENC28J60.h>
#include <m8r/MAX6969.h>
#include <m8r/RTC.h>
#include <m8r/TimerEventMgr.h>

using namespace m8r;

#define ErrorPort Port<B>
#define ErrorBit 1

const uint8_t NumLightSensorValuesToAccumulate = 250;
const uint8_t NumNextBrightnessValuesToMatch = 10;
const uint16_t NumBrightnessIterations = 1000;
const uint16_t MinColonBrightness = 50 * NumBrightnessIterations / 256;

class MyApp : public Application, public EventListener, public IdleEventListener, public ErrorConditionHandler {
    
public:
    MyApp()
        : m_adc(this, 0, ADC_PS_DIV128, ADC_REF_AVCC)
        , m_timerEventMgr(TimerClockDIV1, 12499, 1000) // 1ms timer
        , m_timerEvent(this, 5000, TimerEventOneShot)
        , m_clock(this)
        , m_ethernet(ClockOutDiv2)
        , m_colonAnimator(this, 30)
        , m_accumulatedLightSensorValues(0)
        , m_numAccumulatedLightSensorValues(0)
        , m_numTimesNextBrightnessMatch(0)
        , m_currentBrightness(0)
        , m_nextBrightness(0)
        , m_brightnessCount(0)
        , m_brightnessMatch(0)
        , m_animationValue(0)
    {
        Application::setErrorConditionHandler(this);
        Application::setEventOnIdle(true);
        
        // Testing
        m_shiftReg.setChar('8', true);
        m_shiftReg.setChar('8', true);
        m_shiftReg.setChar('8', true);
        m_shiftReg.setChar('8', true);
        m_shiftReg.latch();
        
        m_colonPort.setBitOutput(0);
        m_colonPort.setBitOutput(1);
        m_colonPort.setPortBit(0);
        m_colonPort.setPortBit(1);
        
        //m_timerEvent.start();
        
        m_adc.setEnabled(true);
        
        sei();
        m_adc.startConversion();
        
        m_colonAnimator.start();
    }
    
    // EventListener override
    virtual void handleEvent(EventType type, uint8_t identifier);
    
    // IdleEventListener override
    virtual void handleIdleEvent();
    
    // ErrorConditionHandler override
    virtual void handleErrorCondition(ErrorType type, ErrorConditionType condition)
    {
        m_errorReporter.reportError(type, condition);
    }    
    
    uint16_t brightness() const { return m_currentBrightness; }

protected:
    void accumulateBrightnessValue(uint8_t value)
    {
        m_accumulatedLightSensorValues += value;
        if (++m_numAccumulatedLightSensorValues >= NumLightSensorValuesToAccumulate) {
            uint16_t newBrightness = ((uint16_t) (g_brightnessTable[m_accumulatedLightSensorValues / NumLightSensorValuesToAccumulate / 32] * NumBrightnessIterations)) >> 8;
            m_accumulatedLightSensorValues = 0;
            m_numAccumulatedLightSensorValues = 0;
            
            if (m_nextBrightness == newBrightness) {
                if (++m_numTimesNextBrightnessMatch >= NumNextBrightnessValuesToMatch) {
                    m_currentBrightness = m_nextBrightness;
                    m_numTimesNextBrightnessMatch = 0;
                }
            }
            else {
                m_nextBrightness = newBrightness;
                m_numTimesNextBrightnessMatch = 0;
            }
        }
    }
    
private:
    ADC m_adc;
    MAX6969<Port<C>, 1, Port<C>, 2, Port<C>, 3, Port<C>, 4> m_shiftReg;
    BlinkErrorReporter<Port<B>, 1> m_errorReporter;
    TimerEventMgr<Timer1> m_timerEventMgr;
    TimerEvent m_timerEvent;
    RTC m_clock;
    ENC28J60<_BV(MSTR), _BV(SPI2X)> m_ethernet;
    Animator m_colonAnimator;
    Port<D> m_colonPort;
    
    uint16_t m_accumulatedLightSensorValues;
    uint8_t m_numAccumulatedLightSensorValues;
    uint8_t m_numTimesNextBrightnessMatch;
    uint16_t m_currentBrightness, m_nextBrightness;
    uint16_t m_brightnessCount;
    uint16_t m_brightnessMatch;
    uint16_t m_colonBrightnessMatch;
    uint8_t m_animationValue;
    
    static uint8_t g_brightnessTable[8];
};

uint8_t MyApp::g_brightnessTable[] = { 30, 60, 90, 120, 150, 180, 210, 255 };

MyApp g_myApp;

void
MyApp::handleEvent(EventType type, uint8_t identifier)
{
    switch(type)
    {
        case EV_ADC:
            accumulateBrightnessValue(m_adc.lastConversion8Bit());
            m_adc.startConversion();
            break;
        case EV_ANIMATOR_EVENT:
            m_animationValue = Animator::sineValue(m_colonAnimator.currentValue());
            break;            
        case EV_TIMER_EVENT:
            NOTE(0x12);
            break;
        case EV_RTC_MINUTES_EVENT: {
            RTCTime t;
            m_clock.currentTime(t);
            
            // FIXME: This is bogus, just for testing
            m_shiftReg.setChar(t.seconds, false);
            m_shiftReg.latch();
            break;
        }
        default:
            break;
    }
}

void
MyApp::handleIdleEvent()
{
    if (m_brightnessCount == 0) {
        uint16_t b = brightness();
        m_brightnessMatch = (uint16_t) b * NumBrightnessIterations;
        b = (b < MinColonBrightness) ? MinColonBrightness : b;
        m_colonBrightnessMatch = ((uint32_t) b * (uint32_t) m_animationValue) >> 8;
        
        if (m_brightnessMatch)
            m_shiftReg.setOutputEnable(true);

        if (m_colonBrightnessMatch) {
            m_colonPort.clearPortBit(0);
            m_colonPort.clearPortBit(1);
        }
    }
    else {
        if (m_brightnessCount == m_brightnessMatch)
            m_shiftReg.setOutputEnable(false);

        if (m_brightnessCount == m_colonBrightnessMatch) {
            m_colonPort.setPortBit(0);
            m_colonPort.setPortBit(1);
        }
    }
    
    if (++m_brightnessCount >= NumBrightnessIterations)
        m_brightnessCount = 0;
}

