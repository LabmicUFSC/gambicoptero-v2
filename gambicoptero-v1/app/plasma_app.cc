// Plasma Dummy Application

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
//
// Author: Hugo
// Documentation: $EPOS/doc/loader			Date: 04 Aug 2007

#include <utility/ostream.h>
#include <display.h>
#include <machine.h>

__USING_SYS

OStream cout;

int main()
{
  cout << "EPOS LOADED\n";

  cout << "LEDs are flashing now!!!\n";
  unsigned int * led = (unsigned int *)(Traits<Machine>::LEDS_ADDRESS);
  for (unsigned int i = 0; i < 0x100 ; i++) {
    *led = ~i;
    for(unsigned int x = 0; x < 0x0007FFFF; x++);
  }
  cout << "I'm done ... CPU should halt now!\n";

  return 0;
}

