        .file        "crtbegin.c"

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
        .section        .ctors,"aw",@progbits
        .align 2
        .type        __CTOR_LIST__, @object
__CTOR_LIST__:
        .long        -1
        .section        .dtors,"aw",@progbits
        .align 2
        .type        __DTOR_LIST__, @object
__DTOR_LIST__:
        .long        -1
        .section        .eh_frame,"a",@progbits
        .align 2
        .type        __EH_FRAME_BEGIN__, @object
__EH_FRAME_BEGIN__:
        .section        .jcr,"aw",@progbits
        .align 2
        .type        __JCR_LIST__, @object
__JCR_LIST__:
        .hidden        __dso_handle
        .globl __dso_handle
        .section        ".data"
        .align 2
        .type        __dso_handle, @object
__dso_handle:
        .long        0
        .section        .data.rel.local,"aw",@progbits
        .align 2
        .type        p.0, @object
p.0:
.LCP0:
        .long        (__DTOR_LIST__+4)@fixup
        .section        ".fixup","aw"
        .align 2
        .long        .LCP0
        .previous
        .lcomm        completed.1,1,1
        .type        completed.1, @object
        .section        ".got2","aw"
.LCTOC1 = .+32768
        .section        ".text"
        .section        ".got2","aw"
.LC0:
        .long completed.1
.LC1:
        .long p.0
.LC2:
        .long __deregister_frame_info
.LC3:
        .long __EH_FRAME_BEGIN__
        .section        ".text"
        .align 2
.LCL0:
        .long .LCTOC1-.LCF0
        .type        __do_global_dtors_aux, @function
__do_global_dtors_aux:
        stwu 1,-32(1)
        mflr 0
        bcl 20,31,.LCF0
.LCF0:
        stw 30,24(1)
        mflr 30
        stw 31,28(1)
        stw 0,36(1)
        lwz 0,.LCL0-.LCF0(30)
        add 30,0,30
        lwz 9,.LC0-.LCTOC1(30)
        lbz 0,0(9)
        cmpwi 7,0,0
        bne- 7,.L1
        lwz 31,.LC1-.LCTOC1(30)
        b .L9
.L11:
        addi 0,9,4
        mtlr 11
        stw 0,0(31)
        blrl
.L9:
        lwz 9,0(31)
        lwz 11,0(9)
        cmpwi 7,11,0
        bne+ 7,.L11
        lwz 0,.LC2-.LCTOC1(30)
        cmpwi 7,0,0
        beq- 7,.L6
        lwz 3,.LC3-.LCTOC1(30)
        bl __deregister_frame_info@plt
.L6:
        lwz 9,.LC0-.LCTOC1(30)
        li 0,1
        stb 0,0(9)
.L1:
        lwz 0,36(1)
        lwz 30,24(1)
        lwz 31,28(1)
        mtlr 0
        addi 1,1,32
        blr
        .align 2
        .type        call___do_global_dtors_aux, @function
call___do_global_dtors_aux:
        mflr 0
        stwu 1,-32(1)
        stw 0,36(1)
                .section        ".fini","ax"
        bl __do_global_dtors_aux@local
                .section        ".text"
        lwz 0,36(1)
        addi 1,1,32
        mtlr 0
        blr
        .lcomm        object.2,24,4
        .type        object.2, @object
        .section        ".got2","aw"
.LC4:
        .long __register_frame_info
        .set .LC5,.LC3
.LC6:
        .long object.2
.LC7:
        .long __JCR_LIST__
.LC8:
        .long _Jv_RegisterClasses
        .section        ".text"
        .align 2
.LCL2:
        .long .LCTOC1-.LCF2
        .type        frame_dummy, @function
frame_dummy:
        mflr 0
        stwu 1,-32(1)
        bcl 20,31,.LCF2
.LCF2:
        stw 30,24(1)
        mflr 30
        stw 0,36(1)
        lwz 0,.LCL2-.LCF2(30)
        add 30,0,30
        lwz 0,.LC4-.LCTOC1(30)
        cmpwi 7,0,0
        beq- 7,.L14
        lwz 3,.LC5-.LCTOC1(30)
        lwz 4,.LC6-.LCTOC1(30)
        bl __register_frame_info@plt
.L14:
        lwz 3,.LC7-.LCTOC1(30)
        lwz 0,0(3)
        cmpwi 7,0,0
        beq- 7,.L13
        lwz 0,.LC8-.LCTOC1(30)
        cmpwi 7,0,0
        beq- 7,.L13
        bl _Jv_RegisterClasses@plt
.L13:
        lwz 0,36(1)
        lwz 30,24(1)
        addi 1,1,32
        mtlr 0
        blr
        .align 2
        .type        call_frame_dummy, @function
call_frame_dummy:
        mflr 0
        stwu 1,-32(1)
        stw 0,36(1)
                .section        ".init","ax"
        bl frame_dummy@local
                .section        ".text"
        lwz 0,36(1)
        addi 1,1,32
        mtlr 0
        blr
        .weak        _Jv_RegisterClasses
        .weak        __deregister_frame_info
        .weak        __register_frame_info
        .ident        "GCC: (GNU) 3.4.1"
