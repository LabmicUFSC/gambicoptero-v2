        .file        "crt1.s"

        .text
        .align 4
        .globl        _start
        .type        _start,@function
_start:
        sync
        nop
        bl        _init
        .align        4
        .globl        __epos_library_app_entry
__epos_library_app_entry:
        bl        main
        /* mr        3,3  ; EABI Standard tells the register. */
        bl        _fini
        bl        _exit
