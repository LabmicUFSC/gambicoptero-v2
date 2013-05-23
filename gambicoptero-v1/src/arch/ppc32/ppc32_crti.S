# 1 "ppc32_crti.S"

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
# 1 "<built-in>"
# 1 "<command line>"
# 1 "ppc32_crti.S"
# crti.s for sysv4

# Copyright (C) 1996 Free Software Foundation, Inc.
# Written By Michael Meissner

# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2, or (at your option) any
# later version.

# In addition to the permissions in the GNU General Public License, the
# Free Software Foundation gives you unlimited permission to link the
# compiled version of this file with other programs, and to distribute
# those programs without any restriction coming from the use of this
# file. (The General Public License restrictions do apply in other
# respects; for example, they cover modification of the file, and
# distribution when not linked into another program.)

# This file is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING. If not, write to
# the Free Software Foundation, 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.

# As a special exception, if you link this library with files
# compiled with GCC to produce an executable, this does not cause
# the resulting executable to be covered by the GNU General Public License.
# This exception does not however invalidate any other reasons why
# the executable file might be covered by the GNU General Public License.


# This file just supplies labeled starting points for the .got* and other
# special sections. It is linked in first before other modules.

        .file "crti.s"


# Head of _init function used for static constructors

        .section .init
        .weak __epos_library_app_entry
        .type __epos_library_app_entry,@function
__epos_library_app_entry:

        .globl _init
        .type _init,@function
_init:
        stwu %r1,-16(%r1)
        mflr %r0
        stw %r31,12(%r1)
        stw %r0,16(%r1)

        bl _GLOBAL_OFFSET_TABLE_-4 # get the GOT address
        mflr %r31

# Head of _fini function used for static destructors
        .section .fini
        .globl _fini
        .type _fini,@function
_fini:
        stwu %r1,-16(%r1)
        mflr %r0
        stw %r31,12(%r1)
        stw %r0,16(%r1)

        bl _GLOBAL_OFFSET_TABLE_-4 # get the GOT address
        mflr %r31

# this code, I don't know why.'
# _environ and its evil twin environ, pointing to the environment
# .section ".sdata","aw"
# .align 2
# .globl _environ
# .space 4
# .weak environ
# .set environ,_environ
