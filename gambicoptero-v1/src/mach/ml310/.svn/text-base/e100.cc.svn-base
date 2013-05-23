// EPOS ML310 Intel PRO/100 (i82559) Ethernet NIC Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/kmalloc.h>
#include <mach/ml310/machine.h>
#include <mach/ml310/e100.h>

__BEGIN_SYS

// Class attributes
E100::Device E100::_devices[UNITS];

// Class Methods
void E100::int_handler(unsigned int interrupt)
{
    E100 * dev = get(interrupt);

    db<ML310_NIC>(TRC) << "E100::int_handler(int=" << interrupt
                    << ",dev=" << dev << ")\n";
    if(!dev)
        db<ML310>(WRN) << "E100::int_handler: handler not found\n";
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
    _csr = io_mem;
    _irq = irq;
    _dma_buf = dma_buf;

    // Distribute the DMA_Buffer allocated by init()

    // Distribute the DMA_Buffer allocated by init()
    Log_Addr log = dma_buf->log_address();
    Phy_Addr phy = dma_buf->phy_address();

    // Initialization Block
    _configCB_phy = phy;
    _configCB = log;
    log += MMU::align128(sizeof(ConfigureCB));
    phy += MMU::align128(sizeof(ConfigureCB));

    _macAddrCB_phy = phy;
    _macAddrCB = log;
    log += MMU::align128(sizeof(_macAddrCB));
    phy += MMU::align128(sizeof(_macAddrCB));

    _dmadump_phy = phy;
    _dmadump = log;
    log += MMU::align128(sizeof(struct mem));
    phy += MMU::align128(sizeof(struct mem));

    // Rx_Desc Ring
    _rx_cur = 0;
    _rx_last_el = RX_BUFS - 1;
    _rx_ring = log;
    _rx_ring_phy = phy;

    db<E100> (TRC) << "E100(_rx_ring malloc of " << RX_BUFS
                   << " units)\n";

    // Rx (RFDs)
    unsigned int i;
    for(i = 0; i < RX_BUFS; i++) {
        log += MMU::align128(sizeof(Rx_Desc));
        phy += MMU::align128(sizeof(Rx_Desc));
        _rx_ring[i].command = 0;
        _rx_ring[i].size = CPU::cpu_to_le16(sizeof(Frame));
        _rx_ring[i].status = CPU::cpu_to_le16(Rx_RFD_AVAILABLE);
        _rx_ring[i].actual_size = 0;
        _rx_ring[i].link = CPU::cpu_to_le32(phy); //next RFD
    }
    _rx_ring[i-1].command = cb_el;
    _rx_ring[i-1].link = CPU::cpu_to_le32(_rx_ring_phy);

    // Tx_Desc Ring
    _tx_cur = 1;
    _tx_prev = 0;
    _tx_ring = log;
    _tx_ring_phy = phy;

    db<E100> (TRC) << "E100(_tx_ring malloc of " << TX_BUFS
                   << " units)\n";

    // TxCBs
    for(i = 0; i < TX_BUFS; i++) {
        log += MMU::align128(sizeof(Tx_Desc));
        phy += MMU::align128(sizeof(Tx_Desc));
        _tx_ring[i].command = CPU::cpu_to_le16(cb_s | cb_cid);
        _tx_ring[i].status = CPU::cpu_to_le16(cb_complete); 
        _tx_ring[i].tbd_array = 0xFFFFFFFF; // simplified mode
        _tx_ring[i].tcb_byte_count = 0;
        _tx_ring[i].threshold = 0xE0;
        _tx_ring[i].tbd_count = 0;
        _tx_ring[i].link = CPU::cpu_to_le32(phy); //next TxCB
    }
    _tx_ring[i-1].link = CPU::cpu_to_le32(_tx_ring_phy);

    // reset device
    reset();
}

void E100::reset()
{
    db<E100>(TRC) << "E100::reset (software reset and self-test)\n";

    // reset the device
    software_reset();

    // do a self-test on the device
    self_test();

    // get MAC address from EEPROM
    _address = Address(eeprom_mac_address(0), eeprom_mac_address(1), eeprom_mac_address(2), eeprom_mac_address(3), eeprom_mac_address(4), eeprom_mac_address(5));
    db<E100>(INF) << "  MAC Address =" << _address << "\n";

    // load zero on NIC's internal CU
    while(exec_command(cuc_load_base, 0));

    // load zero on NIC's internal RU
    while(exec_command(ruc_load_base, 0));

    // set new configuration
    i82559_configure();

    _configCB->command &= CPU::cpu_to_le16(~cb_s);
    _configCB->link = CPU::cpu_to_le32(_macAddrCB_phy);

    // set MAC address
    _macAddrCB->command = CPU::cpu_to_le16(cb_iaaddr);
    _macAddrCB->command &= CPU::cpu_to_le16(~cb_s);
    _macAddrCB->command |= CPU::cpu_to_le16(cb_el);
    memcpy(&_macAddrCB->iaaddr, _address.b, 6);

    i82559_disable_irq();

    // start execution of MAC address and configuration blocks
    while(exec_command(cuc_start, _configCB_phy));
    // wait until end of execution of MAC address and configuration blocks
    while (!(CPU::le16_to_cpu(_macAddrCB->status) & cb_complete));

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

    udelay(10);

}

int E100::send(const Address & dst, const Protocol & prot,
                  const void * data, unsigned int size)
{
    db<E100>(TRC) << "E100::send(src=" << _address
                     << ",dst=" << dst
                     << ",prot=" << prot
                     << ",data=" << data
                     << ",size=" << size
                     << ")\n";

    // wait for a free TxCB
    while(!(CPU::le16_to_cpu(_tx_ring[_tx_cur].status) & cb_complete)) {
        if (_tx_cuc_suspended) {
            _tx_cuc_suspended--;
            while(exec_command(cuc_resume, 0));
        }
    }

    _tx_ring[_tx_cur].status = CPU::cpu_to_le16(Tx_CB_IN_USE);
    _tx_ring[_tx_cur].tcb_byte_count = CPU::cpu_to_le16(size + HEADER_SIZE);

    // put the ethernet frame into de TxCB
    new (_tx_ring[_tx_cur].frame) Frame(_address, dst, CPU::htons(prot), data, size);

    _tx_ring[_tx_cur].command = CPU::cpu_to_le16(cb_s);
    _tx_ring[_tx_cur].command |= CPU::cpu_to_le16(cb_tx | cb_cid); // transmit command

    _tx_ring[_tx_prev].command &= CPU::cpu_to_le16(~cb_s); // remove suspend bit
    ++_tx_prev %= TX_BUFS;

    ++_tx_cur %= TX_BUFS;

    if (_tx_cuc_suspended) {
        _tx_cuc_suspended--;
        while(exec_command(cuc_resume, 0));
    }

    _statistics.tx_packets++;
    _statistics.tx_bytes += size;

    return size;
}

int E100::receive(Address * src, Protocol * prot,
                     void * data, unsigned int size)
{
    // wait until the RFD is filled up by the device
    while(!(CPU::le16_to_cpu(_rx_ring[_rx_cur].status) & cb_complete))
        verifyPendingInterrupts();

    // fill up receive areas/variables
    Frame * frame = (Frame *)_rx_ring[_rx_cur].frame;
    *src = frame->_src;
    *prot = CPU::ntohs(frame->_prot);

    if (CPU::le16_to_cpu(_rx_ring[_rx_cur].actual_size) & 0xC000)
        size = CPU::le16_to_cpu(_rx_ring[_rx_cur].actual_size) & 0x3FFF;
    else
        size = 0;

    memcpy(data, frame->_data, size);

    db<E100>(TRC) << "E100::receive(src=" << *src
                    << ",prot=" << *prot
                    << ",data=" << data
                    << ",size=" << size
                    << ")\n";

    _rx_ring[_rx_cur].command = CPU::cpu_to_le16(cb_el);
    _rx_ring[_rx_cur].status = CPU::cpu_to_le16(Rx_RFD_NOT_FILLED);

    // try to avoid ruc stop interrupts by "walking" the el bit
    _rx_ring[_rx_last_el].command &= CPU::cpu_to_le16(~cb_el);
    _rx_last_el = _rx_cur;

    ++_rx_cur %= RX_BUFS;

    _statistics.rx_packets++;
    _statistics.rx_bytes += size;

    return size;
}

bool E100::verifyPendingInterrupts(void)
{
    if (_rx_ruc_no_more_resources) {
        unsigned short i;

        CPU::int_disable();

        _rx_ruc_no_more_resources--;
        for (i = (_rx_cur + 1) % RX_BUFS; i != _rx_cur; ++i %= RX_BUFS) {
            if (CPU::le16_to_cpu(_rx_ring[i].status) & cb_complete) break;
        }

        if (i) --i;
        else i = RX_BUFS - 1;

        if (i != _rx_last_el) {
            _rx_ring[i].command = CPU::cpu_to_le16(cb_el);
            _rx_ring[_rx_last_el].command &= CPU::cpu_to_le16(~cb_el);
            _rx_last_el = i;
        }

        while(exec_command(ruc_start, _rx_ring_phy + _rx_cur * sizeof(Rx_Desc)));

        CPU::int_enable();

        return true;
    }

    return false;
}

unsigned char E100::eeprom_mac_address(Reg16 addr)
{
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

unsigned short E100::eeprom_read(unsigned short *addr_len, unsigned short addr)
{
    unsigned long cmd_addr_data;
    cmd_addr_data = (EE_READ_CMD(*addr_len) | addr) << 16;
    
    _csr->eeprom_ctrl_lo = eecs | eesk;
    i82559_flush();
    udelay(200);
    
    unsigned short data = 0;
    unsigned char ctrl; 
    for (int i = 31; i >= 0; i--) {
        ctrl = (cmd_addr_data & (1 << i)) ? eecs | eedi : eecs;
        _csr->eeprom_ctrl_lo = ctrl;
        i82559_flush();
        udelay(200);

        _csr->eeprom_ctrl_lo = ctrl | eesk;
        i82559_flush();
        udelay(200);

        ctrl = _csr->eeprom_ctrl_lo;
        if (!(ctrl & eedo) && i > 16) {
            *addr_len -= (i - 16);
            i = 17;
        }

        data = (data << 1) | (ctrl & eedo ? 1 : 0);
    }

    // chip deselect 
    _csr->eeprom_ctrl_lo = 0;
    i82559_flush();
    udelay(200);

    return data;
}

int E100::exec_command(Reg8 cmd, Reg32 dma_addr)
{
    unsigned int i;

    CPU::int_disable();

    // previous command is accepted when SCB clears
    for(i = 0; i < i82559_WAIT_SCB_TIMEOUT; i++) {
        if(!read8(&_csr->scb.cmd_lo))
            break;
        udelay(5);
        if(i > i82559_WAIT_SCB_FAST)
            udelay(10);
    }
    if(cmd != cuc_resume && cmd != ruc_resume)
        write32(dma_addr, &_csr->scb.gen_ptr);

    if(i == i82559_WAIT_SCB_TIMEOUT)
        return 1;

    write8(cmd, &_csr->scb.cmd_lo);

    CPU::int_enable();

    return 0;
}

void E100::handle_int()
{
    CPU::int_disable();

    Reg8 stat_ack = read8(&_csr->scb.stat_ack);
    Reg8 status = read8(&_csr->scb.status);

    if ((stat_ack != stat_ack_not_ours) && (stat_ack != stat_ack_not_present)) {

        // acknowledge interrupt(s) in one PCI write cycle
        write8((stat_ack & ~stat_ack_sw_gen), &_csr->scb.stat_ack);

        // not (!) is needed here! it can't be just (status & cus_idle)!
        if (!(status & cus_idle) && (stat_ack & stat_ack_cu_idle)) {
            kout << "Tx stopped !!!!\n";
        }

        if (stat_ack & stat_ack_frame_rx) {
            kout << "Frame received\n";
        }

        if (((stat_ack & stat_ack_rx) || (stat_ack & stat_ack_rnr)) && (status & rus_idle)) {
            kout << "Rx Stopped\n";
        }

        if ((status & rus_no_resources) && (stat_ack & stat_ack_rnr)) {
            kout << "Missed Frame!!!!\n";
            _rx_ruc_no_more_resources++;
        }

        if ((status & rus_suspended) && (stat_ack & stat_ack_rnr)) {
            kout << "Rx suspended !!!!\n";
        }

        if ((status & cus_suspended)) {
            kout << "Tx suspended !!!!\n";
            if (_tx_frames_sent < _statistics.tx_packets) {
                _tx_cuc_suspended++;
            }
        }
    }
 
    CPU::int_enable();
}

__END_SYS
