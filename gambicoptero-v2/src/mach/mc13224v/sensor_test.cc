// EPOS MC13224V_Sensor Test Program

#include <sensor.h>
#include <alarm.h>

__USING_SYS

bool is_button_pressed()
{
    typedef IO_Map<MC13224V> IO;

    // Check if GPIO_11 (EPOSMote-Startup button 1) is set (button is pressed)
    CPU::out32(IO::GPIO_BASE, CPU::in32(IO::GPIO_BASE) & ~(1 << 11)); // is input
    CPU::out32(IO::GPIO_DATA_SEL0, CPU::in32(IO::GPIO_DATA_SEL0) &  ~(1 << 11)); // input from the pad
    CPU::out32(IO::GPIO_FUNC_SEL0, CPU::in32(IO::GPIO_FUNC_SEL0) & ~(0x3 << (11*2))); // all as GPIO

    return (CPU::in32(IO::GPIO_DATA0) & (1 << 11)); // read pin state
}

int main()
{
    OStream cout;

    cout << "MC13224V_Sensor test\n";

    cout << "Testing MC13224V_Temperature... Hit the push_button to stop.\n";

    Temperature_Sensor temperature;

    while(!is_button_pressed())
    {
        Alarm::delay(1000000);
        cout << "Temperature now is: " << temperature.sample() << " Celcius\n";
    }

    return 0;
}
