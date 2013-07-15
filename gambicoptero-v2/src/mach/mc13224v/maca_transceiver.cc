// EPOS-- MC13224V Transceiver Mediator Implementation

#include <ic.h>
#include <chronometer.h>
#include <machine.h>
#include <mach/mc13224v/maca_transceiver.h>
#include <mach/mc13224v/buck_regulator.h>

__BEGIN_SYS

// --------------------------------------------------------------------
// Edit this (and only this) to change this node's actions on each slot
// You should use these values:
// BCN_RX: Wait for a coordinator beacon and sync to it
// BCN_TX: Send a beacon
// SEQ_TX: This is my garanteed slot, so transmit data
// SEQ_RX: Receive data from some other node
// SEQ_WAIT: Nothing interesting in this slot, just sleep

// Sensor node example
const int MACA_Transceiver::actions[n_actions] = {
	BCN_RX, SEQ_RX, SEQ_WAIT, //Superframe 0
	BCN_RX, SEQ_RX, SEQ_WAIT, //Superframe 1
	BCN_RX, SEQ_TX, SEQ_WAIT, //Superframe 2
	BCN_RX, SEQ_RX, SEQ_WAIT  //Superframe 3
};
/*
// Coordinator node example
const int MACA_Transceiver::actions[n_actions] = {
	BCN_TX, SEQ_WAIT, SEQ_WAIT,
	BCN_TX, SEQ_WAIT, SEQ_WAIT,
	BCN_TX, SEQ_WAIT, SEQ_WAIT,
	BCN_TX, SEQ_WAIT, SEQ_WAIT
};
*/
/*
const int MACA_Transceiver::actions[n_actions] = {
	SEQ_TX, SEQ_TX, SEQ_TX,
	SEQ_TX, SEQ_TX, SEQ_TX,
	SEQ_TX, SEQ_TX, SEQ_TX,
	SEQ_TX, SEQ_TX, SEQ_TX
};
*/
// --------------------------------------------------------------------

//const unsigned int MACA_Transceiver::slot_size[n_slots] = {50000, 50000, 25000}; 
//const unsigned int MACA_Transceiver::slot_size[n_slots] = {250000, 250000, 125000}; //1s, 1s, 500ms
const unsigned int MACA_Transceiver::slot_size[n_slots] = {1000, 1000, 500}; //4ms, 4ms, 2ms
volatile int MACA_Transceiver::current_slot = 0;
volatile unsigned int MACA_Transceiver::current_slot_end = 0;
volatile unsigned char MACA_Transceiver::rx_buffer[MAX_FRAME_SIZE];
volatile unsigned char MACA_Transceiver::dummy_buffer[MAX_FRAME_SIZE];
volatile unsigned char * MACA_Transceiver::tx_buffer;
unsigned int MACA_Transceiver::tx_size;
volatile int MACA_Transceiver::act_idx =0;
unsigned int MACA_Transceiver::beacon_timeout=0;


MACA_Transceiver::get_lqi_func MACA_Transceiver::get_lqi = reinterpret_cast<get_lqi_func>(reinterpret_cast<void *>(GET_LQI_ADDR));

MACA_Transceiver::event_handler * MACA_Transceiver::handler = 0;

volatile unsigned char MACA_Transceiver::fcs_mode = MACA_Transceiver::NO_FCS;

volatile unsigned char MACA_Transceiver::current_action = SEQ_NOP;

unsigned char MACA_Transceiver::ram_values[4];

const unsigned int MACA_Transceiver::addr_seq2[MAX_SEQ2] = {0x8000a050,0x8000a054};
const unsigned int MACA_Transceiver::data_seq2[MAX_SEQ2] = {0x0000047b,0x0000007b};

const unsigned int MACA_Transceiver::addr_cal3_seq1[MAX_CAL3_SEQ1] = {0x80009400,0x80009a04,0x80009a00};
const unsigned int MACA_Transceiver::data_cal3_seq1[MAX_CAL3_SEQ1] = {0x00020017,0x8185a0a4,0x8c900025};

const unsigned int MACA_Transceiver::addr_cal3_seq2[MAX_CAL3_SEQ2] = {0x80009a00,0x80009a00};
const unsigned int MACA_Transceiver::data_cal3_seq2[MAX_CAL3_SEQ2] = {0x8c900021,0x8c900027};

const unsigned int MACA_Transceiver::addr_cal3_seq3[MAX_CAL3_SEQ3] = {0x80009a00};
const unsigned int MACA_Transceiver::data_cal3_seq3[MAX_CAL3_SEQ3] = {0x8c900000};

const unsigned int MACA_Transceiver::addr_cal5[MAX_CAL5] = {0x80009400,0x8000a050,0x8000a054};
const unsigned int MACA_Transceiver::data_cal5[MAX_CAL5] = {0x00000017,0x00000000,0x00000000};

const unsigned int MACA_Transceiver::addr_reg_rep[MAX_DATA] = {0x80004118,0x80009204,0x80009208,0x8000920c,0x80009210,0x80009300,0x80009304,0x80009308,0x8000930c,0x80009310,0x80009314,0x80009318,0x80009380,0x80009384,0x80009388,0x8000938c,0x80009390,0x80009394,0x8000a008,0x8000a018,0x8000a01c,0x80009424,0x80009434,0x80009438,0x8000943c,0x80009440,0x80009444,0x80009448,0x8000944c,0x80009450,0x80009460,0x80009464,0x8000947c,0x800094e0,0x800094e4,0x800094e8,0x800094ec,0x800094f0,0x800094f4,0x800094f8,0x80009470,0x8000981c,0x80009828};
const unsigned int MACA_Transceiver::data_reg_rep[MAX_DATA] = {0x00180012,0x00000605,0x00000504,0x00001111,0x0fc40000,0x20046000,0x4005580c,0x40075801,0x4005d801,0x5a45d800,0x4a45d800,0x40044000,0x00106000,0x00083806,0x00093807,0x0009b804,0x000db800,0x00093802,0x00000015,0x00000002,0x0000000f,0x0000aaa0,0x01002020,0x016800fe,0x8e578248,0x000000dd,0x00000946,0x0000035a,0x00100010,0x00000515,0x00397feb,0x00180358,0x00000455,0x00000001,0x00020003,0x00040014,0x00240034,0x00440144,0x02440344,0x04440544,0x0ee7fc00,0x00000082,0x0000002a};

unsigned char ctov[16] = {0x0b,0x0b,0x0b,0x0a,0x0d,0x0d,0x0c,0x0c,0x0f,0x0e,0x0e,0x0e,0x11,0x10,0x10,0x0f};

const unsigned int MACA_Transceiver::PSMVAL[19] = {0x0000080f,0x0000080f,0x0000080f,0x0000080f,0x0000081f,0x0000081f,0x0000081f,0x0000080f,0x0000080f,0x0000080f,0x0000001f,0x0000000f,0x0000000f,0x00000816,0x0000001b,0x0000000b,0x00000802,0x00000817,0x00000003};
const unsigned int MACA_Transceiver::PAVAL[19]  = {0x000022c0,0x000022c0,0x000022c0,0x00002280,0x00002303,0x000023c0,0x00002880,0x000029f0,0x000029f0,0x000029f0,0x000029c0,0x00002bf0,0x000029f0,0x000028a0,0x00002800,0x00002ac0,0x00002880,0x00002a00,0x00002b00};
const unsigned int MACA_Transceiver::AIMVAL[19] = {0x000123a0,0x000163a0,0x0001a3a0,0x0001e3a0,0x000223a0,0x000263a0,0x0002a3a0,0x0002e3a0,0x000323a0,0x000363a0,0x0003a3a0,0x0003a3a0,0x0003e3a0,0x000423a0,0x000523a0,0x000423a0,0x0004e3a0,0x0004e3a0,0x0004e3a0};

const unsigned char MACA_Transceiver::VCODivI[16] = {0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x30,0x30,0x30,0x30,0x30,0x30,0x30};
const unsigned int MACA_Transceiver::VCODivF[16]  = {0x00355555,0x006aaaaa,0x00a00000,0x00d55555,0x010aaaaa,0x01400000,0x01755555,0x01aaaaaa,0x01e00000,0x00155555,0x004aaaaa,0x00800000,0x00b55555,0x00eaaaaa,0x01200000,0x01555555};


void MACA_Transceiver::set_tx_buffer(volatile unsigned char * buffer, unsigned int sz) {
	tx_buffer = buffer;
	tx_size = sz;
}

void MACA_Transceiver::set_event_handler(event_handler * ev_handler) {
    handler = ev_handler;
}

MACA_Transceiver::event_handler * MACA_Transceiver::get_event_handler() {
    return handler;
}

//unsigned long a, an, at, amin, amax;
//Chronometer chrono;
void MACA_Transceiver::run() {
	//an=0;at=0;amin=1000000;amax=0;
	//chrono.start();
	act_idx = n_actions-1;
	current_slot = n_slots-1;
	current_action = SEQ_WAIT;
	current_slot_end = CPU::in32(IO::MACA_RELCLK) + slot_size[current_slot];
	CPU::out32(IO::MACA_SETIRQ, (1 << IRQ_ACPL));
	return;
}

void MACA_Transceiver::maca_isr() {
	CPU::out32(IO::ITC_INTFRC, 0); // stop forcing interrupts

	IC::disable(IC::IRQ_MACA);

	OStream cout;

	unsigned int maca_irq = CPU::in32(IO::MACA_IRQ);
	bool beacon_handled = false;
	Event call_handler = NO_EVENT;
	
	// Action started
	if(bit_is_set(maca_irq,IRQ_STRT)) {
		CPU::out32(IO::MACA_CLRIRQ, (1 << IRQ_STRT));
		//cout<<"IRQ_STRT action: "<<current_action<<" now: "<<CPU::in32(IO::MACA_RELCLK)<<" slot end: "<<current_slot_end<<"\n";
		//chrono.reset();
		//chrono.start();
		call_handler = DBG;
	}

	// Data indication
	if (bit_is_set(maca_irq,IRQ_DI)) {
		CPU::out32(IO::MACA_CLRIRQ, (1 << IRQ_DI));
//		cout<<"IRQ_DI "<<rx_buffer[1]<<" "<<current_action<<'\n';
		//cout<<"IRQ_DI now: "<<CPU::in32(IO::MACA_RELCLK)<<" slot end: "<<current_slot_end<<"\n";

		// Was waiting for a beacon and received a beacon. Sync to it.
		if(current_action == BCN_RX && (rx_buffer[1] >> 7) == 0)
		{
			current_slot = 0;
			act_idx = n_slots * (rx_buffer[1] % n_slots);
			current_slot_end = CPU::in32(IO::MACA_TIMESTAMP) -TSTXOFFSET+ slot_size[0];
			call_handler = BEACON_READY;
			beacon_handled = true;
			
			// Abort the RX sequence
			ResumeMACASync();
		}
		// Was waiting for data and received data. Report to upper layer.
		else if((current_action == SEQ_RX) && ((rx_buffer[1] >> 7) != 0))
		{
			call_handler = DATA_READY;
		}
	}

	// Action completed
	if (bit_is_set(maca_irq,IRQ_ACPL) || beacon_handled) {
		CPU::out32(IO::MACA_CLRIRQ, (1 << IRQ_ACPL));
		//cout<<"IRQ_ACPL slot: "<<current_slot<<" action: "<<current_action<<" now: "<<CPU::in32(IO::MACA_RELCLK) << " slot end: "<<current_slot_end<<'\n';

		// Stop timers
		CPU::out32(IO::MACA_TMRDIS, 
				( (1<<TMREN_CPL) |
				  (1<<TMREN_STRT) )
				);

		// Was waiting for beacon but timed out. Panic
		if(current_action == BCN_RX && bit_is_set(CPU::in32(IO::MACA_STATUS), STATUS_TIMEOUT))
		{
			current_action = SEQ_NOP;
			maca_off();

			if(handler != 0)
				handler(LOST_COMMUNICATION);

			return;
		}

		/* Calculating next slot boundaries and action.
		 * last_slot_end will be the end of the slot which action just ended,
		 * which is also the beginning of the next slot.
		 * current_slot_end will be the value of the end of the next slot.
		 */
		unsigned int last_slot_end = current_slot_end;
		// Get the next action
		act_idx = (act_idx+1)%n_actions;
		current_action = actions[act_idx];
		// Calculate next slot end
		current_slot = (current_slot + 1) % n_slots;
		current_slot_end += slot_size[current_slot];

		// Late start. Force resync. (this should not happen, but just in case)
		if((CPU::in32(IO::MACA_STATUS) & 0x07) == 0x07)
		{
			cout<<"Late start "<<current_slot<<" "<<current_action<<" "<<CPU::in32(IO::MACA_RELCLK) << " "<<current_slot_end<<'\n';
			current_action = BCN_RX;
			last_slot_end = CPU::in32(IO::MACA_RELCLK) + TIMEOUT_OFFSET;
		}

		// Do the setup required for each action
		if(current_action == SEQ_TX)
		{
			CPU::out32(IO::MACA_TXLEN, tx_size+2);
			CPU::out32(IO::MACA_DMATX, (unsigned int)tx_buffer);
			CPU::out32(IO::MACA_DMARX, (unsigned int)dummy_buffer);
		}
		else if(current_action == BCN_TX)
		{
			// Increment beacon sequence number
			tx_buffer[0] = (tx_buffer[0] & 0x80) + (((tx_buffer[0] & 0x7f) + 1) % 128);

			CPU::out32(IO::MACA_TXLEN, tx_size+2);
			CPU::out32(IO::MACA_DMATX, (unsigned int)tx_buffer);
			CPU::out32(IO::MACA_DMARX, (unsigned int)dummy_buffer);
		}
		else if(current_action == SEQ_RX)
		{
			CPU::out32(IO::MACA_TXLEN, (MAX_FRAME_SIZE << 16));
			CPU::out32(IO::MACA_DMARX, (unsigned int)rx_buffer);
		}
		else if(current_action == BCN_RX)
		{
			CPU::out32(IO::MACA_TXLEN, (MAX_FRAME_SIZE << 16));
			CPU::out32(IO::MACA_DMARX, (unsigned int)rx_buffer);

			current_slot_end = CPU::in32(IO::MACA_RELCLK) + BCN_WAIT_TIMEOUT + TIMEOUT_OFFSET;
		}

		// Set the start and timeout clocks to the next slot boundaries
		CPU::out32(IO::MACA_STARTCLK, last_slot_end);
		CPU::out32(IO::MACA_CPLCLK, current_slot_end - TIMEOUT_OFFSET);
		CPU::out32(IO::MACA_TMREN, (1 << TMREN_STRT));
		CPU::out32(IO::MACA_TMREN, (1 << TMREN_CPL));
		//cout<<"Scheduled "<<last_slot_end<<" "<<current_slot_end-TIMEOUT_OFFSET<<'\n';

		unsigned char maca_action;
		if(current_action == BCN_TX)
			maca_action = SEQ_TX;
		else if(current_action == BCN_RX)
			maca_action = SEQ_RX;
		else maca_action = current_action;

		CPU::out32(IO::MACA_CONTROL, (
					((maca_action != SEQ_TX) << CONTROL_REL)  | // Slightly delay TX start
					((maca_action == SEQ_RX) << CONTROL_AUTO) | // Turn on AUTO mode on RX
					(0 << CONTROL_ASAP)     | // Wait for STARTCLK
					(0 << CONTROL_MODE)     | // No cca
					(1 << CONTROL_NOFC)     | // No frame check
					(1 << CONTROL_PRM)      | // Promiscuous mode
					(4 << CONTROL_PRECOUNT) |
					(maca_action))
				);
	}

	if (bit_is_set(maca_irq,IRQ_SYNC)) {
		CPU::out32(IO::MACA_CLRIRQ, (1 << IRQ_SYNC));
		//cout<<"SYNC\n";
	}
	if (bit_is_set(maca_irq,IRQ_CM)) {
		CPU::out32(IO::MACA_CLRIRQ, (1 << IRQ_CM));
		//cout<<"CM\n";
	}
	if (bit_is_set(maca_irq,IRQ_SFT)) {
		CPU::out32(IO::MACA_CLRIRQ, (1 << IRQ_SFT));
		//cout<<"SFT\n";
	}
	if (bit_is_set(maca_irq,IRQ_FLT)) {
		CPU::out32(IO::MACA_CLRIRQ, (1 << IRQ_FLT));
		//cout<<"FLT\n";
	}
	if (bit_is_set(maca_irq,IRQ_CRC)) {
		CPU::out32(IO::MACA_CLRIRQ, (1 << IRQ_CRC));
		//cout<<"CRC\n";
	}
	if (bit_is_set(maca_irq,IRQ_POLL)) {
		CPU::out32(IO::MACA_CLRIRQ, (1 << IRQ_POLL));
		//cout<<"POLL\n";
	}
	if (!(maca_irq && 0xFE07)) {
		CPU::out32(IO::MACA_CLRIRQ, 0xffff);
	}

	if(handler!=0 && call_handler != NO_EVENT)
		handler(call_handler);

	IC::enable(IC::IRQ_MACA);
}

void MACA_Transceiver::maca_init() {
	reset_maca();

	radio_init();

	flyback_init();

	init_phy();

	// Relative clock is used for delaying TX slightly
	CPU::out32(IO::MACA_CLKOFFSET, CLOCK_OFFSET);

	IC::int_vector(IC::IRQ_MACA, maca_isr);
	IC::enable(IC::IRQ_MACA);
}

void MACA_Transceiver::init_phy() {
	CPU::out32(IO::MACA_CLKDIV,     MACA_CLOCK_DIV);
	CPU::out32(IO::MACA_WARMUP,     0x00180012);
	CPU::out32(IO::MACA_EOFDELAY,   0x00000004);
	CPU::out32(IO::MACA_CCADELAY,   0x0fff0022);
	CPU::out32(IO::MACA_TXCCADELAY, 0x00000025);
	CPU::out32(IO::MACA_FRAMESYNC0, 0x000000A7);
	CPU::out32(IO::MACA_CLK,        0x00000008);
	CPU::out32(IO::MACA_RXACKDELAY, 30);
	CPU::out32(IO::MACA_TXACKDELAY, 68);
	CPU::out32(IO::MACA_RXEND,      180);
	CPU::out32(IO::MACA_MASKIRQ, 
			((1 << IRQ_RST)  |
			 (1 << IRQ_CM)   |
			 (1 << IRQ_FLT)  |
			 (1 << IRQ_CRC)  |
			 (1 << IRQ_DI)   |
			 (1 << IRQ_SFT)  |
			 (1 << IRQ_ACPL) |
			 (1 << IRQ_STRT)  |
			 (1 << IRQ_SYNC)
			));
	CPU::out32(IO::MACA_SLOTOFFSET, 0x00350000);
}

void MACA_Transceiver::reset_maca() {
	volatile unsigned int cnt;

	CPU::out32(IO::MACA_RESET, (1 << RESET_RST));

	for (cnt = 0; cnt < 100; cnt++);

	CPU::out32(IO::MACA_RESET, (1 << RESET_CLK_ON));

	CPU::out32(IO::MACA_CONTROL, SEQ_NOP);

	for (cnt = 0; cnt < 400000; cnt++);

	// Clear all interrupts. 
	CPU::out32(IO::MACA_CLRIRQ, 0xffff);
}

void MACA_Transceiver::flyback_init() {
	unsigned int val8, aux;

	val8 = *(volatile unsigned int *)(RF_BASE+8);
	aux = val8 | 0x0000f7df;
	*(volatile unsigned int *)(RF_BASE + 8)  = aux;
	*(volatile unsigned int *)(RF_BASE + 12) = 0x00ffffff;
	*(volatile unsigned int *)(RF_BASE + 16) = (((unsigned int)0x00ffffff)>>12);
	*(volatile unsigned int *)(RF_BASE)      = 16;
}

void MACA_Transceiver::maca_off() {
	IC::disable(IC::IRQ_MACA);
	// turn off the radio regulators
	CPU::out32(0x80003048, 0x00000f00);
	// hold the maca in reset
	CPU::out32(IO::MACA_RESET, RESET_RST);
}

void MACA_Transceiver::maca_on() {
	// turn the radio regulators back on
	CPU::out32(0x80003048, 0x00000f78);
	// reinitialize the phy
	reset_maca();
	init_phy();

	IC::enable(IC::IRQ_MACA);
}

unsigned char MACA_Transceiver::get_ctov(unsigned int r0, unsigned int r1) {
	r0 = r0 * INIT_CTOV_WORD_1;
	r0 += (r1 << 22);
	r0 += INIT_CTOV_WORD_2;

	r0 = (unsigned int)(((signed int)r0) >> 25);

	return (unsigned char)r0;
}

void MACA_Transceiver::radio_init() {
	volatile unsigned int i;
	// sequence 1
	*(volatile unsigned int *)(0x8000304c) = 0x00607707;

	// seq 1 delay
	for (i = 0; i < 0x161a8; i++);

	// sequence 2
	for (i = 0; i < MAX_SEQ2; i++)
		*(volatile unsigned int *)(addr_seq2[i]) = data_seq2[i];

	// modem val
	*(volatile unsigned int *)0x80009000 = 0x80050100;

	// cal 3 seq 1
	for (i = 0; i < MAX_CAL3_SEQ1; i++)
		*(volatile unsigned int *)(addr_cal3_seq1[i]) = data_cal3_seq1[i];

	// cal 3 delay
	for (i = 0; i < 0x11194; i++);

	// cal 3 seq 2
	for (i = 0; i < MAX_CAL3_SEQ2; i++)
		*(volatile unsigned int *)(addr_cal3_seq2[i]) = data_cal3_seq2[i];

	// cal 3 delay
	for (i = 0; i < 0x11194; i++);

	// cal 3 seq 3
	for (i = 0; i < MAX_CAL3_SEQ3; i++)
		*(volatile unsigned int *)(addr_cal3_seq3[i]) = data_cal3_seq3[i];

	// cal 5
	for (i = 0; i < MAX_CAL5; i++)
		*(volatile unsigned int *)(addr_cal5[i]) = data_cal5[i];

	//reg replacment
	for (i = 0; i < MAX_DATA; i++)
		*(volatile unsigned int *)(addr_reg_rep[i]) = data_reg_rep[i];

	init_from_flash(0x1F000);

	for (i = 0; i < 16; i++)
		ctov[i] = get_ctov(i,ram_values[3]);
}

void MACA_Transceiver::set_demodulator_type(unsigned char demod) {
	unsigned int val = CPU::in32(RF_REG);
	if (demod == DEMOD_NCD)
		val = (val & ~1);
	else
		val = (val | 1);

	CPU::out32(RF_REG, val);
}

void MACA_Transceiver::set_power(unsigned char power) {
	IC::disable(IC::IRQ_MACA);

	CPU::out32(ADDR_POW1, PSMVAL[power]);

	// see http://devl.org/pipermail/mc1322x/2009-October/000065.html
	CPU::out32(ADDR_POW2, 0x00002000 | PAVAL[power]);

	CPU::out32(ADDR_POW3, AIMVAL[power]);

	IC::enable(IC::IRQ_MACA);
}

void MACA_Transceiver::set_channel(unsigned char chan) {
	volatile unsigned int tmp;
	IC::disable(IC::IRQ_MACA);

	tmp = CPU::in32(ADDR_CHAN1);
	tmp = tmp & 0xbfffffff;
	CPU::out32(ADDR_CHAN1, tmp);

	CPU::out32(ADDR_CHAN2, VCODivI[chan]);
	CPU::out32(ADDR_CHAN3, VCODivF[chan]);

	tmp = CPU::in32(ADDR_CHAN4);
	tmp = tmp | 2;
	CPU::out32(ADDR_CHAN4, tmp);

	tmp = CPU::in32(ADDR_CHAN4);
	tmp = tmp | 4;
	CPU::out32(ADDR_CHAN4, tmp);

	tmp = tmp & 0xffffe0ff;
	tmp = tmp | (((ctov[chan])<<8)&0x1F00);
	CPU::out32(ADDR_CHAN4, tmp);

	IC::enable(IC::IRQ_MACA);
}

/**
 * Processes up to 4 words of initialization entries.
 * Returns the number of words processed.
 */
unsigned int MACA_Transceiver::exec_init_entry(volatile unsigned int *entries, unsigned char *valbuf) {
	volatile unsigned int i;

	if (entries[0] <= ROM_END) {
		if (entries[0] == 0) {
			// do delay command
			for (i = 0; i < entries[1]; i++);

			return 2;

		} else if (entries[0] == 1) {
			// do bit set/clear command
			CPU::out32(entries[2], (CPU::in32(entries[2]) & ~entries[1]) | (entries[3] & entries[1]));

			return 4;

		} else if ((entries[0] >= 16) && (entries[0] < 0xfff1)) {
			// store bytes in valbuf
			valbuf[(entries[0]>>4)-1] = entries[1];

			return 2;

		} else if (entries[0] == ENTRY_EOF) {
			return 0;

		} else {
			// invalid command code
			return 0;
		}

	} else { // address isn't in ROM space
		// do store value in address command 
		if ((unsigned int) entries[0] != (unsigned int) IO::CRM_VREG_CNTL)
			CPU::out32(entries[0], entries[1]);

		return 2;
	}
}

unsigned int MACA_Transceiver::init_from_flash(unsigned int addr) {
	MC13224V_Flash::nvmType_t type = MC13224V_Flash::gNvmType_NoNvm_c;
	MC13224V_Flash::nvmErr_t err;
	volatile unsigned int buf[8];
	volatile unsigned int len;
	volatile unsigned int i = 0;

	MC13224V_Buck_Regulator::NVM_1P8V_enable();

	err = MC13224V_Flash::nvm_detect(MC13224V_Flash::gNvmInternalInterface_c, &type);

	MC13224V_Flash::nvm_setsvar(0);

	err = MC13224V_Flash::nvm_read(MC13224V_Flash::gNvmInternalInterface_c, type, (unsigned char *)buf, addr, 8);
	i += 8;

	if (buf[0] == FLASH_INIT_MAGIC) {
		len = buf[1] & 0x0000ffff;

		while (i < (len - 4)) {
			err = MC13224V_Flash::nvm_read(MC13224V_Flash::gNvmInternalInterface_c, type, (unsigned char *) buf, addr + i, 32);

			i += 4 * exec_init_entry(buf, ram_values);
		}

		return i;

	} else
		return 0;
}

/**
 * Do the ABORT-Wait-NOP-Wait sequence in order to prevent MACA malfunctioning.
 * This sequence is synchronous and no interrupts should be triggered when it is done.
 */
void MACA_Transceiver::ResumeMACASync() {
	volatile unsigned int clk, TsmRxSteps, LastWarmupStep, LastWarmupData, LastWarmdownStep, LastWarmdownData;
	volatile unsigned int i;
//	IC::disable(IC::IRQ_MACA);

	// Manual TSM modem shutdown

	// read TSM_RX_STEPS
	TsmRxSteps = (*((volatile unsigned int *)(0x80009204)));

	// isolate the RX_WU_STEPS
	// shift left to align with 32-bit addressing
	LastWarmupStep = (TsmRxSteps & 0x1f) << 2;
	// Read "current" TSM step and save this value for later
	LastWarmupData = (*((volatile unsigned int *)(0x80009300 + LastWarmupStep)));

	// isolate the RX_WD_STEPS
	// right-shift bits down to bit 0 position
	// left-shift to align with 32-bit addressing
	LastWarmdownStep = ((TsmRxSteps & 0x1f00) >> 8) << 2;
	// write "last warmdown data" to current TSM step to shutdown rx
	LastWarmdownData = (*((volatile unsigned int *)(0x80009300 + LastWarmdownStep)));
	(*((volatile unsigned int *)(0x80009300 + LastWarmupStep))) = LastWarmdownData;

	CPU::out32(IO::MACA_CONTROL, SEQ_ABORT);

	// Wait ~8us
	for (clk = CPU::in32(IO::MACA_CLK), i = 0; CPU::in32(IO::MACA_CLK) - clk < 3 && i < 300; i++);

	CPU::out32(IO::MACA_CONTROL, SEQ_NOP);

	// Wait ~8us
	for (clk = CPU::in32(IO::MACA_CLK), i = 0; CPU::in32(IO::MACA_CLK) - clk < 3 && i < 300; i++);

	// restore original "last warmup step" data to TSM (VERY IMPORTANT!!!)
	(*((volatile unsigned int *)(0x80009300 + LastWarmupStep))) = LastWarmupData;

	// Clear all MACA interrupts - we should have gotten the ABORT IRQ
	CPU::out32(IO::MACA_CLRIRQ, 0xFFFF);

//	IC::enable(IC::IRQ_MACA);

}

void MACA_Transceiver::off() {
	CPU::out32(IO::MACA_CONTROL, SEQ_NOP);
}

__END_SYS
