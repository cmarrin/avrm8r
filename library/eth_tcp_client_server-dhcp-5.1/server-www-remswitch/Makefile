# makefile, written by guido socher
#
include ../Makevars.mk
#
LOADARG=-p $(DUDECPUTYPE) -c stk500v2 -e -U flash:w:
# 
#
CC=avr-gcc
OBJCOPY=avr-objcopy
# optimize for size:
CFLAGS=-g -mmcu=$(MCU) -Wall -W -Os -mcall-prologues
#-------------------
.PHONY: all main
#
all: main.hex
	@echo "done"
main: main.hex
	@echo "done"
#-------------------
help: 
	@echo "Usage: make all|main"
	@echo "or"
	@echo "make fuse|rdfuses"
	@echo "or"
	@echo "make load"
	@echo "or"
	@echo "Usage: make clean"
	@echo " "
	@echo " "
	@echo "You have to set the low fuse byte to 0x60 on all new tuxgraphics boards".
	@echo "This can be done with the command (linux/mac if you use avrusb500): make fuse"
#-------------------
main.hex: main.elf 
	$(OBJCOPY) -R .eeprom -O ihex main.elf main.hex 
	avr-size main.elf
	@echo " "
	@echo "Expl.: data=initialized data, bss=uninitialized data, text=code"
	@echo " "

main.elf: main.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o 
	$(CC) $(CFLAGS) -o main.elf -Wl,-Map,main.map main.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o 
websrv_help_functions.o: ../websrv_help_functions.c ../websrv_help_functions.h ip_config.h 
	$(CC) $(CFLAGS) -I. -Os -c ../websrv_help_functions.c
enc28j60.o: ../enc28j60.c ../timeout.h ../enc28j60.h
	$(CC) $(CFLAGS) -I. -Os -c ../enc28j60.c
ip_arp_udp_tcp.o: ../ip_arp_udp_tcp.c ../net.h ../enc28j60.h ip_config.h
	$(CC) $(CFLAGS) -I. -Os -c ../ip_arp_udp_tcp.c
main.o: main.c ../ip_arp_udp_tcp.h ../enc28j60.h ../timeout.h ../net.h ../websrv_help_functions.h ip_config.h
	$(CC) $(CFLAGS) -I. -Os -c main.c
#------------------
load: main.hex
	$(LOADCMD) $(LOADARG)main.hex
#
#-------------------
# Check this with make rdfuses
# 
rdfuses:
	$(LOADCMD) -p $(DUDECPUTYPE) -c stk500v2 -v -q
#
fuse:
	@echo "Setting clock source to external clock on pin xtal1"
	$(LOADCMD) -p  $(DUDECPUTYPE) -c stk500v2 -u -v -U lfuse:w:0x60:m
#
fuses:
	@echo "Setting clock source to external clock on pin xtal1"
	$(LOADCMD) -p  $(DUDECPUTYPE) -c stk500v2 -u -v -U lfuse:w:0x60:m
#
#-------------------
clean:
	rm -f *.o *.map *.elf main.hex 
#-------------------
