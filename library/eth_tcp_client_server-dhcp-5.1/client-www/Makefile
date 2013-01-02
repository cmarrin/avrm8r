# makefile, written by guido socher
#
include ../Makevars.mk
#
LOADARG=-p $(DUDECPUTYPE) -c stk500v2 -e -U flash:w:
#
CC=avr-gcc
OBJCOPY=avr-objcopy
# optimize for size:
CFLAGS=-g -mmcu=$(MCU) -Wall -W -Os -mcall-prologues
#-------------------
.PHONY: all 
#
all: test_identi_ca.hex  test_web_client.hex
	@echo "done"
test_web_client: test_web_client.hex
	@echo "done"
test_identi_ca: test_identi_ca.hex
	@echo "done"
#-------------------
help: 
	@echo "Usage: make all|test_web_client|test_identi_ca"
	@echo "or"
	@echo "make fuse|rdfuses"
	@echo "or"
	@echo "make load|load_test_web_client.hex|load_test_identi_ca"
	@echo "The command make load is the same as make load_test_web_client"
	@echo "or"
	@echo "Usage: make clean"
	@echo " "
	@echo " "
	@echo "You have to set the low fuse byte to 0x60 on all new tuxgraphics boards".
	@echo "This can be done with the command (linux/mac if you use avrusb500): make fuse"
#-------------------
test_identi_ca.hex: test_identi_ca.elf 
	$(OBJCOPY) -R .eeprom -O ihex test_identi_ca.elf test_identi_ca.hex 
	avr-size test_identi_ca.elf
	@echo " "
	@echo "Expl.: data=initialized data, bss=uninitialized data, text=code"
	@echo " "
test_identi_ca.elf: test_identi_ca.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o dnslkup.o
	$(CC) $(CFLAGS) -o test_identi_ca.elf -Wl,-Map,test_identi_ca.map test_identi_ca.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o dnslkup.o
#
test_identi_ca.o: test_identi_ca.c ../ip_arp_udp_tcp.h ../net.h ../enc28j60.h ip_config.h ../dnslkup.h
	$(CC) $(CFLAGS) -I. -Os -c test_identi_ca.c
#------------------
test_web_client.hex: test_web_client.elf 
	$(OBJCOPY) -R .eeprom -O ihex test_web_client.elf test_web_client.hex 
	avr-size test_web_client.elf
	@echo " "
	@echo "Expl.: data=initialized data, bss=uninitialized data, text=code"
	@echo " "
test_web_client.elf: test_web_client.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o dnslkup.o
	$(CC) $(CFLAGS) -o test_web_client.elf -Wl,-Map,test_web_client.map test_web_client.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o dnslkup.o
#
test_web_client.o: test_web_client.c ../ip_arp_udp_tcp.h ../net.h ../enc28j60.h ip_config.h ../dnslkup.h
	$(CC) $(CFLAGS) -I. -Os -c test_web_client.c
#------------------
websrv_help_functions.o: ../websrv_help_functions.c ../websrv_help_functions.h ip_config.h 
	$(CC) $(CFLAGS) -I. -Os -c ../websrv_help_functions.c
dnslkup.o: ../dnslkup.c  ../dnslkup.h
	$(CC) $(CFLAGS) -I. -Os -c ../dnslkup.c
enc28j60.o: ../enc28j60.c ../timeout.h ../enc28j60.h
	$(CC) $(CFLAGS) -I. -Os -c ../enc28j60.c
ip_arp_udp_tcp.o: ../ip_arp_udp_tcp.c ../net.h ../enc28j60.h ip_config.h
	$(CC) $(CFLAGS) -I. -Os -c ../ip_arp_udp_tcp.c
#------------------
load_test_identi_ca: test_identi_ca.hex
	$(LOADCMD) $(LOADARG)test_identi_ca.hex

load_test_web_client: test_web_client.hex
	$(LOADCMD) $(LOADARG)test_web_client.hex
load: test_web_client.hex.hex
	$(LOADCMD) $(LOADARG)test_web_client.hex
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
	rm -f *.o *.map *.elf test_identi_ca.hex  test_web_client.hex
#-------------------
