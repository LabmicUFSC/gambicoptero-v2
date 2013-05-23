#ifndef FLIGHTCONTROLPROCESSOR_H
#define FLIGHTCONTROLPROCESSOR_H

#include "FlightControlQuadPlus.h"
#include <mach/mc13224v/itg3200.h>
#include<utility/PID.h>
#include<utility/receiver_remotePC.h>.h>
#include<utility/headingHoldProcessor.h>
#include<utility/Kinematics.h>
#include<utility/math.h>

__BEGIN_SYS
class FlightControlProcessor {

    ReceiverRemotePc * _receiver;
    Kinematics * _kinematics;
    PIDclass * _pid;
    void processMinMaxCommand();
    short maxLimit;
    short minLimit;

    public:
    FlightControlProcessor(
                Kinematics* kinematics,
                PIDclass * pid,
                ReceiverRemotePc* receiver,
                FlightControlQuadType<FlightControlQuadPlus> * fcp,
                ITG3200 * gyro, int _flightMode = AeroQuad::_flightMode,
                int _throttle = AeroQuad::_throttle
            ) : maxLimit(0), minLimit(0),
        flightMode(_flightMode), throttle(_throttle),
        motorArmed(0){

        _pid = pid;
        _fcQuadPlus = fcp;
        _gyro = gyro;
        _receiver = receiver;//new Receiver;
        _kinematics = kinematics;
        _receiver->initializeReceiver(6);
        headingHoldProcessor = new HeadingHoldProcessor(_receiver, _pid);

    }

    int flightMode;
    FlightControlQuadType<FlightControlQuadPlus> * _fcQuadPlus;
    ITG3200 * _gyro;
    OStream cout;
    static const int ATITUDE_FLIGHT_MODE = 1;
    static const float  ATTITUDE_SCALING = (0.75 * 0.002);
    int motorMinCommand[4];
    int motorMaxCommand[4];
    int throttle;
    int motorArmed;
    HeadingHoldProcessor * headingHoldProcessor;

    void calculateFlightError();
    void processFlightControl();

    /**
     * processThrottleCorrection
     *
     * This function will add some throttle imput if the craft is angled
     * this prevent the craft to loose altitude when angled.
     * it also add the battery throttle correction in case
     * of we are in auto-descent.
     *
     * Special thank to Ziojo for this.
     */
    int  processThrottleCorrection();
    /**
     * processCalibrateESC
     *
     * Proces esc calibration command with the help of the configurator
     */
    void processCalibrateESC();

};


__END_SYS


#endif // FLIGHTCONTROLPROCESSOR_H
