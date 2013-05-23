#ifndef __axi4lite_traits_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __axi4lite_traits_h

__BEGIN_SYS

class AXI4LITE_Common;
template <> struct Traits<AXI4LITE_Common>: public Traits<void>
{
//     static const bool debugged = true;
};

template <> struct Traits<AXI4LITE>: public Traits<AXI4LITE_Common>
{
  static const unsigned int MAX_CPUS = 1;

  static const unsigned int CLOCK                  = 50000000;

  static const unsigned int APPLICATION_STACK_SIZE = 16 * 1024;
  static const unsigned int APPLICATION_HEAP_SIZE  = 16 * 1024;
  static const unsigned int SYSTEM_STACK_SIZE      = 4 * 1024;
  static const unsigned int SYSTEM_HEAP_SIZE       = 16 * APPLICATION_STACK_SIZE;
  static const unsigned int LEDS_ADDRESS           = 0x80000400;

  static const unsigned int PRIORITY_HEAP_BASE_ADDR = 0;
  static const unsigned int PRIORITY_HEAP_SIZE = 4;
  static const unsigned int PRIORITY_HEAP_TOP_ADDR = PRIORITY_HEAP_BASE_ADDR + PRIORITY_HEAP_SIZE - 1;
};

template <> struct Traits<AXI4LITE_IC>: public Traits<AXI4LITE_Common>
{
  static const unsigned int BASE_ADDRESS = 0x80000C00;
  static const unsigned int EDGE_ENABLE = ~0; // All interrupts configured as edge enabled
  static const unsigned int POLARITY = ~0; // All interrupts configured as raising edge
  static const unsigned int NUMBER_OF_INTERRUPTS = 32;
  static const unsigned int CPU_JMP_ADDRESS = 0x0000003C;
  static const bool ATTEND_ONLY_ONE = true; //Attend only one pending interrupt at each CPU Interrupt.
};

template <> struct Traits<AXI4LITE_UART>: public Traits<AXI4LITE_Common>
{
  static const unsigned int CLOCK = Traits<AXI4LITE>::CLOCK;
  static const unsigned int BASE_ADDRESS = 0x80000000;
  static const unsigned int BAUDRATE = 57600;
};

template <> struct Traits<AXI4LITE_Timer>: public Traits<AXI4LITE_Common>
{
  static const unsigned int CLOCK = Traits<AXI4LITE>::CLOCK;
  static const unsigned int FREQUENCY = 10000; // 10 kHz -> 100us
  static const unsigned int BASE_ADDRESS = 0x80000800;
};

template <> struct Traits<AXI4LITE_NIC>: public Traits<void>
{
  typedef LIST<> NICS;
  static const bool enabled = false;
};

__END_SYS

#endif
