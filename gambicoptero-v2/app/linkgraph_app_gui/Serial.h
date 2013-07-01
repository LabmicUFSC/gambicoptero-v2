/*
 * SerialPort.h
 *
 *  Created on: 02/04/2012
 *      Author: arliones
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include "AbstractSubject.h"
#include <string>
#include <SerialStream.h>
#include <boost/thread.hpp>

class Serial : public AbstractSubject<unsigned char>
{
private:
    Serial();
public:
    typedef AbstractSubject<unsigned char>::Observer Observer;
    Serial(const std::string & __port,
           LibSerial::SerialStreamBuf::BaudRateEnum __baud = LibSerial::SerialStreamBuf::BAUD_9600,
           LibSerial::SerialStreamBuf::CharSizeEnum __data = LibSerial::SerialStreamBuf::CHAR_SIZE_8,
           LibSerial::SerialStreamBuf::ParityEnum __parity = LibSerial::SerialStreamBuf::PARITY_NONE,
           short __stop = 1) throw();
    virtual ~Serial();

    void run();

    void start();
    void stop();
    void join();

private:
    struct callable {
        void operator()() { serial->run(); }
        Serial * serial;
    };
    boost::thread thread;
    volatile bool running;
    LibSerial::SerialStream serial_port;
    std::string port;
    int check_period;
};

#endif /* SERIAL_H_ */
