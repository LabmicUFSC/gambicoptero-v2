// EPOS-- MC13224V Mediator Implementation

#include <mach/mc13224v/gpio_pin.h>

__BEGIN_SYS

MC13224V_GPIO_Pin::MC13224V_GPIO_Pin(int pin, GPIO_Functions func)
  : _pin(pin)
{
    function(_pin, func);
}

// Set pin direction as input
void MC13224V_GPIO_Pin::input(int pin)
{
    int bit = pin % 32;
    int reg = (pin >> 5);

    // Set as input
    CPU::out32(IO::GPIO_PAD_DIR0 + (reg << 2),
                   CPU::in32(IO::GPIO_PAD_DIR0 + (reg << 2))
                    & ~(1 << bit)
    );
}

// Set pin direction as output
void MC13224V_GPIO_Pin::output(int pin)
{
    int bit = pin % 32;
    unsigned int reg = IO::GPIO_PAD_DIR0 + ((pin >> 5) << 2);

    // Set as output
    CPU::out32(reg, CPU::in32(reg) | (1 << bit));
}

// Set pin function
void MC13224V_GPIO_Pin::function(int pin, GPIO_Functions func)
{
    int bit = (pin % 16) << 1;
    unsigned int reg = IO::GPIO_FUNC_SEL0 + ((pin >> 4) << 2);

    // Set function
    unsigned int mask = CPU::in32(reg) & ~(0x3 << bit);
    mask |= (func << bit);
    CPU::out32(reg,  mask);
}

// Set pin to High
void MC13224V_GPIO_Pin::set(int pin)
{
    int bit = pin % 32;
    int reg = (pin >> 5);

    output(pin);
    CPU::out32(IO::GPIO_DATA_SET0 + (reg << 2), (1 << bit));
}

// Set pin to Low
void MC13224V_GPIO_Pin::clear(int pin)
{
    int bit = pin % 32;
    int reg = (pin >> 5);

    output(pin);
    CPU::out32(IO::GPIO_DATA_RESET0 + (reg << 2), (1 << bit));
}

// Read pin status
bool MC13224V_GPIO_Pin::get(int pin)
{
    int bit = pin % 32;
    int reg = IO::GPIO_DATA0 + ((pin >> 5) << 2);

    // Get value
    input(pin);
    return ((1 << bit) & CPU::in32(reg));
}

__END_SYS
