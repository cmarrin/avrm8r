//
//  TextLCD.h
//  marrinator
//
//  Created by Chris Marrin on 3/8/14.
//
//

#pragma once

#include "m8r.h"
#include "System.h"

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00
#define LCD_DEFAULT 0x00    // Use this to for default DOTS

namespace m8r {

enum TextLCDControlType { TextLCDControlHome };
static inline DeviceControl TextLCDHome() { return DeviceControl(static_cast<int16_t>(TextLCDControlHome)); }

// Hitachi HD44780 based LCD driver
class TextLCDBase
{
public:
    TextLCDBase()
        : _displayFunction(0)
        , _displayControl(0)
        , _displayMode(0)
        , _initialized(false)
    { }

protected:
    uint8_t _displayFunction;
    uint8_t _displayControl;
    uint8_t _displayMode;

    bool _initialized;

    uint8_t _numlines, _currline;
};

template <uint8_t cols, uint8_t lines, uint8_t dotsize, class RSPin, class RWPin, class EnablePin,
            class D0Pin, class D1Pin, class D2Pin, class D3Pin, 
            class D4Pin, class D5Pin, class D6Pin, class D7Pin>
class TextLCDCommon : TextLCDBase
{
public:
    TextLCDCommon()
    {
        if (_d4Pin.isNull())
            _displayFunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
        else 
            _displayFunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

        if (lines > 1) {
            _displayFunction |= LCD_2LINE;
        }
        _numlines = lines;
        _currline = 0;

        // for some 1 line displays you can select a 10 pixel high font
        if ((dotsize != 0) && (lines == 1)) {
            _displayFunction |= LCD_5x10DOTS;
        }

        // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
        // according to datasheet, we need at least 40ms after power rises above 2.7V
        // before sending commands.
        System::msDelay<50>();
        
        // Now we pull both RS and R/W low to begin commands
        _rsPin = false;
        _enablePin = false;
        if(!_rwPin) { 
            _rwPin = false;
        }
      
        //put the LCD into 4 bit or 8 bit mode
        if (!(_displayFunction & LCD_8BITMODE)) {
            // this is according to the hitachi HD44780 datasheet
            // figure 24, pg 46

            // we start in 8bit mode, try to set 4 bit mode
            write4bits(0x03);
            System::usDelay<4500>(); // wait min 4.1ms

            // second try
            write4bits(0x03);
            System::usDelay<4500>(); // wait min 4.1ms

            // third go!
            write4bits(0x03); 
            System::usDelay<150>();

            // finally, set to 4-bit interface
            write4bits(0x02); 
        } else {
            // this is according to the hitachi HD44780 datasheet
            // page 45 figure 23

            // Send function set command sequence
            command(LCD_FUNCTIONSET | _displayFunction);
            System::usDelay<4500>(); // wait min 4.1ms

            // second try
            command(LCD_FUNCTIONSET | _displayFunction);
            System::usDelay<150>();

            // third go
            command(LCD_FUNCTIONSET | _displayFunction);
        }

        // finally, set # lines, font size, etc.
        command(LCD_FUNCTIONSET | _displayFunction);  

        // turn the display on with no cursor or blinking default
        _displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
        setDisplay(true);

        // clear it off
        clear();

        // Initialize to default text direction (for romance languages)
        _displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

        // set the entry mode
        command(LCD_ENTRYMODESET | _displayMode);
    }

    void control(const DeviceControl& ctl)
    {
        switch(ctl.type) {
            case TextLCDControlHome: home(); break;
        }
    }

    void write(uint8_t value) { if (value == '\n') setCursorPosition(0, 1); else send(value, true); }
    void flush() { }
    int16_t read() { return -1; }
    uint8_t bytesAvailable() const { return 0; }

    void clear()
    {
        command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
        System::usDelay<2000>();  // this command takes a long time!
    }

    void home()
    {
        command(LCD_RETURNHOME);  // set cursor position to zero
        System::usDelay<2000>();  // this command takes a long time!
    }

    void setCursorPosition(uint8_t col, uint8_t row)
    {
        int rowOffsets[] = { 0x00, 0x40, 0x14, 0x54 };
        if ( row >= _numlines ) {
            row = _numlines-1;    // we count rows starting w/0
        }

        command(LCD_SETDDRAMADDR | (col + rowOffsets[row]));
    }
    
    void setDisplay(bool b)
    {
        if (b) {
            _displayControl |= LCD_DISPLAYON;
        } else {
            _displayControl &= ~LCD_DISPLAYON;
        }
        command(LCD_DISPLAYCONTROL | _displayControl);
    }
    
    void setCursor(bool b)
    {
        if (b) {
            _displayControl |= LCD_CURSORON;
        } else {
            _displayControl &= ~LCD_CURSORON;
        }
        command(LCD_DISPLAYCONTROL | _displayControl);
    }

    void setBlink(bool b)
    {
        if (b) {
            _displayControl |= LCD_BLINKON;
        } else {
            _displayControl &= ~LCD_BLINKON;
        }
        command(LCD_DISPLAYCONTROL | _displayControl);
    }
    
    enum ScrollDirection { ScrollLeft, ScrollRight };
    void scroll(ScrollDirection dir)
    {
        command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | ((dir == ScrollLeft) ? LCD_MOVELEFT : LCD_MOVERIGHT));
    }

    void setScrollDirection(ScrollDirection dir)
    {
        if (dir == ScrollLeft) {
            _displayMode |= LCD_ENTRYLEFT;
        } else {
            _displayMode &= ~LCD_ENTRYLEFT;
        }
        command(LCD_ENTRYMODESET | _displayMode);
    }

    void setAutoscroll(bool b)
    {
        if (b) {
            _displayMode |= LCD_ENTRYSHIFTINCREMENT;
        } else {
            _displayMode &= ~LCD_ENTRYSHIFTINCREMENT;
        }
        command(LCD_ENTRYMODESET | _displayMode);
    }

    // Allows us to fill the first 8 CGRAM locations
    // with custom characters
    void createChar(uint8_t location, uint8_t charmap[])
    {
        location &= 0x7; // we only have 8 locations 0-7
        command(LCD_SETCGRAMADDR | (location << 3));
        for (int i=0; i<8; i++) {
            write(charmap[i]);
        }
    }

    private:
    inline void command(uint8_t value) { send(value, false); }


    // write either command or data, with automatic 4/8-bit selection
    void send(uint8_t value, bool mode)
    {
        _rsPin = mode;

        // if there is a RW pin indicated, set it low to Write
        _rwPin = false;
  
        if (_displayFunction & LCD_8BITMODE) {
            write8bits(value); 
        } else {
            write4bits(value >> 4);
            write4bits(value);
        }
    }

    void pulseEnable(void)
    {
        _enablePin = false;
        System::usDelay<1>();    
        _enablePin = true;
        System::usDelay<1>();    
        _enablePin = false;
        System::usDelay<100>();   // commands need > 37us to settle
    }

    void write4bits(uint8_t value)
    {
        _d0Pin = (value & 0x01) != 0;
        _d1Pin = (value & 0x02) != 0;
        _d2Pin = (value & 0x04) != 0;
        _d3Pin = (value & 0x08) != 0;
        pulseEnable();
    }

    void write8bits(uint8_t value)
    {
        _d0Pin = (value & 0x01) != 0;
        _d1Pin = (value & 0x02) != 0;
        _d2Pin = (value & 0x04) != 0;
        _d3Pin = (value & 0x08) != 0;
        _d4Pin = (value & 0x10) != 0;
        _d5Pin = (value & 0x20) != 0;
        _d6Pin = (value & 0x40) != 0;
        _d7Pin = (value & 0x80) != 0;
        pulseEnable();
    }

private:
    RSPin _rsPin;
    RWPin _rwPin;
    EnablePin _enablePin;
    D0Pin _d0Pin;
    D1Pin _d1Pin;
    D2Pin _d2Pin;
    D3Pin _d3Pin;
    D4Pin _d4Pin;
    D5Pin _d5Pin;
    D6Pin _d6Pin;
    D7Pin _d7Pin;
};

template <uint8_t cols, uint8_t lines, uint8_t dotsize, class RSPin, class RWPin, class EnablePin,
          class D0Pin, class D1Pin, class D2Pin, class D3Pin, 
          class D4Pin = NullOutputBit, class D5Pin = NullOutputBit, class D6Pin = NullOutputBit, class D7Pin = NullOutputBit>
class TextLCD : public TextLCDCommon<cols, lines, dotsize, RSPin, RWPin, EnablePin, 
                                     D0Pin, D1Pin, D2Pin, D3Pin, D4Pin, D5Pin, D6Pin, D7Pin>
{
};

}

