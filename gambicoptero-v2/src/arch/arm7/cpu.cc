// EPOS-- ARM7 CPU Mediator Implementation

#include <arch/arm7/cpu.h>
#include <system/config.h>
#include <machine.h>

__BEGIN_SYS

ARM7::OP_Mode ARM7::_mode = ARM7::FULL;

void ARM7::Context::save() volatile
{

}

void ARM7::Context::load() const volatile
{
	db<CPU>(TRC) << "CPU::Context::load(this=" << (void*)this << ")\n";

	ASMV("ldr r0, [%0, #64]\n"
         "msr spsr_cfsx, r0\n"
         "ldmfd %0, {r0-r12,sp,lr,pc}^\n"
		 :
		 : "r" (this)
		 : "r0");
}


void ARM7::switch_context(Context * volatile * o, Context * volatile n)
{
    Context * old = *o;
        
    db<CPU>(TRC) << "ARM7::switch_context(o=" << old <<",n="<<*n<<")\n";

    old->_cpsr = CPU::flags();
    
    ASMV("ldr r2, [%0, #64]" : : "r"(n) : "r2");
    ASMV("msr spsr_cfsx, r2");
    ASMV("stmia %0, {r0-r12,sp,lr,pc} \n"              // pc is always read with a +8 offset
         "ldmfd %1, {r0-r12,sp,lr,pc}^"
          : : "r"(old), "r"(n) :);
    ASMV("nop");                                        // so the pc read above is poiting here
}


void ARM7::power(ARM7::OP_Mode mode)
{
    if (mode == _mode) return;
        _mode = mode;
    
#ifdef __mc13224v__ 
    typedef Machine::IO IO;
    switch(mode)
    {
    case OFF:
        db<CPU>(WRN) << "Shutting system down! No way back from here!\n";
        int_disable();
    case HIBERNATE:
        out32(IO::CRM_SLEEP_CNTL, in32(IO::CRM_SLEEP_CNTL) | 0x1);
        break;
    case DOZE:
        out32(IO::CRM_WU_CNTL, in32(IO::CRM_WU_CNTL) | 0x1);
        out32(IO::CRM_SLEEP_CNTL, in32(IO::CRM_SLEEP_CNTL) | 0x1<<6 | 0x3<<4 | 0x1 );
        while(!(in32(IO::CRM_STATUS) & 0x1));
        out32(IO::CRM_STATUS, 0x1); //writing 1 clears the SLEEP_SYNC bit
        break;
    case FULL:
    default:
        break;
    }
       
#endif
}
extern "C" void __cxa_guard_acquire() {
    CPU::int_disable();
}
extern "C" void __cxa_guard_release() {
    CPU::int_enable();
}

__END_SYS

