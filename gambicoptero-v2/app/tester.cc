#include <machine.h>
#include <thread.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <mach/mc13224v/quadcopter_network.h>

__USING_SYS

OStream cout;

volatile unsigned char last_beacon;
volatile int beacons_lost;
volatile int got_beacons;
volatile int total_beacons;
volatile int data_ok, data_wrong, data;
short a0,a1,a2,a3,a4,a5;
unsigned char b0,b1,b2,b3;
int curr_led;

void enable_leds() 
{
    unsigned int *GPIO_BASE = (unsigned int*)0x80000000;
    *GPIO_BASE = (1<<23) + (1<<11) + (1<<10) + (1<<9) + (1<<8);
}
void cycle_leds()
{
	unsigned int *GPIO_DATA = (unsigned int*)0x80000008;
	curr_led = (curr_led+1)%3;
	*GPIO_DATA = (1<<(8+curr_led));
}
volatile bool network_ok;
void maca_handler(MACA_Transceiver::Event event)
{
	if(event == MACA_Transceiver::LOST_COMMUNICATION)
	{
		cout<<"maca_handler() - LOST_COMMUNICATION\n";
		network_ok=false;
	}
	else if(event == MACA_Transceiver::DATA_READY)
	{
		data++;
		Quadcopter_Network::get_data_feedback(b0,b1,b2,b3);
		if((b0==((unsigned char)a0)) && (b1==((unsigned char)a1)) && (b2==((unsigned char)a2)) && (b3==((unsigned char)a3)))
		{
			//cout<<"Data OK!\n";
			data_ok++;
		}
		else
		{
			data_wrong++;
//			cout<<"Data WRONG\n";
//			cout<<"Data WRONG\n"<<(unsigned char)a0<<" "<<(unsigned char)a1<<" "<<(unsigned char)a2<<" "<<(unsigned char)a3<<"\n"<<b0<<" "<<b1<<" "<<b2<<" "<<b3<<"\n";
		}
	}

	else if(event == MACA_Transceiver::BEACON_READY)
	{
		unsigned char tmp = MACA_Transceiver::rx_buffer[1];
		Quadcopter_Network::get_data_coordinator(a0,a1,a2,a3,a4,a5);
		Quadcopter_Network::set_data_feedback((unsigned char)a0,(unsigned char)a1,(unsigned char)a2,(unsigned char)a3);
		got_beacons++;
		if(last_beacon!=255)
		{
			//cout<<last_beacon<<'\n';
			if(tmp != ((last_beacon+1)%128))
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
		else
			total_beacons++;
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
	data_ok=0;data_wrong=0;data=0;curr_led=0;beacons_lost=0;got_beacons=0;
	network_ok=true;
	last_beacon = 255;
	enable_leds();
	for(int i=0;i<3;i++)
	{
		cycle_leds();
		Alarm::delay(1000000);
	}
	cout<<"Delay done\n";
	Quadcopter_Network::init(&maca_handler);
	Quadcopter_Network::run();
	while(network_ok)
	{
		Alarm::delay(5000000);
		int a = data_ok; int b = data_wrong; int c = data;
		int d = total_beacons; int e = got_beacons; int f =beacons_lost; 
		int g=last_beacon;
		cout<<"Data right: "<<a<<"\n";
		cout<<"Data wrong: "<<b<<"\n";
		cout<<"Total data: "<<c<<"\n";
		cout<<"Total bcns: " << d <<"\n";
		cout<<"Bcns got: " << e <<"\n";
		cout<<"Bcns lost: " << f <<"\n";
		cout<<"Last beacon: " << g <<"\n";
		cout<<"Data: "<<a0<<" "<<a1<<" "<<a2<<" "<<a3<<" "<<a4<<" "<<a5<<"\n";
	}
}
