##############################################################################
# Makefile
# 

PATH		:= $(PATH)

# werken we vanuit ROM of RAM?
# Kies RUN_FROM_ROM (flash jtag of NXP serial tool )) of RUN_FROM_RAM (jtag)
# let op flash jtag werkt alleen vanaf de board versie 4.0 en hoger
MEM             = RUN_FROM_RAM


# Tools
CC			:= arm-elf-gcc
CPP			:= arm-elf-g++
OBJCOPY		:= arm-elf-objcopy
INSIGHT		:= arm-elf-insight

# Tool locations
PATH		:= /cygdrive/c/cygwin/arm/bin:$(PATH)

# Project files
PPR			:= $(wildcard *.ppr)
NAME		:= $(basename $(PPR))
BIN			:= $(NAME).bin
ELF			:= $(NAME).elf
MAP			:= $(NAME).map
HEX			:= $(NAME).hex
ifeq ($(MEM),RUN_FROM_RAM)
LNK             = Linkerscript_RAM
else
LNK             = Linkerscript_ROM
endif
#DEBUG_TYPE	:= wiggler

# Options
CFLAGS		:= -g -pipe -Wall -mcpu=arm7tdmi -mtune=arm7tdmi -mstructure-size-boundary=32 -Wno-multichar -fno-builtin $(INCPATHS)
LDFLAGS		:= -Wl,-Map,$(MAP) -T$(LNK) -nostartfiles

# Extract source files from PSPad project file
FILES		:= $(shell	\
	gawk '	\
		/^\t\t[^\t]/		{ OK=0 }	\
		//					{ if (OK==1) print $0 }	\
		/^\t\t\+compile$$/	{ OK=1 }	\
	' $(PPR))

# Which object files should be built from the source files?
ALL_OBJS	:=	$(patsubst %.cpp,%.o,\
				$(patsubst %.c,%.o,\
				$(patsubst %.s,%.o,\
				$(patsubst %.S,%.o,\
				$(FILES)))))

# Which files can be cleaned?
CLEAN		+= $(ALL_OBJS) $(BIN) $(ELF) $(MAP) $(HEX)

#  openocd_724.exe
.PHONY: all
all: $(NAME)

.PHONY: $(NAME)
$(NAME): $(ELF)	#$(BIN)


.PHONY: debug
debug: $(ELF)
#line is ok for usb debugging, because process is not ocdlib in that case...
	$(if $(shell pslist | grep -i openocd),,cmd /c start openocd-ftd2xx.exe -f openocd_hu.cfg) 
	$(if $(shell pslist | grep -i OcdLib),,cmd /c start OcdLibRemote --cpu ARM7 --device WIGGLER) 
	$(INSIGHT) --command=command.txt $(ELF) 

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $(ELF) $(BIN)
# maak na de compilatie ook een HEX file
$(HEX): $(ELF)
ifeq ($(MEM),RUN_FROM_ROM)
	$(OBJCOPY) -O ihex $(ELF) $(HEX)
endif

all: $(ELF) $(HEX)

$(HEX): $(ELF)

$(ELF): $(ALL_OBJS) $(LNK)
	$(CPP) $(CFLAGS) $(LDFLAGS) -o $@ $(filter-out %crt0.o,$(ALL_OBJS))

# Common include file dependecies
%.c:: %.h
%.cpp:: %.h

# How to make object files?
%.o: %.c; $(CC) $(CFLAGS) -c -o $@ $<
%.o: %.cpp; $(CPP) $(CFLAGS) -c -o $@ $<
%.o: %.s; $(CC) $(CFLAGS) -c -o $@ $<
%.o: %.S; $(CC) $(CFLAGS) -c -o $@ $<
%.o: %.bin; cd $(@D); $(OBJCOPY) -I binary -O elf32-little --rename-section .data=.rodata $(<F) $(@F)

# Remove output files
.PHONY: clean
clean:
	-$(RM) $(CLEAN)




          
