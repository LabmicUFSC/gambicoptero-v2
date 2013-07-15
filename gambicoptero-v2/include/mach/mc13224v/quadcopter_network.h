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

		static int run();
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
//			unsigned char length; //MACA puts length in first byte on RX
			unsigned char header;
			char pwm1; char pwm2;
			char pwm3; char pwm4;
//			unsigned char FCS;
		} feedback_frame;

		const static unsigned int feedback_frame_size = 5;

		typedef struct 
		{
//			unsigned char length; //MACA puts length in first byte on RX
			unsigned char header;
			short accx; short accy; short accz;
			short gyrx; short gyry; short gyrz;
//			unsigned char FCS; //MACA puts the FCS on last byte
		} coordinator_frame;
		
		const static unsigned int coordinator_frame_size = 13;

		static const bool _coordinator = COORDINATOR;
		static const unsigned int _frame_size = (_coordinator ? coordinator_frame_size : feedback_frame_size);

		static volatile unsigned char _send_buffer[_frame_size+2];
	private:
		static MACA_Transceiver * _maca;
};

#endif
