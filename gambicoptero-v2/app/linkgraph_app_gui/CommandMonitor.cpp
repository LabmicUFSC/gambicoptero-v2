/*
 * CommandMonitor.cpp
 *
 *  Created on: 05/04/2012
 *      Author: arliones
 */

#include "CommandMonitor.h"

using namespace std;

CommandMonitor::CommandMonitor() {}

CommandMonitor::~CommandMonitor() {}

void CommandMonitor::run() {
    string input;
    running = true;
    while(running)
    {
        cin >> input;
        notify_all(input);
    }
}

void CommandMonitor::start() {
    callable x;
    x.mon = this;
    thread = boost::thread(x);
}

void CommandMonitor::stop() { running = false; }

void CommandMonitor::join() {
    thread.join();
}
