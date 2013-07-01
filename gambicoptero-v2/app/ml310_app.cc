// ML310_LOADER
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
