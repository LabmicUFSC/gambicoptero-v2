// EPOS MIPS CPU Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <arch/mips32/cpu.h>

__BEGIN_SYS

void MIPS32::Context::save(Context * volatile * o) volatile
{
    // SAVE CONTEXT
    ASM("	.set noreorder				\n"
	"	.set noat				\n"
	"	addi		$29, $29, -140		\n"
	"	sw		$29, 0($4)		\n"  // $4 = $a0 = o = old  
	"	sw		$1,  0($29)		\n"
	"	sw		$2,  4($29)		\n"
	"	sw		$3,  8($29)		\n"
	"	sw		$4,  12($29)		\n"
	"	sw		$5,  16($29)		\n"
	"	sw		$6,  20($29)		\n"
	"	sw		$7,  24($29)		\n"
	"	sw		$8,  28($29)		\n"
	"	sw		$9,  32($29)		\n"
	"	sw		$10, 36($29)		\n"
	"	sw		$11, 40($29)		\n"
	"	sw		$12, 44($29)		\n"
	"	sw		$13, 48($29)		\n"
	"	sw		$14, 52($29)		\n"
	"	sw		$15, 56($29)		\n"
	"	sw		$16, 60($29)		\n"
	"	sw		$17, 64($29)		\n"
	"	sw		$18, 68($29)		\n"
	"	sw		$19, 72($29)		\n"
	"	sw		$20, 76($29)		\n"
	"	sw		$21, 80($29)		\n"
	"	sw		$22, 84($29)		\n"
	"	sw		$23, 88($29)		\n"
	"	sw		$24, 92($29)		\n"
	"	sw		$25, 96($29)		\n"
	"	sw		$26, 100($29)		\n"
	"	sw		$27, 104($29)		\n"
	"	sw		$28, 108($29)		\n"
	"	sw		$29, 112($29)		\n"
	"	sw		$30, 116($29)		\n"
	"	sw		$31, 120($29)		\n"
        //Save Hi and Lo
	"	mfhi		$26			\n"
	"	mflo		$27			\n"
	"	sw		$26, 124($29)   	\n"
	"	sw		$27, 128($29)		\n"
        //Save Status and EPC
	"	mfc0		$26, $12		\n"
	"	mfc0		$27, $14		\n"
	"	sw		$26, 132($29)		\n"
	"	sw		$27, 136($29)		\n"
	"	nop					\n"
	"	.set reorder				\n"
	"	.set at					\n"
	);
}

void MIPS32::Context::load() const volatile
{
    // LOAD CONTEXT
    ASM("	.set noreorder				\n"
	"	.set noat				\n"
	"	add		$29, $4, $0		\n"  // $4 = First Argument -> Context* (this)	
	"	lw		$1,  0($29)		\n"
	"	lw		$2,  4($29)		\n"
	"	lw		$3,  8($29)		\n"
	"	lw		$4,  12($29)		\n"
	"	lw		$5,  16($29)		\n"
	"	lw		$6,  20($29)		\n"
	"	lw		$7,  24($29)		\n"
	"	lw		$8,  28($29)		\n"
	"	lw		$9,  32($29)		\n"
	"	lw		$10, 36($29)		\n"
	"	lw		$11, 40($29)		\n"
	"	lw		$12, 44($29)		\n"
	"	lw		$13, 48($29)		\n"
	"	lw		$14, 52($29)		\n"
	"	lw		$15, 56($29)		\n"
	"	lw		$16, 60($29)		\n"
	"	lw		$17, 64($29)		\n"
	"	lw		$18, 68($29)		\n"
	"	lw		$19, 72($29)		\n"
	"	lw		$20, 76($29)		\n"
	"	lw		$21, 80($29)		\n"
	"	lw		$22, 84($29)		\n"
	"	lw		$23, 88($29)		\n"
	"	lw		$24, 92($29)		\n"
	"	lw		$25, 96($29)		\n"
	"	lw		$28, 108($29)		\n"
	"	lw		$30, 116($29)		\n"
	"	lw		$31, 120($29)		\n"
        //Load Hi and Lo
	"	lw		$26, 124($29)  		\n"
	"	lw		$27, 128($29)   	\n"
	"	mthi		$26			\n"
	"	mflo		$27			\n"
        //Load Status and EPC
	"	lw		$26, 132($29)		\n"
	"	lw		$27, 136($29)		\n"
	"	mtc0		$26, $12		\n"
	"	mtc0		$27, $14		\n"
	//Load K0 and K1
	"	lw		$26, 100($29)		\n"
	"	lw		$27, 104($29)		\n"
	"	addi		$29, $29, 140		\n"
	"	jr		$31			\n"
	"	nop					\n"
	"	.set reorder				\n"
	"	.set at					\n"
	);
}

//void MIPS32::switch_context(Context * volatile * o, Context * volatile n)
//{
   ASMV(".globl  _ZN6System6MIPS3214switch_contextEPVPNS0_7ContextES2_	\n"
	".ent    _ZN6System6MIPS3214switch_contextEPVPNS0_7ContextES2_	\n"
	"_ZN6System6MIPS3214switch_contextEPVPNS0_7ContextES2_:	\n"
	//".frame  $sp,0,$31				\n"
	//".mask   0x00000000,0				\n"
	//".fmask  0x00000000,0				\n"
        // SAVE CONTEXT
	"	.set noreorder				\n"
	"	.set noat				\n"
	"	addi		$29, $29, -140		\n"
	"	sw		$29, 0($4)		\n"  //$4 = $a0 = o = old
	"	sw		$1,  0($29)		\n"
	"	sw		$2,  4($29)		\n"
	"	sw		$3,  8($29)		\n"
	"	sw		$4,  12($29)		\n"
	"	sw		$5,  16($29)		\n"
	"	sw		$6,  20($29)		\n"
	"	sw		$7,  24($29)		\n"
	"	sw		$8,  28($29)		\n"
	"	sw		$9,  32($29)		\n"
	"	sw		$10, 36($29)		\n"
	"	sw		$11, 40($29)		\n"
	"	sw		$12, 44($29)		\n"
	"	sw		$13, 48($29)		\n"
	"	sw		$14, 52($29)		\n"
	"	sw		$15, 56($29)		\n"
	"	sw		$16, 60($29)		\n"
	"	sw		$17, 64($29)		\n"
	"	sw		$18, 68($29)		\n"
	"	sw		$19, 72($29)		\n"
	"	sw		$20, 76($29)		\n"
	"	sw		$21, 80($29)		\n"
	"	sw		$22, 84($29)		\n"
	"	sw		$23, 88($29)		\n"
	"	sw		$24, 92($29)		\n"
	"	sw		$25, 96($29)		\n"
	"	sw		$26, 100($29)		\n"
	"	sw		$27, 104($29)		\n"
	"	sw		$28, 108($29)		\n"
	"	sw		$29, 112($29)		\n"
	"	sw		$30, 116($29)		\n"
	"	sw		$31, 120($29)		\n"
        //Save Hi and Lo
	"	mfhi		$26			\n"
	"	mflo		$27			\n"
	"	sw		$26, 124($29)   	\n"
	"	sw		$27, 128($29)		\n"
        //Save Status and EPC
	"	mfc0		$26, $12		\n"
	"	mfc0		$27, $14		\n"
	"	sw		$26, 132($29)		\n"
	"	sw		$27, 136($29)		\n"
        // LOAD NEW CONTEXT
	"	add		$29, $5, $0    		\n"  // $5 = a1 = n = new
	"	lw		$1,  0($29)		\n"
	"	lw		$2,  4($29)		\n"
	"	lw		$3,  8($29)		\n"
	"	lw		$4,  12($29)		\n"
	"	lw		$5,  16($29)		\n"
	"	lw		$6,  20($29)		\n"
	"	lw		$7,  24($29)		\n"
	"	lw		$8,  28($29)		\n"
	"	lw		$9,  32($29)		\n"
	"	lw		$10, 36($29)		\n"
	"	lw		$11, 40($29)		\n"
	"	lw		$12, 44($29)		\n"
	"	lw		$13, 48($29)		\n"
	"	lw		$14, 52($29)		\n"
	"	lw		$15, 56($29)		\n"
	"	lw		$16, 60($29)		\n"
	"	lw		$17, 64($29)		\n"
	"	lw		$18, 68($29)		\n"
	"	lw		$19, 72($29)		\n"
	"	lw		$20, 76($29)		\n"
	"	lw		$21, 80($29)		\n"
	"	lw		$22, 84($29)		\n"
	"	lw		$23, 88($29)		\n"
	"	lw		$24, 92($29)		\n"
	"	lw		$25, 96($29)		\n"
	"	lw		$28, 108($29)		\n"
	"	lw		$30, 116($29)		\n"
	"	lw		$31, 120($29)		\n"
        //Load Hi and Lo
	"	lw		$26, 124($29)  		\n"
	"	lw		$27, 128($29)   	\n"
	"	mthi		$26			\n"
	"	mflo		$27			\n"
        //Load Status and EPC
	"	lw		$26, 132($29)		\n"
	"	lw		$27, 136($29)		\n"
	"	mtc0		$26, $12		\n"
	"	mtc0		$27, $14		\n"
	//Load K0 and K1
	"	lw		$26, 100($29)		\n"
	"	lw		$27, 104($29)		\n"
	"	jr		$31			\n"
	"	addi		$29, $29, 140		\n"
//	"	nop					\n"
	"	.set reorder				\n"
	"	.set at					\n"
	".end    _ZN6System6MIPS3214switch_contextEPVPNS0_7ContextES2_	\n"
	".size   _ZN6System6MIPS3214switch_contextEPVPNS0_7ContextES2_, .-_ZN6System6MIPS3214switch_contextEPVPNS0_7ContextES2_\n"
	);
//}

__END_SYS
