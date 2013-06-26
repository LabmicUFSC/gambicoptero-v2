#include <mach/mc13224v/quadcopter_network.h>

MACA_Transceiver * Quadcopter_Network::_maca = 0;
unsigned char Quadcopter_Network::_send_buffer[_frame_size];

typedef IO_Map<Machine> IO;
void Quadcopter_Network::init(MACA_Transceiver::event_handler handler/*=0*/)
{
	if(_coordinator)
		_send_buffer[0] = 0;
	else
		_send_buffer[0] = 0x80;

    _maca = new(kmalloc(sizeof(MACA_Transceiver))) MACA_Transceiver();

    CPU::out32(IO::CRM_SYS_CNTL, 0x00000001);
    CPU::out32(IO::CRM_VREG_CNTL, 0x00000f5c);
    for (volatile unsigned int i = 0; i < 0x161a8; i++) { continue; }

    _maca->maca_init();
	_maca->set_event_handler(handler);
	_maca->set_tx_buffer(_send_buffer, 14);

    _maca->set_channel(0); /* 802.15.4 channel 11 */
    _maca->set_power(0x12); /* 4.5dBm */
}

void Quadcopter_Network::force_sync() { _maca->force_sync(); }
void Quadcopter_Network::run() { _maca->run(); }

void Quadcopter_Network::set_data_coordinator( short accx, short accy, short accz,
		                                       short gyrx, short gyry, short gyrz )
{
	if(!_coordinator) return;
	_send_buffer[1] = (unsigned char)(accx>>8);
	_send_buffer[2] = (unsigned char)(accx);
	_send_buffer[3] = (unsigned char)(accy>>8);
	_send_buffer[4] = (unsigned char)(accy);
	_send_buffer[5] = (unsigned char)(accz>>8);
	_send_buffer[6] = (unsigned char)(accz);
	_send_buffer[7] = (unsigned char)(gyrx>>8);
	_send_buffer[8] = (unsigned char)(gyrx);
	_send_buffer[9] = (unsigned char)(gyry>>8);
	_send_buffer[10] = (unsigned char)(gyry);
	_send_buffer[11] = (unsigned char)(gyrz>>8);
	_send_buffer[12] = (unsigned char)(gyrz);
}

void Quadcopter_Network::get_data_coordinator( short &accx, short &accy, short &accz,
		                                       short &gyrx, short &gyry, short &gyrz )
{
	accx = MACA_Transceiver::rx_buffer[2];
	accx <<= 8;
	accx +=	MACA_Transceiver::rx_buffer[3];

	accy = MACA_Transceiver::rx_buffer[4];
	accy <<= 8;
	accy +=	MACA_Transceiver::rx_buffer[5];
	accz = MACA_Transceiver::rx_buffer[6];
	accz <<= 8;
	accz +=	MACA_Transceiver::rx_buffer[7];
	gyrx = MACA_Transceiver::rx_buffer[8];
	gyrx <<= 8;
	gyrx +=	MACA_Transceiver::rx_buffer[9];
	gyry = MACA_Transceiver::rx_buffer[10];
	gyry <<= 8;
	gyry +=	MACA_Transceiver::rx_buffer[11];
	gyrz = MACA_Transceiver::rx_buffer[12];
	gyrz <<= 8;
	gyrz +=	MACA_Transceiver::rx_buffer[13];
}

void Quadcopter_Network::get_data_feedback( unsigned char &pwm1, unsigned char &pwm2,
						  			        unsigned char &pwm3, unsigned char &pwm4 )
{
	pwm1 = MACA_Transceiver::rx_buffer[2]; 
	pwm2 = MACA_Transceiver::rx_buffer[3];
	pwm3 = MACA_Transceiver::rx_buffer[4]; 
	pwm4 = MACA_Transceiver::rx_buffer[5];
}

void Quadcopter_Network::set_data_feedback( unsigned char pwm1, unsigned char pwm2,
										    unsigned char pwm3, unsigned char pwm4 )
{
	if(_coordinator) return;
 	_send_buffer[1] = pwm1; _send_buffer[2] = pwm2;
	_send_buffer[3] = pwm3; _send_buffer[4] = pwm4;
}

void Quadcopter_Network::set_buffer_payload(unsigned char *data)
{
	for(int i=1;i<_frame_size;i++)
		_send_buffer[i] = data[i-1];
}

int Quadcopter_Network::get_buffer_copy(unsigned char *buffer_copy) 
{ 
	for(int i=0; i<_frame_size;i++)
		buffer_copy[i] = _send_buffer[i];
	return _frame_size;
}
