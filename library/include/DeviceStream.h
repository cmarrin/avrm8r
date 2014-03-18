//
//  DeviceStream.h
//  AVRPowerSupply
//
//  Created by Chris Marrin on 3/9/14.
//
//

#pragma once

#include "m8r.h"

namespace m8r {
    struct DeviceControl
    {
        DeviceControl(int16_t t) : type(t) { }
        int16_t type;
    };

    // Device must implement:
    //
    // void write(uint8_t b);
    // void flush();
    // void control(int16_t ctl);
    // int16_t read(); // Must return -1 if no chars
    // uint8_t bytesAvailable() const;
    //
    template<class Device>
    class DeviceStream
    {
    public:
        Device& device() { return _device; }
        
        DeviceStream& operator<<(char v) _INLINE_ { _device.write(v); return *this; }
        DeviceStream& operator<<(uint8_t v) _INLINE_ { write(v, false); return *this; }
        DeviceStream& operator<<(int16_t v) _INLINE_ { write(v, true); return *this; }
        DeviceStream& operator<<(uint16_t v) _INLINE_ { write(v, false); return *this; }
        DeviceStream& operator<<(int32_t v) _INLINE_ { write(v, true); return *this; }
        DeviceStream& operator<<(uint32_t v) _INLINE_ { write(v, false); return *this; }
        DeviceStream& operator<<(const char* s) _INLINE_ { write(s); return *this; }
        DeviceStream& operator<<(const _FlashString s) _INLINE_ { write(s); return *this; }
        DeviceStream& operator<<(const DeviceControl& ctl) _INLINE_ { _device.control(ctl.type); return *this; }
        
        ~DeviceStream() { flush(); }

        void flush() { _device.flush(); }
        char get() { return _device.read(); }
        uint8_t bytesAvailable() const { return _device.bytesAvailable(); }
    
    private:
        void write(uint32_t v, bool isSigned)
        {
            if (!v) {
                _device.write('0');
                return;
            }
            if (isSigned && ((int32_t) v < 0)) {
                _device.write('-');
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
                    _device.write('\r');
                }
                _device.write(*s++);
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
                    _device.write('\r');
                }
                _device.write(c);
                ++p;
            }
        }
        
        Device _device;
    };

}
