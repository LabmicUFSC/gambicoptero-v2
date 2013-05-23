// EPOS PC Intel PRO/100 (i82559) Ethernet NIC Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/pc/machine.h>
#include <mach/pc/e100.h>

__BEGIN_SYS

// Class attributes
E100::Device E100::_devices[UNITS];

// Class Methods
void E100::int_handler(unsigned int interrupt)
{
    E100 * dev = get(interrupt);

    db<E100>(TRC) << "E100::int_handler(int=" << interrupt
		  << ",dev=" << dev << ")\n";
    if(!dev)
	db<E100>(WRN) << "E100::int_handler: handler not found\n";
    else 
	dev->handle_int();
}

// Methods
E100::E100(unsigned int unit)
{
    db<E100>(TRC) << "E100(unit=" << unit << ")\n";

    // Share control
    if(unit >= UNITS) {
	db<E100>(WRN) << "E100: requested unit (" << unit 
		      << ") does not exist!\n";
	return;
    }

    // Share control
    if(_devices[unit].in_use) {
	db<E100>(WRN) << "E100: device already in use!\n";
	return;
    }

    *this = *_devices[unit].device;

    // Lock device
    _devices[unit].in_use = true;
}

E100::~E100()
{
    db<E100>(TRC) << "~E100(unit=" << _unit << ")\n";

    // Unlock device
    _devices[_unit].in_use = false;
}

E100::E100(unsigned int unit, 
	   Log_Addr io_mem, IO_Irq irq, DMA_Buffer * dma_buf)
{
    db<E100>(TRC) << "E100(unit=" << unit << ",io=" << io_mem 
		  << ",irq=" << irq << ",dma=" << dma_buf << ")\n";

    _unit = unit;
    _io_mem = io_mem;
    _irq = irq;
    _dma_buf = dma_buf;
    csr = (CSR_Desc *)io_mem;

    // Distribute the DMA_Buffer allocated by init()
    Log_Addr log = _dma_buf->log_address();
    Phy_Addr phy = _dma_buf->phy_address();

    // Initialization Block
    _configCB_phy = phy;
    configCB = log;
    log += MMU::align128(sizeof(ConfigureCB));
    phy += MMU::align128(sizeof(ConfigureCB));

    _macAddrCB_phy = phy;
    macAddrCB = log;
    log += MMU::align128(sizeof(macAddrCB));
    phy += MMU::align128(sizeof(macAddrCB));

    _dmadump_phy = phy;
    dmadump = log;
    log += MMU::align128(sizeof(struct mem));
    phy += MMU::align128(sizeof(struct mem));

    // Rx_Desc Ring
    _rx_cur = 0;
    _rx_last_el = RX_BUFS - 1;
    _rx_ring = log;
    _rx_ring_phy = phy;

    db<E100> (TRC) << "E100(_rx_ring malloc of " << RX_BUFS << " units)\n";

    // Rx (RFDs)
    unsigned int i;
    for(i = 0; i < RX_BUFS; i++) {
        log += MMU::align128(sizeof(Rx_Desc));
        phy += MMU::align128(sizeof(Rx_Desc));
        _rx_ring[i].command = 0;
        _rx_ring[i].size = sizeof(Frame);
        _rx_ring[i].status = Rx_RFD_AVAILABLE;
        _rx_ring[i].actual_size = 0;
        _rx_ring[i].link = phy; //next RFD
    }
    _rx_ring[i-1].command = cb_el;
    _rx_ring[i-1].link = _rx_ring_phy;

    // Tx_Desc Ring
    _tx_cur = 1;
    _tx_prev = 0;
    _tx_ring = log;
    _tx_ring_phy = phy;

    db<E100> (TRC) << "E100(_tx_ring malloc of " << TX_BUFS << " units)\n";
    // TxCBs
    for(i = 0; i < TX_BUFS; i++) {
        log += MMU::align128(sizeof(Tx_Desc));
        phy += MMU::align128(sizeof(Tx_Desc));
        _tx_ring[i].command = cb_s | cb_cid;
        _tx_ring[i].status = cb_complete; 
        _tx_ring[i].tbd_array = 0xFFFFFFFF; // simplified mode
        _tx_ring[i].tcb_byte_count = 0;
        _tx_ring[i].threshold = 0xE0;
        _tx_ring[i].tbd_count = 0;
        _tx_ring[i].link = phy; //next TxCB
    }
    _tx_ring[i-1].link = _tx_ring_phy;

    // reset
    reset();
}

int E100::exec_command(Reg8 cmd, Reg32 dma_addr)
{
    unsigned int i;

    // previous command is accepted when SCB clears
    for(i = 0; i < i82559_WAIT_SCB_TIMEOUT; i++) {
        if(!read8(&csr->scb.cmd_lo))
            break;
        udelay(5);
        if(i > i82559_WAIT_SCB_FAST)
            udelay(10);
    }
    if(cmd != cuc_resume && cmd != ruc_resume)
        write32(dma_addr, &csr->scb.gen_ptr);

    if(i == i82559_WAIT_SCB_TIMEOUT)
        return 1;

    write8(cmd, &csr->scb.cmd_lo);

    return 0;
}

void E100::reset()
{
    db<E100>(TRC) << "E100::reset (software reset and self-test)\n";

    // Reset the device
    software_reset();

    // Do a self-test on the device
    self_test();

    // Get MAC address from EEPROM
    _address = Address(eeprom_mac_address(0), eeprom_mac_address(1), eeprom_mac_address(2), eeprom_mac_address(3), eeprom_mac_address(4), eeprom_mac_address(5));
    db<E100>(INF) << "E100::reset():MAC=" << _address << "\n";

    // load zero on NIC's internal CU
    while(exec_command(cuc_load_base, 0));
    // load zero on NIC's internal RU
    while(exec_command(ruc_load_base, 0));

    // set new configuration
    i82559_configure();
    configCB->command &= ~cb_s;
    configCB->link = _macAddrCB_phy;

    // set MAC address
    macAddrCB->command = cb_iaaddr;
    macAddrCB->command &= ~cb_s;
    macAddrCB->command |= cb_el;
    macAddrCB->iaaddr = _address;

    i82559_disable_irq();

    while(exec_command(cuc_start, _configCB_phy));
    udelay(2 * 1000);

    while (!(macAddrCB->status & cb_complete));

    _statistics.tx_packets = 0;
    _statistics.rx_bytes = 0;
    _rx_ruc_no_more_resources = 0;
    _tx_cuc_suspended = 1;
    _tx_frames_sent = 0;

    // start send unit
    while(exec_command(cuc_start, _tx_ring_phy));
    udelay(10);

    // start receive unit
    while(exec_command(ruc_start, _rx_ring_phy));
    udelay(10);

    i82559_enable_irq();
    //i82559_disable_irq();
    udelay(10);
}

int E100::send(const Address & dst, const Protocol & prot,
	       const void * data, unsigned int size)
{
    // wait for a free TxCB
    while(!(_tx_ring[_tx_cur].status & cb_complete)) {
        if (_tx_cuc_suspended) {
            //_int_lock.acquire();
            _tx_cuc_suspended = 0;
            while(exec_command(cuc_resume, 0));
            //_int_lock.release();
        }
    }

    _tx_ring[_tx_cur].status = Tx_CB_IN_USE;
    _tx_ring[_tx_cur].tcb_byte_count = (size + HEADER_SIZE);

    // put the ethernet frame into de TxCB
    new (_tx_ring[_tx_cur].frame) Frame(_address, dst, CPU::htons(prot), data, size);

    _tx_ring[_tx_cur].command = cb_s; // suspend bit
    _tx_ring[_tx_cur].command |= (cb_tx | cb_cid); // transmit command

    _tx_ring[_tx_prev].command &= ~cb_s; // remove suspend bit
    ++_tx_prev %= TX_BUFS;

    // we have no guarantee that this command will be accepted by the adapter
    exec_command(cuc_resume, 0);

    ++_tx_cur %= TX_BUFS;

    _statistics.tx_packets++;
    _statistics.tx_bytes += size;

    return size;
}

bool E100::verifyPendingInterrupts(void)
{
    if (_rx_ruc_no_more_resources) {
        unsigned short i;

        //_int_lock.acquire();

        _rx_ruc_no_more_resources--;
        for (i = (_rx_cur + 1) % RX_BUFS; i != _rx_cur; ++i %= RX_BUFS) {
            if ((_rx_ring[i].status & cb_complete)) break;
        }

        if (i) --i;
        else i = RX_BUFS - 1;

        if (i != _rx_last_el) {
            _rx_ring[i].command = cb_el;
            _rx_ring[_rx_last_el].command &= ~cb_el;
            _rx_last_el = i;
        }

        while(exec_command(ruc_start, _rx_ring_phy + _rx_cur * sizeof(Rx_Desc)));

        //_int_lock.release();

        return true;
    }

    return false;
}

int E100::receive(Address * src, Protocol * prot,
		  void * data, unsigned int size)
{
    // wait until the RFD is filled up by the device
    while(!(_rx_ring[_rx_cur].status & cb_complete))
        verifyPendingInterrupts();

    // fill up receive areas/variables
    Frame * frame = (Frame *)_rx_ring[_rx_cur].frame;
    *src = frame->_src;
    *prot = CPU::ntohs(frame->_prot);

    if (_rx_ring[_rx_cur].actual_size & 0xC000)
        size = _rx_ring[_rx_cur].actual_size & 0x3FFF;
    else
        size = 0;

    memcpy(data, frame->_data, size);

    //kout << "recv (size: " << size << " prot: " << *prot <<")\n";

    _rx_ring[_rx_cur].command = cb_el;
    _rx_ring[_rx_cur].status = Rx_RFD_NOT_FILLED;

    // try to avoid ruc stop interrupts by "walking" the el bit
    _rx_ring[_rx_last_el].command &= ~cb_el; // remove previous el bit
    _rx_last_el = _rx_cur;

    ++_rx_cur %= RX_BUFS;

    _statistics.rx_packets++;
    _statistics.rx_bytes += size;

    return size;
}

unsigned short E100::eeprom_read(unsigned short *addr_len, unsigned short addr) {

    unsigned long cmd_addr_data;
    cmd_addr_data = (EE_READ_CMD(*addr_len) | addr) << 16;

    csr->eeprom_ctrl_lo = eecs | eesk;
    i82559_flush();
    udelay(200);

    unsigned short data = 0;
    unsigned char ctrl;
    for (int i = 31; i >= 0; i--) {
        ctrl = (cmd_addr_data & (1 << i)) ? eecs | eedi : eecs;
        csr->eeprom_ctrl_lo = ctrl;
        i82559_flush();
        udelay(200);

        csr->eeprom_ctrl_lo = ctrl | eesk;
        i82559_flush();
        udelay(200);

        ctrl = csr->eeprom_ctrl_lo;
        if (!(ctrl & eedo) && i > 16) {
            *addr_len -= (i - 16);
            i = 17;
        }

        data = (data << 1) | (ctrl & eedo ? 1 : 0);
    }

    csr->eeprom_ctrl_lo = 0;
    i82559_flush();
    udelay(200);

    return data;
}

unsigned char E100::eeprom_mac_address(Reg16 addr) {
    Reg16 eeprom_wc, addr_len = 8;
    Reg16 two_words; // two words of 8 bits (one EEPROM word)
    Reg8 which_word; // first or second word of 8 bits

    // try reading with an 8-bit addr len to discover actual addr len 
    eeprom_read(&addr_len, 0);
    eeprom_wc = 1 << addr_len;

    which_word = addr % 2; // read the first (0) or second (1) word of 8 bits

    addr = (unsigned short) (addr / 2);
    two_words = eeprom_read(&addr_len, addr);

    if (which_word != 0) // second word (bits 8 to 15)
        return (two_words >> 8);
    else // first word (bits 0 to 7)
        return (two_words & 0x00FF);
}

void E100::handle_int() {

    //_int_lock.acquire();
    CPU::int_disable();

    Reg8 stat_ack = read8(&csr->scb.stat_ack);
    Reg8 status = read8(&csr->scb.status);

    if ((stat_ack != stat_ack_not_ours) && (stat_ack != stat_ack_not_present)) {

        // acknowledge interrupt(s) in one PCI write cycle
        write8((stat_ack & ~stat_ack_sw_gen), &csr->scb.stat_ack);

        // not (!) is needed here! it can't be just (status & cus_idle)!
        if (!(status & cus_idle) && (stat_ack & stat_ack_cu_idle)) {
        }

        if (stat_ack & stat_ack_frame_rx) {
        }

        if ((stat_ack & stat_ack_rx) && (status & rus_idle)) {
        }

        if ((status & rus_no_resources) && (stat_ack & stat_ack_rnr)) {
            _rx_ruc_no_more_resources++;
        }

        if ((status & rus_idle) && (stat_ack & stat_ack_rnr)) {
        }

        if ((status & rus_suspended) && (stat_ack & stat_ack_rnr)) {
        }

        if ((status & cus_suspended)) {
            _tx_cuc_suspended++;
            if (_tx_frames_sent < _statistics.tx_packets) {
                _tx_frames_sent = _statistics.tx_packets;
                _tx_cuc_suspended--;
                while(exec_command(cuc_resume, 0));
            }
        }
    }

    CPU::int_enable();
}

void E100::i82559_configure(void)
{
    configCB->command = cb_config;
    memset(&(configCB->config), 0, sizeof(struct config));
    configCB->config.byte_count = 0x16;              /* bytes in this struct */
    configCB->config.rx_fifo_limit = 0x8;            /* bytes in FIFO before DMA */
    configCB->config.direct_rx_dma = 0x1;            /* reserved */
    configCB->config.standard_tcb = 0x1;             /* 1=standard, 0=extended */
    configCB->config.standard_stat_counter = 0x1;    /* 1=standard, 0=extended */
    // zero => recommended in promiscuous mode - FIXME - padding is enabled
    configCB->config.rx_discard_short_frames = 0x0;  /* 1=discard, 0=pass */
    configCB->config.tx_underrun_retry = 0x3;        /* 3 underrun retries */
    configCB->config.tx_dynamic_tbd = 0x0;           /* 1=yes, 0=no FIXME */
    configCB->config.mii_mode = 0x1;                 /* 1=MII mode, 0=503 mode */
    configCB->config.rx_tcpudp_checksum = 0x0;       /* 1=yes 0=no */
    configCB->config.link_status_wake = 0x1;         /* 1=yes 0=no */
    configCB->config.pad10 = 0x6;
    // if you comment the next line it won't work anymore
    configCB->config.no_source_addr_insertion = 0x1; /* 1=no, 0=yes */
    configCB->config.preamble_length = 0x2;          /* 0=1, 1=3, 2=7, 3=15 bytes */
    configCB->config.ifs = 0x6;                      /* x16 = inter frame spacing */
    configCB->config.ip_addr_hi = 0xF2;              /* ARP IP filter - not used */
    configCB->config.pad15_1 = 0x1;
    configCB->config.pad15_2 = 0x1;
    configCB->config.crs_or_cdt = 0x0;               /* 0=CRS only, 1=CRS or CDT */
    //configCB->config.fc_delay_hi = 0x40;           /* time delay for fc frame */
    configCB->config.rx_stripping = 0x1;             /* 1=strip long frames */
    configCB->config.tx_padding = 0x1;               /* 1=pad short frames */
    configCB->config.fc_priority_threshold = 0x7;    /* 7=priority fc disabled */
    configCB->config.pad18 = 0x1;
    configCB->config.pad20_1 = 0x1F;
    configCB->config.fc_priority_location = 0x1;     /* 1=byte#31, 0=byte#19 */
    configCB->config.multi_ia = 0x1;
    configCB->config.pad21_1 = 0x5;
    configCB->config.full_duplex_pin = 0x1;          /* 1=examine FDX# pin */
    configCB->config.full_duplex_force = 0x0;        /* 1=force, 0=auto */
    // specially for promiscuous mode
    configCB->config.rx_save_bad_frames = 0x1;       /* 1=save, 0=discard */
    configCB->config.pad12_0 = 0x1;                  /* 1=yes, 0=no */
    configCB->config.promiscuous_mode = 0x1;         /* 1=on, 0=off */
    configCB->config.wait_after_win = 0x1;           /* 1=on, 0=off */
    configCB->config.multicast_all = 0x0;            /* 1=accept, 0=no */
    configCB->config.magic_packet_disable = 0x0;     /* 1=off, 0=on */
    configCB->config.fc_disable = 0x0;               /* 1=Tx fc off, 0=Tx fc on */
    configCB->config.mwi_enable = 0x1;               /* 1=enable, 0=disable */
    configCB->config.rx_long_ok = 0x0;               /* 1=VLANs ok, 0=standard */
    configCB->config.tco_statistics = 0x1;           /* TCO stats enable */
}

int E100::self_test()
{
    Reg32 dma_addr = _dmadump_phy + offsetof(struct mem, selftest);

    dmadump->selftest.signature = 0;
    dmadump->selftest.result = 0xFFFFFFFF;

    write32(SELFTEST | dma_addr, &csr->port);
    i82559_flush();
    udelay(20 * 1000); // wait for 10 miliseconds

    i82559_disable_irq();

    // Check results of self-test 
    if(dmadump->selftest.result != 0) {
	db<E100>(WRN)  << "Self-test failed: result = " << dmadump->selftest.result << "\n";
	return -1;
    }

    if(dmadump->selftest.signature == 0) {
	db<E100>(WRN)  << "Self-test failed: timed out\n";
	return -1;
    }

    db<E100>(INF) << "YES, I'M HERE!\n";
    return 0;
}

__END_SYS
