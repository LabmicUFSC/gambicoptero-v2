// EPOS MTS3o0 Sensopr Board Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/atmega128/mts300.h>

__BEGIN_SYS

// This is a very rough approximation funcion, but it works fine
// in the range of 0-50 Celsius Degrees.	
int MTS300_Temperature::convert_int(int v) {

    return ((v >> 2) - 51) / 3;

}

float MTS300_Temperature::convert_float(int v) {
    return ((v >> 2) - 51) / 3;

}

int MTS300_Photo::convert_int(int v) {

    return v / 10;

}

float MTS300_Photo::convert_float(int v) {

    return v / (float)1023;

}





__END_SYS
