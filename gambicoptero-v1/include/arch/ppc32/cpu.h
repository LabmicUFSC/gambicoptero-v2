// EPOS PPC32 CPU Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __ppc32_h
#define __ppc32_h

#include <cpu.h>
#include <utility/debug.h>

__BEGIN_SYS

class PPC32: public CPU_Common
{
private:
    static const unsigned int CLOCK = Traits<Machine>::CLOCK;

public:
    //IO Ports
    typedef unsigned int IO_Port;
    typedef unsigned int IO_Irq;

    //Machine State Register
    enum MSR {
        MSR_WE      = 0x00040000,
        MSR_CE      = 0x00020000,
        MSR_EE      = 0x00008000,
        MSR_PR      = 0x00004000,
        MSR_ME      = 0x00001000,
        MSR_DWE     = 0x00000400,
        MSR_DE      = 0x00000200,
        MSR_IR      = 0x00000020,
        MSR_DR      = 0x00000010,
        MSR_DEFAULT = (MSR_CE | MSR_EE)
    };

    // Exception Syndrome Register
    enum ESR {
        ESR_MCI     = 0x80000000,
        ESR_PIL     = 0x08000000,
        ESR_PPR     = 0x04000000,
        ESR_PTR     = 0x02000000,
        ESR_DST     = 0x00800000,
        ESR_DIZ     = 0x00400000,
        ESR_U0F     = 0x00008000
    };

    // SPR
    enum SPR {
        SLER  = 0x3BB,
        SGR   = 0x3B9,
        ESR   = 0x3D4,
        EVPR  = 0x3D6,
        SSR0  = 0x01A,
        SSR1  = 0x01B,
        SSR2  = 0x3DE,
        SSR3  = 0x3BC
    };

    class Context 
    {
      private:
        Reg32 _regs[32];
        Reg32 _msr;     // Machine State Register
        Reg32 _lr;      // Link Register
        Reg32 _ctr;     // Counter Register
        Reg32 _cr;      // Conditional Register
        Reg32 _xer;     // FiXed Point Exception Register
        Reg32 _srr0;    // Save/Restore Register 0
        Reg32 _srr1;    // Save/Restore Register 1
        Reg32 _srr2;    // Save/Restore Register 2
        Reg32 _srr3;    // Save/Restore Register 3

      public:
        Context(Log_Addr entry) : _lr(entry) {
             _msr = MSR_DEFAULT; //(Interrupts Enabled).
             _ctr = 0;
             _xer = 0;
             _srr0 = 0;
             _srr1 = 0;
             _srr2 = 0;
             _srr3 = 0;
             _cr = 0;
        };

        friend Debug & operator << (Debug & db, const Context & c) {
            db << "{msr="    << (void *)c._msr
               << ",lr="     << (void *)c._lr
               << ",ctr="    << (void *)c._ctr
               << ",xer="    << (void *)c._xer
               << ",srr0="   << (void *)c._srr0
               << ",srr1="   << (void *)c._srr1
               << ",srr2="   << (void *)c._srr2
               << ",srr3="   << (void *)c._srr3
               << ",cr="     << (void *)c._cr
               << "}" ;
            return db;
        }

        void save() volatile;
        void load() const volatile;
    };

public:
    PPC32() {}
    ~PPC32() {}

    static Hertz clock() { return CLOCK; }

    static void switch_context(Context * volatile * o, Context * volatile n);

    static void int_enable() {
      int_critical_enable();
      int_nonCritical_enable();
    };

    static void int_disable() {
      int_nonCritical_disable();
      int_critical_disable();
    };

    static void halt() {
      volatile Reg32 value;
      ASMV("mfmsr %0; sync" : "=r" (value) : );
      value |= 0x00040000;
      ASMV("sync; mtmsr %0;" : : "r" (value) );
    }

    static bool tsl(volatile bool & lock) {
      int_disable();
      bool ret = CPU_Common::tsl(lock);
      int_enable();
      return ret;
    }

    static int finc(volatile int & number) {
      int_disable();
      int ret = CPU_Common::finc(number);
      int_enable();
      return ret;
    }

    static int fdec(volatile int & number) {
      int_disable();
      int ret = CPU_Common::fdec(number);
      int_enable();
      return ret;
    }

    static Reg32 htonl(Reg32 v) { return v; }
    static Reg16 htons(Reg16 v) { return v; }
    static Reg32 ntohl(Reg32 v) { return v; }
    static Reg16 ntohs(Reg16 v) { return v; }

    static Reg32 cpu_to_le32(Reg32 v) { return swap32(v); }
    static Reg16 cpu_to_le16(Reg16 v) { return swap16(v); }
    static Reg32 le32_to_cpu(Reg32 v) { return swap32(v); }
    static Reg16 le16_to_cpu(Reg16 v) { return swap16(v); }

    static PPC32::Context * PPC32::init_stack(
	Log_Addr stack, unsigned int size, void (* exit)(),
	int (* entry)()) {
        void (*wrapper)() = &CPU::entry_wrapper;
	Log_Addr sp = stack + size;
	sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(exit);
        sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(entry);
        sp -= sizeof(int); //Frame Header Space
        sp -= sizeof(int); //Frame Header Space
	sp -= sizeof(Context);
	return  new (sp) Context(wrapper);
    }

    template<typename T1>
    static PPC32::Context * PPC32::init_stack(
	Log_Addr stack, unsigned int size, void (* exit)(),
	int (* entry)(T1 a1), T1 a1) {
        void (*wrapper)(T1) = &CPU::entry_wrapper;
	Log_Addr sp = stack + size;
	sp -= sizeof(T1); *static_cast<T1 *>(sp) = a1;
	sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(exit);
        sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(entry);
        sp -= sizeof(int); //Frame Header Space
        sp -= sizeof(int); //Frame Header Space
	sp -= sizeof(Context);
	return new (sp) Context(wrapper);
    }

    template<typename T1, typename T2>
    static PPC32::Context * PPC32::init_stack(
	Log_Addr stack, unsigned int size, void (* exit)(),
	int (* entry)(T1 a1, T2 a2), T1 a1, T2 a2) {
        void (*wrapper)(T1,T2) = &CPU::entry_wrapper;
	Log_Addr sp = stack + size;
	sp -= sizeof(T2); *static_cast<T2 *>(sp) = a2;
	sp -= sizeof(T1); *static_cast<T1 *>(sp) = a1;
	sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(exit);
        sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(entry);
        sp -= sizeof(int); //Frame Header Space
        sp -= sizeof(int); //Frame Header Space
	sp -= sizeof(Context);
	return new (sp) Context(wrapper);
    }

    template<typename T1, typename T2, typename T3>
    static PPC32::Context * PPC32::init_stack(
	Log_Addr stack, unsigned int size, void (* exit)(),
	int (* entry)(T1 a1, T2 a2, T3 a3), T1 a1, T2 a2, T3 a3) {
        void (*wrapper)(T1,T2,T3) = &CPU::entry_wrapper;
	Log_Addr sp = stack + size;
	sp -= sizeof(T3); *static_cast<T3 *>(sp) = a3;
	sp -= sizeof(T2); *static_cast<T2 *>(sp) = a2;
	sp -= sizeof(T1); *static_cast<T1 *>(sp) = a1;
	sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(exit);
        sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(entry);
        sp -= sizeof(int); //Frame Header Space
        sp -= sizeof(int); //Frame Header Space
	sp -= sizeof(Context);
	return new (sp) Context(wrapper);
    }

    //Entry Wrappers for passing parameters through registers...
    static void entry_wrapper(){
        Log_Addr sp = static_cast<Log_Addr>(CPU::sp());
        sp = *((Log_Addr *)sp);
        sp += 8; // Frame Header Space
        Log_Addr entry_point = *static_cast<unsigned int *>(sp);
        sp += sizeof(int);
        Log_Addr implicit_exit = *static_cast<unsigned int *>(sp);
        CPU::sync_io();//Sync CPU IO and Instruction
        int (*entry)() = (int (*)())entry_point;
        (*entry)();
        void (*ret_addr)() = (void (*)())implicit_exit;
        (*ret_addr)();
    }

    template<class T1>
    static void entry_wrapper(T1 a1){
        Log_Addr sp = static_cast<Log_Addr>(CPU::sp());
        sp = *((Log_Addr *)sp);
        sp += 8; // Frame Header Space
        Log_Addr entry_point = *static_cast<unsigned int *>(sp);
        sp += sizeof(int);
        Log_Addr implicit_exit = *static_cast<unsigned int *>(sp);
        int (*entry)(T1) = (int (*)(T1))entry_point;
        sp += sizeof(T1);
        T1 p1 = *static_cast<T1 *>(sp);
        CPU::sync_io();//Sync CPU IO and Instruction
        (*entry)(p1);
        void (*ret_addr)() = (void (*)())implicit_exit;
        (*ret_addr)();
    }

    template<class T1, class T2>
    static void entry_wrapper(T1 a1, T2 a2){
        Log_Addr sp = static_cast<Log_Addr>(CPU::sp());
        sp = *((Log_Addr *)sp);
        sp += 8; // Frame Header Space
        Log_Addr entry_point = *static_cast<unsigned int *>(sp);
        sp += sizeof(int);
        Log_Addr implicit_exit = *static_cast<unsigned int *>(sp);
        int (*entry)(T1,T2) = (int (*)(T1,T2))entry_point;
        sp += sizeof(T1);
        T1 p1 = *static_cast<T1 *>(sp);
        sp += sizeof(T2);
        T2 p2 = *static_cast<T2 *>(sp);
        CPU::sync_io();//Sync CPU IO and Instruction
        (*entry)(p1, p2);
        void (*ret_addr)() = (void (*)())implicit_exit;
        (*ret_addr)();
    }

    template<class T1, class T2, class T3>
    static void entry_wrapper(T1 a1, T2 a2, T3 a3){
        Log_Addr sp = static_cast<Log_Addr>(CPU::sp());
        sp = *((Log_Addr *)sp);
        sp += 8; // Frame Header Space
        Log_Addr entry_point = *static_cast<unsigned int *>(sp);
        sp += sizeof(int);
        Log_Addr implicit_exit = *static_cast<unsigned int *>(sp);
        int (*entry)(T1,T2,T3) = (int (*)(T1,T2,T3))entry_point;
        sp += sizeof(T1);
        T1 p1 = *static_cast<T1 *>(sp);
        sp += sizeof(T2);
        T2 p2 = *static_cast<T2 *>(sp);
        sp += sizeof(T3);
        T3 p3 = *static_cast<T3 *>(sp);
        CPU::sync_io();//Sync CPU IO and Instruction
        (*entry)(p1, p2, p3);
        void (*ret_addr)() = (void (*)())implicit_exit;
        (*ret_addr)();
    }

    static Reg32 fr(){ return reg<3>(); }
    static void fr(const Reg32 fr) { reg<3>(fr); }

    static Reg32 sp() { return reg<1>(); }
    static void sp(const Reg32 sp) { reg<1>(sp); }

    //Not Implemented ... 
    static Reg32 pdp() { return 0; }
    static void pdp(Reg32 pdp) {}

    static void init();

// PPC32 specific *Public* methods.
public:
    // SPR are used by specific Machine Mediators (e.g Timer)
    static Reg32 _mfspr(const unsigned int reg){
      Reg32 value;
      ASMV("mfspr %0,%1" :"=r" ( value ): "i" ( reg ) );
      return value;
    };
    static void _mtspr(const unsigned int reg, unsigned int value) {
      ASMV("mtspr %0,%1" :: "i" ( reg ), "r" ( value ) );
    };
    //Included for ML310_PCI mediator !
    static void sync_io(){
        ASMV("eieio");
    }

// PPC32 implementation methods
private:
    template <unsigned int REG> static Reg32 reg() {
        Reg32 value;
	ASMV("addi %0, %1, 0" : "=r"(value) : "i"(REG));
	return value;
    }
    template <unsigned int REG> static void reg(Reg32 value) {
	ASMV("addi %1, %0, 0" : : "r"(value), "i"(REG));
    }
    static Reg32 lr(){
      Reg32 value;
      ASMV( "mflr %0" : "=r"(value) );
      return value;
    };
    static void lr( Reg32 value ){
      ASMV( "mtlr %0" : : "r"(value) );
    };
    static void _mtdcr( const unsigned int reg, unsigned int value ) {
      ASMV("mtdcr %0,%1" :: "i" ( reg ), "r" ( value ) );
    };
    static void _mfdcr( const unsigned int reg, unsigned int value ) {
      ASMV("mfdcr %0,%1" :"=r" ( value ): "i" ( reg ) );
    };
    static inline void int_critical_enable(){
      volatile Reg32 value;
      ASMV("mfmsr %0; sync" : "=r" (value) : );
      value |= 0x00020000;
      ASMV("mtmsr %0; sync" :: "r" (value) );
    };
    static inline void int_critical_disable(){
      volatile Reg32 value;
      ASMV("mfmsr %0; sync" : "=r" (value) : );
      value &= ~0x00020000;
      ASMV("mtmsr %0; sync" :: "r" (value) );
    };
    static inline void int_nonCritical_enable(){
      ASMV("wrteei 1");
    };
    static inline void int_nonCritical_disable(){
      ASMV("wrteei 0");
    };

private:
    // PPC32 attributes

};

__END_SYS

#endif
