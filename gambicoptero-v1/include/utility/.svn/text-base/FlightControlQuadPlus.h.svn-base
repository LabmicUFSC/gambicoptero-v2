#ifndef FLIGHTCONTROLQUADPLUS_H
#define FLIGHTCONTROLQUADPLUS_H



/*
                 CW

           0....Front....0
           ......***......
      CCW  ......***......  CCW
           ......***......
           0....Back.....0

                 CW
*/
#include<utility/motors.h>
#include<utility/math.h>
//#include<utility/AeroQuad.h>

__USING_SYS

class FlightControlQuad {

public:

    int motorMaxCommand[4]; // = {0,0,0,0};
    int motorMinCommand[4]; //= {0,0,0,0};
    int motorConfiguratorCommand[4]; // = {0,0,0,0};
    int motorAxisCommandPitch;
    int motorAxisCommandRoll;// = 0;
    int motorAxisCommandYaw;
  //  int throttle;
    static const int YAW_DIRECTION = 1;

  //  virtual void applyMotorCommand() = 0;

    FlightControlQuad() : motorAxisCommandPitch(0), motorAxisCommandRoll(0),
        motorAxisCommandYaw(0){

        for (int i = 0; i < 4; ++i){
            motorMaxCommand[i] = 0;
            motorMinCommand[i] = 0;
            motorConfiguratorCommand[i] = 0;
        }
    }
};

template<typename Mode>
class FlightControlQuadType : public FlightControlQuad{};


class FlightControlQuadPlus;

template<>
class FlightControlQuadType<FlightControlQuadPlus> : public FlightControlQuad {

    static const int FRONT = 0;//Motors::MOTOR1;
    static const int REAR =  1;//Motors::MOTOR2;
    static const int RIGHT = 2;//Motors::MOTOR3;
    static const int LEFT =  3;//Motors::MOTOR4;

public:

    FlightControlQuadType<FlightControlQuadPlus>(){

    }

    void write(){ Motors::write(motorCommand);}
    int motorCommand[8];
    virtual void applyMotorCommand(int throttle) {
        db<FlightControlQuadPlus>(TRC) << "--- apllyMotorCommand" << endl;

        const int throttleCorrection = Math::abs(motorAxisCommandYaw*2/4);
        motorCommand[FRONT] = (throttle - throttleCorrection) - motorAxisCommandPitch - (YAW_DIRECTION * motorAxisCommandYaw);
        motorCommand[REAR] =  (throttle - throttleCorrection) + motorAxisCommandPitch - (YAW_DIRECTION * motorAxisCommandYaw);
        motorCommand[RIGHT] = (throttle - throttleCorrection) - motorAxisCommandRoll  + (YAW_DIRECTION * motorAxisCommandYaw);
        motorCommand[LEFT] =  (throttle - throttleCorrection) + motorAxisCommandRoll  + (YAW_DIRECTION * motorAxisCommandYaw);

        db<FlightControlProcessorQuadPlus>(TRC) << "MOTOR FRONT: " << motorCommand[FRONT] << endl
                                       << "MOTOR REAR: " << motorCommand[REAR]
                                       << "MOTOR RIGHT: " << motorCommand[RIGHT] << endl
                                       << "MOTOR LEFT: " << motorCommand[LEFT] << endl;

    }

};






#endif // FLIGHTCONTROLQUADPLUS_H
