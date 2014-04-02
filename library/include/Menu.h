//
//  Menu.h
//  marrinator
//
//  Created by Chris Marrin on 4/2/14.
//
//

#pragma once

#include "m8r.h"

#include "Button.h"

namespace m8r {

class Menu
{
public:
    Menu(ButtonSetBase* buttons) : _buttons(buttons) { }

    void handleEvent(EventType, EventParam);
    
    bool buttonState(uint8_t b) const { return _buttons->state(b); }
    
private:
    ButtonSetBase* _buttons;
};

}
