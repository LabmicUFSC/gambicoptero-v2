#include <system/config.h>
#include <cpu.h>
#include <machine.h>
#include <adc.h>
#include <alarm.h>

__USING_SYS

/************* PROTOTYPES *************/

/* I/O */
void io_init();
void turn_led1_on();
void turn_led1_off();
void turn_led2_on();
void turn_led2_off();
void turn_mos1_on();
void turn_mos1_off();
void turn_mos2_on();
void turn_mos2_off();
void check_start();
void pwm1_set_dc(unsigned short dc);
void pwm1_start();
void pwm1_stop();
void pwm1_full();
void pwm2_set_dc(unsigned short dc);
void pwm2_start();
void pwm2_stop();
void pwm2_full();

ADC analog_port(3);
volatile unsigned int throttle;
static const unsigned int throttle_min = 2047;
static const unsigned int throttle_max = 4095;
static const unsigned int pwm_max = 4095; // ~5860Hz for 24MHz crystal
//static const unsigned int pwm_max = 1600; // 15KHz for 24MHz crystal
static const unsigned int pwm_min = pwm_max/20; // 5% of pwm_max

volatile bool reverse = false;

void update_inputs();

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
void enter_reverse();
void st_init();
void state_machine();

/************* IMPLEMENTATION *************/

/************* Main Control Loop *************/
static const int control_period = 100000;
int main()
{
    io_init();
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
void io_init()
{
    // Config direction
    // All pull-down by default
    // All pull-down enabled by default for all inputs
    CPU::out32(IO::GPIO_PAD_DIR0, 0u // all as input, but:
           | (1 << 10) // GPIO_10: PWM2
           | (1 << 11) // GPIO_11: PWM1
           | (1 << 23) // GPIO_23: LED1
           | (1 << 25) // GPIO_25: LED2
           | (1 << 27) // GPIO_27: MOSFET1
           | (1 << 28) // GPIO_28: MOSFET2
    );
    CPU::out32(IO::GPIO_DATA_SEL0, CPU::in32(IO::GPIO_DATA_SEL0)
            & ~(1 << 26) // GPIO_26: REV
    );

    turn_led1_off();
    turn_led2_off();
    turn_mos1_off();
    turn_mos2_off();

    pwm1_set_dc(0);
    pwm2_set_dc(0);
}

void turn_led1_on()
{
    data0_mask |= (1 << 23);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void turn_led1_off()
{
    data0_mask &= ~(1 << 23);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void turn_led2_on()
{
    data0_mask |= (1 << 25);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void turn_led2_off()
{
    data0_mask &= ~(1 << 25);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void turn_mos1_on()
{
    data0_mask |= (1 << 28);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void turn_mos1_off()
{
    data0_mask &= ~(1 << 28);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void turn_mos2_on()
{
    data0_mask |= (1 << 27);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void turn_mos2_off()
{
    data0_mask &= ~(1 << 27);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void pwm1_set_dc(unsigned short dc)
{
    unsigned short dc_cmpl = pwm_max - dc;
    CPU::out16(IO::TIMER2_CMPLD1, dc);
    if(CPU::in16(IO::TIMER2_COMP1) == 0)
        CPU::out16(IO::TIMER2_COMP1, dc);
    CPU::out16(IO::TIMER2_CMPLD2, dc_cmpl);
    if(CPU::in16(IO::TIMER2_COMP2) == 0)
        CPU::out16(IO::TIMER2_COMP2, dc_cmpl);
}

void pwm1_start()
{
    //turn off timer2
    CPU::out16(IO::TIMER0_ENBL, CPU::in16(IO::TIMER0_ENBL) & ~(1<< 2));

    CPU::out16(IO::TIMER2_CNTR, 0);
    CPU::out16(IO::TIMER2_SCTRL, 0x7);
    CPU::out16(IO::TIMER2_COMP1, CPU::in16(IO::TIMER2_CMPLD1));
    CPU::out16(IO::TIMER2_COMP2, CPU::in16(IO::TIMER2_CMPLD2));
    CPU::out16(IO::TIMER2_CSCTRL, CPU::in16(IO::TIMER2_CSCTRL) | 0x5);
    CPU::out16(IO::TIMER2_CTRL, 0x3024);

    CPU::out32(IO::GPIO_DATA_SEL0, CPU::in32(IO::GPIO_DATA_SEL0)
            | (1 << 10) // GPIO_10: PWM1
    );
    CPU::out32(IO::GPIO_PAD_DIR0, CPU::in32(IO::GPIO_PAD_DIR0)
            | (1 << 10) // GPIO_10: PWM1
    );
    CPU::out32(IO::GPIO_FUNC_SEL0, CPU::in32(IO::GPIO_FUNC_SEL0)
            | (1 << 20) // GPIO_10: PWM1
    );

    // turn on timer
    CPU::out16(IO::TIMER0_ENBL, CPU::in16(IO::TIMER0_ENBL) | (1 << 2));
}

void pwm1_stop()
{
    CPU::out32(IO::GPIO_FUNC_SEL0, CPU::in32(IO::GPIO_FUNC_SEL0)
            | (1 << 20) // GPIO_10: PWM1
    );
    //turn off timer2
    CPU::out16(IO::TIMER0_ENBL, CPU::in16(IO::TIMER0_ENBL) & ~(1<< 2));
    CPU::out16(IO::TIMER2_SCTRL, 0xf);
    CPU::out16(IO::TIMER2_CTRL, 0x3024);
}

void pwm1_full()
{
    pwm1_stop();
    CPU::out32(IO::GPIO_FUNC_SEL0, CPU::in32(IO::GPIO_FUNC_SEL0)
            & ~(1 << 20) // GPIO_10: PWM1
    );
    data0_mask |= (1 << 10);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void pwm2_set_dc(unsigned short dc)
{
    unsigned short dc_cmpl = pwm_max - dc;
    CPU::out16(IO::TIMER3_CMPLD1, dc);
    if(CPU::in16(IO::TIMER3_COMP1) == 0)
        CPU::out16(IO::TIMER3_COMP1, dc);
    CPU::out16(IO::TIMER3_CMPLD2, dc_cmpl);
    if(CPU::in16(IO::TIMER3_COMP2) == 0)
        CPU::out16(IO::TIMER3_COMP2, dc_cmpl);
}

void pwm2_start()
{
    //turn off timer2
    CPU::out16(IO::TIMER0_ENBL, CPU::in16(IO::TIMER0_ENBL) & ~(1<< 3));

    CPU::out16(IO::TIMER3_CNTR, 0);
    CPU::out16(IO::TIMER3_SCTRL, 0x7);
    CPU::out16(IO::TIMER3_COMP1, CPU::in32(IO::TIMER3_CMPLD1));
    CPU::out16(IO::TIMER3_COMP2, CPU::in32(IO::TIMER3_CMPLD2));
    CPU::out16(IO::TIMER3_CSCTRL, CPU::in32(IO::TIMER3_CSCTRL) | 0x5);
    CPU::out16(IO::TIMER3_CTRL, 0x3024);

    CPU::out32(IO::GPIO_DATA_SEL0, CPU::in32(IO::GPIO_DATA_SEL0)
                | (1 << 11) // GPIO_11: PWM2
    );
    CPU::out32(IO::GPIO_PAD_DIR0, CPU::in32(IO::GPIO_PAD_DIR0)
            | (1 << 11) // GPIO_11: PWM2
    );
    CPU::out32(IO::GPIO_FUNC_SEL0, CPU::in32(IO::GPIO_FUNC_SEL0)
            | (1 << 22) // GPIO_11: PWM2
    );

    // turn on timer
    CPU::out16(IO::TIMER0_ENBL, CPU::in16(IO::TIMER0_ENBL) | (1 << 3));
}

void pwm2_stop()
{
    CPU::out32(IO::GPIO_FUNC_SEL0, CPU::in32(IO::GPIO_FUNC_SEL0)
            | (1 << 22) // GPIO_11: PWM2
    );
    //turn off timer2
    CPU::out16(IO::TIMER0_ENBL, CPU::in16(IO::TIMER0_ENBL) & ~(1<< 3));
    CPU::out16(IO::TIMER3_SCTRL, 0xf);
    CPU::out16(IO::TIMER3_CTRL, 0x3024);
}

void pwm2_full()
{
    pwm2_stop();
    CPU::out32(IO::GPIO_FUNC_SEL0, CPU::in32(IO::GPIO_FUNC_SEL0)
            & ~(1 << 22) // GPIO_11: PWM2
    );
    data0_mask |= (1 << 11);
    CPU::out32(IO::GPIO_DATA0, data0_mask);
}

void update_inputs()
{
    static volatile bool is_stoped = false;
    static volatile bool is_full = false;
    throttle = analog_port.get();
    // ajustar 'pwm' aqui se precisar adaptar frequncia do pwm
    unsigned int pwm = throttle; // ~5.8 KHz
//    unsigned int pwm = throttle * ((float)pwm_max / (float)throttle_max); // 15 KHz
    if(pwm < pwm_min)
    {
        if(!is_stoped)
        {
            is_stoped = true;
            is_full = false;
            pwm1_stop();
            pwm2_stop();
        }
    }
    else if((pwm_max - pwm) < pwm_min)
    {
        if(!is_full && (state > ST_START))
        {
            is_full = true;
            if(reverse) pwm2_full();
            else pwm1_full();
        }
    }
    else
    {
        if(is_stoped || is_full)
        {
            if(reverse) pwm2_start();
            else pwm1_start();
        }
        is_stoped = false;
        is_full = false;
        pwm1_set_dc(pwm);
        pwm2_set_dc(pwm);
    }

    reverse = CPU::in32(IO::GPIO_DATA0) & (1 << 26);
}

/************* State Transition Functions *************/

void enter_off()
{
    state = ST_OFF;
    turn_led1_off();
    turn_led2_off();
    turn_mos1_off();
    turn_mos2_off();
    pwm1_stop();
    pwm2_stop();
}

void enter_starting()
{
    state = ST_STARTING;
    turn_led1_on();
    turn_led2_on();
    turn_mos1_off();
    turn_mos2_off();
    pwm1_stop();
    pwm2_stop();
}

void enter_blink()
{
    state = ST_BLINK;
    turn_led1_off();
    turn_led2_off();
}

void enter_start()
{
    state = ST_START;
    turn_led1_off();
    turn_led2_off();
}

void enter_front()
{
    state = ST_FRONT;
    turn_led2_off();
    turn_mos2_off();
    pwm2_stop();
    turn_led1_on();
    turn_mos1_on();
    pwm1_start();
}

void enter_reverse()
{
    state = ST_REVERSE;
    turn_led1_off();
    turn_mos1_off();
    pwm1_stop();
    turn_led2_on();
    turn_mos2_on();
    pwm2_start();
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
        if(throttle < throttle_min)
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
        break;

    case ST_REVERSE:
        if(!reverse)
            enter_starting();
        break;

    }
}
