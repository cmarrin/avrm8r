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
all: basic_web_server.hex test_OKworks.hex test_readSiliconRev.hex
	@echo "done"
#-------------------
help: 
	@echo "Usage: make all|basic_web_server|test_OKworks|test_readSiliconRev"
	@echo "or"
	@echo "make fuse|rdfuses"
	@echo "or"
	@echo "make load|load_basic_web_server|load_test_OKworks|load_test_readSiliconRev"
	@echo "The command make load is the same as make load_basic_web_server"
	@echo "or"
	@echo "Usage: make clean"
	@echo " "
	@echo " "
	@echo "You have to set the low fuse byte to 0x60 on all new tuxgraphics boards".
	@echo "This can be done with the command (linux/mac if you use avrusb500): make fuse"
#-------------------
basic_web_server.hex: basic_web_server.elf 
	$(OBJCOPY) -R .eeprom -O ihex basic_web_server.elf basic_web_server.hex 
	avr-size basic_web_server.elf
	@echo " "
	@echo "Expl.: data=initialized data, bss=uninitialized data, text=code"
	@echo " "

basic_web_server.elf: basic_web_server.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o 
	$(CC) $(CFLAGS) -o basic_web_server.elf -Wl,-Map,basic_web_server.map basic_web_server.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o 
websrv_help_functions.o: ../websrv_help_functions.c ../websrv_help_functions.h ip_config.h 
	$(CC) $(CFLAGS) -I. -Os -c ../websrv_help_functions.c
enc28j60.o: ../enc28j60.c ../timeout.h ../enc28j60.h
	$(CC) $(CFLAGS) -I. -Os -c ../enc28j60.c
ip_arp_udp_tcp.o: ../ip_arp_udp_tcp.c ../net.h ../enc28j60.h ip_config.h
	$(CC) $(CFLAGS) -I. -Os -c ../ip_arp_udp_tcp.c
basic_web_server.o: basic_web_server.c ../ip_arp_udp_tcp.h ../enc28j60.h ../timeout.h ../net.h ../websrv_help_functions.h ip_config.h
	$(CC) $(CFLAGS) -I. -Os -c basic_web_server.c
test_OKworks.hex: test_OKworks.elf 
	$(OBJCOPY) -R .eeprom -O ihex test_OKworks.elf test_OKworks.hex 
	avr-size test_OKworks.elf
	@echo " "
	@echo "Expl.: data=initialized data, bss=uninitialized data, text=code"
	@echo " "

test_OKworks.elf: test_OKworks.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o 
	$(CC) $(CFLAGS) -o test_OKworks.elf -Wl,-Map,test_OKworks.map test_OKworks.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o 
test_OKworks.o: test_OKworks.c ../ip_arp_udp_tcp.h ../enc28j60.h ../timeout.h ../net.h ../websrv_help_functions.h ip_config.h
	$(CC) $(CFLAGS) -I. -Os -c test_OKworks.c
test_readSiliconRev.hex: test_readSiliconRev.elf 
	$(OBJCOPY) -R .eeprom -O ihex test_readSiliconRev.elf test_readSiliconRev.hex 
	avr-size test_readSiliconRev.elf
	@echo " "
	@echo "Expl.: data=initialized data, bss=uninitialized data, text=code"
	@echo " "

test_readSiliconRev.elf: test_readSiliconRev.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o 
	$(CC) $(CFLAGS) -o test_readSiliconRev.elf -Wl,-Map,test_readSiliconRev.map test_readSiliconRev.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o 
test_readSiliconRev.o: test_readSiliconRev.c ../ip_arp_udp_tcp.h ../enc28j60.h ../timeout.h ../net.h ../websrv_help_functions.h ip_config.h
	$(CC) $(CFLAGS) -I. -Os -c test_readSiliconRev.c
#------------------
load_test_OKworks: test_OKworks.hex
	$(LOADCMD) $(LOADARG)test_OKworks.hex

load_test_readSiliconRev: test_readSiliconRev.hex
	$(LOADCMD) $(LOADARG)test_readSiliconRev.hex

load_basic_web_server: basic_web_server.hex
	$(LOADCMD) $(LOADARG)basic_web_server.hex
load: basic_web_server.hex
	$(LOADCMD) $(LOADARG)basic_web_server.hex
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
	rm -f *.o *.map *.elf basic_web_server.hex  test_OKworks.hex  test_readSiliconRev.hex
#-------------------
