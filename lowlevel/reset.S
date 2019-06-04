.section .reset_text, "x"
        .align 2
        .set noreorder
        .global _reset
        .ent _reset

        .set nomicromips
_reset:
    .word 0x10000003     /* MIPS32:    branch forward 0x10 bytes from here  */
                         /* MicroMIPS: ADDI32 $0, $0, 0x0007 (nop)          */
                         /* DO NOT change the relative branch               */
    .word 0x00000000     /* NOP */
        .set    micromips
_reset_micromips:
    jalx    _reset_mips32 # change to MIPS32 ISA
    nop

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

