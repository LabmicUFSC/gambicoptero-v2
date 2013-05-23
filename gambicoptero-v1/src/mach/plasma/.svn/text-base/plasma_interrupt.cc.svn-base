#include <mach/plasma/machine.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <mach/plasma/ic.h>

__BEGIN_SYS

void PLASMA_IC::interrupt_dispatcher() { 
   ASMV(".globl interrupt_service_routine		\n"
	"interrupt_service_routine:					\n"
	".set push									\n"
	".set noreorder								\n"
	".set noat									\n"
	"addi  $29,$29,-96							\n"
	//#save status register an disable interrupts	
	"mfc0  $26, $12		#CP0_status				\n"
    "lui   $27, 0xFFFF							\n"
	"sw    $26,  0($29)							\n"
	//#disable interrupts
	"ori   $27, $27, 0xFFFE						\n"
    "and   $26, $26, $27						\n"
	"mtc0  $26, $12								\n"
	//#save registers
	"sw    $1,   4($29)    #at					\n"
	"sw    $2,   8($29)    #v0					\n"
	"sw    $3,  12($29)    #v1					\n"
	"sw    $4,  16($29)    #a0					\n"
	"sw    $5,  20($29)    #a1					\n"
	"sw    $6,  24($29)    #a2					\n"
	"sw    $7,  28($29)    #a3					\n"
	"sw    $8,  32($29)    #t0					\n"
	"sw    $9,  36($29)    #t1					\n"
	"sw    $10, 40($29)    #t2					\n"
	"sw    $11, 44($29)    #t3					\n"
	"sw    $12, 48($29)    #t4					\n"
	"sw    $13, 52($29)    #t5					\n"
	"sw    $14, 56($29)    #t6					\n"
	"sw    $15, 60($29)    #t7					\n"
	"sw    $24, 64($29)    #t8					\n"
	"sw    $25, 68($29)    #t9					\n"
	"sw    $28, 72($29)    #gp					\n"
	"sw    $30, 76($29)    #fp					\n"
	"sw    $31, 80($29)    #ra					\n"
	//#save multiplier registers
	"mfhi  $26									\n"
	"mflo  $27									\n"
	"sw    $26, 84($29)							\n"
	//#save EPC register	
	"mfc0  $26, $14	       #CP0_EPC				\n"
	"sw    $27, 88($29)							\n"
	"addi  $26, $26, -4    #backup one opcode	\n"
	"sw    $26, 92($29)							\n"
       );
	PLASMA_IC::int_handler(0);
   ASMV("lw    $1,   4($29)    #at				\n"
   	"lw    $2,   8($29)    #v0					\n"
   	"lw    $3,  12($29)    #v1					\n"
   	"lw    $4,  16($29)    #a0					\n"
   	"lw    $5,  20($29)    #a1					\n"	
   	"lw    $6,  24($29)    #a2					\n"
   	"lw    $7,  28($29)    #a3					\n"
   	"lw    $8,  32($29)    #t0					\n"
   	"lw    $9,  36($29)    #t1					\n"
   	"lw    $10, 40($29)    #t2					\n"
   	"lw    $11, 44($29)    #t3					\n"
   	"lw    $12, 48($29)    #t4					\n"
   	"lw    $13, 52($29)    #t5					\n"
   	"lw    $14, 56($29)    #t6					\n"
   	"lw    $15, 60($29)    #t7					\n"
   	"lw    $24, 64($29)    #t8					\n"
	"lw    $25, 68($29)    #t9					\n"
   	"lw    $28, 72($29)    #gp					\n"
   	"lw    $30, 76($29)    #fp					\n"
   	"lw    $31, 80($29)    #ra					\n"
	//#restore multiplier registers
	"lw    $26, 84($29)							\n"
	"lw    $27, 88($29)							\n"
	"mthi  $26									\n"
	"mtlo  $27									\n"
	//#restore CP0 registers
	"lw    $26,  0($29)							\n"
	//"ori   $26, $26, 1      # turn intr bit on	\n"
	"lw    $27, 92($29)     # return address	\n"
	"addi  $29, $29, 96     #adjust stack		\n"
	"jr    $27									\n"
	"mtc0  $26, $12         #Restore Status		\n"
	".set at									\n"
	".set reorder								\n"
	".set pop									\n"
	);
};

__END_SYS
