#ifndef MOTOR_H
#define MOTOR_H


#include <machine.h>
#include <alarm.h>

__USING_SYS

typedef Machine::IO IO;
unsigned short motor[4] = {24096, 24096, 24096, 24096};

unsigned short convert(unsigned short percent) {
       return 24000 + 240 * percent;
}


int initializeMotors() {
    //Alarm::delay(5000000);

    /* GPIO_10 is read from Data Register */
    CPU::out32(IO::GPIO_DATA_SEL0, CPU::in32(IO::GPIO_DATA_SEL0)
            | (1 << 10) // GPIO_10: PWM1
    );
    /* GPIO_10 is output */
    CPU::out32(IO::GPIO_PAD_DIR0, CPU::in32(IO::GPIO_PAD_DIR0)
            | (1 << 10) // GPIO_10: PWM1
            | (1 << 23)
            | (1 << 24)
            | (1 << 25)
            | (1 << 26)
               | (1 << 27)
               |  (1 << 28)
    );
    /* GPIO_10 (01) is TMR2 out */
    CPU::out32(IO::GPIO_FUNC_SEL0, CPU::in32(IO::GPIO_FUNC_SEL0)
            | (1 << 20) // GPIO_10: PWM1
    );

    /* Disable Timer2 */
    CPU::out16(IO::TIMER0_ENBL, CPU::in16(IO::TIMER0_ENBL) & ~(1 << 2));
    /* Reset the counter */
    CPU::out16(IO::TIMER2_CNTR, 0);
    /* Timer2 Status and Control Register */
    CPU::out16(IO::TIMER2_SCTRL,
        CPU::in16(IO::TIMER2_SCTRL)
        /* TCF = Timer Compare Flag*/
        & ~(1 << 15)
        /* TCFIE = Timer Compare Flag Interrupt Enable */
        |  (1 << 14)
        /* TOF = Timer Overflow Flag */
        & ~(1 << 13)
        /* TOFIE = Timer Overflow Flag Interrupt Enable */
        |  (1 << 12)
        /* IEF = Input Edge Flag */
        & ~(1 << 11)
        /* IEFIE = Input Edge Flag Interrupt Enable */
        & ~(1 << 10)
        /* IPS = Input Polarity Select */
        & ~(1 << 9)
        /* INPUT = External Signal Input (READ ONLY) */
        /* CAPTURE_MODE = Input Capture Mode
           CAPTURE_MODE IPS MEANING
           00           X   Capture function disabled
           01           0   Load Capture register on rising edge of input
           01           1   Load Capture register on falling edge of input
           10           0   Load Capture register on falling edge of input
           10           1   Load Capture register on rising edge of input
        */
        & ~(1 << 7)
        & ~(1 << 6)
        /* MSTR = Master Mode Enable */
        & ~(1 << 5)
        /* EEOF = Enable External OFLAG Force Bit */
        & ~(1 << 4)
        /* VAL = Forced OFLAG Value */
        & ~(1 << 3)
        /* FORCE = Force the OFLAG output */
        & ~(1 << 2)
        /* OPS = Output Polarity Select */
        |  (1 << 1)
        /* OEN = Output Enable */
        |  (1 << 0)
    );
    /* Timer2 Comparator Status and Control Register */
    CPU::out16(IO::TIMER2_CSCTRL,
        CPU::in16(IO::TIMER2_CSCTRL)
        /* DBG_EN = Debug Actions (00 is Normal Operation) */
        & ~(1 << 15)
        & ~(1 << 14)
        /* FILT_EN = Input Filter Enable */
        & ~(1 << 13)
        /* (12 - 8) = RESERVED */
        /* TCF2EN = Timer Compare 2 Interrupt Enable */
        & ~(1 << 7)
        /* TCF1EN = Timer Compare 1 Interrupt Enable */
        |  (1 << 6)
        /* TCF2 = Timer Compare 2 Status */
        & ~(1 << 5)
        /* TCF1 = Timer Compare 1 Status */
        & ~(1 << 4)
        /* CL2 = Compare Load Control 2
           00 = No Preload
           01 = Load w/Successful Compare with the value in COMP1
           10 = Load w/Successful Compare with the value in COMP2
           11 = RESERVED */
        & ~(1 << 3)
        & ~(1 << 2)
        /* CL1 = Compare Load Control 1
           00 = No Preload
           01 = Load w/Successful Compare with the value in COMP1
           10 = Load w/Successful Compare with the value in COMP2
           11 = RESERVED */
        & ~(1 << 1)
        & ~(1 << 0)
    );

    /*
    - COUNTER_MODE = 010 (count both rising and falling edges)
    - PRIMARY_CNT_SOURCE = 1001 (24000000Hz / 2 = 12000000Hz)
    - SECONDARY_CNT_SOURCE = 00 (Whatever, we are not going to use it)
    - ONCE = 0 (Counts repeatedly)
    - LENGHT = 0 (Rollover - Doesn't restart on compare)
    - DIR = 0 (Count UP)
    - CO_INIT = 0 (We are not using Co-Channel reinitialization stuff)
    - OUTPUT_MODE = 110 (OFLAG is set on compare match and cleared on rollover)
    */
    CPU::out16(IO::TIMER2_CTRL, 0b0101001000000110);
    /* Set COMP1 to 24096, see the math above */
    CPU::out16(IO::TIMER2_COMP1, 24096); // 24096

    /* Re-enable Timer2 */
    CPU::out16(IO::TIMER0_ENBL, CPU::in16(IO::TIMER0_ENBL) | (1 << 2));


    int powerMAX = 30;
    int aux = 3;

    Alarm::delay(1000000);
 //   while(true){
    /* unsigned short power;
      for(unsigned short i = 0; i != powerMAX; i += 1) {
           power = convert(i);
           motor[0] = power;  //MOTOR 3
          motor[1] = power;  //1
          motor[2] = power;  //4
          motor[3] = power;// + convert(i +10); 2
        //  Alarm::delay(200000);
      }

    /* for(unsigned short i = powerMAX; i != powerMAX + aux; i += 1) {
          power = convert(i);
          motor[3] = power;// + convert(i +10);
          Alarm::delay(200000);
      }

      Alarm::delay(5000000);

      for(unsigned short i = aux + powerMAX; i != powerMAX; i -= 1) {
          power = convert(i);
          motor[3] = power;// + convert(i +10);
          Alarm::delay(200000);
      }

        for(unsigned short i = powerMAX; i != 0; i -= 1) {
            power = convert(i);
            motor[0] = power;
            motor[1] = power;
            motor[2] = power ;
            motor[3] = power;// - convert(10);
          //  Alarm::delay(500000);
        }
   //     Alarm::delay(1000000);
   // } */
}






#endif // MOTOR_H
