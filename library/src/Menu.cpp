//
//  Menu.cpp
//  marrinator
//
//  Created by Chris Marrin on 4/2/14.
//
//

#include "Menu.h"

using namespace m8r;

void Menu::handleEvent(EventType type, EventParam param)
{
    _buttons->handleEvent(type, param);
    switch(type) {
        case EV_IDLE:
            break;
        default: break;
    }
}
