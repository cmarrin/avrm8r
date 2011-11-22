#----------------------------------------------------------------------------
# Configuration file for etherclock
#----------------------------------------------------------------------------

FREQUENCY = 12500000
MCU = atmega168

TARGET = etherclock

M8R_SRC_DIR = ../../library

HEADERS = 

SRC = MyApplication.cpp

AVRDUDE_DEVICE = DRAGON
