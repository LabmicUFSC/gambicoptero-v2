/**********************************************************/

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
.weak __epos_library_app_entry
        .type __epos_library_app_entry,@function
__epos_library_app_entry:
/**********************************************************/

#include "avr8/macros.inc"

	.macro	vector name
	.if (. - __vectors < _VECTORS_SIZE)
	.weak	\name
	.set	\name, __epos_dynamic_handler 
	call	\name
	//XJMP	\name
	.endif
	.endm

	.section .vectors,"ax",@progbits
	.global	__vectors
	.func	__vectors
__vectors:
	XJMP	__init
	vector	__vector_1
	vector	__vector_2
	vector	__vector_3
	vector	__vector_4
	vector	__vector_5
	vector	__vector_6
	vector	__vector_7
	vector	__vector_8
	vector	__vector_9
	vector	__vector_10
	vector	__vector_11
	vector	__vector_12
	vector	__vector_13
	vector	__vector_14
	vector	__vector_15
	vector	__vector_16
	vector	__vector_17
	vector	__vector_18
	vector	__vector_19
	vector	__vector_20
	vector	__vector_21
	vector	__vector_22
	vector	__vector_23
	vector	__vector_24
	vector	__vector_25
	vector	__vector_26
	vector	__vector_27
	vector	__vector_28
	vector	__vector_29
	vector	__vector_30
	vector	__vector_31
	vector	__vector_32
	vector	__vector_33
	vector	__vector_34
	vector	__vector_35
	vector	__vector_36
	vector	__vector_37
	vector	__vector_38
	vector	__vector_39
	vector	__vector_40
	vector	__vector_41
	vector	__vector_42
	vector	__vector_43
	vector	__vector_44
	vector	__vector_45
	vector	__vector_46
	vector	__vector_47
	vector	__vector_48
	vector	__vector_49
	vector	__vector_50
	vector	__vector_51
	vector	__vector_52
	vector	__vector_53
	vector	__vector_54
	vector	__vector_55
	vector	__vector_56
	.endfunc

	/* Handle dynamic interrupts (registered in the Machine) */

	.text
	.global	__epos_dynamic_handler
	.func	__epos_dynamic_handler
__epos_dynamic_handler:
        /* Save context */
        /* Zero (R1), Temp (R0) and SREG(0x3f) */
	push        r1
	push        r0
	in          r0, 0x3f
	push        r0
	/* Offset */	
	push        r24
	/* Call-saved registers */
	push        r28
	push        r29
	/* Get offset */
	in          r29, 0x3e
	in          r28, 0x3d
	ldd         r24, Y+8
	/* Fill empty stack spaces */
	ldd         r1, Y+2
	std         Y+8, r1
	ldd         r1, Y+1
	std         Y+7, r1
	adiw        r28, 2
	out         0x3e, r29
	out         0x3d, r28
	/* Zero = 0 */
	clr         r1
	/* Call-used registers */
	push        r18
	push        r19
	push        r20
	push        r21
	push        r22
	push        r23
	/* r24 (offset) has already been pushed */
	push        r25
	push        r26
	push        r27
	push        r30
	push	    r31
	/* offset = (offset >> 1) - 1 ;  */
	lsr	    r24
	subi	    r24,1
	/* Call interrupt handler */
	call	    __epos_call_handler
	/* Restore context */
	/* Call-used registers */
	pop         r31
	pop         r30
	pop         r27
	pop         r26
	pop         r25
	/* r24 (offset) will be poped latter */
	pop         r23
	pop         r22
	pop         r21
	pop         r20
	pop         r19
	pop         r18
	/* Offset (r24) */
	pop         r24
	/* Zero (R1), Temp (R0) and SREG(0x3f) */
	pop         r0
	out         0x3f, r0
	pop         r0
	pop         r1
	/* Call-saved registers */
	pop         r29
	pop         r28
	/* Return */
	reti	
	.endfunc

	.section .init0,"ax",@progbits
	.weak	__init
;	.func	__init
__init:

#ifndef __AVR_ASM_ONLY__
	.weak	__stack
	.set	__stack, RAMEND

	/* By default, malloc() uses the current value of the stack pointer
	   minus __malloc_margin as the highest available address.

	   In some applications with external SRAM, the stack can be below
	   the data section (in the internal SRAM - faster), and __heap_end
	   should be set to the highest address available for malloc().  */
	.weak	__heap_end
	.set	__heap_end, 0

	.section .init2,"ax",@progbits
	clr	__zero_reg__
	out	_SFR_IO_ADDR(SREG), __zero_reg__
	ldi	r28,lo8(__stack)
#ifdef SPH
	ldi	r29,hi8(__stack)
	out	_SFR_IO_ADDR(SPH), r29
#endif
	out	_SFR_IO_ADDR(SPL), r28

#if BIG_CODE
	/* Only for >64K devices with RAMPZ, replaces the default code
	   provided by libgcc.S which is only linked in if necessary.  */

	.section .init4,"ax",@progbits
	.global __do_copy_data
__do_copy_data:
	ldi	r17, hi8(__data_end)
	ldi	r26, lo8(__data_start)
	ldi	r27, hi8(__data_start)
	ldi	r30, lo8(__data_load_start)
	ldi	r31, hi8(__data_load_start)

	/* On the enhanced core, "elpm" with post-increment updates RAMPZ
	   automatically.  Otherwise we have to handle it ourselves.  */

#ifdef __AVR_ENHANCED__
	ldi	r16, hh8(__data_load_start)
#else
	ldi	r16, hh8(__data_load_start - 0x10000)
.__do_copy_data_carry:
	inc	r16
#endif
	out	_SFR_IO_ADDR(RAMPZ), r16
	rjmp	.__do_copy_data_start
.__do_copy_data_loop:
#ifdef __AVR_ENHANCED__
	elpm	r0, Z+
#else
	elpm
#endif
	st	X+, r0
#ifndef __AVR_ENHANCED__
	adiw	r30, 1
	brcs	.__do_copy_data_carry
#endif
.__do_copy_data_start:
	cpi	r26, lo8(__data_end)
	cpc	r27, r17
	brne	.__do_copy_data_loop
#endif /* BIG_CODE */

#endif /* !__AVR_ASM_ONLY__ */

	.section .init9,"ax",@progbits
	XJMP	main
;	.endfunc

