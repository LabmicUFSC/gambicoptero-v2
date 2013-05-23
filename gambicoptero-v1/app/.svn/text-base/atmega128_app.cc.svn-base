#include <cpu.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <nic.h>
#include <uart.h>

__USING_SYS

typedef union {
	int s;
	char b[2];
} __u16;

OStream cout;
unsigned char count = 1;

int sensor() {
	ATMega128_Photo_Sensor sensor;
	NIC nic;

    CPU::out8(Machine::IO::DDRA, 0xff);
    CPU::out8(Machine::IO::PORTA, ~0);

    __u16 temp;

    while(1) {
		Alarm::delay(1000000);
    	temp.s = sensor.sample();
    	nic.send(NIC::BROADCAST, 0, temp.b, 2);
		CPU::out8(Machine::IO::PORTA, ~count++);
    }
}

int gateway()
{
	CPU::out8(Machine::IO::DDRA, 0xff);
	CPU::out8(Machine::IO::PORTA, ~0);

	UART uart;
	NIC nic;
	NIC::Address addr;
	NIC::Protocol prot;
	__u16 buff;
	int data_size = 0;

	while(true)
	{
		data_size = nic.receive(&addr, &prot, buff.b, 2);
		if(data_size != 2) continue;

		CPU::out8(Machine::IO::PORTA, ~count++);
		uart.put(buff.b[1]);
		uart.put(buff.b[0]);
		buff.s = 0x0000;
	}
}

int main() {
	return sensor();
//	return gateway();
}
