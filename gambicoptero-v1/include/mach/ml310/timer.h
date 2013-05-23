// EPOS ML310 Timer Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __ml310_timer_h
#define __ml310_timer_h

#include <cpu.h>
#include <ic.h>
#include <timer.h>

__BEGIN_SYS

class ML310_Timer:  public Timer_Common
{
private:
    // PPC405_Timer private imports, types and constants
    static const int CLOCK = Traits<ML310>::CLOCK;
    typedef CPU::Reg32 Count;

public:
    //PowerPC 405 Timer SPRs
    enum {
      SPR_TSR = 0x3D8,
      SPR_TCR = 0x3DA,
      SPR_PIT = 0x3DB
    };


public:
    ML310_Timer(int unit = 0) {} // actual initialization is up to init
    ~ML310_Timer() {}
    Hertz frequency() {  return cnt2freq(_count); }
    void frequency(const Hertz & f) {
        _count = freq2cnt(f);
        reset();
        db<ML310_Timer>(INF) << "ML310_Timer::resolution(res=" << frequency()
                             << ",cnt=" << _count << ")\n";
    }

    void enable() { //Verify latter ...
        PPC32::int_disable();
        volatile PPC32::Reg32 value;
        value = PPC32::_mfspr(SPR_TCR);
        value |= 0x04400000;
        PPC32::_mtspr(SPR_TCR, value);
        PPC32::int_enable();
    }

    void disable() { //Verify latter ...
        PPC32::int_disable();
        volatile PPC32::Reg32 value;
        value = PPC32::_mfspr(SPR_TCR);
        value &= ~0x04000000;
        PPC32::_mtspr(SPR_TCR, value);
        PPC32::int_enable();
    }

    void reset() {
        PPC32::_mtspr(SPR_PIT, _count);
    }

    static void init();

private:
    // ML310_Timer implementation methods
    static Hertz cnt2freq(unsigned int c) { return CLOCK / c; }
    static unsigned int freq2cnt(const Hertz & f) { return CLOCK / f; }

private:
    // ML310_Timer attributes
    Count _count;

};

__END_SYS

#endif
