// EPOS MC13224V_One_Wire Mediator Declarations

#ifndef _MC13224V_One_Wire_H
#define _MC13224V_One_Wire_H

#include <gpio_pin.h>

__BEGIN_SYS

class MC13224V_One_Wire
{
private:
    MC13224V_One_Wire();

public:
    MC13224V_One_Wire(int pin);
    ~MC13224V_One_Wire();

    void enable();
    void disable();

	void put(char c);
	char get();

    static void init() {}

private:
    GPIO_Pin _pin;


    // Set the 1-Wire timing "Standard Speed"
    // All constants bellow are calibrated for MC13224V running at 24MHz
    static const unsigned int A = 1;
    static const unsigned int B = 12;
    static const unsigned int C = 6;
    static const unsigned int D = 1;
    static const unsigned int E = 1;
    static const unsigned int F = 11;
    static const unsigned int G = 0;
    static const unsigned int H = 49;
    static const unsigned int I = 7;
    static const unsigned int J = 41;

    static void wait10us(unsigned int rep); // used for C, D, E, H, I, J
    static void wait6us(unsigned int rep);  // used for A, B, F

    void put_bit(bool bit);
    bool get_bit();

    bool reset();

    void write_bit(bool bit);
    bool read_bit();
};
__END_SYS

#endif
