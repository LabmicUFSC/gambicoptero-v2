#include <machine.h>
#include <alarm.h>
#include <timer.h>
#include <display.h>

__USING_SYS

class ZFSM_201_1 {
public:
    typedef MC13224V MCU;
public:
    enum Led {
        LED1 = 23,
        LED2 = 24,
        LED3 = 25
    };

    static void reset() {
        
    }

    static void led_on(Led l) {
        CPU::out32(
            Machine::IO::GPIO_DATA0,
            CPU::in32(Machine::IO::GPIO_DATA0) | (1 << l)
        );        
    }
    
    static void led_off(Led l) {
        CPU::out32(
            Machine::IO::GPIO_DATA0,
            CPU::in32(Machine::IO::GPIO_DATA0) & ~(1 << l)
        );        
    }

    static bool led(Led l) {
        return CPU::in32(Machine::IO::GPIO_DATA0) & (1 << l); 
    }
};

void minha_interrupt();
bool x = true;
MC13224V_Timer_0 t(1, minha_interrupt);
int i = 0;
int j = 1;

void minha_interrupt() {
    if(x) {
        ZFSM_201_1::led_on(ZFSM_201_1::LED1);
        ZFSM_201_1::led_on(ZFSM_201_1::LED2);
        ZFSM_201_1::led_on(ZFSM_201_1::LED3);
    } else {
        ZFSM_201_1::led_off(ZFSM_201_1::LED1);
        ZFSM_201_1::led_off(ZFSM_201_1::LED2);
        ZFSM_201_1::led_off(ZFSM_201_1::LED3);
    }

    if(i == j) { x = false; }
    if(i == 10) { i = 0; x = true; }
    i++;
}

int main() {
    bool fadeIn = true;
    while(1) {
        if(fadeIn) {
            j++;
            if(j == 10) { fadeIn = false; }
        } else {
            j--;
            if(j == 1) { fadeIn = true; }
        }
        Alarm::delay(490000);
    }
}
