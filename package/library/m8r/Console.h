//
//  Console.h
//  marrinator
//
//  Created by Chris Marrin on 3/4/14.
//
//

#include "m8r.h"

namespace m8r {

    template<class USART>
    class Console
    {
    public:
        Console& operator<<(char v) _INLINE_ { _usart.write(v); return *this; }
        Console& operator<<(uint8_t v) _INLINE_ { write(v, false); return *this; }
        Console& operator<<(int16_t v) _INLINE_ { write(v, true); return *this; }
        Console& operator<<(uint16_t v) _INLINE_ { write(v, false); return *this; }
        Console& operator<<(int32_t v) _INLINE_ { write(v, true); return *this; }
        Console& operator<<(uint32_t v) _INLINE_ { write(v, false); return *this; }
        Console& operator<<(const char* s) _INLINE_ { write(s); return *this; }
        Console& operator<<(const _FlashString s) _INLINE_ { write(s); return *this; }
        
        char get()
        {
            return _usart.read();
        }
        bool bytesAvailable() const { return _usart.bytesAvailable(); }
        
    private:
        void write(uint32_t v, bool isSigned)
        {
            if (isSigned && ((int32_t) v < 0)) {
                _usart.write('-');
                v = -v;
            }
            char buf[11];
            char* p = buf + 11;
            *--p = '\0';
            while (v) {
                *--p = (v % 10) + '0';
                v /= 10;
            }
            write(p);
        }
        
        void write(const char* s)
        {
            while (*s != '\0') {
                if (*s == '\n') {
                    _usart.write('\r');
                }
                _usart.write(*s++);
            }
        }
        
        void write(const _FlashString s)
        {
            const char* p = s._s;
            for ( ; ; ) {
                uint8_t c = pgm_read_byte(p);
                if (c == '\0') {
                    return;
                }
                if (c == '\n') {
                    _usart.write('\r');
                }
                _usart.write(c);
                ++p;
            }
        }
        
        USART _usart;
    };

}