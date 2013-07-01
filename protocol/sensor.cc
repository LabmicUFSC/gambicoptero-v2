#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <mach/mc13224v/quadcopter_network.h>

__USING_SYS

OStream cout;

unsigned char last_beacon;
int beacons_lost;
int total_beacons;
short a1,a2,a3,a4,a5,a6;

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
	else if(event == MACA_Transceiver::DATA_READY)
	{
//		cout<<"maca_handler() - DATA_READY\n";
//		cout<<a1<<" "<<a2<<" "<<a3<<" "<<a4<<" "<<a5<<" "<<a6<<"\n";
	}
	else if(event == MACA_Transceiver::BEACON_READY)
	{
		unsigned char tmp = MACA_Transceiver::rx_buffer[1];
		Quadcopter_Network::get_data_coordinator(a1,a2,a3,a4,a5,a6);
		if(last_beacon!=255)
		{
			if(tmp != ((last_beacon+1%128)))
			{
				while(tmp < last_beacon)
				{
					last_beacon = (last_beacon + 1)%128;
					beacons_lost++;
					total_beacons++;
				}
				beacons_lost += tmp - last_beacon;
				total_beacons += tmp - last_beacon;
			}
			else
				total_beacons++;
		}
		last_beacon = tmp;
	}
	else if(event == MACA_Transceiver::SFD_DETECTED)
		;
	else if(event == MACA_Transceiver::FRAME_END)
		;
	else if(event == MACA_Transceiver::DBG)
		cycle_leds();
}

int main() {
	curr_led = 0;
	enable_leds();
	last_beacon = 255;
	beacons_lost =0;
	for(int i=0;i<3;i++)
	{
		cycle_leds();
		Alarm::delay(1000000);
	}
	Alarm::delay(2000000);
	cout<<"Delay done\n";
	Quadcopter_Network::init(&maca_handler);
	Quadcopter_Network::set_data_feedback('a','b','c','d');
	Quadcopter_Network::run();
	while(true)
	{
		Alarm::delay(5000000);
		cout<<"Last beacon: " << last_beacon <<"\n";
		cout<<"Beacons lost: " << beacons_lost <<"\n";
		cout<<"Total beacons: " << total_beacons <<"\n";
		cout<<"Data received: "<<a1<<" "<<a2<<" "<<a3<<" "<<a4<<" "<<a5<<" "<<a6<<"\n";
	}
}
