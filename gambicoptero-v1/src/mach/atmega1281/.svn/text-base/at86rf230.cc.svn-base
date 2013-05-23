// EPOS AT86RF230 Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>

__BEGIN_SYS

AT86RF230::event_handler * AT86RF230::handler = 0;

AT86RF230::Registers::Reg8 AT86RF230::Registers::read(AT86RF230::Registers::Register addr) {
    unsigned char val;

    val = addr & AT86RF230::SPI::REG_ADDR_MASK;
    val |= AT86RF230::SPI::REG_READ;

    CPU::int_disable();

    AT86RF230::SPI::slave_select_low();
    
    /* Send register address */
    val = AT86RF230::SPI::transfer(val);
    
    /* Read register content */
    val = AT86RF230::SPI::transfer(val);

    AT86RF230::SPI::slave_select_high();

    CPU::int_enable();

    return val;
}

void AT86RF230::Registers::write(AT86RF230::Registers::Register addr,
				AT86RF230::Registers::Reg8 data) {
    unsigned char val;

    val = AT86RF230::SPI::REG_WRITE | (AT86RF230::SPI::REG_ADDR_MASK & addr);

    CPU::int_disable();

    AT86RF230::SPI::slave_select_low();

    /* Send register address */
    val = AT86RF230::SPI::transfer(val);

    /* Write register content */
    val = AT86RF230::SPI::transfer(data);

    AT86RF230::SPI::slave_select_high();

    CPU::int_enable();
}

void AT86RF230::frame_read(AT86RF230::Frame * frame) {
    unsigned char val;

    CPU::int_disable();
    unsigned char * data = (frame->data);

    AT86RF230::SPI::slave_select_low();

    /* Send frame read command */
    val = AT86RF230::SPI::transfer(AT86RF230::SPI::FRAME_RECEIVE);

    /* Read frame length */
    frame->length = AT86RF230::SPI::transfer(val);

    /* Updload frame buffer to data pointer */
    for (int i = 0; i < frame->length; i++) {
	data[i] = AT86RF230::SPI::transfer(val);
    }

    /* Read LQI value for this frame */
    frame->lqi = AT86RF230::SPI::transfer(val);

    AT86RF230::SPI::slave_select_high();

    CPU::int_enable();
}

void AT86RF230::frame_write(AT86RF230::Frame * frame) {
    unsigned char val;

    CPU::int_disable();

    AT86RF230::SPI::slave_select_low();

    /* Send frame write command */
    val = AT86RF230::SPI::transfer(AT86RF230::SPI::FRAME_TRANSMIT);

    /* Send frame length */
    val = AT86RF230::SPI::transfer(frame->length);

    /* Download to the frame buffer */
    for (int i = 0; i < frame->length; i++) {
	val = AT86RF230::SPI::transfer(frame->data[i]);
    }

    AT86RF230::SPI::slave_select_high();

    CPU::int_enable();
}

void AT86RF230::init() {
    wait(510);

    /* IO Specific Initialization */
    AVR8::out8(AT86RF230::Registers::DDR_SLP_TR, AVR8::in8(AT86RF230::Registers::DDR_SLP_TR) | AT86RF230::Registers::SLP_TR); 
    AVR8::out8(AT86RF230::Registers::DDR_RST, AVR8::in8(AT86RF230::Registers::DDR_RST) | AT86RF230::Registers::RST);

    /* SPI Specific Initialization */
    AVR8::out8(AT86RF230::SPI::DDR, AVR8::in8(AT86RF230::SPI::DDR) | AT86RF230::SPI::SEL | AT86RF230::SPI::SCLK | AT86RF230::SPI::MOSI);
    AVR8::out8(AT86RF230::SPI::PORT, AVR8::in8(AT86RF230::SPI::PORT) | AT86RF230::SPI::SEL | AT86RF230::SPI::SCLK);
    AVR8::out8(IO::SPCR, AVR8::in8(IO::SPCR) | AT86RF230::SPI::SPE | AT86RF230::SPI::MSTR);
    AVR8::out8(IO::SPSR, AVR8::in8(IO::SPSR) | AT86RF230::SPI::SPI2X);

    wait(510);

    reset();
    AT86RF230::Registers::trx_state(AT86RF230::Registers::FORCE_TRX_OFF);

    wait(510);

    AT86RF230::Registers::irq_mask(0x4F);
    AT86RF230::Registers::batmon(AT86RF230::Registers::batmon() & 0xF0);

    set_interrupt_handler(&int_handler);
    init_interrupts();
}

void AT86RF230::reset() {
    AVR8::out8(AT86RF230::Registers::PORT_RST, AVR8::in8(AT86RF230::Registers::PORT_RST) & ~AT86RF230::Registers::RST);
    AT86RF230::Registers::set_slptr_low();

    wait(6);

    AVR8::out8(AT86RF230::Registers::PORT_RST, AVR8::in8(AT86RF230::Registers::PORT_RST) | AT86RF230::Registers::RST);
}

void AT86RF230::init_interrupts() {
    /* Clear configuration of pins and the registers */
    CPU::out8(AT86RF230::Registers::DDR_IRQ, CPU::in8(AT86RF230::Registers::DDR_IRQ) & ~AT86RF230::Registers::IRQ);
    CPU::out8(AT86RF230::Registers::PORT_IRQ, CPU::in8(AT86RF230::Registers::PORT_IRQ) & ~AT86RF230::Registers::IRQ);
    CPU::out8(IO::EICRB, CPU::in8(IO::EICRB) | 0x0C);
    CPU::out8(IO::EIFR, CPU::in8(IO::EIFR) & ~AT86RF230::Registers::IRQ);

    interrupt_enable();
}

void AT86RF230::interrupt_enable() {
    AVR8::out8(IO::EIMSK, CPU::in8(IO::EIMSK) | AT86RF230::Registers::IRQ);
    AT86RF230::Registers::irq_status();
}

void AT86RF230::interrupt_disable() {
    AVR8::out8(IO::EIMSK, CPU::in8(IO::EIMSK) & ~AT86RF230::Registers::IRQ);
}

void AT86RF230::set_interrupt_handler(IC::Interrupt_Handler handler) {
    IC::int_vector(IC::IRQ_IRQ5, handler);
}

void AT86RF230::reset_state_machine() {
    while (get_state() != TRX_OFF) {
	AT86RF230::Registers::set_slptr_low();

	wait(6);

	AT86RF230::Registers::trx_state((AT86RF230::Registers::trx_state() & ~AT86RF230::Registers::TRX_CMD) | AT86RF230::Registers::FORCE_TRX_OFF);
	wait(1);
    }
}

AT86RF230::State AT86RF230::get_state() {
    unsigned char val = AT86RF230::Registers::trx_status();
    val &= AT86RF230::Registers::STATUS;

    return static_cast<AT86RF230::State> (val);
}

bool AT86RF230::set_state(AT86RF230::State new_state) {
    State original_state = get_state();

    if (new_state == original_state)
	return true;

    if ((original_state == BUSY_RX) || (original_state == BUSY_TX) ||
	    (original_state == BUSY_RX_AACK) || (original_state == BUSY_TX_ARET))
	return false;

    switch (new_state) {
	case TRX_OFF:
	    reset_state_machine();
	    return true;
	case TX_ARET_ON:
	    if (original_state == RX_AACK_ON) {
		AT86RF230::Registers::trx_state((AT86RF230::Registers::trx_state() & ~AT86RF230::Registers::TRX_CMD) | PLL_ON);
		wait(1);
	    }
	    break;
	case RX_AACK_ON:
	    if (original_state == TX_ARET_ON) {
		AT86RF230::Registers::trx_state((AT86RF230::Registers::trx_state() & ~AT86RF230::Registers::TRX_CMD) | RX_ON);
		wait(1);
	    }
	    break;
	case RX_ON:
	    break;
	case PLL_ON:
	    break;
	default:
	    return false;
    }

    AT86RF230::Registers::trx_state((AT86RF230::Registers::trx_state() & ~AT86RF230::Registers::TRX_CMD) | new_state);

    if (original_state == TRX_OFF) {
	wait(180);
    } else {
	wait(1);
    }

    return (new_state == get_state());
}

bool AT86RF230::enter_sleep_mode() {
    if (sleeping())
	return true;

    reset_state_machine();

    if (get_state() == TRX_OFF) {
	AT86RF230::Registers::set_slptr_high();
	return true;
    }

    return false;
}

bool AT86RF230::leave_sleep_mode() {
    if (!sleeping())
	return true;

    AT86RF230::Registers::set_slptr_low();
    wait(880);

    if (get_state() == TRX_OFF)
	return true;

    return false;
}

int AT86RF230::send(unsigned char * data, unsigned int size) {
    if ((size < FRAME_MIN_LENGTH) || (FRAME_MAX_LENGTH < size))
	return 0;

    
    if (!set_state(PLL_ON))
	return 0;

    AT86RF230::Frame frame;
    frame.data   = data;
    frame.length = size;

    AT86RF230::Registers::set_slptr_high();
    AT86RF230::Registers::set_slptr_low();

    frame_write(&frame);

    return size;
}

int AT86RF230::receive(unsigned char * data) {
    AT86RF230::Frame frame;
    frame.data = data;

    frame_read(&frame);

    return frame.length;
}

bool AT86RF230::get_rssi(int * val) {
    AT86RF230::State current_state = get_state();

    if ((current_state == RX_ON) || (current_state == BUSY_RX)) {
	*val = (AT86RF230::Registers::phy_rssi() & AT86RF230::Registers::RSSI);

	return true;
    }

    return false;
}

bool AT86RF230::cca_measurement(CCA_Mode mode, unsigned char ed_threshold) {
    if (!set_state(RX_ON))
	return false;

    /* set cca mode */
    AT86RF230::Registers::phy_cc_cca((AT86RF230::Registers::phy_cc_cca() & ~AT86RF230::Registers::CCA_MODE) | mode);

    /* set energy detection threshold */
    if ((MIN_ED_THRESHOLD < ed_threshold) || (MAX_ED_THRESHOLD < ed_threshold))
	return false;

    AT86RF230::Registers::cca_thres((AT86RF230::Registers::cca_thres() & ~AT86RF230::Registers::CCA_ED_THRES) | ed_threshold);

    /* measure */
    AT86RF230::Registers::phy_cc_cca(AT86RF230::Registers::phy_cc_cca() | AT86RF230::Registers::CCA_REQUEST);

    unsigned char status = AT86RF230::Registers::trx_status();

    wait(140);

    if (status & AT86RF230::Registers::CCA_DONE != AT86RF230::Registers::CCA_DONE)
	return false; /* timeout */
    else if (status & AT86RF230::Registers::CCA_STATUS)
	return false; /* busy */

    return true;
}

void AT86RF230::int_handler() {
    wait(100);

    unsigned char interrupt = AT86RF230::Registers::irq_status();

    if (interrupt & RX_START) {
	if (handler != 0) 
	    handler(SFD_DETECTED);
    }
}

void AT86RF230::set_event_handler(AT86RF230::event_handler * ev_handler) {
    handler = ev_handler;
}

AT86RF230::event_handler * AT86RF230::get_event_handler() {
    return handler;
}

__END_SYS

