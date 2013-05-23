// EPOS PC Intel PRO/100 (i82559) Ethernet NIC Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __e100_h
#define __e100_h

#include <ethernet.h>

__BEGIN_SYS

class i82559
{

protected:
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg16 Reg16;
    typedef CPU::Reg32 Reg32;
    typedef CPU::Log_Addr Log_Addr;
    typedef CPU::Phy_Addr Phy_Addr;
    typedef CPU::IO_Port IO_Port;
    typedef CPU::IO_Irq IO_Irq;
    typedef MMU::DMA_Buffer DMA_Buffer;
    typedef Ethernet::Address MAC_Address;

public:

    #define FRAME_SIZE 1520 //Verify if neeeded

    #define offsetof(TYPE, MEMBER) ((Reg32) &((TYPE *)0)->MEMBER)
    #define write32(b,addr) (*(volatile unsigned int *) (addr) = (b))
    #define read32(addr) (*(volatile unsigned int *) (addr))
    #define MII_LED_CONTROL 0x1B
    #define ETH_ALEN 6
    #define i82559_WAIT_SCB_TIMEOUT 20000
    #define i82559_WAIT_SCB_FAST 20
    #define i82559_WAIT_SCB_TIMEOUT_TRY 20
    #define i82559_WAIT_SCB_FAST_TRY 4

    //  EEPROM_Ctrl bits.
    #define EE_SHIFT_CLK    0x01            // EEPROM shift clock.
    #define EE_CS           0x02            // EEPROM chip select.
    #define EE_DATA_WRITE   0x04            // EEPROM chip data in.
    #define EE_DATA_READ    0x08            // EEPROM chip data out.
    #define EE_ENB          (0x4800 | EE_CS)

    // The EEPROM commands include the always-set leading bit.
    #define EE_WRITE_CMD(a)     (5 << (a))
    #define EE_READ_CMD(a)      (6 << (a))
    #define EE_ERASE_CMD(a)     (7 << (a))
    #define EE_WRITE_EN_CMD(a)  (19 << ((a) - 2))
    #define EE_WRITE_DIS_CMD(a) (16 << ((a) - 2))
    #define EE_ERASE_ALL_CMD(a) (18 << ((a) - 2))
    #define EE_TOP_CMD_BIT(a)   ((a) + 2) // Counts down to zero
    #define EE_TOP_DATA_BIT     (15)      // Counts down to zero
    #define EEPROM_ENABLE_DELAY (1000)    //(10) // Delay at chip select
    #define EEPROM_SK_DELAY     (1000)    // Delay between clock edges *and* data read or transition; 3 of these per bit.
    #define EEPROM_DONE_DELAY   (1000)    // Delay when all done

    enum eeprom_ctrl_lo {
        eesk = 0x01, //EEPROM Data Output (Flash Address[15])
        eecs = 0x02, //EEPROM Chip Select
        eedi = 0x04, //EEPROM Data Input
        eedo = 0x08, //EEPROM Data Output (Flash Address[14])
    };

    /* CSR (Control/Status Registers) */
    typedef struct csr {
        struct {
            volatile Reg8 status;
            volatile Reg8 stat_ack;
            volatile Reg8 cmd_lo;
            volatile Reg8 cmd_hi;
            volatile Reg32 gen_ptr;
        } scb;
        volatile Reg32 port;
        volatile Reg16 flash_ctrl;
        volatile Reg8 eeprom_ctrl_lo;
        volatile Reg8 eeprom_ctrl_hi;
        volatile Reg32 mdi_ctrl;
        volatile Reg32 rx_dma_count;
    } CSR_Desc;

    enum port {
        SOFTWARE_RESET  = 0x0000,
        SELFTEST        = 0x0001,
        SELECTIVE_RESET = 0x0002,
    };

    enum scb_status {
        rus_idle         = 0x00,
        rus_suspended    = 0x04,
        rus_no_resources = 0x08,
        rus_ready        = 0x10,
        rus_mask         = 0x3C,
        cus_idle         = 0xC0, // !cus_idle
        cus_suspended    = 0x40,
    };

    enum scb_cmd {
        //specific interrupt mask bits
        simb_intr_cx_mask  = 0x8000,
        simb_intr_fr_mask  = 0x4000,
        simb_intr_cna_mask = 0x2000,
        simb_intr_rnr_mask = 0x1000,
        simb_intr_er_mask  = 0x0800,
        simb_intr_fcp_mask = 0x0400,
        scb_cmd_si         = 0x0200,
        scb_cmd_m          = 0x0100,
    };

    enum rfd_status {
        rfd_crc_error   = 0x800,
        rfd_align_error = 0x400,
        rfd_out_space   = 0x200,
        rfd_dma_overrun = 0x100,
        rfd_frame_short = 0x080,
        rfd_type        = 0x020,
        rfd_recv_error  = 0x010,
        rfd_no_addr_mat = 0x004,
        rfd_ia_match    = 0x002,
        rfd_recv_colli  = 0x001,
    };

    enum tx_rx_status {
        Rx_RFD_NOT_FILLED = 0x0,
        Rx_RFD_AVAILABLE = 0x1,
        Tx_CB_AVAILABLE = 0x0,
        Tx_CB_IN_USE = 0x1,
    };

    enum cuc_status {
        CUC_RUNNING = 0,
        CUC_IDLE = 1,
        CUC_SUSPENDED = 2,
    };

    enum ruc_status  {
        RUC_RUNNING   = 0,
        RUC_IDLE = 1,
        RUC_SUSPENDED = 2,
    };

    enum cb_status {
        cb_complete = 0x8000,
        cb_ok       = 0x2000,
    };

    enum scb_stat_ack {
        stat_ack_not_ours    = 0x00,
        stat_ack_sw_gen      = 0x04,
        stat_ack_rnr         = 0x10,
        stat_ack_cu_idle     = 0x20,
        stat_ack_frame_rx    = 0x40,
        stat_ack_cu_cmd_done = 0x80,
        stat_ack_not_present = 0xFF,
        stat_ack_rx = (stat_ack_sw_gen | stat_ack_rnr | stat_ack_frame_rx),
        stat_ack_tx = (stat_ack_cu_idle | stat_ack_cu_cmd_done),
    };

    enum scb_cmd_hi {
        irq_mask_none = 0x00,
        irq_mask_all  = 0x01,
        irq_sw_gen    = 0x02,
    };

    enum scb_cmd_lo {
        cuc_nop        = 0x00,
        ruc_start      = 0x01,
        ruc_resume     = 0x02,
        ruc_dma_redir  = 0x03,
        ruc_about      = 0x04,
        ruc_load_head  = 0x05,
        ruc_load_base  = 0x06,
        cuc_start      = 0x10,
        cuc_resume     = 0x20,
        cuc_dump_addr  = 0x40,
        cuc_dump_stats = 0x50,
        cuc_load_base  = 0x60,
        cuc_dump_reset = 0x70,
    };



    struct config {
    /*0*/   Reg8 byte_count:6, pad0:2;
    /*1*/   Reg8 rx_fifo_limit:4, tx_fifo_limit:3, pad1:1;
    /*2*/   Reg8 adaptive_ifs;
    /*3*/   Reg8 mwi_enable:1, type_enable:1, read_align_enable:1,term_write_cache_line:1, pad3:4;
    /*4*/   Reg8 rx_dma_max_count:7, pad4:1;
    /*5*/   Reg8 tx_dma_max_count:7, dma_max_count_enable:1;
    /*6*/   Reg8 pad6:1, direct_rx_dma:1, tco_statistics:1, ci_intr:1, standard_tcb:1, standard_stat_counter:1,
                 rx_discard_overruns:1, rx_save_bad_frames:1;
    /*7*/   Reg8 rx_discard_short_frames:1, tx_underrun_retry:2, pad7:3, tx_two_frames_in_fifo:1, tx_dynamic_tbd:1;
    /*8*/   Reg8 mii_mode:1, pad8:6, csma_disabled:1;
    /*9*/   Reg8 rx_tcpudp_checksum:1, pad9_1:3, vlan_arp_tco:1, link_status_wake:1, pad9_2:2;
    /*10*/  Reg8 pad10:3, no_source_addr_insertion:1, preamble_length:2, loopback:2;
    /*11*/  Reg8 pad11:8;
    /*12*/  Reg8 pad12_0:1, pad12_1:3, ifs:4;
    /*13*/  Reg8 ip_addr_lo;
    /*14*/  Reg8 ip_addr_hi;
    /*15*/  Reg8 promiscuous_mode:1, broadcast_disabled:1, wait_after_win:1, 
                 pad15_1:1, ignore_ul_bit:1, crc_16_bit:1, pad15_2:1, crs_or_cdt:1;
    /*16*/  Reg8 fc_delay_lo;
    /*17*/  Reg8 fc_delay_hi;
    /*18*/  Reg8 rx_stripping:1, tx_padding:1, rx_crc_transfer:1, rx_long_ok:1, fc_priority_threshold:3, pad18:1;
    /*19*/  Reg8 pad19:1, magic_packet_disable:1, fc_disable:1, fc_restop:1, 
                 fc_restart:1, fc_reject:1, full_duplex_force:1, full_duplex_pin:1;
    /*20*/  Reg8 pad20_1:5, fc_priority_location:1, multi_ia:1, pad20_2:1;
    /*21*/  Reg8 pad21_1:3, multicast_all:1, pad21_2:4;
    };

    struct stats {
        Reg32 tx_good_frames, tx_max_collisions, tx_late_collisions,
              tx_underruns, tx_lost_crs, tx_deferred, tx_single_collisions,
              tx_multiple_collisions, tx_total_collisions;
        Reg32 rx_good_frames, rx_crc_errors, rx_alignment_errors,
              rx_resource_errors, rx_overrun_errors, rx_cdt_errors,
              rx_short_frame_errors;
        Reg32 fc_xmt_pause, fc_rcv_pause, fc_rcv_unsupported;
        Reg16 xmt_tco_frames, rcv_tco_frames;
        Reg32 complete;
    };

    enum cb_command {
        cb_nop    = 0x0000,
        cb_iaaddr = 0x0001,
        cb_config = 0x0002,
        cb_multi  = 0x0003,
        cb_tx     = 0x0004,
        cb_ucode  = 0x0005,
        cb_dump   = 0x0006,
        cb_tx_sf  = 0x0008,
        cb_cid    = 0x1f00,
        cb_i      = 0x2000,
        cb_s      = 0x4000,
        cb_el     = 0x8000,
    };

    enum mdi_ctrl {
        mdi_write = 0x04000000,
        mdi_read  = 0x08000000,
        mdi_ready = 0x10000000,
    };

    enum led_state {
        led_on     = 0x01,
        led_off    = 0x04,
        led_on_559 = 0x05,
        led_on_82559 = 0x07,
        led_on_557 = 0x07,
    };

    struct mem {
        struct {
            volatile Reg32 signature;
            volatile Reg32 result;
        } selftest;
        struct stats stats;
        Reg8 dump_buf[596];
    };

    struct Control {
        volatile Reg16 status;
        volatile Reg16 command;
        volatile Reg32 link;
    };

    struct ConfigureCB: public Control {
        struct config config;
    };

    struct MACaddrCB: public Control {
	MAC_Address iaaddr;
    };

    // Transmit and Receive Descriptors (in the Receive Ring Buffer)
    struct Desc: public Control {
    };

    // Receive Descriptor
    struct Rx_Desc: public Desc {
        volatile Reg32 rbd;
        volatile Reg16 actual_size;
        volatile Reg16 size;
        char frame[FRAME_SIZE];
    };

    // Transmit Descriptor
    struct Tx_Desc: public Desc {
        volatile Reg32 tbd_array;
        volatile Reg16 tcb_byte_count;
        volatile Reg8 threshold;
        volatile Reg8 tbd_count;
        char frame[FRAME_SIZE];
    };


public:
    int log2(int n) {
        int log2_n = 0;
        for(; n > 1; n >>= 1, log2_n++);
        return log2_n;
    }

};

class E100: public Ethernet, private i82559
{
private:
    // PCI ID
    static const unsigned int PCI_VENDOR_ID = 0x8086;
    static const unsigned int PCI_DEVICE_ID = 0x1229;
    static const unsigned int PCI_REG_IO = 1;
    static const unsigned int PCI_REG_MEM = 0;

    // Transmit and Receive Ring Bbuffer sizes
    static const unsigned int UNITS = Traits<E100>::UNITS;
    static const unsigned int TX_BUFS =	Traits<E100>::SEND_BUFFERS;
    static const unsigned int RX_BUFS =	Traits<E100>::RECEIVE_BUFFERS;
    static const unsigned int DMA_BUFFER_SIZE = 
        ((sizeof(ConfigureCB) + 15) & ~15U) +
        ((sizeof(MACaddrCB) + 15) & ~15U) +
        ((sizeof(struct mem) + 15) & ~15U) +
         RX_BUFS * ((sizeof(Rx_Desc) + 15) & ~15U) +
         TX_BUFS * ((sizeof(Tx_Desc) + 15) & ~15U); // GCC mess up MMU::align128

    // Share control and interrupt dispatiching info
    struct Device
    {
	E100 * device;
	unsigned int interrupt;
	bool in_use;
    };
	
public:
    typedef CPU::Log_Addr Log_Addr;
    typedef CPU::Phy_Addr Phy_Addr;
    typedef CPU::IO_Irq IO_Irq;
    typedef MMU::DMA_Buffer DMA_Buffer;

public:
    E100(unsigned int unit = 0);

    ~E100();

    int send(const Address & dst, const Protocol & prot,
 	     const void * data, unsigned int size);

    int receive(Address * src, Protocol * prot,
		void * data, unsigned int size);

    void reset();

    unsigned int mtu() { return MTU; }

    const Address & address() { return _address; }

    const Statistics & statistics() { return _statistics; }

    static void init(unsigned int unit);

private:
    E100(unsigned int unit, Log_Addr io_mem, IO_Irq irq, DMA_Buffer * dma);

    void handle_int();

    static void int_handler(unsigned int interrupt);

    bool verifyPendingInterrupts(void);

    unsigned short eeprom_read(unsigned short *addr_len, unsigned short addr);
    unsigned char eeprom_mac_address(Reg16 addr);

    int exec_command(Reg8 cmd, Reg32 dma_addr);

    void i82559_flush() { read8(&csr->scb.status); }
    void i82559_disable_irq() { write8(irq_mask_all, &csr->scb.cmd_hi); }
    void i82559_enable_irq() { write8(irq_mask_none, &csr->scb.cmd_hi); }

    void udelay(long long d) {
        TSC::Time_Stamp end;
        d *= TSC::frequency() / 1000000;
        end = TSC::time_stamp() + d;
        while(end > TSC::time_stamp());
    }

    int self_test();

    void software_reset() {
        write32(SELECTIVE_RESET, &csr->port);
        i82559_flush(); udelay(20 * 1000);
        write32(SOFTWARE_RESET, &csr->port);
        i82559_flush(); udelay(20 * 1000);
        // disable IRQs 
        i82559_disable_irq();
        i82559_flush(); udelay(1000);
    }

    static inline unsigned char read8(const volatile void *addr) {
	return *((volatile unsigned char*) addr);
    };

    static inline void write8(unsigned char b, volatile void *addr) {
	*((volatile unsigned char*) addr) = b;
    };

    static inline unsigned short read16(const volatile void *addr) {
	return *((volatile unsigned short*) addr);
    };

    static inline void write16(unsigned short b, volatile void *addr) {
	*((volatile unsigned short*) addr) = b;
    };

    void i82559_configure(void);

    static E100 * get(unsigned int interrupt) {
	for(unsigned int i = 0; i < UNITS; i++)
	    if(_devices[i].interrupt == interrupt)
		return _devices[i].device;
	return 0;
    };

private:
    unsigned int _unit;

    Address _address;
    Statistics _statistics;

    Log_Addr _io_mem;
    IO_Irq _irq;
    DMA_Buffer * _dma_buf;

    volatile unsigned int _rx_ruc_no_more_resources;
    volatile unsigned int _tx_cuc_suspended;

    CSR_Desc * csr;

    ConfigureCB * configCB;
    Phy_Addr _configCB_phy;

    MACaddrCB * macAddrCB;
    Phy_Addr _macAddrCB_phy;

    struct mem * dmadump;
    Phy_Addr _dmadump_phy;

    int _rx_cur, _rx_last_el;
    Rx_Desc * _rx_ring;
    Phy_Addr _rx_ring_phy;

    int _tx_cur, _tx_prev;
    Tx_Desc * _tx_ring;
    Phy_Addr _tx_ring_phy;

    unsigned int _tx_frames_sent;

    static Device _devices[UNITS];
};

__END_SYS

#endif
