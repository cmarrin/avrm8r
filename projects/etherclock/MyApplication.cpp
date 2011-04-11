#include <m8r.h>
#include <m8r/Application.h>
#include <m8r/MAX6969.h>

using namespace m8r;

MAX6969<PortC, 1, PortC, 0, PortC, 2, PortC, 3> myShiftReg;

void Application::initialize()
{
    setEventOnIdle(true);
}

void Application::processEvent(EventType type)
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
