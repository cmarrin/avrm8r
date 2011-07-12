#----------------------------------------------------------------------------
# General build file for marrinator. You should not have to change this file
#----------------------------------------------------------------------------

ifndef TOOLS_DIR
TOOLS_DIR = /usr/local/avrtools
endif

ifndef M8R_SRC_DIR
M8R_SRC_DIR = .
endif

ifndef OBJECT_FILE_DIR
OBJECT_FILE_DIR = $(M8R_SRC_DIR)/obj
endif

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
ifndef OPT
OPT = s
endif

ifndef INSTALL_DIR
INSTALL_DIR = ~/Library/m8r
endif

ifndef HEADER_DIR
HEADER_DIR = $(M8R_SRC_DIR)/m8r
endif

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

# Place -D or -U options here
CDEFS = -DF_CPU=$(FREQUENCY)UL

#---------------- Compiler Options ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = $(CDEFS)
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -finline-limit=3 -fno-inline-small-functions
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -ffreestanding
CFLAGS += -Wall
CFLAGS += $(patsubst %,-I%,$(C_INCLUDE_PATH))

#---------------- Compiler Options ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CPPFLAGS = $(CDEFS)
CPPFLAGS += -O$(OPT)
CPPFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CPPFLAGS += -finline-limit=10
CPPFLAGS += -ffunction-sections -fdata-sections -ffreestanding 
CPPFLAGS += -Wall 
CPPFLAGS += $(patsubst %,-I%,$(C_INCLUDE_PATH))

#---------------- Assembler Options ----------------
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 


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
NM = ${TOOLS_DIR}/bin/avr-nm
REMOVE = rm -f
COPY = cp
MKDIR = mkdir

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_ARCHIVING = Archiving:
MSG_COMPILING = Compiling:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:
MSG_INSTALLING = Installing project:

# Define all object files.
OBJ := ${CSRC:%.c=${OBJECT_FILE_DIR}/%.o} ${CPPSRC:%.cpp=${OBJECT_FILE_DIR}/%.o} ${ASRC:%.S=${OBJECT_FILE_DIR}/%.o}

HDR := ${HEADERS:%.h=${HEADER_DIR}/%.h}

SRCDIR = $(M8R_SRC_DIR)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CPPFLAGS = -mmcu=$(MCU) -I. $(CPPFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

# Default target.
debug: override CDEFS += -DDEBUG -g -O0
debug: all

release: override CDEFS += -DNDEBUG -O$(OPT)
release: all
    
install:
	@echo
	@echo $(MSG_INSTALLING) $@
	$(MKDIR) -p $(INSTALL_DIR)/Headers/m8r
	$(MKDIR) -p $(INSTALL_DIR)/Sources/m8r
	$(COPY) $(MAIN_HEADER) $(INSTALL_DIR)/Headers
	$(COPY) $(HDR) $(INSTALL_DIR)/Headers/m8r
	$(COPY) $(CPPSRC) $(INSTALL_DIR)/Sources/m8r
	$(COPY) $(CSRC) $(INSTALL_DIR)/Sources/m8r
	$(COPY) Makefile $(INSTALL_DIR)/

all: begin gccversion sizebefore build sizeafter end

build: ar

ar: $(OBJECT_FILE_DIR)/$(TARGET).a

# Display compiler version information.
gccversion : 
	@$(CC) --version

# Archive: create archive output file from object files.
.SECONDARY : $(OBJECT_FILE_DIR)/$(TARGET).a
.PRECIOUS : $(OBJ)
$(OBJECT_FILE_DIR)/%.a: $(OBJ)
	@echo
	@echo $(MSG_ARCHIVING) $@
	$(AR) rcs $@ $^
	$(REMOVE) $^


# Compile: create object files from C source files.
$(OBJECT_FILE_DIR)/%.o : $(SRCDIR)/%.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(MKDIR) -p $(OBJECT_FILE_DIR)
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 

# Compile: create object files from C++ source files.
$(OBJECT_FILE_DIR)/%.o : $(SRCDIR)/%.cpp
	@echo
	@echo $(MSG_COMPILING) $<
	$(MKDIR) -p $(OBJECT_FILE_DIR)
	$(CC) -c $(ALL_CPPFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@

# Compile: create assembler files from C++ source files.
%.s : %.cpp
	$(CC) -S $(ALL_CPPFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
$(OBJECT_FILE_DIR)/%.o : $(SRCDIR)/%.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(MKDIR) -p $(OBJECT_FILE_DIR)
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Create preprocessed source for use in sending a bug report.
%.i : %.c
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@ 

# Create preprocessed source for use in sending a bug report.
%.i : %.cpp
	$(CC) -E -mmcu=$(MCU) -I. $(CPPFLAGS) $< -o $@ 


# Target: clean project.
clean: begin clean_list end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) -rf $(OBJECT_FILE_DIR)/*
	$(REMOVE) $(CSRC:.c=.d)
	$(REMOVE) $(CPPSRC:.cpp=.d)
	$(REMOVE) .dep/*



# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build ar clean clean_list



