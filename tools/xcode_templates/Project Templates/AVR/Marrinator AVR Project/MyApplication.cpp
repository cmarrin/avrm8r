/*
    Example application source. This example creates a MAX6969 device connected to
    PortC, pins 1 (clk), 0 (data), 2 (latch) and 3 (enable). Every idle event it
    writes the number '9' to the MAX6969 and then enables it. 
    
    An error LED is also assigned to PortB, pin 0. An LED on this port will turn
    on whenever an error condition is present.
*/

#include <m8r.h>
#include <m8r/Application.h>
#include <m8r/MAX6969.h>

using namespace m8r;

MAX6969<PortC, 1, PortC, 0, PortC, 2, PortC, 3> myShiftReg;
PortB myErrorPort;
#define MyErrorBit 0

void
Application::initialize()
{
    myShiftReg.setOutputEnable(false);

    myErrorPort.setPortMask(_BV(MyErrorBit));
    myErrorPort.setDDRMask(_BV(MyErrorBit));
    setEventOnIdle(true);
}

void
Application::setErrorCondition(ErrorType error, bool raise)
{
    switch(error) {
        case ERROR_EVENT_OVERRUN:
            if (raise)
                myErrorPort.clearPortMask(_BV(MyErrorBit));
            else
                myErrorPort.setPortMask(_BV(MyErrorBit));
            break;
        default:
            break;
    }
}

void
Application::processEvent(EventType type)
{
    switch(type)
    {
        case EV_IDLE:
            myShiftReg.setChar('9');
            myShiftReg.setOutputEnable(true);
            break;
        default:
            break;
    }
}
