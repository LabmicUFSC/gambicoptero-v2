// EPOS PPC32 CPU Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <arch/ppc32/cpu.h>

__BEGIN_SYS

void PPC32::Context::save() volatile
{
  ASMV("mr              1, 3            \n" //R1 <- this pointer
       "stmw            0, 0(1)         \n" //Save r0 - r31
       "mfmsr           0               \n" //Get Machine State Register
       "mflr            2               \n" //Get Link Register
       "mfctr           3               \n" //Get Counter Register
       "mfcr            4               \n" //Get Condition Register
       "mfxer           5               \n" //Get FixedPointException Register
       "mfsrr0          6               \n" //Get SRR0
       "mfsrr1          7               \n" //Get SRR1
       "mfsrr2          8               \n" //Get SRR2
       "mfsrr3          9               \n" //Get SRR3
       "stw             0, 128(1)       \n" //Save Machine State Register
       "stw             2, 132(1)       \n" //Save Link Register
       "stw             3, 136(1)       \n" //Save Counter Register
       "stw             4, 140(1)       \n" //Save Condition Register
       "stw             5, 144(1)       \n" //Save FixedPointException Register
       "stw             6, 148(1)       \n" //Save Save/Restore Register 0
       "stw             7, 152(1)       \n" //Save Save/Restore Register 1
       "stw             8, 156(1)       \n" //Save Save/Restore Register 2
       "stw             9, 160(1)       \n" //Save Save/Restore Register 3
      );
}

void PPC32::Context::load() const volatile
{
  ASMV("mr              1, 3            \n" //R1 <- this pointer
       "lwz             0, 128(1)       \n" //Load Machine State Register
       "lwz             2, 132(1)       \n" //Load Link Register
       "lwz             3, 136(1)       \n" //Load Counter Register
       "lwz             4, 140(1)       \n" //Load Condition Register
       "lwz             5, 144(1)       \n" //Load FixedPointException Register
       "lwz             6, 148(1)       \n" //Load Save/Restore Register 0
       "lwz             7, 152(1)       \n" //Load Save/Restore Register 1
       "lwz             8, 156(1)       \n" //Load Save/Restore Register 2
       "lwz             9, 160(1)       \n" //Load Save/Restore Register 3
       "mtmsr           0               \n" //Store Machine State Register
       "mtlr            2               \n" //Store Link Register
       "mtctr           3               \n" //Store Counter Register
       "mtcr            4               \n" //Store Condition Register
       "mtxer           5               \n" //Store FixedPointException Register
       "mtsrr0          6               \n" //Store SRR0
       "mtsrr1          7               \n" //Store SRR1
       "mtsrr2          8               \n" //Store SRR2
       "mtsrr3          9               \n" //Store SRR3
       "lwz             0, 0(1)         \n" //Restore r0
       "lmw             2, 8(1)         \n" //Restore r2 - r31
       "addi            1, 1, 164       \n" //Restore StackPointer
      );
}

void PPC32::switch_context(Context * volatile * o, Context * volatile n)
{
  //Dirty thing to do, this rollback function prologue. Other option is
  //find a GCC attribute like AVR "naked" or declare function in ASM ...
  ASMV("addi    1,1,16; sync;");

  ASMV("addi            1, 1, -164      \n" //Alloc CONTEXT on Stack
       //Now we must do the context switch trick, we must save the actual
       //stack pointer as this pointer of first argument on the function
       //located at r3 register (already Saved on current context stack)
       "stw             1, 0(3)         \n" //R3 = context * volatile * o (new stack)
       "stmw            0, 0(1)         \n" //Save r0 - r31
       "mfmsr           0               \n" //Get Machine State Register
       "mflr            2               \n" //Get Link Register
       "mfctr           3               \n" //Get Counter Register
       "mfcr            4               \n" //Get Condition Register
       "mfxer           5               \n" //Get FixedPointException Register
       "mfsrr0          6               \n" //Get SRR0
       "mfsrr1          7               \n" //Get SRR1
       "mfsrr2          8               \n" //Get SRR2
       "mfsrr3          9               \n" //Get SRR3
       "stw             0, 128(1)       \n" //Save Machine State Register
       "stw             2, 132(1)       \n" //Save Link Register
       "stw             3, 136(1)       \n" //Save Counter Register
       "stw             4, 140(1)       \n" //Save Condition Register
       "stw             5, 144(1)       \n" //Save FixedPointException Register
       "stw             6, 148(1)       \n" //Save Save/Restore Register 0
       "stw             7, 152(1)       \n" //Save Save/Restore Register 1
       "stw             8, 156(1)       \n" //Save Save/Restore Register 2
       "stw             9, 160(1)       \n" //Save Save/Restore Register 3
       // Now we really switch context, the second argument, saved on stack at R4
       // contains the this pointer Context located at new Thread Stack.
       "lwz             1, 16(1)        \n" //sp <- new_context sp.
       "lwz             0, 128(1)       \n" //Load Machine State Register
       "lwz             2, 132(1)       \n" //Load Link Register
       "lwz             3, 136(1)       \n" //Load Counter Register
       "lwz             4, 140(1)       \n" //Load Condition Register
       "lwz             5, 144(1)       \n" //Load FixedPointException Register
       "lwz             6, 148(1)       \n" //Load Save/Restore Register 0
       "lwz             7, 152(1)       \n" //Load Save/Restore Register 1
       "lwz             8, 156(1)       \n" //Load Save/Restore Register 2
       "lwz             9, 160(1)       \n" //Load Save/Restore Register 3
       "mtmsr           0               \n" //Store Machine State Register
       "mtlr            2               \n" //Store Link Register
       "mtctr           3               \n" //Store Counter Register
       "mtcr            4               \n" //Store Condition Register
       "mtxer           5               \n" //Store FixedPointException Register
       "mtsrr0          6               \n" //Store SRR0
       "mtsrr1          7               \n" //Store SRR1
       "mtsrr2          8               \n" //Store SRR2
       "mtsrr3          9               \n" //Store SRR3
       "lwz             0, 0(1)         \n" //Restore r0
       "lmw             2, 8(1)         \n" //Restore r2 - r31
       "addi            1, 1, 164       \n" //Restore StackPointer
       "blr                             \n" //Return after function epilogue
      );
}

__END_SYS
