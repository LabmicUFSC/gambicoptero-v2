.file   "arm7_crtend.S"

.section    .ctors
__CTOR_END__:
    .long   0

.section    .dtors
__DTOR_END__:
    .long   0

.section    .eh_frame
__FRAME_END__:
    .long   0

.section    .jcr
__JCR_END__:
    .long   0

.text
.globl  __do_global_ctors_aux
.type   __do_global_ctors_aux, function

__do_global_ctors_aux:
    stmfd sp!, {r4,r5,lr}
    mov r4, #-1
    ldr r5, =__CTOR_END__ - 4
  
    ctors_loop:
        ldr r2, [r5], #-4
        cmp r2, r4
        beq ctors_loop_end
        mov lr, pc
        bx r2
        b ctors_loop
    
    ctors_loop_end:
    ldmfd sp!, {r4,r5,pc}
