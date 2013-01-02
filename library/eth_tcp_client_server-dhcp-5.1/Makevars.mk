# makefile, written by guido socher
#
# Please select according to the type of board you are using:
#MCU=atmega88
#DUDECPUTYPE=m88
#MCU=atmega168
#DUDECPUTYPE=m168
MCU=atmega328p
DUDECPUTYPE=m328p
#MCU=atmega644
#DUDECPUTYPE=m644
#MCU=atmega644
#DUDECPUTYPE=m644p
#
# LOADCMD in this top level Makefile is only used for reading and writing
# fuses. You have to edit the Makefiles in the subdirectories if you want
# to use the "make load" feature.
#
# === Edit this and enter the correct device/com-port:
# linux (plug in the avrusb500 and type dmesg to see which device it is):
LOADCMD=avrdude -P /dev/ttyUSB0

# mac (plug in the programer and use ls /dev/tty.usbserial* to get the name):
#LOADCMD=avrdude -P /dev/tty.usbserial-A9006MOb

# windows (check which com-port you get when you plugin the avrusb500):
#LOADCMD=avrdude -P COM4

# All operating systems: if you have set the default_serial paramter 
# in your avrdude.conf file correctly then you can just use this
# and you don't need the above -P option:
#LOADCMD=avrdude
# === end edit this
#
