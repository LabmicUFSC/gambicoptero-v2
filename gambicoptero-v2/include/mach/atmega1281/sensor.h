// EPOS ATMega1281 Sensor Mediator Declarations

#ifndef __atmega1281_sensor_h
#define __atmega1281_sensor_h

#include <sensor.h>
#include "sht11.h"

__BEGIN_SYS

class ATMega1281_Temperature_Sensor: public Temperature_Sensor_Common
{
private:
    typedef Traits<ATMega1281_Temperature_Sensor>::SENSORS SENSORS;
    static const unsigned int UNITS = SENSORS::Length;

public:
    ATMega1281_Temperature_Sensor() {
	_dev = new Meta_Temperature_Sensor<SENSORS>::Get<0>::Result;
    }
    template<unsigned int UNIT>
    ATMega1281_Temperature_Sensor(unsigned int u) {
	_dev = new typename Meta_Temperature_Sensor<SENSORS>::Get<UNIT>::Result(UNIT);
    }

    ~ATMega1281_Temperature_Sensor() {
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

class ATMega1281_Humidity_Sensor: public Humidity_Sensor_Common
{
private:
    typedef Traits<ATMega1281_Humidity_Sensor>::SENSORS SENSORS;
    static const unsigned int UNITS = SENSORS::Length;

public:
    ATMega1281_Humidity_Sensor() {
	_dev = new Meta_Humidity_Sensor<SENSORS>::Get<0>::Result;
    }
    template<unsigned int UNIT>
    ATMega1281_Humidity_Sensor(unsigned int u) {
	_dev = new typename Meta_Humidity_Sensor<SENSORS>::Get<UNIT>::Result(UNIT);
    }

    ~ATMega1281_Humidity_Sensor() {
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
    Meta_Humidity_Sensor<SENSORS>::Base * _dev;
};

__END_SYS

#endif

