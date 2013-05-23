#include <machine.h>
#include <alarm.h>
#include<mach/mc13224v/motor.h>
#include<utility/ostream.h>


__USING_SYS

OStream cout;
int main(){
    cout << "---- -----" << endl;
    initializeMotors();
//    Alarm::delay(2000000);
    cout << "motors initialized " << endl;

    Alarm::delay(1000000);
    for (int tries = 0 ; tries < 1 ; ++tries) {

         for(unsigned short i = 26500; i <= 27296; i += 10) {
            motor[0] = i;
            motor[1] = i ;//+ 200;
           motor[2] = i ;//+ 400;
            motor[3] = i ;//800;
            cout << "Aumentando: " << i << endl;
            Alarm::delay(100000);

         }
        cout << "Potencia maxima " << endl;
        Alarm::delay(5000000);
        for(unsigned short i = 27296; i >= 26500; i -= 10) {
            motor[0] = i;
            motor[1] = i;
            motor[2] = i;
            motor[3] = i;
            cout << "Diminuindo: " << i << endl;
            Alarm::delay(100000);
        }
        cout << "Potencia minima " << endl;
        Alarm::delay(5000000);

/*
        for(unsigned short i = 24096; i != 28496; i += 100) {
            motor[0] = i;
            Alarm::delay(200000);
        }
        for(unsigned short i = 24096; i != 28496; i += 100) {
            motor[1] = i;
            Alarm::delay(200000);
        }
        for(unsigned short i = 24096; i != 28496; i += 100) {
            motor[2] = i;
            Alarm::delay(200000);
        }
        for(unsigned short i = 24096; i != 28496; i += 100) {
            motor[3] = i;
            Alarm::delay(200000);
        }
        Alarm::delay(1000000);*/
    }
    Alarm::delay(5000000);
        motor[0] = 25000;
        motor[1] = 25000;
        motor[2] = 25000;
        motor[3] = 25000;


    cout << "ended ---------" << endl;
}


