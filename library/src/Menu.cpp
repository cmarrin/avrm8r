//
//  Menu.cpp
//  marrinator
//
//  Created by Chris Marrin on 4/2/14.
//
//

#include "Menu.h"

using namespace m8r;

void MenuBase::handleEvent(EventType type, EventParam param)
{
    _buttons->handleEvent(type, param);
    switch(type) {
        case EV_IDLE:
            switch(_execState) {
                case ExecState::Exec:
                    execute();
                    break;
                case ExecState::Pause:
                    break;
                case ExecState::ButtonWait: {
                    uint8_t button = _buttons->readButton();
                    if (button == NoButton) {
                        break;
                    }
                    if (!_buttons->state(button)) {
                        break;
                    }
                    _execState = ExecState::Exec;
                    handleButton(button);
                    break;
                }
            }
            break;
        case EV_EVENT_TIMER:
            if (param == &_timerEvent) {
                _execState = ExecState::Exec;
            }
        default: break;
    }
}

void MenuBase::execute()
{
    OpType type = opType(_menuOps[_curOp]);
    uint32_t param = menuOpTypeData(_menuOps[_curOp]);
    switch(type) {
        case OpType::Show:
            show(_FlashString((const char*) param));
            _curOp++;
            break;
        case OpType::Pause:
            _curOp++;
            _timerEvent.setIntervalCount((uint16_t) param);
            System::startEventTimer(&_timerEvent);
            _execState = ExecState::Pause;
            break;
        case OpType::State:
            _curOp++;
            break;
        case OpType::Goto:
            gotoState((uint16_t) param);
            break;
        case OpType::Buttons:
            _buttons->readButton(); // Toss any existing button event
            _curOp++;
            _execState = ExecState::ButtonWait;
            break;
        case OpType::XEQ:
            xeq(param);
            _curOp++;
            break;
        default: ASSERT(0, AssertMenuHitEnd);
    }
}

void MenuBase::gotoState(uint16_t state)
{
    for (uint8_t i = 0; ; ++i) {
        OpType type = opType(_menuOps[i]);
        if (type == OpType::End) {
            break;
        }
        if (type == OpType::State) {
            if (menuOpTypeData(_menuOps[i]) == state) {
                _curOp = i;
                return;
            }
        }
    }
}

void MenuBase::handleButton(uint8_t button)
{
    MenuOpType buttonOp = _menuOps[_curOp + button];
    if (opType(buttonOp) == OpType::Param) {
        gotoState(menuOpTypeData(buttonOp));
        _execState = ExecState::Exec;
    }
}

