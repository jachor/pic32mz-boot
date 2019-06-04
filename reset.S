        #.text
        
.section .reset_text, "x"
        .align 2
        .set noreorder
        .global _reset
        .ent _reset

############################
# Begin ISA switching code #
############################
        .set nomicromips
_reset:
        .word 0x10000003     /* MIPS32:    branch forward 0x10 bytes from here  */
                             /* MicroMIPS: ADDI32 $0, $0, 0x0007 (nop)          */
                             /* DO NOT change the relative branch               */
        .word 0x00000000     /* NOP */
        .set    micromips
_reset_micromips:
        jalx     _reset_mips32 # change to MIPS32 ISA
        nop

        .set nomicromips
_reset_mips32:

    # Turn on LED
    /*
	lui $v0, 0xbf86 # KSEG1

	lui $v1, 0 # 0=output
	sw $v1, 0x210($v0) # TRISC
	sw $v1, 0x220($v0) # PORTC
	sw $v1, 0x240($v0) # ODCC
	*/
	
	# setup stack for uncached init
	la     $sp, _kseg1_ram_end
	addiu  $sp, $sp, -0x10

	# Reset CPU to sane state
	jal _setup_environment
	nop
	
	# setup stack for cacheful operation
	la     $sp, _kseg0_ram_end
	addiu  $sp, $sp, -0x10

	la $v0, enter_c_world
	jal $v0
	nop # branch slot

	j _reset
	nop
	
_jump_to_firmware:
    # load $v0 = 0xbd00.0000 (phy program flash start, KSEG1)
	lui     $v0,0xbd00
	addiu   $v0,$v0,0x0000
	jr $v0
	nop		# branch slot
	
	.end _reset

.section .config, "a"
	.global _config
_config:
	.word 0x43000000	# DEVCFG3
	.word 0x00000080	# DEVCFG2
	.word 0x003fc78f	# DEVCFG1
	.word 0x8000877f	# DEVCFG0
	.word 0xffffffff	# DEVCP3
	.word 0xffffffff	# DEVCP2
	.word 0xffffffff	# DEVCP1
	.word 0xffffffff	# DEVCP0
	.word 0xffffffff	# DEVSIGN3
	.word 0xffffffff	# DEVSIGN2
	.word 0xffffffff	# DEVSIGN1
	.word 0xffffffff	# DEVSIGN0

