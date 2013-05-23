// EPOS ML310 PCI Mediator

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __ml310_pci_h
#define __ml310_pci_h

#include <pci.h>

__BEGIN_SYS

class ML310_PCI: public PCI_Common
{
private:
    static const int MAX_BUS = Traits<ML310_PCI>::MAX_BUS;
    static const int MAX_DEV_FN = Traits<ML310_PCI>::MAX_DEV_FN;

    // From Xilinx OPB_PCI IP Manual
    // CONFADD is a 32 bit register accessed only when referenced as a Dword.
    // A Byte or Word reference will NOT "pass through" the Configuration Address
    // Register onto the PCI bus as an I/O cycle. The CONFADD register
    // contains the Bus Number, Device Number, Function Number, and Register
    // Number for which a subsequent configuration access is intended.
    //
    // WARNING -> The above definition follows th PowerPC way for naming bits
    // so bit 0 is the most significant bit (x80000000). In fact, the method
    // cmd is based on little-endian definition, since all the address_space
    // used by PCI is configured as little endian on SLER register.
    //
    // [25:31] -> Reserved Hardwired to 0
    // [24:24] -> Active High Bit (Enable)
    // [16:23] -> bus (8 bits)
    // [13:15] -> function (3 bits)
    // [ 8:12] -> device (5 bits)
    // [ 6: 7] -> Hardwired to 0
    // [ 0: 5] -> register (6 bits) -> Check
    static const int CONFADDR = Traits<ML310_PCI>::BASE_ADDRESS + 0x10c;

    // CONFDATA is a 32 bit read/write window into configuration space.
    // The portion of configuration space that is referenced by CONFDATA is
    // determined by the contents of CONFADDR.
    static const int CONFDATA = Traits<ML310_PCI>::BASE_ADDRESS + 0x110;
    static const int BUS_SUBBUS_NUM_ADDR = Traits<ML310_PCI>::BASE_ADDRESS + 0x114;

public:
    //Other Register
    static const int ISR = Traits<ML310_PCI>::BASE_ADDRESS;
    static const int IPR = Traits<ML310_PCI>::BASE_ADDRESS + 0X04;
    static const int IER = Traits<ML310_PCI>::BASE_ADDRESS + 0X08;
    static const int IID = Traits<ML310_PCI>::BASE_ADDRESS + 0X18;
    static const int GIE = Traits<ML310_PCI>::BASE_ADDRESS + 0X1C;
    static const int BIR = Traits<ML310_PCI>::BASE_ADDRESS + 0X20;
    static const int BIER = Traits<ML310_PCI>::BASE_ADDRESS + 0X28;

public:
    ML310_PCI() {}

    static Locator scan(const Class_Id & c, int order);
    static Locator scan(const Vendor_Id & v, const Device_Id & d, int order);

    static void header(const Locator & l, Header * h);
    static Reg16 command(const Locator & l) {
        return cfg16(l.bus, l.dev_fn, COMMAND);
    }
    static void  command(const Locator & l, Reg16 v) {
        cfg16(l.bus, l.dev_fn, COMMAND, v);
    }
    static Reg16 status(const Locator & l) {
        return cfg16(l.bus, l.dev_fn, STATUS);
    }
    static void  status(const Locator & l, Reg16 v) {
        cfg16(l.bus, l.dev_fn, STATUS, v);
    }

    static void init();

//private:
public:
    static int cmd(CPU::Reg8 bus, CPU::Reg8 dev_fn, CPU::Reg8 addr) {
        return 0x80000000 | (bus << 16) | (dev_fn << 8) | (addr & ~3);
    }

    static CPU::Reg8 cfg8(CPU::Reg8 bus, CPU::Reg8 dev_fn, CPU::Reg8 addr) {
        Reg32 data = cfg32(bus, dev_fn, addr);
        CPU::sync_io();
        return *(Reg8*)((Reg32)&data + 3 - (addr & 3));
    }

    static CPU::Reg16 cfg16(CPU::Reg8 bus, CPU::Reg8 dev_fn, CPU::Reg8 addr) {
        Reg32 data = cfg32(bus, dev_fn, addr);
        CPU::sync_io();
        return *(Reg16 *)(((Reg32)&data + 2 - (addr & 2)));
    }

    static CPU::Reg32 cfg32(CPU::Reg8 bus, CPU::Reg8 dev_fn, CPU::Reg8 addr) {
        *((int *)CONFADDR) = cmd(bus, dev_fn, addr);
        return *((volatile unsigned int *)CONFDATA);
    }

    static void cfg8(CPU::Reg8 bus, CPU::Reg8 dev_fn, CPU::Reg8 addr, CPU::Reg8 value) {
        Reg32 data = cfg32(bus, dev_fn, addr);
        *(Reg8*)((Reg32)&data + 3 - (addr & 3)) = value;
        CPU::sync_io();
        cfg32(bus,dev_fn,addr, data);
    }

    static void cfg16(CPU::Reg8 bus, CPU::Reg8 dev_fn, CPU::Reg8 addr, CPU::Reg16 value) {
        Reg32 data = cfg32(bus, dev_fn, addr);
        *(Reg8*)((Reg32)&data + 2 - (addr & 2)) = value;
        CPU::sync_io();
        cfg32(bus,dev_fn,addr, data);
    }

    static void cfg32(CPU::Reg8 bus, CPU::Reg8 dev_fn, CPU::Reg8 addr, CPU::Reg32 value) {
        *((int *)CONFADDR) = cmd(bus, dev_fn, addr);
        CPU::sync_io();
        *((volatile unsigned int *)(CONFDATA)) = value;
    }

    static CPU::Reg16 vendor_id(CPU::Reg8 bus, CPU::Reg8 dev_fn) {
        return cfg16(bus, dev_fn, VENDOR_ID);
    }

    static CPU::Reg16 device_id(CPU::Reg8 bus, CPU::Reg8 dev_fn) {
        return cfg16(bus, dev_fn, DEVICE_ID);
    }

    static CPU::Reg16 class_id(CPU::Reg8 bus, CPU::Reg8 dev_fn) {
        return cfg16(bus, dev_fn, CLASS_ID);
    }

    static Log_Addr phy2log(const Phy_Addr & addr) { return addr; }

  private:

    static int autoconfig_setup_bar(int bus, int dev_fn, int bar_qnt = 6){

        unsigned int * upper_limit;
        unsigned int bar_addr;

        if(bar_qnt > 6) return -1;

        Header hdr;
        header(Locator(bus, dev_fn), &hdr);

        for(int i = 0; i < bar_qnt; i++){
           if(hdr.region[i].size){
             if(hdr.region[i].memory){
                upper_limit = &_memspace_upper;
             } else {
                upper_limit = &_iospace_upper;
             }
             //Allocate space
             bar_addr = (*upper_limit - hdr.region[i].size) & ~(hdr.region[i].size - 1);
             cfg32(bus, dev_fn, _base_address[i], bar_addr);
             *upper_limit = bar_addr;
           }
        }

        //Map All IRQ do 12 ... ???? 
        cfg8(bus, dev_fn, 0x3C, 12);

        return 0;

    }

    static int autoconfig_bus(int bus) {

        unsigned int last_device_id = 0;
        unsigned int last_vendor_id = 0;
        unsigned int sub_bus = bus;

        for(int dev_fn = 0; dev_fn <= Traits<ML310_PCI>::MAX_DEV_FN; dev_fn++) {

            //Skips Host Bridge from scan ...
            if ( (bus == 0) && ((dev_fn >> 3) == (int)Traits<ML310_PCI>::HOST_BRIDGE_DEV) )
               continue;

            Header hdr;
            header(Locator(bus, dev_fn), &hdr);

            //If header is invalid, continue !
            if (!hdr) continue;

            //Verify if is the same device responding on multiple functions.
            if ( (dev_fn & 0x7) && (hdr.vendor_id == last_vendor_id) && (hdr.device_id == last_device_id) )
               continue;
            last_vendor_id = hdr.vendor_id;
            last_device_id = hdr.device_id;


            if ( hdr.class_id == 0x604 ) {
                db<ML310_PCI>(INF) << "pci_init: PCI Bridge found at " << (int)bus << ":" << (dev_fn >> 3) << "." << (dev_fn & 7) << "\n";
                unsigned int iosave, memsave;

                /* Allocate PCI I/O and/or memory space */
                autoconfig_setup_bar(bus, dev_fn, 2);

                /* Config Initial Primary, Second and Subordinate_Bus Numbers */
                cfg8(bus, dev_fn, PRIMARY_BUS, bus);
                cfg8(bus, dev_fn, SECONDARY_BUS, sub_bus + 1);
                cfg8(bus, dev_fn, SUBORDINATE_BUS, 0xFF);

                /* Round memory allocator to 1MB boundary */
                _memspace_upper &= ~(0x100000 - 1);
                memsave = _memspace_upper;

                /* Round I/O allocator to 4KB boundary */
                _iospace_upper &= ~(0x1000 - 1);
                iosave = _iospace_upper;

                /* Set up memory and I/O filter limits, assume 32-bit I/O space */
                cfg16(bus, dev_fn, MEMORY_LIMIT, ((_memspace_upper - 1) & 0xfff00000) >> 16);
                cfg8(bus, dev_fn, IO_LIMIT, ((_iospace_upper - 1) & 0x0000f000) >> 8);
                cfg16(bus, dev_fn, IO_LIMIT_UPPER16, ((_iospace_upper - 1) & 0xffff0000) >> 16);

                /* Zero upper 32 bits of prefetchable base/limit */
                cfg32(bus, dev_fn, PREF_BASE_UPPER32, 0);
                cfg32(bus, dev_fn, PREF_LIMIT_UPPER32, 0);

                /* Scan the Bridge Second bus */
                sub_bus = autoconfig_bus(sub_bus+1);

                /* Update subordinate bus number */
                cfg8(bus, dev_fn, SUBORDINATE_BUS, sub_bus);

                /* Round memory allocator to 1MB boundary. If no space used, allocate minimum. */
                _memspace_upper &= ~(0x100000 - 1);
                if (memsave == _memspace_upper)
                    _memspace_upper -= 0x00100000;
                cfg16(bus, dev_fn, MEMORY_BASE, _memspace_upper >> 16);

                /* Allocate 1MB for pre-fretch */
                cfg16(bus, dev_fn, PREF_MEMORY_LIMIT, ((_memspace_upper - 1) & 0xFFF00000) >> 16);
                _memspace_upper -= 0x100000;
                cfg16(bus, dev_fn, PREF_MEMORY_BASE, _memspace_upper >> 16);

                /* Round I/O allocator to 4KB boundary */
                _iospace_upper &= ~(0x1000 - 1);
                if (iosave == _iospace_upper)
                     _iospace_upper -= 0x1000;

                cfg8(bus, dev_fn, IO_BASE, (_iospace_upper & 0x0000f000) >> 8);
                cfg8(bus, dev_fn, IO_BASE_UPPER16, _iospace_upper >> 16);

                /* Enable memory and I/O accesses, enable bus master */
                unsigned int cmdstat;
                cmdstat = cfg32(bus, dev_fn, COMMAND);
                cmdstat = cmdstat | ( COMMAND_IO | COMMAND_MEMORY | COMMAND_MASTER );
                cfg32(bus, dev_fn, COMMAND, cmdstat);

            } else if ( hdr.class_id == 0x607 ) {
                db<ML310_PCI>(INF) << "pci_init: Cardbus Bridge found at " << (int)bus << ":" << (dev_fn >> 3) << "." << (dev_fn & 7) << "\n";
            } else {
                if(bus) db<ML310_PCI>(INF) << "+";
                for(int i=0; i < bus*2; i++){
                   db<ML310_PCI>(INF) << "-";
                }
                if(bus) db<ML310_PCI>(INF) << "> ";
                db<ML310_PCI>(INF) << "pci_init: " << (int)bus << ":" << (dev_fn >> 3) << "." << (dev_fn & 7)
                     << " Class " << (void*)(int)hdr.class_id << " " << (void*)(int)hdr.vendor_id
                     << ":" << (void*)(int)hdr.device_id
                     << " (rev " << (int)hdr.revision_id
                     << ")\n";

                //Setup PCI Bars.
                autoconfig_setup_bar(bus, dev_fn);

                /* Enable memory and I/O accesses, enable bus master */
                unsigned int cmdstat;
                cmdstat = cfg32(bus, dev_fn, COMMAND);
                cmdstat = cmdstat | ( COMMAND_IO | COMMAND_MEMORY | COMMAND_MASTER );
                cfg32(bus, dev_fn, COMMAND, cmdstat);
            }
        }
        return sub_bus;
    }

  private:
    static Phy_Addr _phy_io_mem;
    static unsigned int _memspace_upper;
    static unsigned int _iospace_upper;
    static Reg32 _base_address[Region::N];
};

__END_SYS

#endif
