#----------------------------------------------------------------------------
# Configuration file for marrinator
#----------------------------------------------------------------------------

FREQUENCY = 12500000
MCU = atmega168

MAKEFILE_DIR = .
INSTALL_DIR = ~/Library/m8r

HEADER_DIR = $(M8R_SRC_DIR)/m8r

TARGET = libm8r

CSRC = main.c

MAIN_HEADER = m8r.h

HEADERS = \
    ADC.h \
    Animator.h \
    Application.h \
    BlinkErrorReporter.h \
    ENC28J60.h \
    Event.h \
    EventListener.h \
    EventSourceEnums.h \
    MAX6969.h \
    RTC.h \
    ShiftReg.h \
    SPI.h \
    STP08CDC596.h \
    Timer.h \
    TimerEvent.h \
    TimerEventMgr.h \

CPPSRC = \
    ADC.cpp \
    Animator.cpp \
    Application.cpp \
    ENC28J60.cpp \
    EventListener.cpp \
    MAX6969.cpp \
    RTC.cpp \
    ShiftReg.cpp \
    Timer.cpp \
    TimerEvent.cpp \
    TimerEventMgr.cpp
