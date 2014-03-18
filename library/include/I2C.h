//
//  I2C.h
//  marrinator
//
//  Created by Chris Marrin on 3/15/14.
//
//

#pragma once

#include "m8r.h"
#include "twi.h"
#include "DeviceStream.h"

namespace m8r {

class I2CMaster
{
public:
    static const int16_t I2CStop = 256;
    static inline DeviceControl address(uint8_t a) { return DeviceControl(static_cast<int16_t>(a)); }
    static inline DeviceControl stop() { return DeviceControl(I2CStop); }

	I2CMaster()
        : _address(-1)
    {
        twi_init();
    }

    void write(uint8_t b)
    {
        if (_address < 0) {
            return;
        }
        write(_address, &b, 1, false);
    }
    
    void write(uint8_t address, uint8_t* buf, uint8_t size, bool stop = true)
    {
        uint8_t result = twi_writeTo(address, buf, size, false, stop);
        ASSERT(result != 1, AssertI2CSendBufferTooBig);
        ASSERT(result != 2, AssertI2CSendNoAckOnAddress);
        ASSERT(result != 3, AssertI2CSendNoAckOnData);
        ASSERT(result == 0, AssertI2CSendError);
    }
     
    void control(int16_t ctl)
    {
        if (ctl >= 0 && ctl <=255) {
            _address = ctl;
        } else if (ctl == I2CStop) {
            twi_stop();
        }
    }

    int16_t read() { return -1; }
    
    uint8_t read(uint8_t address, uint8_t* buf, uint8_t size, bool stop = true)
    {
        if (!size) {
            return 0;
        }
        uint8_t result = twi_readFrom(address, buf, size, stop);
        ASSERT(result != 0, AssertI2CReceiveNoBytes);
        ASSERT(result == size, AssertI2CReceiveWrongSize);
        return result;
    }
    
    uint8_t bytesAvailable() const { return 0; }
    
    void flush() { }

private:
    int16_t _address;
};

}
