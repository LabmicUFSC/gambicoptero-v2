.file   "arm7_crtbegin.S"

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
    
.section    .ctors
__CTOR_LIST__:
    .long   -1
    
.section    .dtors 
__DTOR_LIST__:
    .long   -1
    
.section    .jcr
__JCR_LIST__:
    .hidden __dso_handle
    .globl  __dso_handle

    .data
    .align  4
    .type   __dso_handle,object
__dso_handle:
    .long   0
    .align  4
    .type   p.0,object
p.0:
    .long   __DTOR_LIST__+4
    .local  completed.1
    .comm   completed.1,1,1

.text
.type   __do_global_dtors_aux, function

__do_global_dtors_aux:
    ldr r0, = __DTOR_LIST__ + 4
    mov r1, #0

    dtors_loop:
        cmp r0, r1
        beq dtors_loop_end
        ldr r2, [r0]
        stmfd sp!, {r0-r1}
        mov lr, pc
        bx r2
        ldmfd sp!, {r0-r1}
        sub r0, r0, #-4
        b dtors_loop

    dtors_loop_end:
    bx lr

.section    .fini
    bl __do_global_dtors_aux
