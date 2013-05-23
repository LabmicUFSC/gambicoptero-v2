/* 4 slots for argument spill area.  1 for cpreturn, 1 for stack.

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
   Return spill offset of 40 and 20.  Aligned to 16 bytes for n32.  */

	.section .init,"ax",@progbits
/*	.weak   __epos_library_app_entry
	.type   __epos_library_app_entry,@function */
	.globl	_init
	.type	_init,@function
/* __epos_library_app_entry: */
_init:
#ifdef __mips64
	daddu   $sp,$sp,-48
	sd      $31,40($sp)
#else
/*	addu	$sp,$sp,-32 */
/*	sw	$31,20($sp) */
	addu	$sp,$sp,-4
	sw	$31,0($sp)
#endif

	.section .fini,"ax",@progbits
	.globl	_fini
	.type	_fini,@function
_fini:
#ifdef __mips64
	daddu   $sp,$sp,-48
	sd      $31,40($sp)
#else
/*	addu	$sp,$sp,-32 */
/*	sw	$31,20($sp) */
	addu	$sp,$sp,-4
	sw	$31,0($sp)
#endif

	.text
	.weak   main
	.type   main,@function

	.weak   __epos_library_app_entry
	.type   __epos_library_app_entry,@function

__epos_library_app_entry:
main:
	jr $31
