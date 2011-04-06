#include <m8r.h>
#include <m8r/Application.h>
#include <m8r/MAX6969.h>

using namespace m8r;

MAX6969<PortC, 3, PortC, 2, PortC, 1, PortC, 4> myShiftReg;

void Application::initialize()
{
    myShiftReg.setChar('8');
    myShiftReg.setOutputEnable(true);
}

void Application::processEvent(EventType type)
{
    switch(type)
    {
        /*
        case EV_TIMER0:
            if (myIsDisplayOn) {
                myShiftReg.setOutputEnable(false);
                myBrightnessTimer.setValue(myBrightness);
                myIsDisplayOn = false;
                
                // start another ADC conversion
                myADC.startConversion();
            }
            else {
                myShiftReg.setOutputEnable(true);
                myBrightnessTimer.setValue(255-myBrightness);
                myIsDisplayOn = true;
            }
            break;
        */
        /*
        case EV_TIMER1:
        {
            myRTC.onOutputCompare();
            setTime();
            myShiftReg.setOutputEnable(true);
            break;
        }
        */
        default:
            break;
    }
}
