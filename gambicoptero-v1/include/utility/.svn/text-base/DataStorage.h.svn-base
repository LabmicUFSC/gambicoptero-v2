#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include<utility/PID.h>
#include<mach/mc13224v/adxl345.h>
#include<mach/mc13224v/itg3200.h>
#include<utility/AeroQuad.h>
#include<utility/receiver.h>

__BEGIN_SYS
class DataStorage {

    Receiver * _receiver;
public:
    const int _minArmedThrottle;
    DataStorage(Receiver* receiver, int minArmedThrottle= AeroQuad::minArmedThrottle);
    void initializeEEPROM(MC13224V_ADXL345 * ac, ITG3200 * gyro, PIDclass * pid);

};
__END_SYS
#endif // DATASTORAGE_H
