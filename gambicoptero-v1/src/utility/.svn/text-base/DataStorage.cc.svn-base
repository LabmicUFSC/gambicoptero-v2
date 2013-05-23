#include<utility/DataStorage.h>


__USING_SYS

DataStorage::DataStorage(Receiver *receiver,
                         int minArmedThrottle /*= AeroQuad::minArmedThrottle*/) : _minArmedThrottle(minArmedThrottle) {
    _receiver = receiver;
}

void DataStorage::initializeEEPROM(MC13224V_ADXL345 *ac, ITG3200 *gyro, PIDclass * pid)
{
    pid->PID[PIDclass::RATE_XAXIS_PID_IDX].P = 100.0;
    pid->PID[PIDclass::RATE_XAXIS_PID_IDX].I = 0.0;
    pid->PID[PIDclass::RATE_XAXIS_PID_IDX].D = -300.0;
    pid->PID[PIDclass::RATE_YAXIS_PID_IDX].P = 100.0;
    pid->PID[PIDclass::RATE_YAXIS_PID_IDX].I = 0.0;
    pid->PID[PIDclass::RATE_YAXIS_PID_IDX].D = -300.0;
    pid->PID[PIDclass::ZAXIS_PID_IDX].P = 200.0;
    pid->PID[PIDclass::ZAXIS_PID_IDX].I = 5.0;
    pid->PID[PIDclass::ZAXIS_PID_IDX].D = 0.0;
    pid->PID[PIDclass::ATTITUDE_XAXIS_PID_IDX].P = 4.0;
    pid->PID[PIDclass::ATTITUDE_XAXIS_PID_IDX].I = 0.0;
    pid->PID[PIDclass::ATTITUDE_XAXIS_PID_IDX].D = 0.0;
    pid->PID[PIDclass::ATTITUDE_YAXIS_PID_IDX].P = 4.0;
    pid->PID[PIDclass::ATTITUDE_YAXIS_PID_IDX].I = 0.0;
    pid->PID[PIDclass::ATTITUDE_YAXIS_PID_IDX].D = 0.0;
    pid->PID[PIDclass::HEADING_HOLD_PID_IDX].P = 3.0;
    pid->PID[PIDclass::HEADING_HOLD_PID_IDX].I = 0.1;
    pid->PID[PIDclass::HEADING_HOLD_PID_IDX].D = 0.0;
    // AKA PID experiements
    pid->PID[PIDclass::ATTITUDE_GYRO_XAXIS_PID_IDX].P = 100.0;
    pid->PID[PIDclass::ATTITUDE_GYRO_XAXIS_PID_IDX].I = 0.0;
    pid->PID[PIDclass::ATTITUDE_GYRO_XAXIS_PID_IDX].D = -300.0;
    pid->PID[PIDclass::ATTITUDE_GYRO_YAXIS_PID_IDX].P = 100.0;
    pid->PID[PIDclass::ATTITUDE_GYRO_YAXIS_PID_IDX].I = 0.0;
    pid->PID[PIDclass::ATTITUDE_GYRO_YAXIS_PID_IDX].D = -300.0;

    pid->PID[PIDclass::ALTITUDE_HOLD_PID_IDX].P = 25.0;
    pid->PID[PIDclass::ALTITUDE_HOLD_PID_IDX].I = 0.6;
    pid->PID[PIDclass::ALTITUDE_HOLD_PID_IDX].D = 0.0;
    pid->PID[PIDclass::ALTITUDE_HOLD_PID_IDX].windupGuard = 25.0; //this prevents the 0.1 I term to rise too far
    pid->PID[PIDclass::ZDAMPENING_PID_IDX].P = 0.0;
    pid->PID[PIDclass::ZDAMPENING_PID_IDX].I = 0.0;
    pid->PID[PIDclass::ZDAMPENING_PID_IDX].D = 0.0;


    // Accel Cal
    ac->accelScaleFactor[AeroQuad::XAXIS] = 0.034; //  1.0;
    ac->runTimeAccelBias[AeroQuad::XAXIS] = 0.034;
    ac->accelScaleFactor[AeroQuad::YAXIS] = 0.034;
    ac->runTimeAccelBias[AeroQuad::YAXIS] = 0.034;
    ac->accelScaleFactor[AeroQuad::ZAXIS] = 0.034;
    ac->runTimeAccelBias[AeroQuad::ZAXIS] = 1;//0.0;

    pid->windupGuard = 1000.0;

    // AKA - added so that each PID has its own windupGuard, will need to be removed once each PID's range is established and put in the eeprom
    for (short i = AeroQuad::XAXIS; i <= PIDclass::ZDAMPENING_PID_IDX; i++ ) {
        if (i != PIDclass::ALTITUDE_HOLD_PID_IDX) {
            pid->PID[i].windupGuard = pid->windupGuard;
        }
    }

#define LASTCHANNEL 6

    _receiver->receiverXmitFactor = 1.0;
    //Defined on constructor of dataStorage class
  //  AeroQuad::_minArmedThrottle = 1150;
    gyro->gyroSmoothFactor = 1.0;
    // AKA - old setOneG not in SI - accel->setOneG(500);
    ac->accelOneG = -9.80665; // AKA set one G to 9.8 m/s^2
    for (short channel = AeroQuad::XAXIS; channel < LASTCHANNEL; channel++) {
        _receiver->receiverSlope[channel] = 1.0;
        _receiver->receiverOffset[channel] = 0.0;
        _receiver->receiverSmoothFactor[channel] = 1.0;
    }
    _receiver->receiverSmoothFactor[AeroQuad::ZAXIS] = 0.5;

   // AeroQuad::_flightMode = AeroQuad::RATE_FLIGHT_MODE;
    AeroQuad::_headingHoldConfig = AeroQuad::ON;
    // aref = 5.0; // Use 3.0 if using a v1.7 shield or use 2.8 for an AeroQuad Shield < v1.7

}

