.file "arm7_crt1.S"

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

.section .text
.align 4
.global _start
_start:
    bl _init

.align  4
.global __epos_library_app_entry
__epos_library_app_entry:
    b  main     /* Do not link! LR was loaded by Context::load() */
    bl  _fini
    bl  _exit
