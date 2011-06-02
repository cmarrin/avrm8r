#include <m8r.h>
#include <m8r/Application.h>
#include <m8r/ADC.h>
#include <m8r/MAX6969.h>

using namespace m8r;

#define ErrorPort Port<B>
#define ErrorBit 0

#define NumBrightnessValuesToAccumulate 100

class MyApp {
public:
    MyApp()
    : m_adc(0, ADC_PS_DIV128, ADC_REF_AVCC)
    , m_accumulatedBrightness(0)
    , m_numAccumulatedBrightness(0)
    , m_currentBrightness(0)
    , m_brightnessCount(0)
    {
        // Testing
        m_shiftReg.setChar('7');
        m_shiftReg.latch();
        
        m_errorPort.setPortBit(ErrorBit);
        m_errorPort.setBitOutput(ErrorBit);
        Application::application().setEventOnIdle(true);
        
        m_adc.setEnabled(true);
        
        sei();
        m_adc.startConversion();
    }
    
    void setErrorCondition(ErrorType error, bool raise);
    void processEvent(EventType type);
    
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
    MAX6969<Port<C>, 1, Port<C>, 2, Port<C>, 4, Port<C>, 3> m_shiftReg;
    ErrorPort m_errorPort;
    
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
MyApp::setErrorCondition(ErrorType error, bool raise)
{
    switch(error) {
        case ERROR_EVENT_OVERRUN:
        case ERROR_USER:
            if (raise)
                m_errorPort.clearPortBit(ErrorBit);
            else
                m_errorPort.setPortBit(ErrorBit);
            break;
        default:
            break;
    }
}

void
MyApp::processEvent(EventType type)
{
    switch(type)
    {
        case EV_ADC: {
            accumulateBrightnessValue(m_adc.getLastConversion8Bit());
            m_adc.startConversion();
            break;
        }
        case EV_IDLE:
            if (++m_brightnessCount == 0) {
                m_shiftReg.setOutputEnable(true);
                m_brightnessMatch = brightness();
                setErrorCondition(ERROR_USER, true);
            }
            else if (m_brightnessCount == m_brightnessMatch) {
                m_shiftReg.setOutputEnable(false);
                setErrorCondition(ERROR_USER, false);
            }
            break;
        default:
            break;
    }
}

// Application overrides
void
Application::setErrorCondition(ErrorType error, bool raise)
{
    g_myApp.setErrorCondition(error, raise);
}

void
Application::processEvent(EventType type)
{
    g_myApp.processEvent(type);
}
