// EPOS CPU Mediator Common Package

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __cpu_h
#define __cpu_h

#include <system/config.h>

__BEGIN_SYS

class CPU_Common
{	
protected:
    CPU_Common() {}

public:
    typedef unsigned char Reg8;
    typedef unsigned short Reg16;
    typedef unsigned long Reg32;
    typedef unsigned long long Reg64;

    class Log_Addr {
    public:
        Log_Addr() {}
        Log_Addr(const Log_Addr & a) : _addr(a._addr) {}
        Log_Addr(unsigned int a) : _addr(a) {}
        template <typename T>
        Log_Addr(T * a) : _addr(reinterpret_cast<unsigned int>(a)) {}

        operator unsigned int() const { return _addr; }

        template <typename T>
        operator T *() const { return reinterpret_cast<T *>(_addr); }

        template <typename T>
        bool operator==(T a) const { return (_addr == (unsigned int)a); }
        template <typename T>
        bool operator< (T a) const { return (_addr < (unsigned int)a); }
        template <typename T>
        bool operator> (T a) const { return (_addr > (unsigned int)a); }
        template <typename T>
        bool operator>=(T a) const { return (_addr >= (unsigned int)a); }
        template <typename T>
        bool operator<=(T a) const { return (_addr <= (unsigned int)a); }

        template <typename T>
        Log_Addr operator-(T a) const { return _addr - (unsigned int)a; }
        template <typename T>
        Log_Addr operator+(T a) const { return _addr + (unsigned int)a; }
        template <typename T>
        Log_Addr & operator+=(T a) { _addr += a; return *this; }
        template <typename T>
        Log_Addr & operator-=(T a) { _addr -= a; return *this; }
        template <typename T>
        Log_Addr & operator&=(T a) { _addr &= a; return *this; }

        friend Debug & operator << (Debug & db, Log_Addr a)
        { db << (void *)a._addr; return db; }

    private:
        unsigned int _addr;
    };
    typedef Log_Addr Phy_Addr;

    typedef unsigned long Hertz;

    class Context;

public:
    static void halt() { for(;;); }

    static bool tsl(volatile bool & lock) {
        bool old = lock;
        lock = 1;
        return old;
    }

    static int finc(volatile int & value) {
        int old = value;
        value++;
        return old;
    }

    static int fdec(volatile int & value) {
        int old = value;
        value--;
        return old;
    }

    static int cas(volatile int & value, int compare, int replacement) {
        int old = value;
        if(value == compare) {
            value = replacement;
        }
        return old;
    }

protected:
    static Reg32 swap32(Reg32 v) {
        return (((v << 24) & 0xff000000) | ((v <<  8) & 0x00ff0000) |
                ((v >>  8) & 0x0000ff00) | ((v >> 24) & 0x000000ff));
    }

    static Reg16 swap16(Reg16 v) {
        return ((v << 8) & 0xFF00) | ((v >> 8) & 0x00FF);
    }

};

__END_SYS

#ifdef __CPU_H
#include __CPU_H
#endif

#endif
