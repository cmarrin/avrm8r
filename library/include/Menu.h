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

typedef uint32_t MenuOpType;

class MenuBase
{
public:
    enum class ExecState { Exec, Pause, ButtonWait };
    enum class OpType : uint32_t { Param = 0, 
                                   Pause = 1L << 24,
                                   State = 2L << 24,
                                   Goto = 3L << 24,
                                   Buttons = 4L << 24,
                                   Show = 5L << 24,
                                   XEQ = 6L << 24,
                                   End = 7L << 24,
                                   Unknown = 8L << 24
                                };

    MenuBase(ButtonSetBase* buttons, const MenuOpType* menu) : _buttons(buttons), _menuOps(menu) { }

    void handleEvent(EventType, EventParam);
    bool buttonState(uint8_t b) const { return _buttons->state(b); }
    void handleButton(uint8_t button);
    void execute();
    void gotoState(uint16_t state);

    virtual void show(const _FlashString&) = 0;
    virtual void xeq(uint32_t p) = 0;
    
    static constexpr MenuOpType menuOpType(OpType type) { return static_cast<MenuOpType>(type); }
    static constexpr OpType opType(MenuOpType type) { return static_cast<OpType>(type & 0xff000000); }
    static constexpr uint32_t menuOpTypeData(MenuOpType type) { return static_cast<uint32_t>(type & 0x00ffffff); }
    
    static MenuOpType Pause(uint16_t p) { return menuOpType(OpType::Pause) | p; }
    static MenuOpType State(uint16_t p) { return menuOpType(OpType::State) | p; }
    static MenuOpType Goto(uint16_t p) { return menuOpType(OpType::Goto) | p; }
    static MenuOpType Buttons() { return menuOpType(OpType::Buttons); }
    static MenuOpType End() { return menuOpType(OpType::End); }
    static MenuOpType Show(const _FlashString& p) { return menuOpType(OpType::Show) | (uint32_t) p._s; }

//    class MenuOp
//    {
//    public:
//        constexpr MenuOp(OpType type, uint32_t p) : _type(type), _param(p) { }
//        
//        void execute(MenuBase*) const;
//        
//        OpType opType() const { return _type; }
//    
//    private:
//        OpType _type;
//        
//    protected:
//        uint32_t _param;
//    };
//
//    class Pause : public MenuOp
//    {
//    public:
//        constexpr Pause(uint16_t p) : MenuOp(OpType::Pause, (uint32_t) p) { }
//
//        void execute(MenuBase* menu) const
//        {
//            menu->_paused = true;
//            menu->_curOp++;
//            menu->_timerEvent.setIntervalCount((uint16_t) _param);
//            System::startEventTimer(&menu->_timerEvent);
//            menu->_execState = ExecState::Pause;
//        }
//    };
//    
//    class State : public MenuOp
//    {
//    public:
//        constexpr State(uint8_t p) : MenuOp(OpType::State, (uint32_t) p) { }
//
//        void execute(MenuBase* menu) const { menu->_curOp++; }
//        
//        uint8_t state() const { return (uint8_t) _param; }
//    };
//    
//    class Goto : public MenuOp
//    {
//    public:
//        constexpr Goto(uint8_t p) : MenuOp(OpType::Goto, (uint32_t) p) { }
//
//        void execute(MenuBase* menu) const { menu->gotoState((uint8_t) _param); }
//    };
//    
//    class ButtonsBase : public MenuOp
//    {
//    public:
//        constexpr ButtonsBase() : MenuOp(OpType::Buttons, 0) { }
//        
//        void handleButton(MenuBase*, uint8_t button) const;
//
//        void execute(MenuBase* menu) const
//        {
//            menu->_buttons->readButton(); // Toss any existing button event
//            menu->_buttonOp = this;
//            menu->_execState = ExecState::ButtonWait;
//        }
//        
//        uint8_t* buttons() const { return (uint8_t*) _param; }
//    };
//    
//    template<uint8_t... args>
//    class Buttons : public ButtonsBase
//    {
//    public:
//        Buttons()
//        {
//            uint8_t* buttons = static_cast<uint8_t*>(malloc(sizeof...(args) + 1));
//            _param = (uint32_t) buttons;
//            *buttons++ = sizeof...(args);
//            for (uint8_t i = 0; i < sizeof...(args); ++i) {
//                *buttons++ = _buttons[i];
//            }
//        }
//        ~Buttons()
//        {
//            free((uint8_t*) _param);
//        }
//
//    private:
//        static constexpr uint8_t _buttons[sizeof...(args)] = { args... };
//    };
//
//    class Show : public MenuOp
//    {
//    public:
//        constexpr Show(const char* s) : MenuOp(OpType::Show, (uint32_t) s) { }
//        
//        void execute(MenuBase* menu) const
//        {
//            menu->show((const char*) _param);
//            menu->_curOp++;
//        }
//    };
//    
//    class XEQBase : public MenuOp
//    {
//    public:
//        constexpr XEQBase(uint32_t p) : MenuOp(OpType::XEQ, p) { }
//        
//        void execute(MenuBase* menu) const { menu->xeq(_param); }
//    };
//
//    class End : public MenuOp
//    {
//    public:
//        constexpr End() : MenuOp(OpType::End, 0){ }
//    };
//

private:
    ButtonSetBase* _buttons;
    const MenuOpType* _menuOps;
    int16_t _curOp = 0;
    ExecState _execState = ExecState::Exec;
    OneShotTimerEvent _timerEvent;
};

template<class Callee>
class Menu : public MenuBase
{
public:
    typedef void (*MenuFunc)(Callee*);
    
    Menu(ButtonSetBase* buttons, const MenuOpType* menu, Callee* callee) : MenuBase(buttons, menu), _callee(callee) { }

    static MenuOpType XEQ(MenuFunc p) { return menuOpType(OpType::XEQ) | (uint32_t) p; }

    virtual void xeq(uint32_t p) override
    {
        ((MenuFunc) p)(_callee);
    }

//    class XEQ : public XEQBase
//    {
//    public:
//        constexpr XEQ(MenuFunc p) : XEQBase((uint32_t) p) { }
//    };
    
private:
    Callee* _callee;
};

}
