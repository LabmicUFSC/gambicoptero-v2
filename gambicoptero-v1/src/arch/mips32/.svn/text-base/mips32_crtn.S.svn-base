/* 4 slots for argument spill area.  1 for cpreturn, 1 for stack.

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
   Return spill offset of 40 and 20.  Aligned to 16 bytes for n32.  */

#ifdef	__mips16
#define RA $7
#else
#define RA $31
#endif

	.section .init,"ax",@progbits
#ifdef __mips64
	ld      RA,40($sp)
	daddu	$sp,$sp,48
#else
/*	lw	RA,20($sp) */
/*	addu	$sp,$sp,32 */
	lw	RA,0($sp)
	addu	$sp,$sp,4
#endif
	j	RA

	.section .fini,"ax",@progbits
#ifdef	__mips64
	ld	RA,40($sp)
	daddu	$sp,$sp,48
#else
/*	lw	RA,20($sp) */
/*	addu	$sp,$sp,32 */
	lw	RA,0($sp)
	addu	$sp,$sp,4
#endif
	j	RA
