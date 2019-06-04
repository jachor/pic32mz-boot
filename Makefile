MIPS_TOOLCHAIN:=mipsel-linux-gnu-

TARGET=boot
CFLAGS=-Os -nostdlib -mno-abicalls -fno-pic
COBJS=main.o init.o debug.o lowlevel/simple_printf.o
ASOBJS=reset.o

MIPS_OBJCOPY=$(MIPS_TOOLCHAIN)objcopy
MIPS_OBJDUMP=$(MIPS_TOOLCHAIN)objdump
MIPS_AS=$(MIPS_TOOLCHAIN)as
MIPS_CC=$(MIPS_TOOLCHAIN)gcc
MIPS_LD=$(MIPS_TOOLCHAIN)ld
OBJS=$(COBJS) $(ASOBJS)

.PHONY: all clean

all: $(TARGET).elf $(TARGET).srec $(TARGET).dump

clean:
	rm -f *.o $(TARGET).elf $(TARGET).srec

$(TARGET).elf: linker.ld $(OBJS)
	$(MIPS_LD) -nostdlib -T linker.ld $(OBJS) -o $@

$(TARGET).srec: $(TARGET).elf
	$(MIPS_OBJCOPY) -O srec $(TARGET).elf $(TARGET).srec
	
$(TARGET).dump: $(TARGET).elf
	$(MIPS_OBJDUMP) -xd $(TARGET).elf > $(TARGET).dump

%.o: %.c
	$(MIPS_CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(MIPS_AS) -mips32 $< -o $@

# deps
debug.o: debug.h lowlevel/ports.h
main.o: debug.h lowlevel/ports.h
