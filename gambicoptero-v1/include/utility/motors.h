#ifndef MOTORS_H
#define MOTORS_H
#include<alarm.h>

class Motors {

    static const int _factor = 27;
public:


    enum NUMBER {
        MOTOR1 = 0,
        MOTOR2,
        MOTOR3,
        MOTOR4
    };

    enum NB_Motors{
        FOUR_Motors = 4,
        SIX_Motors = 6,
        EIGHT_Motors = 8
    };

    static const int FRONT = Motors::MOTOR1;
    static const int REAR = Motors::MOTOR2;
    static const int RIGHT = Motors::MOTOR3;
    static const int LEFT = Motors::MOTOR4;


    static void writeMotors();
    static void commandAllMotors(int command);
   /* static void pulseMotors(short nbPulse) {
        for (short i = 0; i < nbPulse; i++) {
            commandAllMotors(MINCOMMAND + 100);
            Alarm::delay(250);
            commandAllMotors(MINCOMMAND);
            Alarm::delay(250);
        }
    } */
    static const int  MINCOMMAND = 1000;
    static const int  MAXCOMMAND = 2000;
    static void write(int motorCommand[]);
    static void startMotors();
    static void shutDown();
 };

//int Motors::motorCommand[8] = {0,0,0,0,0,0,0,0};

//NB_Motors numberOfMotors = FOUR_Motors;

//está no motor.h da pasta mc13244v
//void initializeMotors(NB_Motors numbers = FOUR_Motors);






#endif // MOTORS_H
