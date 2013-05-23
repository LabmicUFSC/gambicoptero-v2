// EPOS IA32 CPU Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <arch/ia32/cpu.h>

__BEGIN_SYS

// Class attributes
unsigned int IA32::_cpu_clock;
unsigned int IA32::_bus_clock;

// Class methods
void IA32::Context::save() volatile
{
    ASM("	pushl	%ebp						\n"
 	"	movl	%esp, %ebp					\n"
	"	movl    8(%ebp), %esp	# sp = this	 		\n"
	"	addl    $40, %esp      	# sp += sizeof(Context)		\n"
	"	pushl	4(%ebp)		# push eip			\n"
	"	pushfl							\n"
	"	pushl	%eax						\n"
	"	pushl	%ecx						\n"
	"	pushl	%edx						\n"
	"	pushl	%ebx						\n"
	"	pushl   %ebp		# push esp			\n"
	"	pushl   (%ebp)		# push ebp			\n"
	"	pushl	%esi						\n"
	"	pushl	%edi						\n"
	"	movl    %ebp, %esp					\n"
	"	popl    %ebp						\n");
}

void IA32::Context::load() const volatile
{
    // POPA ignores the ESP saved by PUSHA. ESP is just normally incremented. 
    ASM("	movl    4(%esp), %esp	# sp = this			\n"
	"	popal							\n"
 	"	popfl							\n");
}

void IA32::switch_context(Context * volatile * o, Context * volatile n)
{
    // PUSHA saves an extra "esp" (which is always "this"),
    // but saves several instruction fetchs.
    ASM("	pushfl				\n"
	"	pushal				\n"
	"	movl    40(%esp), %eax	# old	\n" 
	"	movl    %esp, (%eax)		\n"
	"	movl    44(%esp), %esp	# new	\n"
	"	popal				\n"
	"	popfl				\n");
}

__END_SYS
