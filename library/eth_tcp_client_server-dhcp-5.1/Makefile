# makefile, written by guido socher
#
# !! NOTE !! you need to open the Makevars.mk file found in this directory
#            and adapt common microcontroller and programmer dependent settings
#            in that file.
#
include Makevars.mk
#
#
all:
	${MAKE} -C server-www-remswitch DUDECPUTYPE="$(DUDECPUTYPE)" MCU="$(MCU)" all
	${MAKE} -C server-www-simple DUDECPUTYPE="$(DUDECPUTYPE)" MCU="$(MCU)" all
	${MAKE} -C client-www-dhcp DUDECPUTYPE="$(DUDECPUTYPE)" MCU="$(MCU)" all
	${MAKE} -C client-www DUDECPUTYPE="$(DUDECPUTYPE)" MCU="$(MCU)" all
	${MAKE} -C client-tuxgr-email DUDECPUTYPE="$(DUDECPUTYPE)" MCU="$(MCU)" all
	@echo "done"
#
load: 
	@echo "Note: the load command can only be used from induvidual sub-directories. Go to the sub-directory you want and load the type of code you would like to load from there"
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
	${MAKE} -C server-www-remswitch clean
	${MAKE} -C server-www-simple clean
	${MAKE} -C client-www-dhcp clean
	${MAKE} -C client-www clean
	${MAKE} -C client-tuxgr-email clean
	@echo "OK: all clean"
#-------------------
