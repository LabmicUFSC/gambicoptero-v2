#ifndef QUADCOPTER_NETWORK_H
#define QUADCOPTER_NETWORK_H

#include "maca_transceiver.h"

#define COORDINATOR false

__USING_SYS;

class Quadcopter_Network
{
	public:

		static int get_buffer_copy(unsigned char *buffer_copy);
		static void set_buffer_payload(unsigned char *data);

		static void force_sync();
		static void run();
		static void init(MACA_Transceiver::event_handler handler = 0);
		static void set_data_coordinator( short accx, short accy, short accz,
					   	  	    		  short gyrx, short gyry, short gyrz );
		static void get_data_coordinator( short &accx, short &accy, short &accz,
					   	  	    		  short &gyrx, short &gyry, short &gyrz );
		static void set_data_feedback( unsigned char pwm1, unsigned char pwm2,
					   			   	   unsigned char pwm3, unsigned char pwm4 );
		static void get_data_feedback( unsigned char &pwm1, unsigned char &pwm2,
					   			   	   unsigned char &pwm3, unsigned char &pwm4 );

		typedef struct
		{
			unsigned char header;
			char pwm1; char pwm2;
			char pwm3; char pwm4;
		} feedback_frame;

		const static unsigned int feedback_frame_size = 6;

		typedef struct 
		{
			unsigned char header;
			short accx; short accy; short accz;
			short gyrx; short gyry; short gyrz;
		} coordinator_frame;
		
		const static unsigned int coordinator_frame_size = 14;

		static const bool _coordinator = COORDINATOR;
		static const unsigned int _frame_size = (_coordinator ? coordinator_frame_size : feedback_frame_size);

		static unsigned char _send_buffer[_frame_size];
	private:
		static MACA_Transceiver * _maca;
};

#endif
