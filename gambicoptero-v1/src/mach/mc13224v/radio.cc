// EPOS-- MC13224v Radio (Transceiver) NIC Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/kmalloc.h>
#include <mach/mc13224v/machine.h>
#include <mach/mc13224v/radio.h>
#include <mach/mc13224v/buck_regulator.h>

__BEGIN_SYS

MC13224V_Transceiver * Radio_Wrapper::device = 0;

typedef IO_Map<Machine> IO;

void Radio_Wrapper::init() {
    device = new(kmalloc(sizeof(MC13224V_Transceiver))) MC13224V_Transceiver();

    CPU::out32(IO::CRM_SYS_CNTL, 0x00000001);
    CPU::out32(IO::CRM_VREG_CNTL, 0x00000f5c);
    for (volatile unsigned int i = 0; i < 0x161a8; i++) { continue; }

    MC13224V_Transceiver::maca_init();

    device->set_channel(0); /* 802.15.4 channel 11 */
    device->set_power(0x12); /* 4.5dBm */
}

void Radio_Wrapper::set_event_handler(MC13224V_Transceiver::event_handler * handler) {
    device->set_event_handler(handler);
}

int Radio_Wrapper::send(unsigned char * data, unsigned int size) {
    volatile MC13224V_Transceiver::packet_t * p;

    do {
        p = device->get_free_packet();
    } while (!p);

    device->fill_packet(p, data, size);
    device->tx_packet(p);
    device->post_tx();

    return size;
}

int Radio_Wrapper::receive(unsigned char * data) {
    volatile MC13224V_Transceiver::packet_t * p;
    do {
        p = device->rx_packet();
    } while (!p);

    for (int i = 0; i < p->length; i++) {
        data[i] = p->data[i + 1];
    }

    int size = p->length;
    device->free_packet(p);

    return size;
}

void Radio_Wrapper::off() {
}

void Radio_Wrapper::listen() {
    device->post_receive();
}

void Radio_Wrapper::reset() {
    device->reset_maca();
}

bool Radio_Wrapper::cca() {
    return device->cca_measurement();
}

unsigned int Radio_Wrapper::lqi() {
    return device->get_lqi();
}

unsigned int Radio_Wrapper::rssi() {
    return 0;
}

__END_SYS

