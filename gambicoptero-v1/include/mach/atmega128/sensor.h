// EPOS ATMega128 Sensor Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __atmega128_sensor_h
#define __atmega128_sensor_h

#include <sensor.h>
#include "mts300.h"
#include "adxl202.h"

__BEGIN_SYS


class ATMega128_Temperature_Sensor: public Temperature_Sensor_Common
{
private:
    typedef Traits<ATMega128_Temperature_Sensor>::SENSORS SENSORS;
    static const unsigned int UNITS = SENSORS::Length;

public:
    ATMega128_Temperature_Sensor() {
	_dev = new Meta_Temperature_Sensor<SENSORS>::Get<0>::Result;
    }
    template<unsigned int UNIT>
    ATMega128_Temperature_Sensor(unsigned int u) {
	_dev = new typename Meta_Temperature_Sensor<SENSORS>::Get<UNIT>::Result(UNIT);
    }

    ~ATMega128_Temperature_Sensor() {
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


class ATMega128_Photo_Sensor: public Photo_Sensor_Common
{
private:
    typedef Traits<ATMega128_Photo_Sensor>::SENSORS SENSORS;
    static const unsigned int UNITS = SENSORS::Length;

public:
    ATMega128_Photo_Sensor() {
	_dev = new Meta_Photo_Sensor<SENSORS>::Get<0>::Result;
    }
    template<unsigned int UNIT>
    ATMega128_Photo_Sensor(unsigned int u) {
	_dev = new typename Meta_Photo_Sensor<SENSORS>::Get<UNIT>::Result(UNIT);
    }

    ~ATMega128_Photo_Sensor() {
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
    Meta_Photo_Sensor<SENSORS>::Base * _dev;
};


class ATMega128_Accelerometer: public Accelerometer_Common
{
private:
    typedef Traits<ATMega128_Accelerometer>::SENSORS SENSORS;
    static const unsigned int UNITS = SENSORS::Length;

public:
    ATMega128_Accelerometer() {
	_dev = new Meta_Accelerometer<SENSORS>::Get<0>::Result;
    }
    template<unsigned int UNIT>
    ATMega128_Accelerometer(unsigned int u) {
	_dev = new typename Meta_Accelerometer<SENSORS>::Get<UNIT>::Result(UNIT);
    }

    ~ATMega128_Accelerometer() {
	delete _dev;
    }

    int sample_x() {
	return _dev->sample_x();
    }

    int get_x() {
	return _dev->get_x();
    }

    bool enable_x() {
	return _dev->enable_x();
    }

    void disable_x() {
	_dev->disable_x();
    }

    bool data_ready_x() {
	return _dev->data_ready_x();
    }

    int sample_y() {
	return _dev->sample_y();
    }

    int get_y() {
	return _dev->get_y();
    }

    bool enable_y() {
	return _dev->enable_y();
    }

    void disable_y() {
	_dev->disable_y();
    }

    bool data_ready_y() {
	return _dev->data_ready_y();
    }

    static void init();

private:
    Meta_Accelerometer<SENSORS>::Base * _dev;
};


__END_SYS





#endif
