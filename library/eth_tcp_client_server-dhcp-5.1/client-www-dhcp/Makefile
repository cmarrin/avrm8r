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
all: test_dhcp_www_client_pushbutton.hex test_dhcp_www_client_adc0.hex
	@echo "done"
#-------------------
help: 
	@echo "Usage: make all|test_dhcp_www_client_pushbutton|test_dhcp_www_client_adc0"
	@echo "or"
	@echo "make fuse|rdfuses"
	@echo "or"
	@echo "make load|load_test_dhcp_www_client_pushbutton|load_test_dhcp_www_client_adc0"
	@echo "The command make load is the same as make load_test_dhcp_www_client_pushbutton"
	@echo "or"
	@echo "Usage: make clean"
	@echo " "
	@echo " "
	@echo "You have to set the low fuse byte to 0x60 on all new tuxgraphics boards".
	@echo "This can be done with the command (linux/mac if you use avrusb500): make fuse"
#-------------------
test_dhcp_www_client_adc0.hex: test_dhcp_www_client_adc0.elf 
	$(OBJCOPY) -R .eeprom -O ihex test_dhcp_www_client_adc0.elf test_dhcp_www_client_adc0.hex 
	avr-size test_dhcp_www_client_adc0.elf
	@echo " "
	@echo "Expl.: data=initialized data, bss=uninitialized data, text=code"
	@echo " "
test_dhcp_www_client_adc0.elf: test_dhcp_www_client_adc0.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o dnslkup.o dhcp_client.o analog-10bit.o
	$(CC) $(CFLAGS) -o test_dhcp_www_client_adc0.elf -Wl,-Map,test_dhcp_www_client_adc0.map test_dhcp_www_client_adc0.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o dnslkup.o dhcp_client.o analog-10bit.o
#
test_dhcp_www_client_adc0.o: test_dhcp_www_client_adc0.c ../ip_arp_udp_tcp.h ../net.h ../enc28j60.h ip_config.h ../dnslkup.h ../dhcp_client.h analog-10bit.h
	$(CC) $(CFLAGS) -I. -Os -c test_dhcp_www_client_adc0.c
#------------------
test_dhcp_www_client_pushbutton.hex: test_dhcp_www_client_pushbutton.elf 
	$(OBJCOPY) -R .eeprom -O ihex test_dhcp_www_client_pushbutton.elf test_dhcp_www_client_pushbutton.hex 
	avr-size test_dhcp_www_client_pushbutton.elf
	@echo " "
	@echo "Expl.: data=initialized data, bss=uninitialized data, text=code"
	@echo " "
test_dhcp_www_client_pushbutton.elf: test_dhcp_www_client_pushbutton.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o dnslkup.o dhcp_client.o
	$(CC) $(CFLAGS) -o test_dhcp_www_client_pushbutton.elf -Wl,-Map,test_dhcp_www_client_pushbutton.map test_dhcp_www_client_pushbutton.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o dnslkup.o dhcp_client.o
#
test_dhcp_www_client_pushbutton.o: test_dhcp_www_client_pushbutton.c ../ip_arp_udp_tcp.h ../net.h ../enc28j60.h ip_config.h ../dnslkup.h ../dhcp_client.h
	$(CC) $(CFLAGS) -I. -Os -c test_dhcp_www_client_pushbutton.c
#------------------
websrv_help_functions.o: ../websrv_help_functions.c ../websrv_help_functions.h ip_config.h 
	$(CC) $(CFLAGS) -I. -Os -c ../websrv_help_functions.c
dhcp_client.o: ../dhcp_client.c  ../dhcp_client.h ../net.h ../enc28j60.h ../ip_arp_udp_tcp.h ip_config.h
	$(CC) $(CFLAGS) -I. -Os -c ../dhcp_client.c
dnslkup.o: ../dnslkup.c  ../dnslkup.h ../ip_arp_udp_tcp.h ../net.h ip_config.h
	$(CC) $(CFLAGS) -I. -Os -c ../dnslkup.c
enc28j60.o: ../enc28j60.c ../timeout.h ../enc28j60.h
	$(CC) $(CFLAGS) -I. -Os -c ../enc28j60.c
analog-10bit.o: analog-10bit.c analog-10bit.h 
	$(CC) $(CFLAGS) -Os -c analog-10bit.c
ip_arp_udp_tcp.o: ../ip_arp_udp_tcp.c ../net.h ../enc28j60.h ip_config.h
	$(CC) $(CFLAGS) -I. -Os -c ../ip_arp_udp_tcp.c
#------------------
load_test_dhcp_www_client_adc0: test_dhcp_www_client_adc0.hex
	$(LOADCMD) $(LOADARG)test_dhcp_www_client_adc0.hex
load_test_web_client: test_dhcp_www_client_pushbutton.hex
	$(LOADCMD) $(LOADARG)test_dhcp_www_client_pushbutton.hex
load: test_dhcp_www_client_pushbutton.hex
	$(LOADCMD) $(LOADARG)test_dhcp_www_client_pushbutton.hex
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
	rm -f *.o *.map *.elf test_dhcp_www_client_pushbutton.hex test_dhcp_www_client_adc0.hex
#-------------------
