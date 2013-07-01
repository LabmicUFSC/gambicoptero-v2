#include <system/config.h>
#include <machine.h>
#include <alarm.h>
#include <uart.h>
#include <nic.h>

__USING_SYS

const unsigned char SINK_ID = 0x01;
const unsigned int DATA_SIZE = 32;

char msg[DATA_SIZE];

void turn_on_led1() {
    unsigned int *GPIO_BASE = (unsigned int*)0x80000000;
    *GPIO_BASE = 1 << 23;
}


int bmv_get_data(MC13224V_UART u) {
	unsigned char c;
	int i = 0;

	while(1) {
		c = u.get();
		while(c != '\n') {
			msg[i++] = c;
			c = u.get();
		}
		msg[i] = '\n';

		return i+1;
	}
}

NIC * nic;

int main() {
	Alarm::delay(2000000);
	UART u = UART(19200, 8, 0, 1, 0);
	nic = new NIC();
	int size;

	while(1) {
		size = bmv_get_data(u);
		nic->send(NIC::BROADCAST, (NIC::Protocol) 1, &msg, size);
		Alarm::delay(50000);
	}

	return 0;
}
