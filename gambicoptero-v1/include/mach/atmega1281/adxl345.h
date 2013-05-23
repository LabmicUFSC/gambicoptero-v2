// EPOS ADXL345 Accelerometer Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __adxl345_h
#define __adxl345_h

#include <system/memory_map.h>
#include <usart.h>

__BEGIN_SYS

class ADXL345
{
private:
	typedef IO_Map<Machine> IO;
	typedef AVR8::Reg8 Reg8;

	enum {
		POWER_CTL	= 0x2D,
		DATA_FORMAT	= 0x31,
		DATAX0		= 0x32,
		DATAX1		= 0x33,
		DATAY0		= 0x34,
		DATAY1		= 0x35,
		DATAZ0		= 0x36,
		DATAZ1		= 0x37,
		FIFO_CTL	= 0x38,
	};

public:
	ADXL345() : _write_mask(0x00), _read_mask(0x80), _sample_x(0),
			_sample_y(0), _sample_z(0) {
		ddrb(ddrb() | 1<<5); 
		portb(portb() | 1<<5); 
		write(DATA_FORMAT, ((1<<1) + (0<<0)));
	}

	~ADXL345() {
		disable_sensor();
	}

	bool enable_x() {
		enable_sensor();

		return true;
	}

	bool enable_y() {
		enable_sensor();

		return true;
	}

	bool enable_z() {
		enable_sensor();

		return true;
	}

	void disable_x() { 
		disable_sensor();
	}

	void disable_y() { 
		disable_sensor();
	}

	void disable_z() { 
		disable_sensor();
	}

	int sample_x() { 
		_sample_x = (int)read(DATAX0);
		_sample_x |= (int)(read(DATAX1)) << 8;

		return _sample_x;
	}

	int sample_y() { 
		_sample_y = (int)read(DATAY0);
		_sample_y |= (int)(read(DATAY1)) << 8;

		return _sample_y;
	}

	int sample_z() { 
		_sample_z = (int)read(DATAZ0);
		_sample_z |= (int)(read(DATAZ1)) << 8;

		return _sample_z;
	}

	int get_x() {
		return _sample_x;
	}

	int get_y() {
		return _sample_y;
	}

	int get_z() {
		return _sample_z;
	}

	bool data_ready_x() {
		return true;
	}

	bool data_ready_y() {
		return true;
	}

	bool data_ready_z() {
		return true;
	}

private:
	Reg8 ddrb() {
		return AVR8::in8(IO::DDRB);
	}

	void ddrb(Reg8 value) { 
		AVR8::out8(IO::DDRB, value);
	}   

	Reg8 portb() {
		return AVR8::in8(IO::PORTB);
	}

	void portb(Reg8 value) { 
		AVR8::out8(IO::PORTB, value);
	}   

	void start_transmission() {
		portb(portb() & ~(1<<5)); 
	}

	void end_transmission() {
		portb(portb() | 1<<5); 
	}

	void write(Reg8 address, Reg8 data) {
		start_transmission();
		_usart.put(_write_mask + address);
		_usart.put(data);

		for(int i = 0; i < 28; i++) {
			continue;
		}

		end_transmission();
	}

	Reg8 read(Reg8 address) {
		Reg8 data;

		start_transmission();
		_usart.put(_read_mask + address);
		_usart.put(0xAA);

		for(int i = 0; i < 28; i++) {
			continue;
		}

		data = _usart.get();
		end_transmission();

		return data;
	}

	void enable_sensor() {
		write(POWER_CTL, 0x08);
	}

	void disable_sensor() {
		write(POWER_CTL, 0x00);
	}

	USART _usart;
	Reg8 _write_mask;
	Reg8 _read_mask;
	int _sample_x;
	int _sample_y;
	int _sample_z;
};

__END_SYS

#endif
