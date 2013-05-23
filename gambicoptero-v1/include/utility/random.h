// EPOS Random Number Generator Utility Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __random_h
#define __random_h

#include <system/config.h>

__BEGIN_SYS

// Implementation of http://en.wikipedia.org/wiki/Linear_congruential_generator
// with A = 2 and variable C on a simplification of:
// X1 = aX0 + c
// as X1 = (X0 << 1) xor C

class Pseudo_Random
{
private:
	static const unsigned long int A = 1103515245;
	static const unsigned long int C = 12345;
	static const unsigned long int M = 4294967295U;

public:
    Pseudo_Random() {}
	
    static unsigned long int random() {
	    //_seed = (_seed << 1) ^ n;
    	_seed = ((_seed * A) + C) % M;
	    return _seed;
    }

    static void seed(unsigned long int value) {
    	_seed = value;
    }

private:
    static unsigned long int _seed;
};

__END_SYS

#endif
