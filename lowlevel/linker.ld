/* vim: set ts=4 sw=4 et : */

_ram                = 0x00000000;
_ram_size           = 0x00080000;
_bootflash          = 0x1fc00000;
_bootflash_size     = 0x0000ff00;
_data_alignment     = 0x40;

_kseg0_shift        = 0x80000000;
_kseg1_shift        = 0xa0000000;

MEMORY {
    /* non-cacheable, non-mappable */
    kseg1_ram (rw): ORIGIN = _kseg1_shift + _ram, LENGTH = _ram_size
    kseg1_bootflash (rxl) : ORIGIN = _kseg1_shift + _bootflash, LENGTH = _bootflash_size
    kseg0_ram (rw): ORIGIN = _kseg0_shift + _ram, LENGTH = _ram_size
    kseg0_bootflash (rxl): ORIGIN = _kseg0_shift + _bootflash, LENGTH = _bootflash_size

    flash (rxl) : ORIGIN = _bootflash, LENGTH = _bootflash_size
    flash_config (rxl) : ORIGIN = _bootflash + 0xffc0, LENGTH = 0x40
}

SECTIONS {
    .bss : {
        . = ALIGN(_data_alignment);
        PROVIDE(_bss_start = .);
        *(.bss)
        *(.bss.*)
        *(.sbss)
        *(.sbss.*)
        . = ALIGN(_data_alignment);
        PROVIDE(_bss_end = .);
    } >kseg0_ram

    .text.boot : {
        *(.reset_text)
        *(.text.nocache)
        . = ALIGN(_data_alignment);
        _ld_phy_boot_end = ABSOLUTE(.) - _kseg1_shift;
    } >kseg1_bootflash AT>flash

    .text _ld_phy_boot_end + _kseg0_shift : {
        *(.text .text.*)
        *(.rodata .rodata.*)
        . = ALIGN(_data_alignment);
        _ld_phy_text_end = ABSOLUTE(.) - _kseg0_shift;
    } >kseg0_bootflash AT>flash

    .data : {
        PROVIDE(_data_start = ALIGN(_data_alignment));
        *(.data .data.*)
        . = ALIGN(_data_alignment);
        PROVIDE(_data_end = ALIGN(_data_alignment));
    } >kseg0_ram AT>flash
    PROVIDE(_data_src_start = LOADADDR(.data));

    .text.config : {
        KEEP(*(.config))
    } >flash_config

    PROVIDE(_kseg0_ram_end = _kseg0_shift + _ram_size);
    PROVIDE(_kseg1_ram_end = _kseg1_shift + _ram_size);

    /DISCARD/ : {
        *(.reginfo)
        *(.pdr)
        *(.MIPS.abiflags)
    }
}

ENTRY(_reset)
