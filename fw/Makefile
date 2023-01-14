# -*- mode: makefile-gmake-*-
# user defines ################################################################
TARGET = main
MCU = msp430g2232
DEFINES =
CFLAGS = $(DEFINES) -mmcu=$(MCU) -g3 -Os -Wunused -ffunction-sections -MP -MMD -Wall
# ASFLAGS = -mmcu=$(MCU) -x assembler-with-cpp -Wa,-gstabs
LDFLAGS = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map,--gc-sections

# calculated vars #############################################################
SRC = $(TARGET).c

CFILES = $(filter %.c, $(SRC))
OBJECTS = $(CFILES:.c=.o)

# binaries ####################################################################
CC      	= msp430-elf-gcc
OBJCOPY 	= msp430-elf-objcopy
GD          = msp430-elf-gdb
SZ          = msp430-elf-size
RM      	= rm -rf
MKDIR		= mkdir -p

# user targets ################################################################
.PHONY: all clean program debug program-noload size

all: $(TARGET).elf

clean:
	$(RM) $(OBJECTS) $(TARGET.elf) $(OBJECTS:.o=.d)

program: $(TARGET).elf
	mspdebug tilib "prog $<"

debug: $(TARGET).elf
	$(GD) -x init.gdb $<

size : $(OBJECTS)
	$(SZ) $^ | sort -n

GEN_SMAP = ./gen_map.py
GEN_CMAP = ./gen_char_map.py

# internal targets ############################################################

$(TARGET).elf: $(sort $(OBJECTS))
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@

$(OBJECTS): %.o: %.c
	$(CC) -c $(CFLAGS) $(INC) -o $@ $<

segmap.c: %.c: %.map $(GEN_SMAP)
	$(GEN_SMAP) $< > $@

charmap.c: %.c: %.map $(GEN_CMAP)
	$(GEN_CMAP) $< > $@

-include  $(OBJECTS:.o=.d)

# debug targets ###############################################################
print-% :
	@echo $* = $($*)
