#include <system/config.h>
#include <cpu.h>
#include <machine.h>
#include <adc.h>
#include <alarm.h>
#include <flash.h>

__USING_SYS

/************* PROTOTYPES *************/

/* I/O */
void check_config();
void leave_config();
void store_throttle_levels();
void load_throttle_levels();
void io_init();
void frt_led_on();
void frt_led_off();
void rev_led_on();
void rev_led_off();
void set_speed();
void set_motor_output(int mask);
void update_inputs();

// PINS
enum IO_Map {
    CONFIG_Pin = 8, // ADC1_VREFH (REG+4)
    R1_Pin  = 24, // KBI2
    R2_Pin  = 28, // KBI6
    R3_Pin  = 11, // TMR3
    R4_Pin  = 27, // KBI5
    R5_Pin  = 10, // TMR2
    LED_FRT_Pin = 23, // KBI1
    LED_REV_Pin = 25, // KBI3
    REV_Pin = 26  // KBI4
};

static const int speed_map[12] = {
                                  0, // Neutro
                                  (1 << R1_Pin) | (1 << R3_Pin), // Frente 1
                                  (1 << R1_Pin) | (1 << R3_Pin) | (1 << R4_Pin), // Frente 2
                                  (1 << R1_Pin) | (1 << R2_Pin), // Frente 3
                                  (1 << R1_Pin) | (1 << R2_Pin) | (1 << R5_Pin), // Frente 4
                                  (1 << R1_Pin) | (1 << R2_Pin) | (1 << R3_Pin) | (1 << R4_Pin) | (1 << R5_Pin), // Frente 5
                                  (1 << R3_Pin), // RŽ 1
                                  (1 << R3_Pin) | (1 << R4_Pin), // RŽ 2
                                  (1 << R2_Pin), // RŽ 3
                                  (1 << R2_Pin) | (1 << R5_Pin), // RŽ 4
                                  (1 << R2_Pin) | (1 << R3_Pin) | (1 << R4_Pin) | (1 << R5_Pin) // RŽ 5
                                 };

ADC analog_port(3);
static volatile unsigned int throttle;
static volatile unsigned int throttle_min = 4095;
static volatile unsigned int throttle_max = 0;
static volatile unsigned int throttle_levels[5] = {0, 0, 0, 0, 0};
static volatile bool reverse = false;
static volatile bool in_config = false;
static const unsigned int config_time = 10000000; // Microseconds

/* ST */

enum States
{
    ST_OFF = 0,
    ST_STARTING,
    ST_BLINK,
    ST_START,
    ST_FRONT,
    ST_REVERSE
};
static volatile unsigned int state = ST_OFF;

void enter_off();
void enter_starting();
void enter_blink();
void enter_start();
void enter_front();
void during_front();
void enter_reverse();
void during_reverse();
void st_init();
void state_machine();

/************* IMPLEMENTATION *************/

/************* Main Control Loop *************/
static const int control_period = 100000;
int main()
{
    io_init();

    check_config();

    st_init();

    while(true)
    {
        update_inputs();
        state_machine();
        Alarm::delay(control_period);
    }
}

/************* I/O Functions *************/

typedef Machine::IO IO;
volatile unsigned int data0_mask = 0x0;

void check_config()
{
    CPU::out32(IO::GPIO_FUNC_SEL2, CPU::in32(IO::GPIO_FUNC_SEL2)
            | (1 << 16)
    );
    CPU::out32(IO::GPIO_DATA_SEL1, CPU::in32(IO::GPIO_DATA_SEL1)
            & ~(1 << CONFIG_Pin)
    );

    Alarm::delay(10000);

    in_config = !(CPU::in32(IO::GPIO_DATA1) & (1 << CONFIG_Pin));

    Alarm::delay(10000);

    CPU::out32(IO::GPIO_FUNC_SEL2, CPU::in32(IO::GPIO_FUNC_SEL2));

    if(in_config)
    {
        kout << "entering config\n";
        Function_Handler handler(leave_config);
        Alarm alarm(config_time, &handler, 1);

        bool leds_on = true;
        while(in_config)
        {
            if(leds_on)
            {
                frt_led_on();
                rev_led_off();
                leds_on = false;
            }
            else
            {
                frt_led_off();
                rev_led_on();
                leds_on = true;
            }
            throttle = analog_port.get();
            if(throttle > throttle_max) throttle_max = throttle;
            if(throttle < throttle_min) throttle_min = throttle;
            Alarm::delay(control_period);
        }

        float range = throttle_max - throttle_min;
        throttle_levels[0] = throttle_min + (range * 0.244f);
        throttle_levels[1] = throttle_min + (range * 0.415f);
        throttle_levels[2] = throttle_min + (range * 0.586f);
        throttle_levels[3] = throttle_min + (range * 0.757f);
        throttle_levels[4] = throttle_min + (range * 0.928f);

        kout << "New throttle levels:\n";
        kout << "1: " << throttle_levels[0] << "\n";
        kout << "2: " << throttle_levels[1] << "\n";
        kout << "3: " << throttle_levels[2] << "\n";
        kout << "4: " << throttle_levels[3] << "\n";
        kout << "5: " << throttle_levels[4] << "\n";

        store_throttle_levels();
    }

    load_throttle_levels();

    kout << "Throttle levels in use:\n";
    kout << "1: " << throttle_levels[0] << "\n";
    kout << "2: " << throttle_levels[1] << "\n";
    kout << "3: " << throttle_levels[2] << "\n";
    kout << "4: " << throttle_levels[3] << "\n";
    kout << "5: " << throttle_levels[4] << "\n";
}

void leave_config()
{
    in_config = false;
}

void store_throttle_levels()
{
    Flash::erase(Flash::address2sector(Flash::DATA_BASE));
    Flash::write(
            Flash::DATA_BASE,
            (char*) throttle_levels,
            5 * sizeof(unsigned int)
    );
}

void load_throttle_levels()
{
    Flash::read(
            Flash::DATA_BASE,
            (char*) throttle_levels,
            5 * sizeof(unsigned int)
    );
}

void io_init()
{
    // Config direction
    // All pull-down by default
    // All pull-down enabled by default for all inputs
    CPU::out32(IO::GPIO_PAD_DIR0, 0u // all as input, but:
           | (1 << R1_Pin) | (1 << R2_Pin) | (1 << R3_Pin)
           | (1 << R4_Pin) | (1 << R5_Pin)
           | (1 << LED_FRT_Pin) | (1 << LED_REV_Pin)
    );
    CPU::out32(IO::GPIO_DATA_SEL0, CPU::in32(IO::GPIO_DATA_SEL0)
            & ~(1 << REV_Pin)
    );

    set_motor_output(speed_map[0]);
    rev_led_off();
    frt_led_off();
}
void frt_led_on()
{
    data0_mask |= (1 << LED_FRT_Pin);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void frt_led_off()
{
    data0_mask &= ~(1 << LED_FRT_Pin);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void rev_led_on()
{
    data0_mask |= (1 << LED_REV_Pin);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void rev_led_off()
{
    data0_mask &= ~(1 << LED_REV_Pin);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void set_motor_output(int mask)
{
    data0_mask &= ~((1 << R1_Pin) | (1 << R2_Pin) | (1 << R3_Pin) | (1 << R4_Pin) | (1 << R5_Pin));
    data0_mask |= mask;
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void set_speed()
{
    for(int i = 4; i >= 0; i--)
    {
        if(throttle > throttle_levels[i])
        {
            set_motor_output(speed_map[(i+1) + (reverse? 5 : 0)]);
            return;
        }
    }
    set_motor_output(speed_map[0]);
}


void update_inputs()
{
    throttle = analog_port.get();
    reverse = CPU::in32(IO::GPIO_DATA0) & (1 << REV_Pin);
}

/************* State Transition Functions *************/

void enter_off()
{
    state = ST_OFF;
    rev_led_off();
    frt_led_off();
    set_motor_output(speed_map[0]);
}

void enter_starting()
{
    state = ST_STARTING;
    rev_led_on();
    frt_led_on();
    set_motor_output(speed_map[0]);
}

void enter_blink()
{
    state = ST_BLINK;
    rev_led_off();
    frt_led_off();
}

void enter_start()
{
    state = ST_START;
    rev_led_off();
    frt_led_off();
}

void enter_front()
{
    state = ST_FRONT;
    rev_led_off();
    frt_led_on();
    set_motor_output(speed_map[0]);
}

void enter_reverse()
{
    state = ST_REVERSE;
    frt_led_off();
    rev_led_on();
    set_motor_output(speed_map[0]);
}

void during_front()
{
    set_speed();
}

void during_reverse()
{
    set_speed();
}

/************* State Machine *************/
void st_init()
{
    enter_off();
}

void state_machine()
{
    switch(state)
    {
    case ST_OFF:
        enter_starting();
        break;

    case ST_STARTING:
        if(throttle < throttle_levels[0])
            enter_start();
        else
            enter_blink();
        break;

    case ST_BLINK:
        enter_starting();
        break;

    case ST_START:
        if(reverse)
            enter_reverse();
        else
            enter_front();
        break;

    case ST_FRONT:
        if(reverse)
            enter_starting();
        during_front();
        break;

    case ST_REVERSE:
        if(!reverse)
            enter_starting();
        during_reverse();
        break;

    }
}
