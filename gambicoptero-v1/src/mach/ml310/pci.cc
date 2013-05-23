// EPOS ML310 PCI Mediator

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <mach/ml310/pci.h>

__BEGIN_SYS

// Class attributes
ML310_PCI::Phy_Addr ML310_PCI::_phy_io_mem;

unsigned int ML310_PCI::_memspace_upper = Traits<ML310_PCI>::MEM_SPACE_UPPER + 1;
unsigned int ML310_PCI::_iospace_upper = Traits<ML310_PCI>::IO_SPACE_UPPER + 1;

ML310_PCI::Reg32 ML310_PCI::_base_address[Region::N] = {
    BASE_ADDRESS_0, BASE_ADDRESS_1,
    BASE_ADDRESS_2, BASE_ADDRESS_3,
    BASE_ADDRESS_4, BASE_ADDRESS_5
};

// Methods
void ML310_PCI::header(const ML310_PCI::Locator & l, ML310_PCI::Header * h)
{
    PPC32 cpu;

    h->vendor_id = cfg16(l.bus, l.dev_fn, VENDOR_ID);
    if((h->vendor_id != 0) && (h->vendor_id != 0xffff)) {
        h->locator = l;
        h->device_id = cfg16(l.bus, l.dev_fn, DEVICE_ID);
        h->command = cfg16(l.bus, l.dev_fn, COMMAND);
        h->status = cfg16(l.bus, l.dev_fn, STATUS);
        h->revision_id = cfg8(l.bus, l.dev_fn, REVISION_ID);
        h->class_prog = cfg8(l.bus, l.dev_fn, CLASS_PROG);
        h->class_id = cfg16(l.bus, l.dev_fn, CLASS_ID);
        h->cache_line_size = cfg8(l.bus, l.dev_fn, CACHE_LINE_SIZE);
        h->latency_time = cfg8(l.bus, l.dev_fn, LATENCY_TIMER);
        h->type = cfg8(l.bus, l.dev_fn, HEADER_TYPE);
        h->bist = cfg8(l.bus, l.dev_fn, BIST);
        for(unsigned int i = 0; i < Region::N; i++) {
            cpu.int_disable();
            h->region[i].phy_addr = cfg32(l.bus, l.dev_fn, _base_address[i]);
            cfg32(l.bus, l.dev_fn, _base_address[i], ~0); 
            h->region[i].size = cfg32(l.bus, l.dev_fn, _base_address[i]);
            cfg32(l.bus, l.dev_fn, _base_address[i], h->region[i].phy_addr); 
            cpu.int_enable();
            if( (h->region[i].phy_addr & BASE_ADDRESS_SPACE_MASK) || 
                ( (h->type&0x7f) == HEADER_TYPE_BRIDGE || (h->type&0x7f) == HEADER_TYPE_CARDBUS) || h->class_id == 257) { // I/O
                h->region[i].memory = false;
                //h->region[i].phy_addr &= BASE_ADDRESS_IO_MASK;
                h->region[i].phy_addr = h->region[i].phy_addr & BASE_ADDRESS_IO_MASK;
                h->region[i].log_addr = ~0;
                h->region[i].size = 
                    ~(h->region[i].size & BASE_ADDRESS_IO_MASK) + 1;
            } else {
                h->region[i].memory = true;
                //h->region[i].phy_addr &= BASE_ADDRESS_MEM_MASK;
                h->region[i].phy_addr = h->region[i].phy_addr & BASE_ADDRESS_MEM_MASK;
                h->region[i].log_addr = phy2log(h->region[i].phy_addr);
                h->region[i].size =
                    ~(h->region[i].size & BASE_ADDRESS_MEM_MASK) + 1;
            }
            h->cardbus_cis = cfg32(l.bus, l.dev_fn, CARDBUS_CIS);
            h->subsystem_vendor_id = 
                cfg16(l.bus, l.dev_fn, SUBSYSTEM_VENDOR_ID);
            h->subsystem_device_id = 
                cfg16(l.bus, l.dev_fn, SUBSYSTEM_DEVICE_ID);
            h->rom_address = cfg32(l.bus, l.dev_fn, ROM_ADDRESS);
            h->interrupt_line = cfg8(l.bus, l.dev_fn, INTERRUPT_LINE);
            h->interrupt_pin = cfg8(l.bus, l.dev_fn, INTERRUPT_PIN);
            h->min_gnt = cfg8(l.bus, l.dev_fn, MIN_GNT);
            h->max_lat = cfg8(l.bus, l.dev_fn, MAX_LAT);
        }
    } else
        h->locator = Locator(Locator::INVALID, Locator::INVALID);
}

ML310_PCI::Locator ML310_PCI::scan(const ML310_PCI::Class_Id & c, int order)
{
    db<ML310_PCI>(TRC) << "ML310_PCI::scan(class=" << c
                    << ",order=" << order << ")\n";

    for(int bus = 0 ; bus <= MAX_BUS; bus++)
        for(int dfn = 0; dfn <= MAX_DEV_FN; dfn++)
            if(class_id(bus, dfn) == c){
                db<ML310_PCI>(TRC) << "";
                if(!order--)
                    return Locator(bus, dfn);
            }
    return Locator(Locator::INVALID, Locator::INVALID);
}

ML310_PCI::Locator ML310_PCI::scan(const ML310_PCI::Vendor_Id & v, 
                             const ML310_PCI::Device_Id & d, int order)
{
    db<ML310_PCI>(TRC) << "ML310_PCI::scan(vend=" << v
                    << ",dev=" << d
                    << ",order=" << order << ")\n";

    for(int bus = 0 ; bus <= MAX_BUS; bus++)
        for(int dfn = 0; dfn <= MAX_DEV_FN; dfn++)
            if((vendor_id(bus, dfn) == v) && (device_id(bus, dfn) == d)) {
                db<ML310_PCI>(TRC) << ""; //Totally Nasty. Verify latter!
                if(!order--)
                    return Locator(bus, dfn);
            }
    return Locator(Locator::INVALID, Locator::INVALID);
}

__END_SYS
