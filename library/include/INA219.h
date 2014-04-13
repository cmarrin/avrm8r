//
//  INA219.h
//  marrinator
//
//  Created by Chris Marrin on 3/15/14.
//
//

#pragma once

#include "m8r.h"
#include "I2C.h"

// Adapted from:

//====================
/**************************************************************************/
/*! 
    @file     Adafruit_INA219.h
    @author   K. Townsend (Adafruit Industries)
	@license  BSD (see license.txt)
	
	This is a library for the Adafruit INA219 breakout board
	----> https://www.adafruit.com/products/???
	
	Adafruit invests time and resources providing this open source code, 
	please support Adafruit and open-source hardware by purchasing 
	products from Adafruit!

	@section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/
//====================

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define INA219_ADDRESS                         (0x40)    // 1000000 (A0+A1=GND)
    #define INA219_READ                            (0x01)
/*=========================================================================*/

/*=========================================================================
    CONFIG REGISTER (R/W)
    -----------------------------------------------------------------------*/
    #define INA219_REG_CONFIG                      (0x00)
    /*---------------------------------------------------------------------*/
    #define INA219_CONFIG_RESET                    (0x8000)  // Reset Bit
	
    #define INA219_CONFIG_BVOLTAGERANGE_MASK       (0x2000)  // Bus Voltage Range Mask
    #define INA219_CONFIG_BVOLTAGERANGE_16V        (0x0000)  // 0-16V Range
    #define INA219_CONFIG_BVOLTAGERANGE_32V        (0x2000)  // 0-32V Range
	
    #define INA219_CONFIG_GAIN_MASK                (0x1800)  // Gain Mask
    #define INA219_CONFIG_GAIN_1_40MV              (0x0000)  // Gain 1, 40mV Range
    #define INA219_CONFIG_GAIN_2_80MV              (0x0800)  // Gain 2, 80mV Range
    #define INA219_CONFIG_GAIN_4_160MV             (0x1000)  // Gain 4, 160mV Range
    #define INA219_CONFIG_GAIN_8_320MV             (0x1800)  // Gain 8, 320mV Range
	
    #define INA219_CONFIG_BADCRES_MASK             (0x0780)  // Bus ADC Resolution Mask
    #define INA219_CONFIG_BADCRES_9BIT             (0x0080)  // 9-bit bus res = 0..511
    #define INA219_CONFIG_BADCRES_10BIT            (0x0100)  // 10-bit bus res = 0..1023
    #define INA219_CONFIG_BADCRES_11BIT            (0x0200)  // 11-bit bus res = 0..2047
    #define INA219_CONFIG_BADCRES_12BIT            (0x0400)  // 12-bit bus res = 0..4097
    
    #define INA219_CONFIG_SADCRES_MASK             (0x0078)  // Shunt ADC Resolution and Averaging Mask
    #define INA219_CONFIG_SADCRES_9BIT_1S_84US     (0x0000)  // 1 x 9-bit shunt sample
    #define INA219_CONFIG_SADCRES_10BIT_1S_148US   (0x0008)  // 1 x 10-bit shunt sample
    #define INA219_CONFIG_SADCRES_11BIT_1S_276US   (0x0010)  // 1 x 11-bit shunt sample
    #define INA219_CONFIG_SADCRES_12BIT_1S_532US   (0x0018)  // 1 x 12-bit shunt sample
    #define INA219_CONFIG_SADCRES_12BIT_2S_1060US  (0x0048)	 // 2 x 12-bit shunt samples averaged together
    #define INA219_CONFIG_SADCRES_12BIT_4S_2130US  (0x0050)  // 4 x 12-bit shunt samples averaged together
    #define INA219_CONFIG_SADCRES_12BIT_8S_4260US  (0x0058)  // 8 x 12-bit shunt samples averaged together
    #define INA219_CONFIG_SADCRES_12BIT_16S_8510US (0x0060)  // 16 x 12-bit shunt samples averaged together
    #define INA219_CONFIG_SADCRES_12BIT_32S_17MS   (0x0068)  // 32 x 12-bit shunt samples averaged together
    #define INA219_CONFIG_SADCRES_12BIT_64S_34MS   (0x0070)  // 64 x 12-bit shunt samples averaged together
    #define INA219_CONFIG_SADCRES_12BIT_128S_69MS  (0x0078)  // 128 x 12-bit shunt samples averaged together
	
    #define INA219_CONFIG_MODE_MASK                (0x0007)  // Operating Mode Mask
    #define INA219_CONFIG_MODE_POWERDOWN           (0x0000)
    #define INA219_CONFIG_MODE_SVOLT_TRIGGERED     (0x0001)
    #define INA219_CONFIG_MODE_BVOLT_TRIGGERED     (0x0002)
    #define INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED (0x0003)
    #define INA219_CONFIG_MODE_ADCOFF              (0x0004)
    #define INA219_CONFIG_MODE_SVOLT_CONTINUOUS    (0x0005)
    #define INA219_CONFIG_MODE_BVOLT_CONTINUOUS    (0x0006)
    #define INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS (0x0007)	
/*=========================================================================*/

/*=========================================================================
    SHUNT VOLTAGE REGISTER (R)
    -----------------------------------------------------------------------*/
    #define INA219_REG_SHUNTVOLTAGE                (0x01)
/*=========================================================================*/

/*=========================================================================
    BUS VOLTAGE REGISTER (R)
    -----------------------------------------------------------------------*/
    #define INA219_REG_BUSVOLTAGE                  (0x02)
/*=========================================================================*/

/*=========================================================================
    POWER REGISTER (R)
    -----------------------------------------------------------------------*/
    #define INA219_REG_POWER                       (0x03)
/*=========================================================================*/

/*=========================================================================
    CURRENT REGISTER (R)
    -----------------------------------------------------------------------*/
    #define INA219_REG_CURRENT                     (0x04)
/*=========================================================================*/

/*=========================================================================
    CALIBRATION REGISTER (R/W)
    -----------------------------------------------------------------------*/
    #define INA219_REG_CALIBRATION                 (0x05)
/*=========================================================================*/

namespace m8r {

class INA219
{
public:
    enum VoltageRange { Range16V, Range32V };

	INA219() : _address(0) { }
    
    void setAddress(uint8_t address) { _address = address; }
    
    // lsbMicroamps should be a number that 1000 can be evenly divided by to provide maximum
    // computational accuracy. It must also be between the maximum expected current divided 
    // by 4096 and 32767, preferably closer to the latter. Furthermore, the maximum current
    // must be less than the voltage range times the shunt resistance.
    void setCalibration(VoltageRange range, uint16_t lsbMicroamps, uint16_t shuntMilliohms)
    {
        setConfiguration(range);
        updateCalibration(lsbMicroamps, shuntMilliohms);
        writeRegister(INA219_REG_CALIBRATION, _calibrationValue);
    }
    
    void setConfiguration(VoltageRange range)
    {
        updateConfiguration(range);
        writeRegister(INA219_REG_CONFIG, _configValue);
    }
    
    void trigger() { writeRegister(INA219_REG_CONFIG, _configValue); }
    
    uint16_t configuration() { return readRegister(INA219_REG_CONFIG); }
    uint16_t calibration() { return readRegister(INA219_REG_CALIBRATION); }
    
    int16_t busMilliVolts() { return static_cast<int16_t>((readRegister(INA219_REG_BUSVOLTAGE) >> 3) * 4); }
    int16_t shuntMilliVolts() { return static_cast<int16_t>(readRegister(INA219_REG_SHUNTVOLTAGE)); }
    int32_t shuntMicroAmps()
    {
        // Sometimes a sharp load will reset the INA219, which will
        // reset the cal register, meaning CURRENT and POWER will
        // not be available ... avoid this by always setting a cal
        // value even if it's an unfortunate extra step
        writeRegister(INA219_REG_CALIBRATION, _calibrationValue);
        return static_cast<int32_t>(readRegister(INA219_REG_CURRENT)) * 1000 / _currentDivider_mA;
    }


private:
    void writeRegister(uint8_t reg, uint16_t value)
    {
        uint8_t buf[3] = { reg, static_cast<uint8_t>(value >> 8), static_cast<uint8_t>(value) };
        _i2c.write(_address, buf, 3);
    }
    uint16_t readRegister(uint8_t reg)
    {
        _i2c.write(_address, &reg, 1);
        uint8_t buf[2];
        _i2c.read(_address, buf, 2);
        return (static_cast<uint16_t>(buf[0]) << 8) | static_cast<uint16_t>(buf[1]);
    }
    
    void updateCalibration(uint16_t maxMillamps, uint16_t maxMilliohms);
    void updateConfiguration(VoltageRange);
    
    static I2CMaster _i2c;

    uint8_t _address;
    uint16_t _calibrationValue;
    uint16_t _configValue;
    
    // The following multipliers are used to convert raw current and power
    // values to mA and mW, taking into account the current config settings
    uint32_t _currentDivider_mA;
    uint32_t _powerDivider_mW;
};

}
