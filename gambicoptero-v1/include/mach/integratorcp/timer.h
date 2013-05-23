#ifndef __integrator_timer_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __integrator_timer_h

#include <timer.h>
#include <ic.h>

__BEGIN_SYS


class IntegratorCP_Timer: public Timer_Common
{
protected:
    static const CPU::Reg8 IRQ            = IC::TIMERINT0;
    
    static const CPU::Reg32 TIMER_LOAD    = 0x13000000L;            
    static const CPU::Reg32 TIMER_VALUE   = 0x13000004L;
    static const CPU::Reg32 TIMER_CONTROL = 0x13000008L;
    static const CPU::Reg32 TIMER_CLEAR   = 0x1300000CL;
    
    static const CPU::Reg8 BIT_ENABLE    = 7;
    static const CPU::Reg8 BIT_MODE      = 6; // 0 = free, 1 = periodic
    static const CPU::Reg8 BIT_IE        = 5; // interrupt enable
    static const CPU::Reg8 BIT_R         = 4; // unused
    static const CPU::Reg8 BIT_PRESCALE1 = 3;
    static const CPU::Reg8 BIT_PRESCALE0 = 2;
    static const CPU::Reg8 BIT_SIZE      = 1; // 0 = 16bit, 1 = 32bit
    static const CPU::Reg8 BIT_ONESHOT   = 0; // 1 = oneshot
   
   void load(CPU::Reg32 val) {
        CPU::out32(TIMER_LOAD | (_channel * 0x100) , val);
   }
   
   CPU::Reg32 load() {
        return CPU::in32(TIMER_LOAD | (_channel * 0x100));
   }
   
   void value(CPU::Reg32 val) {
        CPU::out32(TIMER_VALUE | (_channel * 0x100) , val);
   }
   
   CPU::Reg32 value() {
        return CPU::in32(TIMER_VALUE | (_channel * 0x100));
   }
   
   void control(CPU::Reg32 val) {
        CPU::out32(TIMER_CONTROL | (_channel * 0x100) , val);
   }
   
   CPU::Reg32 control() {
        return CPU::in32(TIMER_CONTROL | (_channel * 0x100));
   }
   
   CPU::Reg8 irq() { return IRQ + _channel; }
  
   static Handler * _handler[3];

public:
    typedef unsigned long Hertz;   
    typedef short Channel;
    enum {
        TSC,
        ALARM,
        SCHEDULER
    };
    
    static const unsigned int CLOCK = Traits<Machine>::CLOCK;
    static const unsigned int FREQUENCY = 1000000;
    
    IntegratorCP_Timer(const Hertz & freq,
         const Handler * hand,const Channel & channel) :
         _channel(channel)
    {
        db<Timer>(TRC) << "Timer(chan="<<channel<<
                          ",hdl="<<(void*)hand<<
                          ",freq="<<freq<<")\n";
        
        frequency(freq);
        handler(hand);
        
        CPU::Reg32 c = control();
        
        c |= (1UL << BIT_MODE); // set periodic
        c |= (1UL << BIT_SIZE); // 32bit counter
        c |= (1UL << BIT_ENABLE); 
        
        control(c);
        enable();
        
    }

    void handler(Handler * hand) {
        int intr = irq(); 
        IC::int_vector(intr, &IntegratorCP_Timer::int_handler);   
        _handler[_channel] = hand;
        IC::enable(intr);
    }

    Hertz frequency() { 
        if (_channel == 0)
            return CLOCK / load();
        else
            return FREQUENCY / load();
    }
    
    void frequency(const Hertz & f) {
        db<Timer>(TRC) << "Timer_"<<_channel<<"::frequency(f="<<f<<")\n";
        if (_channel == 0)
            load(CLOCK / f);
        else
            load(FREQUENCY / f);
    };

    static void init() {}

    static void int_handler(IC::Interrupt_Id id);

    void reset() {
        db<IC>(TRC) << "Timer_"<<_channel<<"::reset()\n";
        value(load());
    }
    
    void enable(){
        db<IC>(TRC) << "Timer_"<<_channel<<"::enable()\n";
        control(control() | (1L << BIT_ENABLE));
    }

    void disable() {
        db<IC>(TRC) << "Timer_"<<_channel<<"::disable()\n";
        control(control() & ~(1L << BIT_ENABLE));
    }

    Tick read() {
        return value();
    }

private:
    Channel _channel;

};

// Timer used by Alarm
class Alarm_Timer: public IntegratorCP_Timer
{
public:
    static const unsigned int FREQUENCY = 10;

public:
    Alarm_Timer(const Handler * handler):
    IntegratorCP_Timer(FREQUENCY, handler, ALARM) {}
};


// Timer used by Thread::Scheduler
class Scheduler_Timer: public IntegratorCP_Timer
{
private:
    typedef unsigned long Microsecond;

public:
    Scheduler_Timer(const Microsecond & quantum, const Handler * handler): 
    IntegratorCP_Timer(1000000 / quantum, handler, SCHEDULER) {}
};

// Timer used by ARM7_TSC
class TSC_Timer: public IntegratorCP_Timer
{
public:
    TSC_Timer(const Hertz & freq,const Handler * h) 
    : IntegratorCP_Timer(1, h, TSC) {}
};

__END_SYS

#endif

