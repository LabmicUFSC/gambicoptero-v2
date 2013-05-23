/**
    Motors Test
    Based on MotorTest from AeroQuad
    (Libraries/AQ_Motors/Examples/MotorTest)
*/

/* EPOS includes */
#include <alarm.h>
#include <display.h>
/* AeroQuad includes*/
#include <aeroquad/AQ_Motors/Motors_PWM.h>

/** Initializes the motors */
void initMotors(NB_Motors motorConfig) {
    initializeMotors(motorConfig);
}

/** Test individual motor (range 0-3) */
void testMotor(int motor) {
    cout << "Test motor " << motor << endln;
    for(int motorTrust = 1000; motorTrust < 1200; motorTrust += 10) {
        motorCommand[motor] = motorTrust;
        writeMotors();
        Alarm::delay(200);
    }
    for(int motorTrust = 1200; motorTrust > 1000; motorTrust -= 10) {
        motorCommand[motor] = motorTrust;
        writeMotors();
        Alarm::delay(200);
    }
    motorCommand[motor] = 1000;
    writeMotors();
}

int main() {
    cout << "=============== START MOTOR TEST ==================" << endln;
    for(int motor = 0; motor < 4; motor++) {
        testMotor(motor);
        Alarm::delay(1000);
    }
}
