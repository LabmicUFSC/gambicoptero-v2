// EPOS-- MC13224V Transceiver Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __mc13224v_transceiver_h
#define __mc13224v_transceiver_h

#include <machine.h>

__BEGIN_SYS

class MC13224V_Transceiver
{
private:
    typedef IO_Map<Machine> IO;

    enum Register_Bits {
        // MACA_CONTROL
        CONTROL_ISM      = 20,
        CONTROL_PRECOUNT = 16,  // Preamble Repeat Counter
        CONTROL_RTSO     = 15,  // Reset Slot Counter
        CONTROL_ROLE     = 13,  // Set if PAN coordinator
        CONTROL_NOFC     = 12,  // Set to disable FCS
        CONTROL_PRM      = 11,  // Set for promiscuous mode
        CONTROL_REL      = 10,  // 1 for relative, 0 for absolute clock
        CONTROL_ASAP     = 9,   // 1 start ASAP, 0 timer start
        CONTROL_BCN      = 8,   // 1 beacon only, 0 for all packets
        CONTROL_AUTO     = 7,   // 1 continuous rx, 0 rx only once
        CONTROL_LFSR     = 6,   // 1 use polynomial for Turbolink
        CONTROL_TM       = 5,   // 1 test mode, 0 normal mode
        CONTROL_MODE     = 3,   // 2 slotted CSMA-CA, 1 non slotted CSMA-CA, 0 no CCA
        CONTROL_SEQUENCE = 0,   // 7 ED, 6 CCA, 5 TxPoll, 4 Rx, 3 Tx, 2 Wait, 1 Abort, 0 Nop

        // MACA_STATUS
        STATUS_TIMEOUT = 15,
        STATUS_CRC     = 14,
        STATUS_BUSY    = 13,
        STATUS_OVR     = 12,
        STATUS_NO_ACK  = 5,
        STATUS_CODE    = 0,

        // MACA_IRQ
        IRQ_STRT = 15,
        IRQ_SYNC = 14,
        IRQ_CM   = 13,
        IRQ_CRC  = 12,
        IRQ_FLT  = 11,
        IRQ_SFT  = 10,
        IRQ_LVL  = 9,
        IRQ_RST  = 4,
        IRQ_WU   = 3,
        IRQ_DI   = 2,
        IRQ_POLL = 1,
        IRQ_ACPL = 0,

        // MACA_RESET
        RESET_CLK_ON = 1,
        RESET_RST    = 0,

        // MACA_TMREN
        TMREN_SFT  = 2,
        TMREN_CPL  = 1,
        TMREN_STRT = 0,
    };

    // MACA_CONTROL Sequence Codes
    enum {
        SEQ_NOP,
        SEQ_ABORT,
        SEQ_WAIT,
        SEQ_TX,
        SEQ_RX,
        SEQ_TXPOLL,
        SEQ_CCA,
        SEQ_ED,
    };

    // MACA_CONTROL Transmission Modes
    enum {
        MODE_NO_CCA,
        MODE_NON_SLOTTED_CSMA_CA,
        MODE_SLOTTED_CSMA_CA,
    };

    enum {
        DEMOD_NCD, // -100dBm, 24.2mA
        DEMOD_DCD, // -96dBm,  22.2mA
    };

    // Checksum filtering
    enum {
        USE_FCS,
        NO_FCS,
    };

    static const unsigned int FLASH_INIT_MAGIC = 0x00000abc;
    static const unsigned int ROM_END          = 0x0013ffff;
    static const unsigned int ENTRY_EOF        = 0x00000e0f;
    static const unsigned int RF_BASE          = 0x80009a00;
    static const unsigned int GET_LQI_ADDR     = 0x0000e04d;

    static const unsigned int MAX_SEQ1 = 2;
    static const unsigned int MAX_SEQ2 = 2;

    static const unsigned int MAX_CAL3_SEQ1 = 3;
    static const unsigned int MAX_CAL3_SEQ2 = 2;
    static const unsigned int MAX_CAL3_SEQ3 = 1;

    static const unsigned int MAX_CAL5 = 4;
    static const unsigned int MAX_DATA = 43;

    static const unsigned int INIT_CTOV_WORD_1 = 0x00dfbe77;
    static const unsigned int INIT_CTOV_WORD_2 = 0x023126e9;

    static const unsigned int RF_REG = 0x80009400;

    static const unsigned int ADDR_POW1 = 0x8000a014;
    static const unsigned int ADDR_POW2 = ADDR_POW1 + 12;
    static const unsigned int ADDR_POW3 = ADDR_POW1 + 64;

    static const unsigned int ADDR_CHAN1 = 0x80009800;
    static const unsigned int ADDR_CHAN2 = ADDR_CHAN1 + 12;
    static const unsigned int ADDR_CHAN3 = ADDR_CHAN1 + 16;
    static const unsigned int ADDR_CHAN4 = ADDR_CHAN1 + 48;

    static const unsigned int addr_seq2[MAX_SEQ2];
    static const unsigned int data_seq2[MAX_SEQ2];

    static const unsigned int addr_cal3_seq1[MAX_CAL3_SEQ1];
    static const unsigned int data_cal3_seq1[MAX_CAL3_SEQ1];

    static const unsigned int addr_cal3_seq2[MAX_CAL3_SEQ2];
    static const unsigned int data_cal3_seq2[MAX_CAL3_SEQ2];

    static const unsigned int addr_cal3_seq3[MAX_CAL3_SEQ3];
    static const unsigned int data_cal3_seq3[MAX_CAL3_SEQ3];

    static const unsigned int addr_cal5[MAX_CAL5];
    static const unsigned int data_cal5[MAX_CAL5];

    static const unsigned int addr_reg_rep[MAX_DATA];
    static const unsigned int data_reg_rep[MAX_DATA];

    static const unsigned int PSMVAL[19];
    static const unsigned int PAVAL[19];
    static const unsigned int AIMVAL[19];

    static const unsigned char VCODivI[16];
    static const unsigned int VCODivF[16];

    static bool bit_is_set(unsigned int val, unsigned int bit) {
        return (((val & (1 << bit)) >> bit) == 1);
    }

public:
    MC13224V_Transceiver() {}
    ~MC13224V_Transceiver() {}

    enum Event {
        SFD_DETECTED = 0,
        FRAME_END
    };

    typedef void (event_handler)(Event);

    static const unsigned int NUM_PACKETS     = 32;
    static const unsigned int MACA_CLOCK_DIV  = 95; // for 250kHz clock
    static const unsigned int CLK_PER_BYTE    = 8;
    static const unsigned int RECV_SOFTIMEOUT = 1024 * 128 * CLK_PER_BYTE;
    static const unsigned int CPL_TIMEOUT     = 2 * 128 * CLK_PER_BYTE;
    static const unsigned int MAX_PACKET_SIZE = 125 + 2; // payload + checksum

    struct packet {
        unsigned char length; // does not include FCS checksum
        volatile struct packet * left;
        volatile struct packet * right;
        // offset into data for first byte of the packet payload
        // On TX this should be 0
        // On RX this should be 1 since the maca puts the length as the first byte*/
        unsigned char offset;
        unsigned char data[MAX_PACKET_SIZE + 1]; // +2 for FCS; + 1 since maca returns the length as the first byte
    };

    typedef struct packet packet_t;

    // maca initialization and on off routines
    static void maca_init();
    static void maca_off();
    static void maca_on();

    // maca configuration interface
    void set_power(unsigned char power);
    void set_channel(unsigned char chan);

    void set_demodulator_type(unsigned char demod);

    // maca packet interface
    void tx_packet(volatile packet_t * p);
    void fill_packet(volatile packet_t * p, unsigned char * data, unsigned int size);
    volatile packet_t * rx_packet();
    static volatile packet_t * get_free_packet();
    static void free_packet(volatile packet_t * p);
    static void free_all_packets();
    static void add_to_rx(volatile packet_t * p);

    typedef unsigned int (*get_lqi_func)(void);
    static get_lqi_func get_lqi;

    static void post_receive();
    static void post_tx();
    static void free_tx_head();
    static void maca_isr();
    static void decode_status();

    static bool cca_measurement();
    static void reset_maca();
    static void init_phy();
    static void flyback_init();
    static void ResumeMACASync();
    static void radio_init();
    static unsigned char get_ctov(unsigned int r0, unsigned int r1);
    static unsigned int init_from_flash(unsigned int addr);
    static unsigned int exec_init_entry(volatile unsigned int *entries, unsigned char *valbuf);

    void set_event_handler(event_handler ev_handler);
    event_handler * get_event_handler();

private:
    static volatile bool cca_result;

    static volatile unsigned char fcs_mode;
    static volatile unsigned char current_action;

    static volatile unsigned int maca_entry;

    static volatile packet_t packet_pool[NUM_PACKETS];
    static volatile packet_t * rx_head;
    static volatile packet_t * tx_head;
    static volatile packet_t * free_head, * rx_end, * tx_end, * dma_tx, * dma_rx;
    static volatile packet_t dummy_ack;

    static unsigned char ram_values[4];

    static event_handler * handler;
};

__END_SYS

#endif

