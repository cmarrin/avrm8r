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
TOOLS_DIR = /usr/local/avrtools
endif

ifndef M8R_SRC_DIR
M8R_SRC_DIR = ~/Library/m8r
endif

C_INCLUDE_PATH = $(M8R_SRC_DIR)

ifndef OBJECT_FILE_DIR
OBJECT_FILE_DIR = $(CURDIR)/obj
endif

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
ifndef OPT
OPT = s
endif

# Place -D or -U options here
CDEFS = -DF_CPU=$(FREQUENCY)UL
CDEFS += $(if $(filter yes true 1, $(DEBUG)), -DDEBUG -g -O0, -DNDEBUG -O$(OPT))

#---------------- Compiler Options ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = $(CDEFS)
CFLAGS += $(if $(filter yes true 1, $(DEBUG)), -DDEBUG -g -O0, -DNDEBUG -O$(OPT))
CFLAGS += -mmcu=$(MCU) -I.
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -finline-limit=10
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
CPPFLAGS = -fno-exceptions 

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

HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A $(TARGET).elf

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

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CPPFLAGS = -mmcu=$(MCU) -I. $(CPPFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

# Default target.
#all: begin gccversion sizebefore build sizeafter finished end

ar: begin gccversion $(OBJECT_FILE_DIR)/$(LIB_TARGET).a finished end

# Archive: create library archive from library object files.
$(OBJECT_FILE_DIR)/$(LIB_TARGET).a: $(LIB_OBJ)
	@echo "targ=" $(OBJECT_FILE_DIR)/$(LIB_TARGET).a
	@echo $(MSG_ARCHIVING) $@
	$(AR) rcs $@ $^
	$(REMOVE) $^

begin:
	@echo
	@echo "targ=" $(OBJECT_FILE_DIR)/$(LIB_TARGET).a
	@echo $(MSG_BEGIN)
	@$(MKDIR) -p $(OBJECT_FILE_DIR)

finished:
	@echo $(MSG_ERRORS_NONE)

end:
	@echo $(MSG_END)
	@echo

# Display size of file.
sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi

sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi

# Display compiler version information.
gccversion : 
	@$(CC) --version

#### Generating assembly ####
# asm from C
%.s: %.c
	$(CC) -S $(CFLAGS) $< -o $(OBJECT_FILE_DIR)/$@

# asm from (hand coded) asm
%.s: %.S
	$(CC) -S $(ASMFLAGS) $< > $(OBJECT_FILE_DIR)/$@

# asm from C++
.cpp.s .cc.s .C.s :
	$(CC) -S $(CFLAGS) $(CPPFLAGS) $< -o $(OBJECT_FILE_DIR)/$@

#### Generating object files ####
# object from C
#.c.o: 
$(OBJECT_FILE_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# object from C++ (.cc, .cpp, .C files)
#.cc.o .cpp.o .C.o :
$(OBJECT_FILE_DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# object from asm
#.S.o :
$(OBJECT_FILE_DIR)/%.o: %.S
	$(CC) $(ASMFLAGS) -c $< -o $@

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
.PHONY : all install
