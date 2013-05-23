        .file        "crtend.c"

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
        .section        .ctors,"aw",@progbits
        .align 2
        .type        __CTOR_END__, @object
__CTOR_END__:
        .long        0
        .section        .dtors,"aw",@progbits
        .align 2
        .type        __DTOR_END__, @object
__DTOR_END__:
        .long        0
        .section        .eh_frame,"a",@progbits
        .align 2
        .type        __FRAME_END__, @object
__FRAME_END__:
        .long        0
        .section        .jcr,"aw",@progbits
        .align 2
        .type        __JCR_END__, @object
__JCR_END__:
        .long        0
        .section        ".got2","aw"
.LCTOC1 = .+32768
        .section        ".text"
        .section        ".got2","aw"
.LC0:
        .long __CTOR_END__
        .section        ".text"
        .align 2
.LCL0:
        .long .LCTOC1-.LCF0
        .type        __do_global_ctors_aux, @function
__do_global_ctors_aux:
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
        lwz 0,-4(9)
        addi 31,9,-4
        b .L8
.L10:
        mtlr 0
        blrl
        lwzu 0,-4(31)
.L8:
        cmpwi 7,0,-1
        bne+ 7,.L10
        lwz 0,36(1)
        lwz 30,24(1)
        lwz 31,28(1)
        mtlr 0
        addi 1,1,32
        blr
        .align 2
        .type        call___do_global_ctors_aux, @function
call___do_global_ctors_aux:
        mflr 0
        stwu 1,-32(1)
        stw 0,36(1)
                .section        ".init","ax"
        bl __do_global_ctors_aux@local
                .section        ".text"
        lwz 0,36(1)
        addi 1,1,32
        mtlr 0
        blr
        .ident        "GCC: (GNU) 3.4.1"
