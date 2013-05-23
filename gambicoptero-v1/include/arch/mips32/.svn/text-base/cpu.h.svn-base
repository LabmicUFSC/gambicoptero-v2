// EPOS MIPS32 CPU Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mips32_h
#define __mips32_h

#include <cpu.h>
#include <utility/debug.h>

__BEGIN_SYS

class MIPS32: public CPU_Common
{
private:
    static const unsigned int CLOCK = Traits<Machine>::CLOCK;

public:
	typedef Reg32 CP0Reg;
	static const unsigned int STATUS_INT_MASK	= 0x0000ff00; // reg 12 (status) of Cp0

	enum STATUS_REG {
		STATUS_CURRENT		= 0x0003,   // 00000000.00000011
		STATUS_PREVIOUS		= 0x000C,   // 00000000.00001100
		STATUS_OLD		    = 0x0030,   // 00000000.00110000     //BELLOW I'M NOT SURE ABOUT BITS
		STATUS_MASK_INT		= 0x0010,   // 00000001.00000000     intr 0
		STATUS_MASK_ADDRL	= 0x0020,   // 00000010.00000000     intr 4
		STATUS_MASK_ADDRS	= 0x0040,   // 00000100.00000000     intr 5
		STATUS_MASK_IBUS	= 0x0080,   // 00001000.00000000     intr 6
		STATUS_MASK_DBUS	= 0x0100,   // 00010000.00000000     intr 7
		STATUS_MASK_SYSCALL	= 0x0200,   // 00100000.00000000     intr 8
		STATUS_MASK_BKPT	= 0x0400,   // 01000000.00000000     intr 9
		STATUS_MASK_RI		= 0x0800,   // 10000000.00000000     intr 10
		STATUS_MASK_OVF		= 0x0000    // 00000000.00000000     intr 12  //and this??
	};

	enum CAUSE {
		CAUSE_EXCEPTION_CODE		= 0x003C,   // 00000000.00111100
		CAUSE_PENDING_INTERRUPTS	= 0xF800,   // 11111000.00000000   //FIVE BITS?  FROM 15 TO 11 OR FROM 14 TO 10?
	};

	enum CP {	
		CP0_INDEX		= 0,
		CP0_RANDOM		= 1,
		CP0_ENTRYLO0	= 2,
		CP0_ENTRYLO1	= 3,
		CP0_CONTEXT		= 4,
		CP0_COUNT		= 9,  // R4000-only. Shame on R3000!
		CP0_ENTRYHI		= 10,
		CP0_COMPARE		= 11, // R4000-only. Shame on R3000!
		CP0_STATUS		= 12,
		CP0_CAUSE		= 13,
		CP0_EPC			= 14,
		CP0_PRID		= 15,
	};

    // CPU Context
    class Context     {
    public:
	Context(Log_Addr entry, Log_Addr vstack) { 
		//adjust special registers to class attributes
		_gp = CPU::reg<28>();
		//Trick: Store in K0 an pointer to virtual stack 
		// created on init_stack for use in entry_wrapper.
		_t0 = vstack;
		_status = cp0regs<12>();
		_status = _status | 0x01; //Default STATUS = Interrupts Enabled
		_ra = entry;
	}
	
	void save(Context * volatile * o) volatile;
	void load() const volatile;
	
	friend Debug & operator << (Debug & db, const Context & c) {
		db << "{at=" << c._at << ",v0=" << c._v0 << ",v1=" << c._v1
		   << ",a0=" << c._a0 << ",a1=" << c._a1 << ",a2=" << c._a2
		   << ",a3=" << c._a3 << ",t0=" << c._t0 << ",t1=" << c._t1
		   << ",t2=" << c._t2 << ",t3=" << c._t3 << ",t4=" << c._t4
		   << ",t5=" << c._t5 << ",t6=" << c._t6 << ",t7=" << c._t7
		   << ",s0=" << c._s0 << ",s1=" << c._s1 << ",s2=" << c._s2
		   << ",s3=" << c._s3 << ",s4=" << c._s4 << ",s5=" << c._s5
		   << ",s6=" << c._s6 << ",s7=" << c._s7 << ",t8=" << c._t8
		   << ",t9=" << c._t9 << ",k0=" << c._k0 << ",k1=" << c._k1
		   << ",gp=" << (void *)c._gp   << ",sp=" << (void *)c._sp
		   << ",fp=" << (void *)c._fp   << ",ra=" << (void *)c._ra 
		   << " status=" << (void *)c._status     << "}" ;
		return db;
	}
	
	private:
	Reg32 _at;	//1
	Reg32 _v0;	//2
	Reg32 _v1;	//3
	Reg32 _a0;	//4
	Reg32 _a1;	//5
	Reg32 _a2;	//6
	Reg32 _a3;	//7
	Reg32 _t0;	//8
	Reg32 _t1;	//9
	Reg32 _t2;	//10
	Reg32 _t3;	//11
	Reg32 _t4;	//12
	Reg32 _t5;	//13
	Reg32 _t6;	//14
	Reg32 _t7;	//15
	Reg32 _s0;	//16
	Reg32 _s1;	//17
	Reg32 _s2;	//18
	Reg32 _s3;	//19
	Reg32 _s4;	//20
	Reg32 _s5;	//21
	Reg32 _s6;	//22
	Reg32 _s7;	//23
	Reg32 _t8;	//24
	Reg32 _t9;	//25
	Reg32 _k0;	//26
	Reg32 _k1;	//27
	Reg32 _gp;	//28
	Reg32 _sp;	//29
	Reg32 _fp;	//30
	Reg32 _ra;	//31
	Reg32 _mhi;	//mfhi
	Reg32 _mlo;	//mflo
	Reg32 _status;  //Status
	Reg32 _epc;     //Exception Program Counter
    };//end class context

public:
    MIPS32() {}

    static Hertz clock() { return CLOCK; }

    static void switch_context(Context * volatile * o, Context * volatile n);

    static void int_enable() {
	    CP0Reg status = cp0regs<12>();
        status = status | 0x01;
        cp0regs<12>(status);
    }

    static void int_disable() {
    	CP0Reg status = cp0regs<12>();
        status = status & 0xFFFFFFFE;
        cp0regs<12>(status);
    }

    // TODO: This method should be refined.
    static void halt() { 
        int_enable();
	//for(;;); 
    }

    static bool tsl(volatile bool & lock) {
	bool was = int_enabled();
	int_disable();
	bool olock = CPU_Common::tsl(lock);
	if (was) int_enable();
	return olock;
    }
 
    static int finc(volatile int & number) {
	bool was = int_enabled();
        int_disable();
	int old = CPU_Common::finc(number);
	if (was) int_enable();
	return old;
    }

    static int fdec(volatile int & number) {
	bool was = int_enabled();
	int_disable();
	int old = CPU_Common::fdec(number);
	if (was) int_enable();
	return old;
    }

    static Reg32 htonl(Reg32 v) { return v; }
    static Reg16 htons(Reg16 v) { return v; }
    static Reg32 ntohl(Reg32 v) { return v; }
    static Reg16 ntohs(Reg16 v) { return v; }

    static Reg32 cpu_to_le32(Reg32 v) { return swap32(v); }
    static Reg16 cpu_to_le16(Reg16 v) { return swap16(v); }
    static Reg32 le32_to_cpu(Reg32 v) { return swap32(v); }
    static Reg16 le16_to_cpu(Reg16 v) { return swap16(v); }

    static Context * init_stack(
	Log_Addr stack, unsigned int size, void (* exit)(),
	int (* entry)()) {
	void (*wrapper)() = &CPU::entry_wrapper;
	Log_Addr sp = stack + size;
	sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(exit);  // $ra
	sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(entry); // $fp
	Log_Addr vstack = sp;
	sp -= sizeof(Context);
	return  new (sp) Context(wrapper, vstack);
    }

    template<typename T1>
    static Context * init_stack(
	Log_Addr stack, unsigned int size, void (* exit)(),
	int (* entry)(T1 a1), T1 a1) {
        void (*wrapper)(T1) = &CPU::entry_wrapper;
	Log_Addr sp = stack + size;
	sp -= sizeof(T1); *static_cast<T1 *>(sp) = a1;
	sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(exit);
	sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(entry);
	Log_Addr vstack = sp;
	sp -= sizeof(Context);
	return new (sp) Context(wrapper, vstack);
    }

    template<typename T1, typename T2>
    static Context * init_stack(
	Log_Addr stack, unsigned int size, void (* exit)(),
	int (* entry)(T1 a1, T2 a2), T1 a1, T2 a2) {
        void (*wrapper)(T1,T2) = &CPU::entry_wrapper;
	Log_Addr sp = stack + size;
	sp -= sizeof(T2); *static_cast<T2 *>(sp) = a2;
	sp -= sizeof(T1); *static_cast<T1 *>(sp) = a1;
	sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(exit);
	sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(entry);
	Log_Addr vstack = sp;
	sp -= sizeof(Context);
	return new (sp) Context(wrapper, vstack);
    }

    template<typename T1, typename T2, typename T3>
    static Context * init_stack(
	Log_Addr stack, unsigned int size, void (* exit)(),
	int (* entry)(T1 a1, T2 a2, T3 a3), T1 a1, T2 a2, T3 a3) {
        void (*wrapper)(T1,T2,T3) = &CPU::entry_wrapper;
	Log_Addr sp = stack + size;
	sp -= sizeof(T3); *static_cast<T3 *>(sp) = a3;
	sp -= sizeof(T2); *static_cast<T2 *>(sp) = a2;
	sp -= sizeof(T1); *static_cast<T1 *>(sp) = a1;
	sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(exit);
	sp -= sizeof(int); *static_cast<int *>(sp) = Log_Addr(entry);
	Log_Addr vstack = sp;
	sp -= sizeof(Context);
	return new (sp) Context(wrapper, vstack);
    }

   static void entry_wrapper(){
	Log_Addr vstack = static_cast<Log_Addr>(reg<8>());
	Log_Addr entry_point = *static_cast<unsigned int *>(vstack);
	vstack += sizeof(int);
	Log_Addr implicit_exit = *static_cast<unsigned int *>(vstack);
	int (*entry)() = (int (*)())entry_point;
	(*entry)();
	void (*ret_addr)() = (void (*)())implicit_exit;
	(*ret_addr)();
   }

    template<class T1>
    static void entry_wrapper(T1 a1){
	Log_Addr vstack = static_cast<Log_Addr>(reg<8>());
	Log_Addr entry_point = *static_cast<unsigned int *>(vstack);
	vstack += sizeof(int);
	Log_Addr implicit_exit = *static_cast<unsigned int *>(vstack);
	vstack += sizeof(T1);
        T1 p1 = *static_cast<T1 *>(vstack);
	int (*entry)(T1) = (int (*)(T1))entry_point;
	(*entry)(p1);
	void (*ret_addr)() = (void (*)())implicit_exit;
	(*ret_addr)();
   }

   template<class T1, class T2>
   static void entry_wrapper(T1 a1, T2 a2){
	Log_Addr vstack = static_cast<Log_Addr>(reg<8>());
	Log_Addr entry_point = *static_cast<unsigned int *>(vstack);
	vstack += sizeof(int);
	Log_Addr implicit_exit = *static_cast<unsigned int *>(vstack);
	vstack += sizeof(T1);
        T1 p1 = *static_cast<T1 *>(vstack);
	vstack += sizeof(T2);
        T2 p2 = *static_cast<T2 *>(vstack);
	int (*entry)(T1,T2) = (int (*)(T1,T2))entry_point;
	(*entry)(p1,p2);
	void (*ret_addr)() = (void (*)())implicit_exit;
	(*ret_addr)();
   }

   template<class T1, class T2, class T3>
   static void entry_wrapper(T1 a1, T2 a2, T3 a3){
	Log_Addr vstack = static_cast<Log_Addr>(reg<8>());
	Log_Addr entry_point = *static_cast<unsigned int *>(vstack);
	vstack += sizeof(int);
	Log_Addr implicit_exit = *static_cast<unsigned int *>(vstack);
	vstack += sizeof(T1);
        T1 p1 = *static_cast<T1 *>(vstack);
	vstack += sizeof(T2);
        T2 p2 = *static_cast<T2 *>(vstack);
	vstack += sizeof(T3);
        T2 p3 = *static_cast<T3 *>(vstack);
	int (*entry)(T1,T2,T3) = (int (*)(T1,T2,T3))entry_point;
	(*entry)(p1,p2,p3);
	void (*ret_addr)() = (void (*)())implicit_exit;
	(*ret_addr)();
   }

   // Processor registers
   static Reg32 sp() { return reg<29>(); }
   static void sp(Reg32 value) { reg<29>(value); }

   static Reg32 fr() { return reg<2>(); }
   static void fr(Reg32 value) { reg<2>(value); }

   //Not implemented
   static Reg16 pdp() { return 0; }
   static void pdp(Reg16 pdp) {}

   static void init();

// MIPS32 Private Methods
private:
   // Coprocessor 0 registers
   template <unsigned int REG> static CP0Reg volatile cp0regs() {
	CP0Reg value; 
	ASMV("mfc0 %0, $%1" : "=r"(value) : "i"(REG)); 
	return value;
   }
   template <unsigned int REG> static void cp0regs(CP0Reg value) {
	ASMV("mtc0 %0, $%1" : : "r"(value), "i"(REG));
   }
   template <unsigned int REG> static Reg32 reg() {
        Reg32 value;
	ASMV("add %0, $0, $%1" : "=r"(value) : "i"(REG));
	return value;
   }
   template <unsigned int REG> static void reg(Reg32 value) {
	ASMV("add $%1, %0, $0" : : "r"(value), "i"(REG));
   }
   static Reg32 gp() {
	Reg32 value; 
	ASMV("add %0,$0,$28" : "=r"(value)); 
	return value;
   }
   static void gp(Reg32 value) {
	ASMV("add $28,%0,$0" : : "r"(value));
   }
   static Reg32 ra() {
	Reg32 value; 
	ASMV("add %0,$0,$31" : "=r"(value)); 
	return value;
   }
   static void ra(Reg32 value) {
	ASMV("add $31,%0,$0" : : "r"(value));
   }
   static bool int_enabled() {
	CP0Reg value = cp0regs<12>();
	return (value & 0x01 == 1);
   }

private:
    // MIPS32 attributes

};

__END_SYS

#endif
