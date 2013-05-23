#ifndef HEADINGHOLDPROCESSOR_H
#define HEADINGHOLDPROCESSOR_H
#include<utility/AeroQuad.h>
#include<utility/AQMath.h>
#include<utility/math.h>
#include<utility/FlightControlQuadPlus.h>
#include<arch/arm7/tsc.h>
#include<utility/receiver_remotePC.h>
#include<utility/PID.h>

__BEGIN_SYS
class HeadingHoldProcessor {

    short  headingHoldConfig;
    float headingHold;// calculated adjustment for quad to go to heading (PID output)
    float heading ; // measured heading from yaw gyro (process variable)
    float relativeHeading; // current heading the quad is set to (set point)
    short  headingHoldState ;
    unsigned long headingTime;
    float setHeading;
    ReceiverRemotePc * _receiver;
    PIDclass* _pid;
    public:
    HeadingHoldProcessor(ReceiverRemotePc * receiver, PIDclass *pid,
                         int _headingHoldConfig = AeroQuad::_headingHoldConfig) :
        headingHoldConfig(_headingHoldConfig){
        headingTime = ARM7_TSC::time_stamp();
        setHeading = 0;
        headingTime = 0;
        setHeading = 0;
        headingHold         = 0; // calculated adjustment for quad to go to heading (PID output)
        heading             = 0; // measured heading from yaw gyro (process variable)
        relativeHeading     = 0; // current heading the quad is set to (set point)
        headingHoldState    = 0;//OFF;
        _receiver = receiver;
        _pid = pid;
    }

    /**
 * processHeading
 *
 * This function will calculate the craft heading correction depending
 * of the users command. Heading correction is process with the gyro
 * or a magnetometer
 */


    void processHeading(ITG3200 * gyro, FlightControlQuad * fcq );


};
__END_SYS
#endif // HEADINGHOLDPROCESSOR_H
