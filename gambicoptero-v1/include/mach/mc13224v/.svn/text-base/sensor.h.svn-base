// EPOS MC13224V Sensor Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mc13224v_sensor_h
#define __mc13224v_sensor_h

#include <sensor.h>
#include "ertj1vg103fa.h"
#include "bma180.h"
#include "itg3200.h"
__BEGIN_SYS

class MC13224V_Temperature_Sensor: public Temperature_Sensor_Common
{
private:
    typedef Traits<MC13224V_Temperature_Sensor>::SENSORS SENSORS;
    static const unsigned int UNITS = SENSORS::Length;

public:
    MC13224V_Temperature_Sensor() {
        _dev = new Meta_Temperature_Sensor<SENSORS>::Get<0>::Result;
    }
    template<unsigned int UNIT>
    MC13224V_Temperature_Sensor(unsigned int u) {
        _dev = new typename Meta_Temperature_Sensor<SENSORS>::Get<UNIT>::Result(UNIT);
    }

    ~MC13224V_Temperature_Sensor() {
        delete _dev;
    }

    int sample() {
        return _dev->sample();
    }

    int get() {
        return _dev->get();
    }

    bool enable() {
        return _dev->enable();
    }

    void disable() {
        _dev->disable();
    }

    bool data_ready() {
        return _dev->data_ready();
    }

    static void init();

private:
    Meta_Temperature_Sensor<SENSORS>::Base * _dev;
};



class MC13224V_Accelerometer:public Accelerometer_Common
{
private:
   typedef Traits<MC13224V_Accelerometer>::SENSORS SENSORS;
    static const unsigned int UNITS = SENSORS::Length;
    Meta_Accelerometer<SENSORS>::Base * _dev;
public:



    MC13224V_Accelerometer(){
        _dev = new Meta_Accelerometer<SENSORS>::Get<0>::Result;
    }

    //template<unsigned int UNIT>
    MC13224V_Accelerometer(unsigned int u){
        _dev = new /*typename*/ Meta_Accelerometer<SENSORS>::Get<1>::Result;
    }

    ~MC13224V_Accelerometer(){ delete _dev; }


    int sample_x() {
        return _dev->sample_x();
    }

    virtual int get_x() {
        return _dev->get_x();
    }

    virtual bool enable_x() {
        return Accelerometer::enable_x();
    }

    virtual void disable_x() {
        Accelerometer::disable_x();
    }

    virtual bool data_ready_x() {
        return Accelerometer::data_ready_x();
    }

    virtual int sample_y() {
        return Accelerometer::sample_y();
    }

    virtual int get_y() {
        return _dev->get_y();
    }

    virtual bool enable_y() {
        return Accelerometer::enable_y();
    }

    virtual void disable_y() {
        Accelerometer::disable_y();
    }

    virtual bool data_ready_y() {
        return Accelerometer::data_ready_y();
    }

    virtual int get_z(){
        return _dev->get_z();
    }

    virtual void calibrate(){
        _dev->calibrate();
    }



};

__END_SYS

#endif
