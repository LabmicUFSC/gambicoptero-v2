// EPOS PC Interrupt Controller Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __pc_ic_h
#define __pc_ic_h

#include <cpu.h>
#include <ic.h>
#include <mach/pc/memory_map.h>

__BEGIN_SYS

// Intel 8259A Interrupt Controller (master and slave are seen as a unit)
class i8259A
{
private:
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg16 Reg16;

    static const unsigned int HARD_INT = 0x20;

public:
    // I/O Ports
    enum {
	MASTER_CMD	= 0x20,
	MASTER_IMR	= 0x21,
	SLAVE_CMD	= 0xa0,
	SLAVE_IMR	= 0xa1
    };

    // Commands
    enum {
	SELECT_IRR	= 0x0a,
	SELECT_ISR	= 0x0b,
	ICW1		= 0x11, // flank, cascaded, more ICWs
	ICW4		= 0x01,
	EOI		= 0x20
    };

    // IRQs
    typedef unsigned int IRQ;
    static const unsigned int IRQS = 16;
    enum {
	IRQ_TIMER	= 0,
	IRQ_KEYBOARD	= 1,
	IRQ_CASCADE	= 2
    };

    // Interrupts
    static const unsigned int INTS = 64;
    enum {
	INT_TIMER	= HARD_INT + IRQ_TIMER,
	INT_KEYBOARD	= HARD_INT + IRQ_KEYBOARD
    };

public:
    i8259A() {}

    static int irq2int(int i) { return i + HARD_INT; }
    static int int2irq(int i) { return i - HARD_INT; }

    static void enable() { imr(1 << IRQ_CASCADE); }
    static void enable(int i) { imr(imr() & ~(1 << int2irq(i))); }
    static void disable() { imr(~(1 << IRQ_CASCADE)); }
    static void disable(int i) { imr(imr() | (1 << int2irq(i))); }

    static void remap(Reg8 base = HARD_INT) {
	// Configure Master PIC
	IA32::out8(MASTER_CMD, ICW1);
	IA32::out8(MASTER_IMR, base);              // ICW2 is the base
	IA32::out8(MASTER_IMR, 1 << IRQ_CASCADE);  // ICW3 = IRQ2 cascaded
	IA32::out8(MASTER_IMR, ICW4);

	// Configure Slave PIC
	IA32::out8(SLAVE_CMD, ICW1);
	IA32::out8(SLAVE_IMR, base + 8); // ICW2 is the base
	IA32::out8(SLAVE_IMR, 0x02);     // ICW3 = cascaded from IRQ1
	IA32::out8(SLAVE_IMR, ICW4);
    }

    static void reset() { remap(); disable(); }

    static Reg16 irr() { // Pending interrupts
	IA32::out8(MASTER_CMD, SELECT_IRR);
	IA32::out8(SLAVE_CMD, SELECT_IRR);
	return IA32::in8(MASTER_CMD) | (IA32::in8(SLAVE_CMD) << 8);
    }

    static Reg16 isr() { // In-service interrupts
	IA32::out8(MASTER_CMD, SELECT_ISR);
	IA32::out8(SLAVE_CMD, SELECT_ISR);
	return IA32::in8(MASTER_CMD) | (IA32::in8(SLAVE_CMD) << 8);
    }

    static Reg16 imr() { // Interrupt mask
	return IA32::in8(MASTER_IMR) | (IA32::in8(SLAVE_IMR) << 8);
    }

    static void imr(Reg16 mask) {
	IA32::out8(MASTER_IMR, mask);
	IA32::out8(SLAVE_IMR, mask >> 8);
    }

    static void eoi() { // End of interrupt
    	if(isr() & (1 << IRQ_CASCADE)) // was it an slave PIC interrupt?
	    IA32::out8(SLAVE_CMD, EOI);
    	IA32::out8(MASTER_CMD, EOI); // always send EOI to master
    }
};

// Intel IA-32 APIC (internal, not tested with 82489DX)
class APIC
{
private:
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg16 Reg16;
    typedef CPU::Reg32 Reg32;
    typedef CPU::Log_Addr Log_Addr;

    static const unsigned int HARD_INT = 0x20;

public:
    // Interrupts
    static const unsigned int INTS = 64;
    enum {
	INT_TIMER	= i8259A::INT_TIMER
    };

    // Default mapping addresses
    enum {
	LOCAL_APIC_PHY_ADDR	= 0xfee00000,
	LOCAL_APIC_LOG_ADDR	= Memory_Map<PC>::APIC_MEM,
	IO_APIC_PHY_ADDR	= 0xfec00000,
	IO_APIC_LOG_ADDR	= Memory_Map<PC>::APIC_MEM +
	                          (IO_APIC_PHY_ADDR - LOCAL_APIC_PHY_ADDR)
    };

    // Memory-mapped registers
    enum {
	ID =			0x020,	// Task priority 
	VERSION =		0x030,	// Task priority 
	TPR =			0x080,	// Task priority 
	APR =			0x090,	// Arbitration priority 
	PPR =			0x0a0,	// Processor priority 
	EOI =			0x0b0,	// End of interrupt 
	RRR =			0x0c0,	// Remote read 
	LDR =			0x0d0,	// Logical destination 
	DFR =			0x0e0,	// Destination format 
	SVR =			0x0f0,	// Spurious interrupt vector 
	ISR0_32 =		0x100,	// In-service 
	ISR32_63 =		0x110,	// In-service 
	ISR64_95 =		0x120,	// In-service 
	ISR96_127 =		0x130,	// In-service 
	ISR128_159 =		0x140,	// In-service 
	ISR160_191 =		0x150,	// In-service 
	ISR192_223 =		0x160,	// In-service 
	ISR224_255 =		0x170,	// In-service 
	TMR0_32 =		0x180,	// Trigger mode 
	TMR32_63 =		0x190,	// Trigger mode 
	TMR64_95 =		0x1a0,	// Trigger mode 
	TMR96_127 =		0x1b0,	// Trigger mode 
	TMR128_159 =		0x1c0,	// Trigger mode 
	TMR160_191 =		0x1d0,	// Trigger mode 
	TMR192_223 =		0x1e0,	// Trigger mode 
	TMR224_255 =		0x1f0,	// Trigger mode 
	IRR0_32 =		0x200,	// Interrupt request 
	IRR32_63 =		0x210,	// Interrupt request 
	IRR64_95 =		0x220,	// Interrupt request 
	IRR96_127 =		0x230,	// Interrupt request 
	IRR128_159 =		0x240,	// Interrupt request 
	IRR160_191 =		0x250,	// Interrupt request 
	IRR192_223 =		0x260,	// Interrupt request 
	IRR224_255 =		0x270,	// Interrupt request 
	ESR =			0x280,	// Error status 
	LVT_CMCI =		0x2f0,	// LVT CMCI 
	ICR0_31 =		0x300,	// Interrupt command 
	ICR32_63 =		0x310,	// Interrupt command 
	LVT_TIMER =		0x320,	// LVT timer 
	LVT_THERMAL =		0x330,	// LVT thermal sensor 
	LVT_PERF =		0x340,	// LVT performance monitor 
	LVT_LINT0 =		0x350,	// LVT interrupt line 0 
	LVT_LINT1 =		0x360,	// LVT interrupt line 1 
	LVT_ERROR =		0x370,	// LVT error 
	TIMER_INITIAL =		0x380,	// Timer's initial count 
	TIMER_CURRENT =		0x390,	// Timer's current count 
	TIMER_PRESCALE =	0x3e0	// Timer's BUS CLOCK prescaler 
    };

    // Flags
    enum {
	// Local APIC ID Register
	ID_SHIFT		= 24,
	ID_MASK			= 0xff000000,

	// Spurious Interrupt Vector Register
	SVR_VECTOR		= 0x000000ff,
	SVR_APIC_ENABLED	= (1 << 8),
	SVR_FOCUS_DISABLED	= (1 << 9),

	// Error Status Register
	ESR_SEND_CHECK		= (1 << 0),
	ESR_RECV_CHECK		= (1 << 1),
	ESR_SEND_ACCEPT		= (1 << 2),
	ESR_RECV_ACCEPT		= (1 << 3),
	ESR_SEND_ILLEGAL_VECTOR	= (1 << 5),
	ESR_RECV_ILLEGAL_VECTOR	= (1 << 6),
	ESR_ILLEGAL_REGISTER	= (1 << 7),

	// Interrupt Command Register (64 bits)
	ICR_DEST		= (3 << 18),
	ICR_SELF		= (1 << 18),
	ICR_ALL			= (1 << 19),
	ICR_OTHERS		= (3 << 18),
	ICR_TRIGMOD		= (1 << 15),
	ICR_EDGE		= (0 << 15),
	ICR_LEVEL		= (1 << 15),
	ICR_ASSERTED		= (1 << 14),
	ICR_DEASSERT		= (0 << 14),
	ICR_ASSERT		= (1 << 14),
	ICR_STATUS		= (1 << 12),
	ICR_IDLE		= (0 << 12),
	ICR_PENDING		= (1 << 12),
	ICR_DESTMODE		= (1 << 11),
	ICR_PHY			= (0 << 11),
	ICR_LOG			= (1 << 11),
	ICR_DELMODE		= (7 <<  8),
	ICR_FIXED		= (0 <<  8),
	ICR_LOWPRI		= (1 <<  8),
	ICR_SMI			= (2 <<  8),
	ICR_NMI			= (4 <<  8),
	ICR_INIT		= (5 <<  8),
	ICR_STARTUP		= (6 <<  8),

	// Local Vector Table
	LVT_MASKED		= (1 << 16),
	LVT_EDGE		= (0 << 15),
	LVT_LEVEL		= (1 << 15),
	LVT_FIXED		= (0 << 8),
	LVT_SMI			= (2 << 8),
	LVT_NMI			= (4 << 8),
	LVT_EXTINT		= (7 << 8),
	LVT_INIT		= (5 << 8),

	// Local Timer (32 bits)
	TIMER_PERIODIC		= (1 << 17),
	TIMER_ONE_SHOT		= (0 << 17),
	TIMER_MASKED		= LVT_MASKED,
	TIMER_PRESCALE_BY_1	= 0xb,
	TIMER_PRESCALE_BY_2	= 0x0,
	TIMER_PRESCALE_BY_4	= 0x8,
	TIMER_PRESCALE_BY_8	= 0x2,
	TIMER_PRESCALE_BY_16	= 0xa,
	TIMER_PRESCALE_BY_32	= 0x1,
	TIMER_PRESCALE_BY_64	= 0x9,
	TIMER_PRESCALE_BY_128	= 0x3,
    };

public:
    APIC() {}

    static int irq2int(int i) { return i + HARD_INT; }
    static int int2irq(int i) { return i - HARD_INT; }

    static void remap(Log_Addr addr = LOCAL_APIC_LOG_ADDR) {
	_base = addr;
    }

    static void enable() {
	Reg32 v = read(SVR);
	v |= SVR_APIC_ENABLED;
	write(SVR, v);
    }
    static void enable(int i) { enable(); }

    static void disable() {
	Reg32 v  = read(SVR);
	v &= ~SVR_APIC_ENABLED;
	write(SVR, v);
    }
    static void disable(int i) { disable(); }

    static Reg32 read(unsigned int reg) {
	return *static_cast<volatile Reg32 *>(_base + reg);
    }
    static void write(unsigned int reg, Reg32 v) {
	*static_cast<volatile Reg32 *>(_base + reg) = v;
    }

    static int id() {
	return (read(ID) & ID_MASK) >> ID_SHIFT;
    }
    static int version() {
	return read(VERSION);
    }

    static void ipi_init() {
	// Broadcast INIT IPI to all APs excluding self
	write(ICR0_31, ICR_OTHERS | ICR_LEVEL | ICR_ASSERT | ICR_INIT);
 	while((read(ICR0_31) & ICR_PENDING));
    }

    static void ipi_start(Log_Addr entry);

    static void reset(Log_Addr addr = LOCAL_APIC_LOG_ADDR) {
	// APIC must be on very early in the boot process, so it is
	// subject to memory remappings. We also cannot be sure about 
	// global constructors here
	remap(addr);
	if(Traits<Thread>::smp) {
	    clear();
	    enable();
	    connect();
	} else
	    disable();
    }

    static void eoi() { // End of interrupt
    	APIC::write(APIC::EOI, 0);
    }

    static void config_timer(Reg32 count, bool interrupt, bool periodic) {
	Reg32 v = INT_TIMER;
	v |= (interrupt) ? 0 : TIMER_MASKED;
	v |= (periodic) ? TIMER_PERIODIC : 0;
	write(TIMER_INITIAL, count / 16);
	write(TIMER_PRESCALE, TIMER_PRESCALE_BY_16);
	reset_timer();
	write(LVT_TIMER, v);
    }

    static void enable_timer() { 
	write(LVT_TIMER, read(LVT_TIMER) & ~TIMER_MASKED);
    }
    static void disable_timer() { 
	write(LVT_TIMER, read(LVT_TIMER) | TIMER_MASKED);
    }

    static Reg32 read_timer() {
	return read(TIMER_CURRENT);
    }
    
    static void reset_timer() {
	disable();
	write(TIMER_CURRENT, read(TIMER_INITIAL));
	enable();
    }

private:
    static int maxlvt()	{
	Reg32 v = read(VERSION);
	// 82489DXs do not report # of LVT entries
	return (v & 0xf) ? (v >> 16) & 0xff : 2;
    }

    static void clear() {
	int lvts = maxlvt();

	// Masking an LVT entry on a P6 can trigger a local APIC error
	// if the vector is zero. Mask LVTERR first to prevent this
	if(lvts >= 3)
	    write(LVT_ERROR, 1 | LVT_MASKED); // any non-zero vector

	// Careful: we have to set masks only first to deassert
	// any level-triggered sources
	write(LVT_TIMER, read(LVT_TIMER) | LVT_MASKED);
	write(LVT_LINT0, read(LVT_LINT0) | LVT_MASKED);
	write(LVT_LINT1, read(LVT_LINT1) | LVT_MASKED);
	if(lvts >= 4) {
	    write(LVT_PERF, read(LVT_PERF) | LVT_MASKED);
	    write(LVT_THERMAL, read(LVT_THERMAL) | LVT_MASKED);
	}

	// Clean APIC state
	write(LVT_TIMER, LVT_MASKED);
	write(LVT_LINT0, LVT_MASKED);
	write(LVT_LINT1, LVT_MASKED);
	if(lvts >= 3)
	    write(LVT_ERROR, LVT_MASKED);
	if(lvts >= 4) {
	    write(LVT_PERF, LVT_MASKED);
	    write(LVT_THERMAL, LVT_MASKED);
	}
	if(read(VERSION) & 0xf) { // !82489DX
	    if(lvts > 3)
		write(ESR, 0);
	    read(ESR);
	}
    }

    static void connect()	{
	CPU::out8(0x70, 0x22);
	CPU::out8(0x01, 0x23);
    }

    static void disconnect()	{
	CPU::out8(0x70, 0x22);
	CPU::out8(0x00, 0x23);
    }

private:
    static Log_Addr _base;
};

// PC_IC uses i8259A on single-processor machines and the APIC timer on MPs
class PC_IC: public IC_Common, 
	     private IF<Traits<Thread>::smp, APIC, i8259A>::Result
{
private:
    typedef IF<Traits<Thread>::smp, APIC, i8259A>::Result Base;

    typedef CPU::Reg32 Reg32;
    typedef CPU::Log_Addr Log_Addr;

public:

    using Base::INT_TIMER;

public:
    PC_IC() {}

    static Interrupt_Handler int_vector(Interrupt_Id i) {
	return (i < INTS) ? _int_vector[i] : 0;
    }

    static void int_vector(Interrupt_Id i, Interrupt_Handler h) {
	db<IC>(INF) << "IC::int_vector(int=" << i << ",h=" 
		    << (void *)h <<")\n";
	if(i < INTS) _int_vector[i] = h;
    }

    static void enable() {
	db<IC>(INF) << "IC::enable()\n";
	Base::enable();
    }

    static void enable(int i) {
	db<IC>(INF) << "IC::enable(int=" << i << ")\n";
	Base::enable(i);
    }

    static void disable() {
	db<IC>(INF) << "IC::disable()\n";
	Base::disable();
    }

    static void disable(int i) {
	db<IC>(INF) << "IC::disable(int=" << i << ")\n";
	Base::disable(i);
    }

    using Base::eoi;
    using Base::irq2int;
    using Base::int2irq;

    static void init();

private:
    static void int_dispatch();

    static void int_not(Interrupt_Id i);
    static void exc_not(Interrupt_Id i,
			Reg32 error, Reg32 eip, Reg32 cs, Reg32 eflags);
    static void exc_pf (Interrupt_Id i,
			Reg32 error, Reg32 eip, Reg32 cs, Reg32 eflags);
    static void exc_gpf(Interrupt_Id i,
			Reg32 error, Reg32 eip, Reg32 cs, Reg32 eflags);
    static void exc_fpu(Interrupt_Id i,
			Reg32 error, Reg32 eip, Reg32 cs, Reg32 eflags);

private:
    static Interrupt_Handler _int_vector[INTS];
};

__END_SYS

#endif
