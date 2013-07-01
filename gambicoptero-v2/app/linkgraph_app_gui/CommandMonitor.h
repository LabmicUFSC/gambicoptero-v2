/*
 * CommandMonitor.h
 *
 *  Created on: 05/04/2012
 *      Author: arliones
 */

#ifndef COMMANDMONITOR_H_
#define COMMANDMONITOR_H_

#include "AbstractSubject.h"
#include <string>
#include <boost/thread.hpp>

class CommandMonitor : public AbstractSubject<std::string>
{
public:
    typedef AbstractSubject<std::string>::Observer Observer;

    CommandMonitor();
    virtual ~CommandMonitor();

    void run();

    void start();
    void stop();
    void join();

private:
    struct callable {
        void operator()() { mon->run(); }
        CommandMonitor * mon;
    };
    volatile bool running;
    boost::thread thread;
};

#endif /* COMMANDMONITOR_H_ */
