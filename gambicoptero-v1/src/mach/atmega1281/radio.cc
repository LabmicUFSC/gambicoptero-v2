// EPOS-- ATMega1281 Radio (AT86RF230) NIC Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/malloc.h>
#include <mach/atmega1281/machine.h>
#include <mach/atmega1281/radio.h>

__BEGIN_SYS

AT86RF230 * Radio_Wrapper::device = new AT86RF230();

void Radio_Wrapper::init() {
    AT86RF230::init();
}

void Radio_Wrapper::set_event_handler(AT86RF230::event_handler * handler) {
    device->set_event_handler(handler);
}

int Radio_Wrapper::send(unsigned char * data, unsigned int size) {
    return device->send(data, size);
}

int Radio_Wrapper::receive(unsigned char * data) {
    return device->receive(data);
}

void Radio_Wrapper::off() {
    device->reset_state_machine();
}

void Radio_Wrapper::listen() {
    device->set_state(AT86RF230::RX_ON);
}

void Radio_Wrapper::reset() {
    device->reset();
}

bool Radio_Wrapper::cca() {
    return device->cca_measurement(AT86RF230::ENERGY_ABOVE_THRESHOLD, 0);
}

unsigned int Radio_Wrapper::lqi() {
    return 0;
}

unsigned int Radio_Wrapper::rssi() {
    int * i;
    if (device->get_rssi(i))
        return *i;

    return 0;
}

__END_SYS

