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
.PHONY: all 
#
all: test_emailnotify.hex
	@echo "done"
test_emailnotify: test_emailnotify.hex
	@echo "done"
#-------------------
help: 
	@echo "Usage: make all|test_emailnotify"
	@echo "or"
	@echo "make fuse|rdfuses"
	@echo "or"
	@echo "make load|load_test_emailnotify"
	@echo "The command make load is the same as make load_test_emailnotify"
	@echo "or"
	@echo "Usage: make clean"
	@echo " "
	@echo " "
	@echo "You have to set the low fuse byte to 0x60 on all new tuxgraphics boards".
	@echo "This can be done with the command (linux/mac if you use avrusb500): make fuse"
#-------------------
test_emailnotify.hex: test_emailnotify.elf 
	$(OBJCOPY) -R .eeprom -O ihex test_emailnotify.elf test_emailnotify.hex 
	avr-size test_emailnotify.elf
	@echo " "
	@echo "Expl.: data=initialized data, bss=uninitialized data, text=code"
	@echo " "
test_emailnotify.elf: test_emailnotify.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o 
	$(CC) $(CFLAGS) -o test_emailnotify.elf -Wl,-Map,test_emailnotify.map test_emailnotify.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o 
#
test_emailnotify.o: test_emailnotify.c ../ip_arp_udp_tcp.h ../net.h ../enc28j60.h ip_config.h 
	$(CC) $(CFLAGS) -I. -Os -c test_emailnotify.c
#------------------
websrv_help_functions.o: ../websrv_help_functions.c ../websrv_help_functions.h ip_config.h 
	$(CC) $(CFLAGS) -I. -Os -c ../websrv_help_functions.c
enc28j60.o: ../enc28j60.c ../timeout.h ../enc28j60.h
	$(CC) $(CFLAGS) -I. -Os -c ../enc28j60.c
ip_arp_udp_tcp.o: ../ip_arp_udp_tcp.c ../net.h ../enc28j60.h ip_config.h
	$(CC) $(CFLAGS) -I. -Os -c ../ip_arp_udp_tcp.c
#------------------
load_test_emailnotify: test_emailnotify.hex
	$(LOADCMD) $(LOADARG)test_emailnotify.hex
load: test_emailnotify.hex
	$(LOADCMD) $(LOADARG)test_emailnotify.hex
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
	rm -f *.o *.map *.elf test_emailnotify.hex
#-------------------
