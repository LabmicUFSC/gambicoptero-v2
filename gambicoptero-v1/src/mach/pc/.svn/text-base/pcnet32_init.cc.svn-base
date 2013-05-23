// EPOS PC AMD PCNet II (Am79C970A) Ethernet NIC Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/kmalloc.h>
#include <mach/pc/machine.h>
#include <mach/pc/pcnet32.h>

__BEGIN_SYS

void PCNet32::init(unsigned int unit)
{
    db<Init, PCNet32>(TRC) << "PCNet32::init(unit=" << unit << ")\n";

    // Scan the PCI bus for device
    PC_PCI::Locator loc = PC_PCI::scan(PCI_VENDOR_ID, PCI_DEVICE_ID, unit);
    if(!loc) {
	db<Init, PCNet32>(WRN) << "PCNet32::init: PCI scan failed!\n";
	return;
    }

    // Try to enable IO regions and bus master
    PC_PCI::command(loc, PC_PCI::command(loc) 
		    | PC_PCI::COMMAND_IO | PC_PCI::COMMAND_MASTER);

    // Get the config space header and check it we got IO and MASTER
    PC_PCI::Header hdr;
    PCI::header(loc, &hdr);
    if(!hdr) {
	db<Init, PCNet32>(WRN) << "PCNet32::init: PCI header failed!\n";
	return;
    }
    db<Init, PCNet32>(INF) << "PCNet32::init: PCI header=" << hdr << "}\n";
    if(!(hdr.command & PC_PCI::COMMAND_IO))
	db<Init, PCNet32>(WRN) << "PCNet32::init: I/O unaccessible!\n";
    if(!(hdr.command & PC_PCI::COMMAND_MASTER))
	db<Init, PCNet32>(WRN) << "PCNet32::init: not master capable!\n";

    // Get I/O base port
    IO_Port io_port = hdr.region[PCI_REG_IO].phy_addr;
    db<Init, PCNet32>(INF) << "PCNet32::init: I/O port at " 
			   << (void *)(int)io_port << "\n";

    // Get I/O irq
    IO_Irq irq = hdr.interrupt_line;
    db<Init, PCNet32>(INF) << "PCNet32::init: PCI interrut pin "
			   << hdr.interrupt_pin << " routed to IRQ "
			   << hdr.interrupt_line << "\n";

    // Allocate a DMA Buffer for init block, rx and tx rings
    DMA_Buffer * dma_buf = new(kmalloc(sizeof(MMU::DMA_Buffer))) 
	DMA_Buffer(DMA_BUFFER_SIZE);

    // Initialize the device
    PCNet32 * dev = new (kmalloc(sizeof(PCNet32)))
	PCNet32(unit, io_port, irq, dma_buf);

    // Register the device
    _devices[unit].in_use = false;
    _devices[unit].device = dev;
    _devices[unit].interrupt = IC::irq2int(irq);
    
    // Install interrupt handler
    IC::int_vector(_devices[unit].interrupt, &int_handler);

    // Enable interrupts for device
    IC::enable(_devices[unit].interrupt);
}

__END_SYS
