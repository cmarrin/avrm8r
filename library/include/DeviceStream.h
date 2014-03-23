//
//  DeviceStream.h
//  AVRPowerSupply
//
//  Created by Chris Marrin on 3/9/14.
//
//

#pragma once

#include "m8r.h"
#include "FixedPoint.h"

namespace m8r {
    struct DeviceControl
    {
        DeviceControl(uint8_t t = 0, uint8_t p = 0) : type(t), param(p) { }
        uint8_t type;
        uint8_t param;
    };

    // Device must implement:
    //
    // void write(uint8_t b);
    // void flush();
    // void control(const DeviceControl& ctl);
    // int16_t read(); // Must return -1 if no chars
    // uint8_t bytesAvailable() const;
    //
    template<class Device, bool sendCR = false>
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
        DeviceStream& operator<<(const DeviceControl& ctl) _INLINE_ { _device.control(ctl); return *this; }
        
        ~DeviceStream() { flush(); }

        void flush() { _device.flush(); }
        char get() { return _device.read(); }
        uint8_t bytesAvailable() const { return _device.bytesAvailable(); }
    
    private:
        void write(uint32_t v, bool isSigned)
        {
            char buf[12];
            buf[11] = '\0';
            write(FixedPoint::toString(v, isSigned, buf, 11));
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
                if (sendCR && c == '\n') {
                    _device.write('\r');
                }
                _device.write(c);
                ++p;
            }
        }
        
        Device _device;
    };

}
