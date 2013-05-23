// EPOS AVR8 CPU Mediator Declarations 

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __avr8_h
#define __avr8_h

#include <cpu.h>
#include <utility/debug.h>

__BEGIN_SYS

class AVR8: public CPU_Common
{
private:
    typedef Traits<CPU> _Traits;

    static const unsigned long CLOCK = Traits<Machine>::CLOCK;

public:
    // CPU Flags
    typedef Reg8 Flags;
    enum {    
        FLAG_CARRY           = 0x01,
        FLAG_ZERO            = 0x02,
        FLAG_NEGATIVE        = 0x04,
        FLAG_TWO_COMPL_OF    = 0x08,
        FLAG_SIGN            = 0x10,
        FLAG_HALFCARRY       = 0x20,
        FLAG_BIT_COPY_ST     = 0x40,
        FLAG_INTERRUPT       = 0x80,
        FLAG_DEFAULTS        = FLAG_INTERRUPT, 
        FLAG_CLEAR           = ~FLAG_DEFAULTS
    };

private:
    //Sleep register
    enum {
#if defined (__AVR_ATmega128__)
    MCUCR = 0x35,
	SE    = 0x20,
	SM0   = 0x08,
	SM1   = 0x10,
	SM2   = 0x04
#elif defined (__AVR_AT90CAN128__)
    // on at90can128 these bits are in SMCR
    SMCR  = 0x33,
    SE    = 0x01,
    SM0   = 0x02,
    SM1   = 0x04,
    SM2   = 0x08,
    MCUCR = SMCR
#elif defined (__AVR_ATmega1281__)
    // on atmega1281 these bits are in SMCR
    SMCR  = 0x33,
    SM2   = 0x08,
    SM1   = 0x04,
    SM0   = 0x02,
    SE    = 0x01,
    MCUCR = SMCR
#elif defined (__AVR_ATmega16__)
    MCUCR = 0x35,
	SE    = 0x40,
	SM0   = 0x10,
	SM1   = 0x20,
	SM2   = 0x80
#endif
    };
public:
    // CPU Context
    class Context
    {
    public:
        Context(Log_Addr entry) :_sreg(FLAG_DEFAULTS),
                                 _pc((entry << 8) | (entry >> 8)){}     
        Context() {}

        void save() volatile;
        void load() const volatile;

        friend Debug & operator << (Debug & db, const Context & c) {
	    db << "{sp="    << &c
	       << ",sreg=" << c._sreg
	       << "}" ;
	    return db;
        }

    public:
        Reg8 _sreg;
        Reg8 _r0;
//      Reg8 _r1;        // r1 is always 0, thus not part of ctx
        Reg8 _r2;
        Reg8 _r3;
        Reg8 _r4;
        Reg8 _r5;
        Reg8 _r6;
        Reg8 _r7;
        Reg8 _r8;
        Reg8 _r9;
        Reg8 _r10;
        Reg8 _r11;
        Reg8 _r12;
        Reg8 _r13;
        Reg8 _r14;
        Reg8 _r15;
        Reg8 _r16;
        Reg8 _r17;
        Reg8 _r18;
        Reg8 _r19;
        Reg8 _r20;
        Reg8 _r21;
        Reg8 _r22;
        Reg8 _r23;
        Reg8 _r24;
        Reg8 _r25;
        Reg8 _r26;
        Reg8 _r27;
        Reg8 _r28;
        Reg8 _r29;
        Reg8 _r30;
        Reg8 _r31;
        Reg16 _pc;        
    };

public:
    AVR8() {} 
 
    Hertz clock() { return CLOCK; };

    static void int_enable() { ASMV("sei"); };
    static void int_disable() { ASMV("cli"); };
    static void halt() { power(_Traits::IDLE); }

    static void switch_context(Context * volatile * o, Context * volatile n);

    static Flags flags() { return sreg(); }
    static void flags(Flags flags) { sreg(flags); }

    static Reg16 sp() { return sphl(); }
    static void sp(Reg16 sp) { sphl(sp); }

    static Reg16 fr() { return r25_24(); }
    static void fr(Reg16 fr) { r25_24(fr); }

    static Reg16 pdp() { return 0; }
    static void pdp(Reg16 pdp) {}
    
    static Log_Addr ip() { return pc(); }    

    static bool tsl(volatile bool & lock) {
        int_disable();
        register bool old = CPU_Common::tsl(lock);
        int_enable();
        return old;
    }
    static int finc(volatile int & value) {
        int_disable();
        register int old = CPU_Common::finc(value);
        int_enable();
        return old;
    }
    static int fdec(volatile int & value) {
        int_disable();
        register int old = CPU_Common::fdec(value);
        int_enable();
        return old;
    }

    static Reg32 htonl(Reg32 v) { return swap32(v); }
    static Reg16 htons(Reg16 v) { return swap16(v); }
    static Reg32 ntohl(Reg32 v) { return htonl(v); }
    static Reg16 ntohs(Reg16 v) { return htons(v); }

    static Context * init_stack(
	Log_Addr stack, unsigned int size, 
	void (* exit)(), int (* entry)()) {
	Log_Addr fp = stack;
	Log_Addr sp = stack + size;
	sp -= sizeof(int); 
	*static_cast<int *>(sp) = swap16(Log_Addr(exit));
	sp -= sizeof(Context);
	Context * ctx = new (sp) Context(entry);
	ctx->_r28 = static_cast<Reg8>(fp);
 	ctx->_r29 = static_cast<Reg8>(fp >> 8);
	return ctx;
    }

    template<typename T1>
    static Context * init_stack(
	Log_Addr stack, unsigned int size,
	void (* exit)(), int (* entry)(T1 a1), T1 a1) {
	Log_Addr sp = stack + size;
	sp -= sizeof(int); 
	*static_cast<int *>(sp) = swap16(Log_Addr(exit));
	sp -= sizeof(Context);
	Context * ctx = new (sp) Context(entry);

	unsigned char arg_size = (sizeof(T1) % 2) ? 
	    sizeof(T1) + 1 : sizeof(T1);
	unsigned char * ctx_reg = &(ctx->_r26) - arg_size;
	*reinterpret_cast<T1 *>(ctx_reg) = a1;

	return ctx;
    }

    template<typename T1, typename T2>
    static Context * init_stack(
	Log_Addr stack, unsigned int size, void (* exit)(),
	int (* entry)(T1 a1, T2 a2), T1 a1, T2 a2) {
	Log_Addr sp = stack + size;
	sp -= sizeof(int); 
	*static_cast<int *>(sp) = swap16(Log_Addr(exit));
	sp -= sizeof(Context);
	Context * ctx = new (sp) Context(entry);

	unsigned char arg_size = (sizeof(T1) % 2) ? 
	    sizeof(T1) + 1 : sizeof(T1);
	unsigned char * ctx_reg = &(ctx->_r26) - arg_size;
	*reinterpret_cast<T1 *>(ctx_reg) = a1;
	
	arg_size = (sizeof(T2) % 2) ? 
	    sizeof(T2) + 1 : sizeof(T2);
	ctx_reg -= arg_size;
	*reinterpret_cast<T2 *>(ctx_reg) = a2;


	return ctx;
    }


    template<typename T1, typename T2, typename T3>
    static Context * init_stack(
	Log_Addr stack, unsigned int size, void (* exit)(),
	int (* entry)(T1 a1, T2 a2, T3 a3), T1 a1, T2 a2, T3 a3) {
	Log_Addr sp = stack + size;
	sp -= sizeof(int); 
	*static_cast<int *>(sp) = swap16(Log_Addr(exit));
	sp -= sizeof(Context);
	Context * ctx = new (sp) Context(entry);

	unsigned char arg_size = (sizeof(T1) % 2) ? 
	    sizeof(T1) + 1 : sizeof(T1);
	unsigned char * ctx_reg = &(ctx->_r26) - arg_size;
	*reinterpret_cast<T1 *>(ctx_reg) = a1;
	
	arg_size = (sizeof(T2) % 2) ? 
	    sizeof(T2) + 1 : sizeof(T2);
	ctx_reg -= arg_size;
	*reinterpret_cast<T2 *>(ctx_reg) = a2;

	arg_size = (sizeof(T3) % 2) ? 
	    sizeof(T3) + 1 : sizeof(T3);
	ctx_reg -= arg_size;
	*reinterpret_cast<T3 *>(ctx_reg) = a3;

	return ctx;
    }

    // AVR8 specific methods
    
    static Reg8 sreg() { return in8(0x3f); }
    static void sreg(Reg8 value) { out8(0x3f, value); }

    static Reg16 sphl() { return in16(0x3d); }
    static void sphl(Reg16 value) { out16(0x3d, value); }    
    
    static Reg16 r25_24(){
        Reg16 value;
        ASMV("mov	%A0,r24		\n"
             "mov	%B0,r25		\n"
             : "=r" (value)
	     :);
        return value;
    }    
    static void r25_24(Reg16 value){
        ASMV("mov	r24,%A0		\n"
             "mov	r25,%B0		\n"
             :
	     : "r" (value));    
    }
    
    static Log_Addr pc(){
        Log_Addr value;
        ASMV("	call	1f		\n"
             "1:	in	%A0,0x3e	\n"
             "	in	%B0,0x3d	\n"
             "	pop	r1		\n"
             "	pop	r1		\n"
             "	clr	r1		\n"
             : "=r" (value)
             :);
        return value;  
    }
    
    // We cannot use inline ASM anymore, as some of the extended cores 
    // IO registers cannot be accessed with in/out instructions.
    // The compiler is smart enough to generate in/out or ld/st accordingly.
    static Reg8 in8(const unsigned char port) {
	return (*(volatile unsigned char *)(port + 0x20));
    }
    static Reg16 in16(const unsigned char port) {
        Reg16 value = (*(volatile unsigned char *)(port + 0x20));
	value |= ((Reg16)(*(volatile unsigned char *)(port + 1 + 0x20)))<<8;
	return value; 
    }  
    static void out8(unsigned char port, Reg8 value) {
        (*(volatile unsigned char *)(port + 0x20)) = value;
    }
    static void out16(const unsigned char port, Reg16 value) {
	(*(volatile unsigned char *)(port + 1 + 0x20)) = (Reg8)(value>>8);  // Must write high byte first
	(*(volatile unsigned char *)(port + 0x20)) = (Reg8)value;
    }  

    static void reboot() { ASM("jmp 0"); }

    static void init();

    static void power(Traits<CPU>::Power_Modes mode) {
        switch(mode) {
	    case _Traits::IDLE:
	        out8(MCUCR,in8(MCUCR) & ~((SM0 | SM1 | SM2)));
	        break;
	    case _Traits::ADC_NOISE_REDUCTION:
	        out8(MCUCR,in8(MCUCR) & ~((SM1 | SM2)));
	        out8(MCUCR,in8(MCUCR) | SM0);
	        break;
		case _Traits::POWER_DOWN:
			out8(MCUCR,in8(MCUCR) & ~((SM0 | SM2)));
			out8(MCUCR,in8(MCUCR) | SM1);
			break;
		case _Traits::POWER_SAVE:
			out8(MCUCR,in8(MCUCR) & ~(SM2));
			out8(MCUCR,in8(MCUCR) | (SM0 | SM1));
			break;
		case _Traits::NATIVE_STANDBY:
			out8(MCUCR,in8(MCUCR) & ~(SM0));
			out8(MCUCR,in8(MCUCR) | (SM1 | SM2));
			break;
		case _Traits::EXTENDED_STANDBY:
			out8(MCUCR,in8(MCUCR) | (SM0 | SM1 | SM2));
			break;
		case _Traits::FULL: //if running it is already at FULL
		default:
			return;
		}
		ASMV("sleep");
	}

};

__END_SYS

#endif
