.section .reset_text, "x"
        .align 2
        .set noreorder
        .global _reset
        .ent _reset

_reset:
    # Entry point that should work on both mips32 and micromips.
        .set nomicromips
    # MicroMIPS: some arithmetics (sll)
    # MIPS32: jump to entry point
    b       _reset_mips32
    # both: nop (branch slot for MIPS32)
    nop
_reset_micromips:
        .set    micromips
    # change to MIPS32 ISA, jump to entry point
    jalx    _reset_mips32
    nop

    # This will be always executed in mips32
        .set nomicromips
_reset_mips32:
	# setup stack for uncached init
	la      $sp, _kseg1_ram_end-0x10
    # initialize cache
	jal     _initialize_cache
	nop
	
	# setup stack for cacheful operation
	la      $sp, _kseg0_ram_end-0x10

    # run early init
    la      $v0, _initialize
    jalr    $v0
    nop

    # enter main
	la      $v0, main
	jalr    $v0
	nop

    # reboot when main finishes
	j       _reset
	nop
	.end _reset
