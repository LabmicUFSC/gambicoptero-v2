#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <mach/mc13224v/quadcopter_network.h>

__USING_SYS

NIC * nic;
OStream cout;

void enable_leds() 
{
    unsigned int *GPIO_BASE = (unsigned int*)0x80000000;
    *GPIO_BASE = (1<<23) + (1<<11) + (1<<10) + (1<<9) + (1<<8);
}
int curr_led;
void cycle_leds()
{
	unsigned int *GPIO_DATA = (unsigned int*)0x80000008;
	curr_led = (curr_led+1)%3;
	*GPIO_DATA = (1<<(8+curr_led));
}

void maca_handler(MACA_Transceiver::Event event)
{
	if(event == MACA_Transceiver::LOST_COMMUNICATION)
		cout<<"maca_handler() - LOST_COMMUNICATION\n";
	else if(event == MACA_Transceiver::SFD_DETECTED)
		cout<<"maca_handler() - SFD_DETECTED\n";
	else if(event == MACA_Transceiver::FRAME_END)
		cout<<"maca_handler() - FRAME_END\n";	
	else if(event == MACA_Transceiver::DBG)
		cycle_leds();
}

int main() {
	curr_led = 0;
	enable_leds();
	for(int i=0;i<3;i++)
	{
		cycle_leds();
		Alarm::delay(1000000);
	}
	Alarm::delay(2000000);
	cout<<"Delay done\n";
	Quadcopter_Network::init(&maca_handler);
	Quadcopter_Network::set_data_coordinator(1,2,3,4,5,6);
	Quadcopter_Network::run();
}
