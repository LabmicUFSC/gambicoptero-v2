// ML310_LOADER

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
//
// Desc: LOADER is created by INIT as the first process when the system
//	 is configured to support multiple tasks. It creates all
//	 application tasks from the image received in ??? and then waits
//	 wait for them to finish.
//
// Author: Hugo
// Documentation: $EPOS/doc/loader			Date: 11 Oct 2005

#include <utility/ostream.h>
#include <display.h>
#include <machine.h>

__USING_SYS

OStream cout;

int main()
{
  cout << "EPOS LOADED\n";

  cout << "LEDs are flashing now!!!\n";
  unsigned int * led = (unsigned int *)(Traits<Machine>::LEDS_BASEADDR);
  for (unsigned int i = 0; i < 0x100 ; i++) {
    *led = ~i;
    for(unsigned int x = 0; x < 0x007FFFFF; x++);
  }

  return 0;
}
