// EPOS ML310 PCI Mediator

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <machine.h>

__BEGIN_SYS

void ML310_PCI::init()
{
    db<ML310_PCI>(TRC) << "ML310_PCI::init()\n";

    PPC32::int_disable();

    /* HostBridge Self-Configuration */
    cfg32(0, 0x40, 0x04, 0x4701FFFF); 
    CPU::sync_io();
    /* max latency timer on bridge */
    cfg32(0, 0x40, 0x0C, 0x0000FF00);
    /* max bus number */
    *((volatile unsigned int *)BUS_SUBBUS_NUM_ADDR) = 0xff000000;
    CPU::sync_io();

    //Initialize TI2250 Pci2Pci Bridge -> Not working, must be reviewed !!!
    cfg32(0, 0x48, 0x04, 0x02100007);
    cfg32(0, 0x48, 0x18, 0x00010100);
    CPU::sync_io();

    //--------------------------------------------------------------------
    // IDE initialization.
    //
    // - Enable the IDE controller, and both channel's I/O pad drivers.
    // - Set the primary channel to use IRQ14 in level-sensitive mode.
    // - Set the secondary channel to use IRQ15 in level-sensitive mode.
    //
    // Note that the primary channel can be used in edge-triggered mode,
    // but edge-triggered mode is apparently not supported for the 
    // secondary channel.  To keep things consistent, both are used in
    // level-sensitive mode.  This requires that the 8259 be programmed
    // to the correct level/edge mode for IRQ14, IRQ15.
    //cfg8(0,  0x10, 0x58, 0x4c);         // Enable IDE controller.
    //cfg8(0,  0x10, 0x44, 0x0d);         // Level, IRQ14.
    //cfg8(0,  0x10, 0x75, 0x0f);         // Level, IRQ15.
    // The M5229 IDE controller defaults to compatible mode, which
    // will force Linux to use PIO-only mode for IDE devices attached
    // to it.  If the M5229 is placed in native mode, Linux will
    // enable DMA by default.  Uncomment one of the following two
    // lines to enable DMA mode.
    // pci_cfgwr08(0, 11, 0x09, 0xfa);              // Compat mode.
    //cfg8(0, 0x58, 0x09, 0xff);         // Native mode.
    //--------------------------------------------------------------------
    // Interrupt routing.
    //cfg8(0,  0x10, 0x48, 0x00);         // INTB = disabled, INTA = disabled.
    //cfg8(0,  0x10, 0x49, 0x00);         // INTD = disabled, INTC = disabled.
    //cfg8(0,  0x10, 0x4a, 0x00);         // INTF = disabled, INTE = disabled.
    //cfg8(0,  0x10, 0x4b, 0x60);         // Audio = INT7, Modem = disabled.
    //cfg8(0,  0x10, 0x74, 0x06);         // USB = INT7.


    /* Now let's do something funny! scan buses looking for devices and allocating
     * their base_addresses since we don't have a BIOS :) The method above is recursive !*/
    autoconfig_bus(0);

    PPC32::int_enable();
}

__END_SYS
