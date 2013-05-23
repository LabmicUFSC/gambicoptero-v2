#	.file	1 "crtstuff.c"

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
#	.section .mdebug.abi32
	.section .text
	.previous
	.text
	.align	2
__do_global_dtors_aux:
	subu	$sp,$sp,24
	sw	$16,16($sp)
	lui	$2,%hi(__DTOR_LIST__+4) # high
	sw	$31,20($sp)
	lw	$3,%lo(__DTOR_LIST__+4)($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L8
	addiu	$16,$2,%lo(__DTOR_LIST__+4) # low
	.set	macro
	.set	reorder

$L6:
	.set	noreorder
	.set	nomacro
	jal	$31,$3
	addu	$16,$16,4
	.set	macro
	.set	reorder

	lw	$2,0($16)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L6
	move	$3,$2
	.set	macro
	.set	reorder

$L8:
	lw	$31,20($sp)
	lw	$16,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,24
	.set	macro
	.set	reorder

	.align	2
fini_dummy:
	subu	$sp,$sp,24
	sw	$31,16($sp)
 #APP
	.section	.fini
 #NO_APP
	jal	__do_global_dtors_aux
 #APP
	.text
 #NO_APP
	lw	$31,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,24
	.set	macro
	.set	reorder

	.lcomm	force_to_data,0
 #APP
	.section	.ctors,"aw",@progbits
 #NO_APP
	.align	2
	.type	__CTOR_LIST__, @object
__CTOR_LIST__:
	.word	-1

 #APP
	.section	.dtors,"aw",@progbits
 #NO_APP
	.align	2
	.type	__DTOR_LIST__, @object
__DTOR_LIST__:
	.word	-1
