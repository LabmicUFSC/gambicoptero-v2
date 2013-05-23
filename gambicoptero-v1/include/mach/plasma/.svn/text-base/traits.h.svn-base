#ifndef __plasma_traits_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __plasma_traits_h

__BEGIN_SYS

class PLASMA_Common;
template <> struct Traits<PLASMA_Common>: public Traits<void>
{
//     static const bool debugged = true;
};

template <> struct Traits<PLASMA>: public Traits<PLASMA_Common>
{
  static const unsigned int MAX_CPUS = 1;

  static const unsigned int CLOCK                  = 50000000;

  static const unsigned int APPLICATION_STACK_SIZE = 16 * 1024;
  static const unsigned int APPLICATION_HEAP_SIZE  = 16 * 1024;
  static const unsigned int SYSTEM_STACK_SIZE      = 4 * 1024;
  static const unsigned int SYSTEM_HEAP_SIZE       = 16 * APPLICATION_STACK_SIZE;
  static const unsigned int LEDS_ADDRESS           = 0x20000030;

  static const unsigned int PRIORITY_HEAP_BASE_ADDR = 0;
  static const unsigned int PRIORITY_HEAP_SIZE = 4;
  static const unsigned int PRIORITY_HEAP_TOP_ADDR = PRIORITY_HEAP_BASE_ADDR + PRIORITY_HEAP_SIZE - 1;
};

template <> struct Traits<PLASMA_IC>: public Traits<PLASMA_Common>
{
  static const unsigned int BASE_ADDRESS   = 0x20000010;
  static const unsigned int MASK_ADDRESS   = BASE_ADDRESS;
  static const unsigned int STATUS_ADDRESS = BASE_ADDRESS + 0x0010;
  static const unsigned int INT_VECTOR_ADDRESS     = 0x0000003C;

};

template <> struct Traits<PLASMA_UART>: public Traits<PLASMA_Common>
{
  static const unsigned int CLOCK        = 1843200; // 1.8432 MHz - Verify !
  static const unsigned int BASE_ADDRESS = 0x20000000;
  static const unsigned int DATA_ADDRESS = BASE_ADDRESS;
  static const unsigned int STATUS_ADDRESS = Traits<PLASMA_IC>::STATUS_ADDRESS;
};

template <> struct Traits<PLASMA_Timer>: public Traits<PLASMA_Common>
{
  static const unsigned int FREQUENCY = Traits<PLASMA>::CLOCK / (1<<18);
  static const unsigned int BASE_ADDRESS = 0x20000060;
  static const unsigned int DATA_ADDRESS = BASE_ADDRESS;
  static const unsigned int WRITE_ADDRESS = BASE_ADDRESS + 0x0010;
};

template <> struct Traits<PLASMA_NIC>: public Traits<void>
{
  typedef LIST<> NICS;
  static const bool enabled = false;
};

__END_SYS

#endif
