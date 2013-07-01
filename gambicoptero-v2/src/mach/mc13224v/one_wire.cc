// EPOS MC13224V_One_Wire Mediator Implementation

#include <machine.h>
#include <mach/mc13224v/one_wire.h>

__BEGIN_SYS

MC13224V_One_Wire::MC13224V_One_Wire(int pin)
 : _pin(pin)
{
    db<MC13224V_One_Wire>(TRC) << "MC13224V_One_Wire::MC13224V_One_Wire()\n";

}

MC13224V_One_Wire::~MC13224V_One_Wire()
{
    db<MC13224V_One_Wire>(TRC) << "MC13224V_One_Wire::~MC13224V_One_Wire()\n";

}

void MC13224V_One_Wire::enable()
{}

void MC13224V_One_Wire::disable()
{}

void MC13224V_One_Wire::put(char c)
{
    db<MC13224V_One_Wire>(TRC) << "void MC13224V_One_Wire::put(" << c << ")\n";

    // Write each bit, LSB first
    for (int i = 0; i < 8; i++)
    {
        put_bit(c & 0x01);
        c >>= 1;
    }
}

char MC13224V_One_Wire::get()
{
    db<MC13224V_One_Wire>(TRC) << "char MC13224V_One_Wire::get()\n";

    char ret = 0;
    for (int i = 0; i < 8; i++)
    {
        ret >>= 1;
        // if result is one, then set MS bit
        if (get_bit())
            ret |= 0x80;
    }
    return ret;
}

// This function generates a pause of ~10 us.
// It is used for pauses:
// C:  60us, rep=6;
// D:  10us, rep=1;
// E:   9us, rep=1;
// H: 480us, rep=49;
// I:  70us, rep=7;
// J: 410us, rep=41.
void MC13224V_One_Wire::wait10us(unsigned int rep)
{
    for (volatile unsigned int i = 0; i < rep*0x11; i++);
}

// This function generates a pause of ~6us.
// It is used for the pauses:
// A:  6us, rep=1;
// B: 64us, rep=12;
// F: 55us, rep=11.
void MC13224V_One_Wire::wait6us(unsigned int rep)
{
    for (volatile unsigned int i = 0; i < rep*0x9; i++);
}

// Write a One-Wire write bit. Provide 10us recovery time.
void MC13224V_One_Wire::put_bit(bool bit)
{
    if (bit)
    {
        // Write '1' bit
        _pin.clear();
        wait6us(A);
        _pin.set();
        wait6us(B); // Complete the time slot and 10us recovery
    }
    else
    {
        // Write '0' bit
        _pin.clear();
        wait10us(C);
        _pin.set();
        wait10us(D);
    }
}

// Read a One-Wire bit and return it. Provide 10us recovery time.
bool MC13224V_One_Wire::get_bit()
{
    int ret;

    _pin.clear();
    wait6us(A);
    _pin.set();
    wait10us(E);
    ret = _pin.get() & 0x01; // Sample the bit value from the slave
    wait6us(F); // Complete the time slot and 10us recovery

    return ret;
}

// Generate a One-Wire reset.
// Return 1 if no presence detect was found,
// return 0 otherwise.
bool MC13224V_One_Wire::reset()
{
    _pin.set();
    wait10us(G);
    _pin.clear();
    wait10us(H);
    _pin.set();
    wait10us(I);
    int ret = _pin.get() ^ 0x01; // Sample for presence pulse from slave
    wait10us(J);

    return ret;
}

__END_SYS
