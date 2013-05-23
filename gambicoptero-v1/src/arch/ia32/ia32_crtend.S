	.file	"crtstuff.c"

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

	.section	.ctors,"aw",@progbits
	.align 4
	.type	__CTOR_END__, @object
__CTOR_END__:
	.long	0

	.section	.dtors,"aw",@progbits
	.align 4
	.type	__DTOR_END__, @object
__DTOR_END__:
	.long	0

	.section	.eh_frame,"a",@progbits
	.align 4
	.type	__FRAME_END__, @object
__FRAME_END__:
	.long	0

	.section	.jcr,"aw",@progbits
	.align 4
	.type	__JCR_END__, @object
__JCR_END__:
	.long	0

	.text
	.p2align 4,,15
	.globl	__do_global_ctors_aux
	.type	__do_global_ctors_aux, @function
__do_global_ctors_aux:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$4, %esp
	movl	$__CTOR_END__-4, %ebx
	movl	__CTOR_END__-4, %eax
	jmp	.L8
	.p2align 4,,7
.L10:
	subl	$4, %ebx
	call	*%eax
	movl	(%ebx), %eax
.L8:
	cmpl	$-1, %eax
	jne	.L10
	popl	%eax
	popl	%ebx
	popl	%ebp
	ret

	.section	.init
	call __do_global_ctors_aux

	.text
	.section	.note.GNU-stack,"",@progbits
