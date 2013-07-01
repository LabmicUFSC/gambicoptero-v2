.file "arm7_crtn.S"

.section .init
ldr pc, =__epos_library_app_entry

.section .fini
.globl _fini
.type _fini,function
bx lr
