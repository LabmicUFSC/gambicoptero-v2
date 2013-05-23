#include <utility/motors.h>
#include<mach/mc13224v/motor.h>
#include <alarm.h>

__USING_SYS

void Motors::shutDown(){
        motor[FRONT] = 0;//Motors::motorCommand[FRONT] * 26.5 ;
        motor[REAR] = 0;//Motors::motorCommand[REAR] * 26.5;
        motor[RIGHT] = 0;//Motors::motorCommand[RIGHT] * 26.5;
        motor[LEFT] = 0;//Motors::motorCommand[LEFT]* 26.5;

}

void Motors::write(int motorCommand[]){
    motor[FRONT] = motorCommand[FRONT] *  _factor;
    motor[REAR] = motorCommand[REAR] * _factor;
   motor[RIGHT] = motorCommand[RIGHT] * _factor;
   motor[LEFT] = motorCommand[LEFT]* _factor;
}

void Motors::startMotors(){
    db<ITG3200>(TRC) << "---- -----" << endl;
    initializeMotors();
    db<ITG3200>(TRC) << "motors initialized " << endl;
    Alarm::delay(1000000);
    int factor = 27;
    for(unsigned short i = 1000 ; i <= 1001; i += 10) {
            motor[0] = i * factor;
            motor[1] = i * factor;//+ 200;
            motor[2] = i * factor;//+ 400;
            motor[3] = i * factor;//800;
            db<ITG3200>(TRC) << "Aumentando: " << i << endl;
           // Alarm::delay(1000000);
         }
        db<ITG3200>(TRC) << "Potencia maxima " << endl;
        Alarm::delay(1000000);
        for(unsigned short i = 1010; i >= 1000; i -= 10) {
            motor[0] = i* factor;
            motor[1] = i* factor;
            motor[2] = i * factor;
            motor[3] = i* factor;
            db<ITG3200>(TRC) << "Diminuindo: " << i << endl;
            Alarm::delay(50000);
        }
        //db<ITG3200>(TRC) << "Potencia minima " << endl;
        Alarm::delay(1000000);
       // db<ITG3200>(TRC) << "ended ---------" << endl;
    }
