// EPOS MTS3o0 Sensopr Board Implementation

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
