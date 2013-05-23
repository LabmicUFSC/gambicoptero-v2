// EPOS SHT11 Sensor Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/atmega1281/sht11.h>
#include <alarm.h>

__BEGIN_SYS

unsigned char SHT11::write_byte(unsigned char value) {
    unsigned char i     = 0x80;
    unsigned char error = 0;

    make_data_output(); 
    ASMV("nop"::);

    while (i) { 
        if (i & value)
            set_data1();
        else  
            set_data0();

        set_sck1(); 
        ASMV("nop"::);
        ASMV("nop"::);
        ASMV("nop"::);
        set_sck0();
        ASMV("nop"::);
        ASMV("nop"::);
        ASMV("nop"::);

        i = (i >> 1);
    }

    make_data_input();
    pull_up();
    set_sck1(); 
    ASMV("nop"::);
    ASMV("nop"::);
    ASMV("nop"::);

    if (data_ready())
        error = 1;

    ASMV("nop"::);
    set_sck0();

    return error; 
}

unsigned char SHT11::read_byte(unsigned char ack) {
    unsigned char i   = 0x80;
    unsigned char val = 0;

    make_data_input(); 
    pull_up();

    while (i) { 
        set_sck1(); 
        ASMV("nop"::);

        if (data_ready())
            val = (val | i); 

        set_sck0();
        ASMV("nop"::);
        i= (i >> 1);
    } 

    make_data_output(); 

    if (ack)
        set_data0();
    else
        set_data1();

    set_sck1();
    ASMV("nop"::);
    ASMV("nop"::);
    ASMV("nop"::);
    set_sck0();
    ASMV("nop"::);
    make_data_input(); 
    pull_up();

    return val;
}

void SHT11::transmission_start() {
    make_sck_output();
    set_sck0();

    make_data_output(); 
    set_data1();

    ASMV("nop"::);
    set_sck1();
    ASMV("nop"::);
    set_data0();
    ASMV("nop"::);
    set_sck0();
    ASMV("nop"::);
    ASMV("nop"::);
    ASMV("nop"::);
    set_sck1();
    ASMV("nop"::);
    set_data1();
    ASMV("nop"::);
    set_sck0();
    ASMV("nop"::);

    make_data_input(); 
    pull_up();
}

void SHT11::connection_reset() {
    unsigned char i;

    make_sck_output();
    set_sck0();

    make_data_output(); 
    set_data1();

    for (i = 0; i < 9; i++) {
        set_sck1();
        ASMV("nop"::);
        ASMV("nop"::);
        ASMV("nop"::);
        set_sck0();
        ASMV("nop"::);
        ASMV("nop"::);
        ASMV("nop"::);
    }

    transmission_start(); 
}

unsigned char SHT11::soft_reset() {
    connection_reset(); 

    return write_byte(RESET); 
}

unsigned char SHT11::measure(unsigned int *p_value, unsigned char mode) {
    unsigned char i = 0;

    transmission_start(); 
    *p_value = 0;

    if (write_byte(mode)) return 1;

    while (i < 240) {
        Alarm::delay(3000);
        if (data_ready() == 0) {
            i = 0;
            break;
        }
        i++;
    }

    if (i) return 3;

    i = read_byte(ACK); 
    *p_value = (i << 8) | read_byte(NO_ACK); 

    return 0;
}

int SHT11_Temperature::convert_int(int value) {
    return value * 0.01 -39.6;
}

float SHT11_Temperature::convert_float(int value) {
    return value * 0.01 -39.6;
}

unsigned int SHT11_Humidity::convert_int(int value) {
    return -4.0 + 0.0405 * value - 0.0000028 * value * value;
}

float SHT11_Humidity::convert_float(int value) {
    return -4.0 + 0.0405 * value - 0.0000028 * value * value;
}

__END_SYS

