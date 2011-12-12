#----------------------------------------------------------------------------
# General build file for marrinator. You should not have to change this file
#----------------------------------------------------------------------------

# Default values in case they are not defined
ifndef FREQUENCY
FREQUENCY = 8000000
endif

ifndef MCU
MCU = atmega168
endif

ifndef TOOLS_DIR
TOOLS_DIR = /sw
endif

ifndef M8R_SRC_DIR
M8R_SRC_DIR = .
endif

INSTALL_DIR = ~/Library/m8r

ifndef BUILT_PRODUCTS_DIR
BUILT_PRODUCTS_DIR = $(CURDIR)/obj
endif

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
ifndef OPT
OPT = s
endif

LIB_HEADER_DIR = $(M8R_SRC_DIR)/m8r
LIB_TARGET = libm8r

LIB_MAIN_HEADER = m8r.h

LIB_HEADERS = \
    ADC.h \
    Animator.h \
    Application.h \
    BlinkErrorReporter.h \
    Button.h \
    ENC28J60.h \
    EventListener.h \
    EventSourceEnums.h \
    MAX6969.h \
    Network.h \
    NetworkInterface.h \
    NTPClient.h \
    RTC.h \
    ShiftReg.h \
    Socket.h \
    SPI.h \
    STP08CDC596.h \
    TCPSocket.h \
    TimerBase.h \
    Timer0.h \
    Timer1.h \
    Timer2.h \
    TimerEventMgr.h \
    UDPSocket.h \

LIB_SRC = \
    main.c \
    ADC.cpp \
    Animator.cpp \
    Application.cpp \
    Button.cpp \
    ENC28J60.cpp \
    Network.cpp \
    NTPClient.cpp \
    RTC.cpp \
    ShiftReg.cpp \
    Socket.cpp \
    TCPSocket.cpp \
    Timer0.cpp \
    Timer1.cpp \
    Timer2.cpp \
    TimerEventMgr.cpp \
    UDPSocket.cpp \

LIB_OBJ := $(LIB_SRC)
LIB_OBJ := $(LIB_OBJ:%.c=$(BUILT_PRODUCTS_DIR)/%.o)
LIB_OBJ := $(LIB_OBJ:%.cpp=$(BUILT_PRODUCTS_DIR)/%.o)
LIB_OBJ := $(LIB_OBJ:%.S=$(BUILT_PRODUCTS_DIR)/%.o)
LIB_SRC := ${LIB_SRC:%=$(M8R_SRC_DIR)/%}

OBJ := $(SRC)
OBJ := $(OBJ:%.c=$(BUILT_PRODUCTS_DIR)/%.o)
OBJ := $(OBJ:%.cpp=$(BUILT_PRODUCTS_DIR)/%.o)
OBJ := $(OBJ:%.S=$(BUILT_PRODUCTS_DIR)/%.o)
SRC := ${SRC:%=${M8R_SRC_DIR}/%}

# Place -D or -U options here
CDEFS = -DF_CPU=$(FREQUENCY)UL

# Set DEBUG flag in the environment:
#
#	'no'     - no debugging
#	'yes'    - full debugging (usually only needed when reading listing file)
#	'report' - compile in debugging statements (error reporting, asserts, etc.)
#
CDEFS += $(if $(filter release no false 0, $(DEBUG)), -DNDEBUG -O$(OPT), )
CDEFS += $(if $(filter debug yes true 1, $(DEBUG)), -DDEBUG -g -O0, )
CDEFS += $(if $(filter report 2, $(DEBUG)), -DDEBUG -O$(OPT), )

#---------------- Compiler Options ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = $(CDEFS)
CFLAGS += -DMCU=$(MCU) -mmcu=$(MCU) -I.
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -finline-limit=10 -fno-inline-small-functions 
CFLAGS += -ffreestanding
CFLAGS += -mcall-prologues 

CFLAGS += -Wall
CFLAGS += $(patsubst %,-I%,$(M8R_SRC_DIR))
CFLAGS += $(patsubst %,-I%,$(LIB_HEADER_DIR))

#---------------- Compiler Options ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CPPFLAGS = -fno-exceptions 

#---------------- Assembler Options ----------------
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 

LDFLAGS = -L$(BUILT_PRODUCTS_DIR) -lm8r 
LDFLAGS += -Wl,-gc-sections -Wl,--relax
LDFLAGS += -Wl,-Map=$(BUILT_PRODUCTS_DIR)/$(TARGET).map,--cref
LDFLAGS += -mmcu=$(MCU)

# -------- AVR Dude ----------
ifndef AVRDUDE_DEVICE
AVRDUDE_DEVICE = STK500
endif

ifndef AVRDUDE_PORT
AVRDUDE_PORT = $(AVRDUDE_PORT_$(AVRDUDE_DEVICE))
endif

ifndef AVRDUDE_PROGRAMMER
AVRDUDE_PROGRAMMER = $(AVRDUDE_PROGRAMMER_$(AVRDUDE_DEVICE))
endif

AVRDUDE_PORT_DRAGON = usb
AVRDUDE_PORT_STK500 = /dev/cu.usbserial
AVRDUDE_PROGRAMMER_DRAGON = dragon_isp
AVRDUDE_PROGRAMMER_STK500 = stk500v1

#---------------- Library Options ----------------
# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# If this is left blank, then it will use the Standard printf version.
PRINTF_LIB = 
#PRINTF_LIB = $(PRINTF_LIB_MIN)
#PRINTF_LIB = $(PRINTF_LIB_FLOAT)


# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# If this is left blank, then it will use the Standard scanf version.
SCANF_LIB = 
#SCANF_LIB = $(SCANF_LIB_MIN)
#SCANF_LIB = $(SCANF_LIB_FLOAT)

MATH_LIB =
#MATH_LIB = -lm

# Define programs and commands.
SHELL = PATH=$${PATH}:/usr/local/bin /bin/sh
CC = ${TOOLS_DIR}/bin/avr-gcc
AR = ${TOOLS_DIR}/bin/avr-ar
OBJCOPY = ${TOOLS_DIR}/bin/avr-objcopy
OBJDUMP = ${TOOLS_DIR}/bin/avr-objdump
SIZE = ${TOOLS_DIR}/bin/avr-size
AVRDUDE = /usr/local/bin/avrdude
NM = ${TOOLS_DIR}/bin/avr-nm
REMOVE = rm -f
COPY = cp
MKDIR = mkdir

HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = sh $(M8R_SRC_DIR)/section_sizes $(SIZE) $(BUILT_PRODUCTS_DIR)/$(TARGET).elf; echo "================================="

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CPPFLAGS = -mmcu=$(MCU) -I. $(CPPFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

build_library: begin $(BUILT_PRODUCTS_DIR)/$(LIB_TARGET).a finished

# Archive: create library archive from library object files.
$(BUILT_PRODUCTS_DIR)/$(LIB_TARGET).a: $(LIB_OBJ)
	@echo
	@echo "Archiving:" $@
	$(AR) rcs $@ $^
	$(REMOVE) $^

build_app: build_library sizebefore $(BUILT_PRODUCTS_DIR)/$(TARGET).hex $(BUILT_PRODUCTS_DIR)/$(TARGET).lss sizeafter

program_app: build_app
	@echo
	@echo Programming:
	@echo
	$(AVRDUDE) -C /usr/local/etc/avrdude.conf -c $(AVRDUDE_PROGRAMMER) -P $(AVRDUDE_PORT) -p $(MCU) -U flash:w:$(BUILT_PRODUCTS_DIR)/$(TARGET).hex:i
begin:
	@echo
	@echo "-------- begin --------"
	@$(MKDIR) -p $(BUILT_PRODUCTS_DIR)

finished:
	@echo
	@echo "Errors: none"

end:
	@echo
	@echo "-------- end --------"

# Display size of file.
sizebefore:
	@if [ -f $(BUILT_PRODUCTS_DIR)/$(TARGET).elf ]; then echo; echo "========== size before =========="; $(ELFSIZE); fi

sizeafter:
	@if [ -f $(BUILT_PRODUCTS_DIR)/$(TARGET).elf ]; then echo; echo "========== size after ==========="; $(ELFSIZE); fi

# Display compiler version information.
gccversion : 
	@$(CC) --version

install_library: FORCE
	@echo
	@echo "Installing:"
	@echo
	$(MKDIR) -p $(INSTALL_DIR)/m8r
	$(COPY) $(LIB_MAIN_HEADER) $(INSTALL_DIR)/
	$(COPY) ${LIB_HEADERS:%.h=${LIB_HEADER_DIR}/%.h} $(INSTALL_DIR)/m8r
	$(COPY) $(LIB_SRC) $(INSTALL_DIR)/
	$(COPY) shared.mk section_sizes $(INSTALL_DIR)/

FORCE:

#### Generating object files ####
# object from C
#.c.o: 
$(BUILT_PRODUCTS_DIR)/%.o: $(M8R_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# object from C++ (.cc, .cpp, .C files)
#.cc.o .cpp.o .C.o :
$(BUILT_PRODUCTS_DIR)/%.o: $(M8R_SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILT_PRODUCTS_DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# object from asm
#.S.o :
$(BUILT_PRODUCTS_DIR)/%.o: $(M8R_SRC_DIR)/%.S
	$(CC) $(ASMFLAGS) -c $< -o $@

$(BUILT_PRODUCTS_DIR)/$(TARGET).elf: $(BUILT_PRODUCTS_DIR)/$(LIB_TARGET).a $(OBJ)
	@echo
	@echo "Linking"
	@echo
	$(CC) $(OBJ) $(LDFLAGS) -o $(BUILT_PRODUCTS_DIR)/$(TARGET).elf

$(BUILT_PRODUCTS_DIR)/$(TARGET).hex: $(BUILT_PRODUCTS_DIR)/$(TARGET).elf
	@echo
	@echo "Creating .hex file"
	@echo
	$(OBJCOPY) -O ihex $(BUILT_PRODUCTS_DIR)/$(TARGET).elf $(BUILT_PRODUCTS_DIR)/$(TARGET).hex 

$(BUILT_PRODUCTS_DIR)/$(TARGET).lss: $(BUILT_PRODUCTS_DIR)/$(TARGET).elf
	@echo
	@echo "Creating listing file"
	@echo
	${TOOLS_DIR}/bin/avr-objdump -lhS --demangle $(BUILT_PRODUCTS_DIR)/$(TARGET).elf > $(BUILT_PRODUCTS_DIR)/$(TARGET).lss

# Target: clean project.
clean: begin clean_list end

clean_list :
	@echo
	@echo "Cleaning project:"
	@echo
	$(REMOVE) -rf $(BUILT_PRODUCTS_DIR)/*
	$(REMOVE) $(CSRC:.c=.d)
	$(REMOVE) $(CPPSRC:.cpp=.d)
	$(REMOVE) .dep/*
    
show_event_enums:
	@$(CC) -E $(CFLAGS) -I$(M8R_SRC_DIR)/m8r $(M8R_SRC_DIR)/print_event_enums.c > $(BUILT_PRODUCTS_DIR)/show_event_enums.c
	@gcc $(BUILT_PRODUCTS_DIR)/show_event_enums.c -o $(BUILT_PRODUCTS_DIR)/show_event_enums
	$(BUILT_PRODUCTS_DIR)/show_event_enums

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
