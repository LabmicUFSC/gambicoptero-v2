.file "arm7_crti.S"

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

.section .init
.align 2

        /* INTERRUPT VECTORS */
        ldr pc, _start_addr
        ldr pc, _undefined_instruction_addr
        ldr pc, _software_interrupt_addr
        ldr pc, _prefetch_abort_addr
        ldr pc, _data_abort_addr
        ldr pc, _reserved_addr
        ldr pc, _irq_handler_addr
        ldr pc, _fiq_handler_addr

        /* These vectors and code are used for rom patching.*/
        .org 0x20
        .code 16
        _RPTV_0_START:
        bx lr

        .org 0x60
        _RPTV_1_START:
        bx lr

        .org 0xa0
        _RPTV_2_START:
        bx lr

        .org 0xe0
        _RPTV_3_START:
        bx lr

        .org 0x120
        ROM_var_start: .word 0
        .org 0x7ff
        ROM_var_end:   .word 0
        .code 32

_start_addr:                .word _start
_undefined_instruction_addr:.word _undefined_instruction
_software_interrupt_addr:   .word _prefetch_abort
_prefetch_abort_addr:       .word _undefined_instruction
_data_abort_addr:           .word _data_abort
_reserved_addr:             .word _reserved
_irq_handler_addr:          .word _irq_handler
_fiq_handler_addr:          .word _fiq_handler

_stack_addr: .word 0x00418000 /* should be a symbol */

.equ ARM_MODE_USR,		0x10
.equ ARM_MODE_FIQ,		0x11
.equ ARM_MODE_IRQ,		0x12
.equ ARM_MODE_SVC,		0x13

.equ IRQ_BIT, 			0x80;
.equ FIQ_BIT,			0x40;

.global _init
.type _init,function
_init:

	msr cpsr_c, #ARM_MODE_SVC | IRQ_BIT | FIQ_BIT
	ldr sp, _stack_addr

/* clear bss */
    mov r0, #0
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__
  
    bss_clean_loop:
        cmp r1, r2
        strlo r0, [r1], #4
        blo bss_clean_loop

/* this sould not be here */

.ifdef mc13224v
    /* initialize rom data */
    .set _rom_data_init, 0x108d0
    ldr r12, =_rom_data_init
    mov lr, pc
    mov pc, r12
.endif

    bl __do_global_ctors_aux

.section .fini
.global _fini
_fini:
    b _fini
