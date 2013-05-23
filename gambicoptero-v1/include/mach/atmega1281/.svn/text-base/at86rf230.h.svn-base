// EPOS AT86RF230 Transceiver Mediator

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __at86rf230_h
#define __at86rf230_h

#include <cpu.h>
#include <ic.h>
#include <machine.h>

__BEGIN_SYS

class AT86RF230
{
public:
    class SPI
    {
    public:
	typedef IO_Map<Machine> IO;

	/* SPI Command Definition */
	enum {
	    REG_READ       = 0x80,
	    REG_WRITE      = 0xC0,
	    REG_ADDR_MASK  = 0x7F,
	    FRAME_RECEIVE  = 0x20,
	    FRAME_TRANSMIT = 0x60,
	    SRAM_READ      = 0x00,
	    SRAM_WRITE     = 0x40,
	};

	/* SPI Registers Bits */
	enum {
	    /* SPCR */
	    SPIE  = 0x80,
	    SPE   = 0x40,
	    DORD  = 0x20,
	    MSTR  = 0x10,
	    CPOL  = 0x08,
	    CPHA  = 0x04,
	    SPR1  = 0x02,
	    SPR0  = 0x01,
	    /* SPSR */
	    SPIF  = 0x80,
	    WCOL  = 0x40,
	    SPI2X = 0x01,
	};

	enum {
	    DDR  = IO::DDRB,
	    PORT = IO::PORTB,
	    PIN  = IO::PINB,

	    SEL  = 0x01,
	    SCLK = 0x02,
	    MOSI = 0x04,
	    MISO = 0x08,
	};

	/**
	 * Initiate the SPI transaction.
	 */
	static inline void slave_select_low() {
	    AVR8::out8(PORT, AVR8::in8(PORT) & ~(SEL));
	}

	/**
	 * Terminate the SPI transaction.
	 */
	static inline void slave_select_high() {
	    AVR8::out8(PORT, AVR8::in8(PORT) | (SEL));
	}

	/**
	 * Each transfer sequence starts by transferring a command byte from 
	 * SPI master via MOSI with MSB first. This command byte defines the 
	 * access mode and additional mode-dependent information.
	 *
	 * @param The command byte.
	 * return The SPI data output (on MISO).
	 */
	static unsigned char transfer(unsigned char value) {
	    AVR8::out8(IO::SPDR, value);
	    while ((AVR8::in8(IO::SPSR) & (SPIF)) == 0);

	    return AVR8::in8(IO::SPDR);
	}
    };

    class Registers
    {
    private:
	typedef IO_Map<Machine> IO;
	typedef CPU::Reg8 Reg8;

	/* AT86RF230 Registers Address */
	enum Register {
	    TRX_STATUS   = 0x01,
	    TRX_STATE    = 0x02,
	    TRX_CTRL_0   = 0x03,
	    PHY_TX_PWR   = 0x05,
	    PHY_RSSI     = 0x06,
	    PHY_ED_LEVEL = 0x07,
	    PHY_CC_CCA   = 0x08,
	    CCA_THRES    = 0x09,
	    IRQ_MASK     = 0x0E,
	    IRQ_STATUS   = 0x0F,
	    VREG_CTRL    = 0x10,
	    BATMON       = 0x11,
	    XOSC_CTRL    = 0x12,
	    FTN_CTRL     = 0x18,
	    PLL_CF 	 = 0x1A,
	    PLL_DCU	 = 0x1B,
	    PART_NUM     = 0x1C,
	    VERSION_NUM  = 0x1D,
	    MAN_ID_0     = 0x1E,
	    MAN_ID_1     = 0x1F,
	    SHORT_ADDR_0 = 0x20,
	    SHORT_ADDR_1 = 0x21,
	    PAN_ID_0     = 0x22,
	    PAN_ID_1     = 0x23,
	    IEEE_ADDR_0  = 0x24,
	    IEEE_ADDR_1  = 0x25,
	    IEEE_ADDR_2  = 0x26,
	    IEEE_ADDR_3  = 0x27,
	    IEEE_ADDR_4  = 0x28,
	    IEEE_ADDR_5  = 0x29,
	    IEEE_ADDR_6  = 0x2A,
	    IEEE_ADDR_7  = 0x2B,
	    XAH_CTRL     = 0x2C,
	    CSMA_SEED_0  = 0x2D,
	    CSMA_SEED_1  = 0x2E,
	};

	/**
	 * Reads data from one of the radio transceiver's registers.
	 *
	 * @param addr Register address to read from.
	 * return The actual value of the read register.
	 */
	static Reg8 read(Register addr);

	/**
	 * Writes a data value to one of the radio transceiver's registers.
	 *
	 * @param addr Address of the register to write.
	 * @param data Data value to write.
	 */
	static void write(Register addr, Reg8 data);

    public:
	enum {
	    RST  	 = 0x80,
	    DDR_RST      = IO::DDRA,
	    PORT_RST     = IO::PORTA,
	    PIN_RST      = IO::PINA,

	    SLP_TR 	 = 0x10,
	    DDR_SLP_TR   = IO::DDRB,
	    PORT_SLP_TR  = IO::PORTB,
	    PIN_SLP_TR   = IO::PINB,

	    IRQ  	 = 0x20,
	    DDR_IRQ      = IO::DDRE,
	    PORT_IRQ     = IO::PORTE,
	    PIN_IRQ      = IO::PINE,
	};

	/* AT86RF230 Registers Bits */
	enum Register_Bits {
	    /* TRX_STATUS */
	    CCA_DONE	  	= 0x80,
	    CCA_STATUS 	  	= 0x40,
	    STATUS 		= 0x1F,
	    /* TRX_STATE */
	    TRAC_STATUS 	= 0xE0,
	    TRX_CMD     	= 0x1F,
	    /* TRX_CTRL_0 */
	    PAD_IO 		= 0xC0,
	    PAD_IO_CLKM 	= 0x30,
	    CLKM_SHA_SEL 	= 0x08,
	    CLKM_CTRL 	  	= 0x07,
	    /* PHY_TX_PWR */
	    TX_AUTO_CRC_ON 	= 0x80,
	    TX_PWR 		= 0x0F,
	    /* PHY_RSSI */
	    RX_CRC_VALID 	= 0x80,
	    RSSI 		= 0x1F,
	    /* PHY_CC_CCA */
	    CCA_REQUEST 	= 0x80,
	    CCA_MODE 	  	= 0x60,
	    CHANNEL 	  	= 0x1F,
	    /* CCA_THRES */
	    CCA_ED_THRES 	= 0x0F,
	    /* IRQ_MASK */
	    MASK_BAT_LOW 	= 0x80,
	    MASK_TRX_UR 	= 0x40,
	    MASK_TRX_END 	= 0x08,
	    MASK_RX_START 	= 0x04,
	    MASK_PLL_UNLOCK   	= 0x02,
	    MASK_PLL_LOCK   	= 0x01,
	    /* IRQ_STATUS */
	    BAT_LOW 	  	= 0x80,
	    TRX_UR 		= 0x40,
	    TRX_END 	  	= 0x08,
	    RX_START 	  	= 0x04,
	    PLL_UNLOCK 	  	= 0x02,
	    PLL_LOCK 	  	= 0x01,
	    /* VREG_CTRL */
	    AVREG_EXT 	  	= 0x80,
	    AVDD_OK 	  	= 0x40,
	    DVREG_EXT 	  	= 0x08,
	    DVDD_OK 	  	= 0x04,
	    /* BATMON */
	    BATMON_OK 	  	= 0x20,
	    BATMON_HR 	  	= 0x10,
	    BATMON_VTH 	  	= 0x0F,
	    /* XOSC_CTRL */
	    XTAL_MODE 	  	= 0xF0,
	    XTAL_TRIM 	  	= 0x0F,
	    /* PLL_CF */
	    PLL_CF_START 	= 0x80,
	    /* PLL_DCU */
	    PLL_DCU_START 	= 0x80,
	    /* XAH_CTRL */
	    MAX_FRAME_RETRIES 	= 0xF0,
	    MAX_CSMA_RETRIES  	= 0x0E,
	    /* CSMA_SEED_1 */
	    MIN_BE 	      	= 0xC0,
	    AACK_SET_PD   	= 0x20,
	    I_AM_COORD 	  	= 0x08,
	    CSMA_SEED 	  	= 0x07
	};

	enum Command {
	    NOP	      	  = 0x00,
	    TX_START      = 0x02,
	    FORCE_TRX_OFF = 0x03,
	    RX_ON 	  = 0x06,
	    TRX_OFF       = 0x08,
	    PLL_ON 	  = 0x09,
	    RX_AACK_ON    = 0x16,
	    TX_ARET_ON    = 0x19
	};

	static void set_slptr_low() {
	    AVR8::out8(PORT_SLP_TR, AVR8::in8(PORT_SLP_TR) & ~SLP_TR);
	}

	static void set_slptr_high() {
	    AVR8::out8(PORT_SLP_TR, AVR8::in8(PORT_SLP_TR) | SLP_TR);
	}

	/* AT86RF230 Register Access Methods */
	static Reg8 trx_status() { return read(TRX_STATUS); }
	static void trx_status(Reg8 value) { write(TRX_STATUS,value); }
	static Reg8 trx_state() { return read(TRX_STATE); }
	static void trx_state(Reg8 value) { write(TRX_STATE,value); }
	static Reg8 trx_ctrl_0() { return read(TRX_CTRL_0); }
	static void trx_ctrl_0(Reg8 value) { write(TRX_CTRL_0,value); }
	static Reg8 phy_tx_pwr() { return read(PHY_TX_PWR); }
	static void phy_tx_pwr(Reg8 value) { write(PHY_TX_PWR,value); }
	static Reg8 phy_rssi() { return read(PHY_RSSI); }
	static void phy_rssi(Reg8 value) { write(PHY_RSSI,value); }
	static Reg8 phy_ed_level() { return read(PHY_ED_LEVEL); }
	static void phy_ed_level(Reg8 value) { write(PHY_ED_LEVEL,value); }
	static Reg8 phy_cc_cca() { return read(PHY_CC_CCA); }
	static void phy_cc_cca(Reg8 value) { write(PHY_CC_CCA,value); }
	static Reg8 cca_thres() { return read(CCA_THRES); }
	static void cca_thres(Reg8 value) { write(CCA_THRES,value); }
	static Reg8 irq_mask() { return read(IRQ_MASK); }
	static void irq_mask(Reg8 value) { write(IRQ_MASK,value); }
	static Reg8 irq_status() { return read(IRQ_STATUS); }
	static void irq_status(Reg8 value) { write(IRQ_STATUS,value); }
	static Reg8 vreg_ctrl() { return read(VREG_CTRL); }
	static void vreg_ctrl(Reg8 value) { write(VREG_CTRL,value); }
	static Reg8 batmon() { return read(BATMON); }
	static void batmon(Reg8 value) { write(BATMON,value); }
	static Reg8 xosc_ctrl() { return read(XOSC_CTRL); }
	static void xosc_ctrl(Reg8 value) { write(XOSC_CTRL,value); }
	static Reg8 ftn_ctrl() { return read(FTN_CTRL); }
	static void ftn_ctrl(Reg8 value) { write(FTN_CTRL,value); }
	static Reg8 pll_cf() { return read(PLL_CF); }
	static void pll_cf(Reg8 value) { write(PLL_CF,value); }
	static Reg8 pll_dcu() { return read(PLL_DCU); }
	static void pll_dcu(Reg8 value) { write(PLL_DCU,value); }
	static Reg8 part_num() { return read(PART_NUM); }
	static void part_num(Reg8 value) { write(PART_NUM,value); }
	static Reg8 version_num() { return read(VERSION_NUM); }
	static void version_num(Reg8 value) { write(VERSION_NUM,value); }
	static Reg8 man_id_0() { return read(MAN_ID_0); }
	static void man_id_0(Reg8 value) { write(MAN_ID_0,value); }
	static Reg8 man_id_1() { return read(MAN_ID_1); }
	static void man_id_1(Reg8 value) { write(MAN_ID_1,value); }
	static Reg8 short_addr_0() { return read(SHORT_ADDR_0); }
	static void short_addr_0(Reg8 value) { write(SHORT_ADDR_0,value); }
	static Reg8 short_addr_1() { return read(SHORT_ADDR_1); }
	static void short_addr_1(Reg8 value) { write(SHORT_ADDR_1,value); }
	static Reg8 pan_id_0() { return read(PAN_ID_0); }
	static void pan_id_0(Reg8 value) { write(PAN_ID_0,value); }
	static Reg8 pan_id_1() { return read(PAN_ID_1); }
	static void pan_id_1(Reg8 value) { write(PAN_ID_1,value); }
	static Reg8 ieee_addr_0() { return read(IEEE_ADDR_0); }
	static void ieee_addr_0(Reg8 value) { write(IEEE_ADDR_0,value); }
	static Reg8 ieee_addr_1() { return read(IEEE_ADDR_1); }
	static void ieee_addr_1(Reg8 value) { write(IEEE_ADDR_1,value); }
	static Reg8 ieee_addr_2() { return read(IEEE_ADDR_2); }
	static void ieee_addr_2(Reg8 value) { write(IEEE_ADDR_2,value); }
	static Reg8 ieee_addr_3() { return read(IEEE_ADDR_3); }
	static void ieee_addr_3(Reg8 value) { write(IEEE_ADDR_3,value); }
	static Reg8 ieee_addr_4() { return read(IEEE_ADDR_4); }
	static void ieee_addr_4(Reg8 value) { write(IEEE_ADDR_4,value); }
	static Reg8 ieee_addr_5() { return read(IEEE_ADDR_5); }
	static void ieee_addr_5(Reg8 value) { write(IEEE_ADDR_5,value); }
	static Reg8 ieee_addr_6() { return read(IEEE_ADDR_6); }
	static void ieee_addr_6(Reg8 value) { write(IEEE_ADDR_6,value); }
	static Reg8 ieee_addr_7() { return read(IEEE_ADDR_7); }
	static void ieee_addr_7(Reg8 value) { write(IEEE_ADDR_7,value); }
	static Reg8 xah_ctrl() { return read(XAH_CTRL); }
	static void xah_ctrl(Reg8 value) { write(XAH_CTRL,value); }
	static Reg8 csma_seed_0() { return read(CSMA_SEED_0); }
	static void csma_seed_0(Reg8 value) { write(CSMA_SEED_0,value); }
	static Reg8 csma_seed_1() { return read(CSMA_SEED_1); }
	static void csma_seed_1(Reg8 value) { write(CSMA_SEED_1,value); }
    };

    static const unsigned int FRAME_MIN_LENGTH = 0x03;
    static const unsigned int FRAME_MAX_LENGTH = 0x7F;

    static const int MIN_CHANNEL = 0x0B;
    static const int MAX_CHANNEL = 0x1A;

    static const int MIN_ED_THRESHOLD = 0x00;
    static const int MAX_ED_THRESHOLD = 0x0F;

    enum Operating_Mode {
	BASIC,
	EXTENDED
    };

    enum State {
	/* Basic Radio Transceiver States */
	RX_ON 		   = 0x06, 
	TRX_OFF 	   = 0x08, 
	PLL_ON 		   = 0x09,
       	SLEEP 		   = 0x0F,
       	RX_AACK_ON 	   = 0x16,
       	TX_ARET_ON 	   = 0x19,
       	RX_ON_NOCLK 	   = 0x1C,
       	RX_AACK_ON_NOCLK   = 0x1D,
	/* Transient Radio Transceiver States */
	P_ON 		   = 0x00,
       	BUSY_RX 	   = 0x01,
       	BUSY_TX 	   = 0x02,
       	BUSY_RX_AACK 	   = 0x11,
       	BUSY_TX_ARET 	   = 0x12,
       	BUSY_RX_AACK_NOCLK = 0x1E,

       	STATE_TRANSITION_IN_PROGRESS = 0x1F
    };

    enum Event {
	SFD_DETECTED,
	FRAME_END
    };

    /* Interrupt Description */
    enum Interrupt {
	BAT_LOW    = 0x80,
	TRX_UR     = 0x40,
	TRX_END    = 0x08,
	RX_START   = 0x04,
	PLL_UNLOCK = 0x02,
	PLL_LOCK   = 0x01,

	ALL_INTERRUPTS = 0xCF
    };

    enum CCA_Mode {
	ENERGY_ABOVE_THRESHOLD = 0x20,
	CARRIER_SENSE = 0x40,
	CARRIER_SENSE_WITH_ENERGY_ABOVE_THRESHOLD = 0x60
    };

    typedef struct {
	unsigned char length;
	unsigned char * data;
	unsigned char lqi;
    } Frame;

    typedef unsigned int Microseconds;
    typedef IO_Map<Machine> IO;
    typedef void (event_handler)(Event);

public:
    AT86RF230() {}
    ~AT86RF230() {}

    static void wait(Microseconds us) {
	for (unsigned int i = 0; i <= us * (Traits<Machine>::CLOCK / 1000000UL); i++); 
    }

    void frame_read(Frame * frame);
    void frame_write(Frame * frame);

    static void init_interrupts();
    static void interrupt_handler();

    void reset_state_machine();
    static State get_state();
    bool set_state(State new_state);

    bool enter_sleep_mode();
    bool leave_sleep_mode();
    bool sleeping() {
	return (AVR8::in8(Registers::PORT_SLP_TR) != 0); 
    }

    static void int_handler();

    static void init();
    static void reset();

    static void interrupt_enable();
    static void interrupt_disable();
    static void set_interrupt_handler(IC::Interrupt_Handler handler);

    static void set_event_handler(event_handler * handler);
    event_handler * get_event_handler();

    int send(unsigned char * data, unsigned int size);
    int receive(unsigned char * data);

    bool get_rssi(int * value);
    bool cca_measurement(CCA_Mode mode, unsigned char ed_thresold);

private:
    static event_handler * handler;
};

__END_SYS

#endif

