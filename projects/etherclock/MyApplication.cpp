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
            //myShiftReg.setOutputEnable(false);
            myShiftReg.setChar('7');
            myShiftReg.setOutputEnable(true);
            break;
        default:
            break;
    }
}
