/*
 * SerialPort.cpp
 *
 *  Created on: 02/04/2012
 *      Author: arliones
 */

#include "Serial.h"
#include "exceptions.h"
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

using namespace std;
using namespace LibSerial;


Serial::Serial(const std::string & __port,
               LibSerial::SerialStreamBuf::BaudRateEnum __baud,
               LibSerial::SerialStreamBuf::CharSizeEnum __data,
               LibSerial::SerialStreamBuf::ParityEnum __parity,
               short __stop) throw()
 : port(__port)
{

    stringstream err;

    serial_port.Open(__port.c_str());
    if(!serial_port.good()) {
        err << "Could not open serial port. (" << __port << ")";
        throw BadParameterExecption(err.str().c_str());
    }

    serial_port.SetBaudRate(__baud);
    if(! serial_port.good()) {
        throw BadParameterExecption("Could not set the baud rate.");
    }

    serial_port.SetCharSize(__data);
    if(!serial_port.good()) {
        throw BadParameterExecption("Could not set the character size.");
    }

    serial_port.SetParity(__parity);
    if(!serial_port.good()) {
        throw BadParameterExecption("Could not disable the parity.");
    }

    serial_port.SetNumOfStopBits(__stop);
    if(!serial_port.good()) {
        throw BadParameterExecption("Could not set the number of stop bits.");
    }

    serial_port.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
    if(!serial_port.good()) {
        throw BadParameterExecption("Could not use hardware flow control.");
    }

    serial_port.unsetf(std::ios_base::skipws);

    // as nyquist told us
    check_period = (1000000/__baud) / 2;
}

Serial::~Serial()
{
    stop();
    serial_port.Close();
}

void Serial::run()
{
//    cout << "Serial::run()" << endl;
//
//    static const string data[5] = { "!1|2|3|4|\n", "!2|2|1|4|\n", "!3|3|2|6|1|\n", "!4|2|5|6|\n", "!2|2|3|6|\n" };
//
//    for(int i = 0; i < 5; i++)
//    {
//        string::const_iterator it = data[i].begin();
//        for(; it != data[i].end(); ++it)
//            notify_all(*it);
//        sleep(3);
//    }

    running = true;
    while(running)
    {
//        (cout << "\tWaiting...").flush();
        while(serial_port.rdbuf()->in_avail() == 0) {
            usleep(check_period);
        }
//        (cout << "received: ").flush();
        while(serial_port.rdbuf()->in_avail() > 0)
        {
            char next_byte;
            serial_port.get(next_byte);
            notify_all(next_byte);
        }
    }
}

void Serial::start() {
    callable x;
    x.serial = this;
    thread = boost::thread(x);
}

void Serial::stop() { running = false; }

void Serial::join() {
    thread.join();
}
