// EPOS PC AMD PCNet II (Am79C970A) Ethernet NIC Mediator Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/pc/machine.h>
#include <mach/pc/pcnet32.h>
#include <utility/malloc.h>

__BEGIN_SYS

// Class attributes
PCNet32::Device PCNet32::_devices[UNITS];

// Class Methods
void PCNet32::int_handler(unsigned int interrupt)
{
    PCNet32 * dev = get(interrupt);

    db<PCNet32>(TRC) << "PCNet32::int_handler(int=" << interrupt
		     << ",dev=" << dev << ")\n";
    if(!dev)
	db<PCNet32>(WRN) << "PCNet32::int_handler: handler not found!\n";
    else 
	dev->handle_int();
}

// Methods
PCNet32::PCNet32(unsigned int unit)
{
    db<PCNet32>(TRC) << "PCNet32(unit=" << unit << ")\n";

    // Share control
    if(unit >= UNITS) {
	db<PCNet32>(WRN) << "PCNet32: requested unit (" << unit 
			 << ") does not exist!\n";
	return;
    }

    // Share control
    if(_devices[unit].in_use) {
	db<PCNet32>(WRN) << "PCNet32: device already in use!\n";
	return;
    }
    
    *this = *_devices[unit].device;

    // Lock device
    _devices[unit].in_use = true;
}

PCNet32::~PCNet32()
{
    db<PCNet32>(TRC) << "~PCNet32(unit=" << _unit << ")\n";

    // Unlock device
    _devices[_unit].in_use = false;
}

PCNet32::PCNet32(unsigned int unit, 
		 IO_Port io_port, IO_Irq irq, DMA_Buffer * dma_buf)
{
    db<PCNet32>(TRC) << "PCNet32(unit=" << unit << ",io=" << io_port 
		     << ",irq=" << irq << ",dma=" << dma_buf << ")\n";

    _unit = unit;
    _io_port = io_port;
    _irq = irq;
    _dma_buf = dma_buf;

    // Distribute the DMA_Buffer allocated by init()
    Log_Addr log = _dma_buf->log_address();
    Phy_Addr phy = _dma_buf->phy_address();

    // Initialization Block
    _iblock = log;
    _iblock_phy = phy;
    log += MMU::align128(sizeof(Init_Block));
    phy += MMU::align128(sizeof(Init_Block));

    // Rx_Desc Ring
    _rx_cur = 0;
    _rx_ring = log;
    _rx_ring_phy = phy;
    log += RX_BUFS * MMU::align128(sizeof(Rx_Desc));
    phy += RX_BUFS * MMU::align128(sizeof(Rx_Desc));

    // Tx_Desc Ring
    _tx_cur = 0;
    _tx_ring = log;
    _tx_ring_phy = phy;
    log += TX_BUFS * MMU::align128(sizeof(Tx_Desc));
    phy += TX_BUFS * MMU::align128(sizeof(Tx_Desc));

    // Rx Buffers
    for(unsigned int i = 0; i < RX_BUFS; i++) {
	_rx_buffer[i] = log;
	_rx_ring[i].phy_addr = phy;
	_rx_ring[i].size = -sizeof(Frame); // 2's comp.
	_rx_ring[i].misc = 0;
	_rx_ring[i].status = Desc::OWN; // Owned by NIC
	log += MMU::align128(sizeof(Frame));
	phy += MMU::align128(sizeof(Frame));
    }

    // Tx Buffers
    for(unsigned int i = 0; i < TX_BUFS; i++) {
	_tx_buffer[i] = log;
	_tx_ring[i].phy_addr = phy;
	_tx_ring[i].misc = 0;
	_tx_ring[i].status = 0; // Owned by host
	log += MMU::align128(sizeof(Frame));
	phy += MMU::align128(sizeof(Frame));
    }

    // Reset device
    reset();
}

void PCNet32::reset()
{
    // Reset the device
    s_reset();

    // Software style => PCI, 32 bits, burst mode (pg 147)
    bcr(20, BCR20_SWSTYLE2);

    // Get MAC address from PROM
    _address = Address(prom(0), prom(1), prom(2), prom(3), prom(4), prom(5));
    db<PCNet32>(INF) << "PCNet32::reset: MAC=" << _address << "\n";

    // Enable autoselect port
    bcr(2, BCR2_ASEL);

    // Enable full-duplex
    bcr(9, BCR9_FDEN);

    // Disable INIT interrupt and transmit stop on underflow
    csr(3, CSR3_TINTM | CSR3_IDONM | CSR3_DXSUFLO);

    // Enable auto padding of frames
    csr(4, CSR4_DMAPLUS | CSR4_APAD_XMT);

    // Adjust interrupts
    csr(5, CSR5_TOKINTD | /* CSR5_LTINTEN | */ CSR5_SINTE 
 	| CSR5_SLPINTE | CSR5_EXDINTE | CSR5_MPINTE);

    // Enable burst read and write
    bcr(18, bcr(18) | BCR18_BREADE | BCR18_BWRITE);

    // Promiscuous mode
//     csr(15, csr(15) | CSR15_PROM);
	
    // Set transmit start point to full frame	
    csr(80, csr(80) | 0x0c00); // XMTSP = 11

    // Setup a init block
    _iblock->mode = 0x0000;
    _iblock->rlen = log2(RX_BUFS) << 4;
    _iblock->tlen = log2(TX_BUFS) << 4;
    _iblock->mac_addr = _address;        
    _iblock->filter1 = 0;             
    _iblock->filter2 = 0;             
    _iblock->rx_ring = _rx_ring_phy;
    _iblock->tx_ring = _tx_ring_phy;
    csr(1, _iblock_phy & 0xffff); 
    csr(2, _iblock_phy >> 16); 
 
    // Initialize the device
    csr(0, CSR0_IENA | CSR0_INIT);
    for(int i = 0; (i < 100) && !(csr(0) & CSR0_IDON); i++);
    if(!(csr(0) & CSR0_IDON))
	db<PCNet32>(WRN) << "PCNet32::reset: initialization failed!\n";

    // Get MAC address from CSR
    csr(0, CSR0_IDON | CSR0_STOP);
    Address csr_addr(csr(PADR0), csr(PADR1), csr(PADR2));

    if(_address != csr_addr) {
        db<PCNet32>(WRN) << "PCNet32::reset: initialization failed!\n";
        db<PCNet32>(WRN) << "PCNet32::reset: MAC(ROM)=" << _address << "\n";
        db<PCNet32>(WRN) << "PCNet32::reset: MAC(CSR)=" << csr_addr << "\n";
    }

    // Activate sending and receiving 
    csr(0, CSR0_IENA | CSR0_STRT);
}

int PCNet32::send(const Address & dst, const Protocol & prot,
		  const void * data, unsigned int size)
{
    db<PCNet32>(TRC) << "PCNet32::send(s=" << _address
		     << ",d=" << dst
		     << ",p=" << hex << prot << dec
		     << ",dt=" << data
		     << ",sz=" << size
		     << ")\n";

    // Wait for a free buffer
    while(_tx_ring[_tx_cur].status & Rx_Desc::OWN);

    // Assemble the Ethernet frame
    new (_tx_buffer[_tx_cur])
	Frame(_address, dst, CPU::htons(prot), data, size);

    _tx_ring[_tx_cur].size = -(size + HEADER_SIZE); // 2's comp.

    // Status must be set last, since it can trigger a send
    _tx_ring[_tx_cur].status = Tx_Desc::OWN | Tx_Desc::STP | Tx_Desc::ENP;

    // Trigger an immediate send poll
    csr(0, csr(0) | CSR0_TDMD);

    _statistics.tx_packets++;
    _statistics.tx_bytes += size;

    ++_tx_cur %= TX_BUFS;

    return size;
}

int PCNet32::receive(Address * src, Protocol * prot,
		     void * data, unsigned int size)
{
    // Wait for a frame in the ring buffer
    while(_rx_ring[_rx_cur].status & Rx_Desc::OWN);

    // Disassemble the Ethernet frame
    Frame * frame = _rx_buffer[_rx_cur];
    *src = frame->_src;
    *prot = CPU::ntohs(frame->_prot);
    unsigned int s = (_rx_ring[_rx_cur].misc & 0x00000fff) - HEADER_SIZE;

    // Copy data
    memcpy(data, frame->_data, (s > size) ? size : s);
 
    // Release the buffer to the NIC
    _rx_ring[_rx_cur].status = Rx_Desc::OWN;

    _statistics.rx_packets++;
    _statistics.rx_bytes += s;

    ++_rx_cur %= RX_BUFS;

    db<PCNet32>(TRC) << "PCNet32::receive(s=" << *src
		     << ",p=" << hex << *prot << dec
		     << ",dt=" << data
		     << ",sz=" << s
		     << ")\n";

    return s;
}

void PCNet32::handle_int()
{
    CPU::int_disable();

    while(csr(0) & CSR0_INTR) {
	int csr0 = csr(0);
	int csr4 = csr(4);
	int csr5 = csr(5);

	// Clear interrupts
	csr(0, csr0);
	csr(4, csr4);
	csr(5, csr5);

	// Initialization done?
	if(csr0 & CSR0_IDON) {
	    // This should never happen, since IDON is disabled in reset()
	    // and all the initialization is conctrolled via polling, so if
	    // we are here, it must be due to a hardware induced reset.
	    // All we can do is to try to reset the nic!
	    db<PCNet32>(WRN) << "PCNet32::handle_int: initialization done!\n";
	    reset();
	}

	// Transmition?
	if(csr0 & CSR0_TINT) {
	    db<PCNet32>(INF) << "PCNet32::handle_int: transmition\n";
	}

 	// Receive?
 	if(csr0 & CSR0_RINT) {
 	    db<PCNet32>(INF) << "PCNet32::handle_int: receive\n";
 	    notify(CPU::ntohs(_rx_buffer[_rx_cur]->_prot));
 	}

        // Error?
	if(csr0 & CSR0_ERR) {
	    db<PCNet32>(INF) << "PCNet32::handle_int: ";

	    // Memory Error?
	    if(csr0 & CSR0_MERR) {
		db<PCNet32>(WRN) << " memory error\n";
	    }
	    
	    // Missed Frame
	    if(csr0 & CSR0_MISS) {
		db<PCNet32>(WRN) << " missed frame\n";
		_statistics.rx_overruns++;
	    }

	    // Collision?
	    if(csr0 & CSR0_CERR) {
		db<PCNet32>(INF) << " collision error\n";
		_statistics.collisions++;
	    }

	    // Bable transmitter time-out?
	    if(csr0 & CSR0_BABL) {
		db<PCNet32>(INF) << " transmitter time-out\n";
		_statistics.tx_overruns++;
	    }
	}

	// Missed frame counter overflow?
	if(csr4 & CSR4_MFCO) {
	    db<PCNet32>(INF) << "PCNet32::handle_int: missed frame counter overflow\n";
	}

	// User interrupt?
	if(csr4 & CSR4_UINT) {
	    db<PCNet32>(INF) << "PCNet32::handle_int: user interrupt\n";
	}

	// Receive collision counter overflow?
	if(csr4 & CSR4_RCVCCO) {
	    db<PCNet32>(INF) << "Receive Collision Counter Overflow\n";
	}

	// Transmit start?
	if(csr4 & CSR4_TXSTRT) {
	    db<PCNet32>(INF) << "PCNet32::handle_int: transmit start\n";
	}

	// Jabber error?
	if(csr4 & CSR4_JAB) {
	    db<PCNet32>(INF) << "PCNet32::handle_int: jabber error\n";
	}

	// System interrupt?
	if(csr5 & CSR5_SINT) {
	    db<PCNet32>(INF) << "PCNet32::handle_int: system interrupt\n";
	}

	// Sleep interrupt?
	if(csr5 & CSR5_SLPINT) {
	    db<PCNet32>(INF) << "PCNet32::handle_int: sleep\n";
	}

	// Excessive deferral?
	if(csr5 & CSR5_EXDINT){
	    db<PCNet32>(INF) << "PCNet32::handle_int: excessive deferral\n";
	}

	// Magic Packet interrupt?
	if(csr5 & CSR5_MPINT) {
	    db<PCNet32>(INF) << "PCNet32::handle_int: magic packet\n";
	}
    }

    CPU::int_enable();
}

__END_SYS
