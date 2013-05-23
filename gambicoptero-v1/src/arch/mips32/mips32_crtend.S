	.file	1 "crtstuff.c"

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
	# .section .mdebug.abi32
	# .previous
	.text
	.align	2
__do_global_ctors_aux:
	subu	$sp,$sp,32
	lui	$2,%hi(__CTOR_END__) # high
	sw	$16,16($sp)
	addiu	$2,$2,%lo(__CTOR_END__) # low
	sw	$31,24($sp)
	sw	$17,20($sp)
	lw	$4,-4($2)
	li	$3,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$4,$3,$L8
	addu	$16,$2,-4
	.set	macro
	.set	reorder

	move	$3,$4
	li	$17,-1			# 0xffffffffffffffff
$L6:
	.set	noreorder
	.set	nomacro
	jal	$31,$3
	addu	$16,$16,-4
	.set	macro
	.set	reorder

	lw	$2,0($16)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$17,$L6
	move	$3,$2
	.set	macro
	.set	reorder

$L8:
	lw	$31,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	j	$31
	addu	$sp,$sp,32
	.set	macro
	.set	reorder

	.align	2
init_dummy:
	subu	$sp,$sp,24
	sw	$31,16($sp)
 #APP
	.section	.init
 #NO_APP
	jal	__do_global_ctors_aux
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

	.type __CTOR_END__, @object

#	.lcomm	__CTOR_END__,4
__CTOR_END__:
	.word 0

 #APP
	.section	.dtors,"aw",@progbits
 #NO_APP

	.type __DTOR_END__, @object

#	.lcomm	__DTOR_END__,4
__DTOR_END__:
	.word 0
