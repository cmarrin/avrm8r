//
//  INA219.cpp
//  marrinator
//
//  Created by Chris Marrin on 3/15/14.
//
//

#include "INA219.h"

#include "I2C.h"

using namespace m8r;

I2CMaster INA219::_i2c;

void INA219::updateCalibration(VoltageRange range, uint16_t lsbMicroamps, uint16_t shuntMilliohms)
{
    _calibrationValue = static_cast<uint16_t>(static_cast<uint32_t>(4096) * shuntMilliohms / lsbMicroamps / 10);
    _configValue = (range == Range32V) ? INA219_CONFIG_BVOLTAGERANGE_32V : INA219_CONFIG_BVOLTAGERANGE_16V;
    _configValue |= INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT | 
                    INA219_CONFIG_SADCRES_12BIT_1S_532US |INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
                    
    _currentDivider_mA = 1000 / lsbMicroamps;
    
    // FIXME: What is the real value here?
    _powerDivider_mW = 1;
}
