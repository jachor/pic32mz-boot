TARGET:=boot
LINKER_SCRIPT:=lowlevel/linker.ld
SRCS:=$(wildcard *.c)
SRCS+=$(wildcard lowlevel/*.c lowlevel/*.S)
CFLAGS:=-Os -nostdlib -mno-abicalls -fno-pic -I$(shell pwd)
LDFLAGS=-nostdlib

MIPS_TOOLCHAIN:=mipsel-linux-gnu-

MIPS_OBJCOPY=$(MIPS_TOOLCHAIN)objcopy
MIPS_OBJDUMP=$(MIPS_TOOLCHAIN)objdump
MIPS_AS=$(MIPS_TOOLCHAIN)as
MIPS_CC=$(MIPS_TOOLCHAIN)gcc
MIPS_LD=$(MIPS_TOOLCHAIN)ld

OBJDIR=.obj
COBJS:=$(addprefix $(OBJDIR)/,$(patsubst %.c,%.o, $(filter %.c, $(SRCS))))
ASOBJS:=$(addprefix $(OBJDIR)/,$(patsubst %.S,%.o, $(filter %.S, $(SRCS))))
OBJS:=$(COBJS) $(ASOBJS)
DEPS:=$(patsubst %.o,%.d,$(COBJS))
dep_file_name=$(patsubst %.o,%.d,$@)
temp_dep_file_name=$(patsubst %.o,%.Td,$@)

define announce
	@tput setaf 2
	@echo "> $1"
	@tput sgr0
endef

.PHONY: all clean

all: $(TARGET).elf $(TARGET).srec $(TARGET).bin

clean:
	$(call announce, "Cleaning up")
	@rm -f \
		$(OBJS) $(DEPS) \
		$(TARGET).dump $(TARGET).map $(TARGET).elf $(TARGET).srec

$(TARGET).elf: $(LINKER_SCRIPT) $(OBJS)
	$(call announce, "Linking" $@)
	@$(MIPS_LD) $(LDFLAGS) -T $(LINKER_SCRIPT) $(OBJS) -Map $(TARGET).map -o $@
	@$(MIPS_OBJDUMP) -xd $(TARGET).elf > $(TARGET).dump
	@size -x $(TARGET).elf

$(TARGET).srec: $(TARGET).elf
	$(call announce, "Generating SREC" $@)
	@$(MIPS_OBJCOPY) -O srec $(TARGET).elf $(TARGET).srec

$(TARGET).bin: $(TARGET).srec
	$(call announce, "Generating binary" $@)
	@$(MIPS_OBJCOPY) -I srec -O binary $(TARGET).srec $(TARGET).bin

$(COBJS): $(OBJDIR)/%.o: %.c
	$(call announce, "Compiling" $<)
	@mkdir -p $(@D)
	@$(MIPS_CC) $(CFLAGS) \
		-MT $@ -MMD -MP -MF $(temp_dep_file_name) \
		-c $< -o $@
	@mv -f $(temp_dep_file_name) $(dep_file_name)

$(ASOBJS): $(OBJDIR)/%.o: %.S
	$(call announce, "Assembling" $<)
	@$(MIPS_AS) -mips32 $< -o $@

-include $(DEPS)
