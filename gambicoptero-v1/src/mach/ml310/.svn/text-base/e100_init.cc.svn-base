// EPOS ML310 Intel PRO/100 (i82559) Ethernet NIC Mediator Initialization

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/kmalloc.h>
#include <mach/ml310/machine.h>
#include <mach/ml310/e100.h>

__BEGIN_SYS

void E100::init(unsigned int unit)
{
    db<Init, E100>(TRC) << "E100::init(unit=" << unit << ")\n";

    // Scan the PCI bus for device
    ML310_PCI::Locator loc = ML310_PCI::scan(PCI_VENDOR_ID, PCI_DEVICE_ID, unit);
    if(!loc) {
	db<Init, E100>(WRN) << "E100::init: PCI scan failed!\n";
	return;
    }

    // Try to enable IO regions and bus master
    ML310_PCI::command(loc, ML310_PCI::command(loc) 
		    | ML310_PCI::COMMAND_MEMORY | ML310_PCI::COMMAND_MASTER);

    // Get the config space header and check it we got IO and MASTER
    ML310_PCI::Header hdr;
    PCI::header(loc, &hdr);
    if(!hdr) {
	db<Init, E100>(WRN) << "E100::init: PCI header failed!\n";
	return;
    }
    db<Init, E100>(INF) << "E100::init: PCI header=" << hdr << "}\n";
    if(!(hdr.command & ML310_PCI::COMMAND_MEMORY))
	db<Init, E100>(WRN) << "E100::init: I/O memory unaccessible!\n";
    if(!(hdr.command & ML310_PCI::COMMAND_MASTER))
	db<Init, E100>(WRN) << "E100::init: not master capable!\n";

    // Get I/O base port
    Log_Addr io_mem = hdr.region[PCI_REG_MEM].log_addr;
    db<Init, E100>(INF) << "E100::init: I/O memory at " 
			<< hdr.region[PCI_REG_MEM].phy_addr
			<< " mapped to " 
			<< hdr.region[PCI_REG_MEM].log_addr << "\n";

    // Get I/O irq
    IO_Irq irq = hdr.interrupt_line;
    db<Init, E100>(INF) << "E100::init: PCI interrut pin "
			<< hdr.interrupt_pin << " routed to IRQ "
			<< hdr.interrupt_line << "\n";

    // Allocate a DMA Buffer for init block, rx and tx rings
    DMA_Buffer * dma_buf = new(kmalloc(sizeof(MMU::DMA_Buffer))) 
	DMA_Buffer(DMA_BUFFER_SIZE);

    // Initialize the device
    E100 * dev = new (kmalloc(sizeof(E100)))
	E100(unit, io_mem, irq, dma_buf);

    // Register the device
    _devices[unit].in_use = false;
    _devices[unit].device = dev;
    _devices[unit].interrupt = Machine::irq2int(irq);

    // Install interrupt handler
    ML310::int_vector(ML310::irq2int(irq), &int_handler);

    // Enable interrupts for device
    IC::enable(irq);
}

__END_SYS
