/**
    Motors base class
*/

#ifndef _AQUAD_MOTORS_H_
#define _AQUAD_MOTORS_H_

template <unsigned int N_MOTORS>
class Motors {
public:
  //  const unsigned char MOTOR1 = 0;
  //  const unsigned char MOTOR2 = 1;
  //  const unsigned char MOTOR3 = 2;
  //  const unsigned char MOTOR4 = 3;
    //const unsigned char MOTOR5 = 4;
    //const unsigned char MOTOR6 = 5;
    //const unsigned char MOTOR7 = 6;
    //const unsigned char MOTOR8 = 7;

    //const short MIN_COMMAND = 1000;
    //const short MAX_COMMAND = 2000;

    int _motor_command[N_MOTORS];

    virtual void initialize_motors() = 0;
    virtual void write_motors() = 0;
    virtual void command_all_motors() = 0;

    void pulse_motors(unsigned char nb_pulse);
};

#endif
