// EPOS AXI4LITE_Timer Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <machine.h>
#include <ic.h>
#include <timer.h>

__USING_SYS

OStream cout;

void timer_handler(){
  static unsigned int seconds = 0;
  static unsigned int minutes = 0;

  cout << " => ";
  if (minutes < 10)
		cout << "0";
  cout << minutes << ":";
  if (seconds < 10)
		cout << "0";
  cout << seconds << "\n";
	
  seconds++;

  if(seconds == 60){
     minutes++;
     seconds = 0;
  }
}

int main()
{
    cout << "AXI4LITE_Timer test\n";

    AXI4LITE_Timer timer(1, &timer_handler, AXI4LITE_Timer::TEST);
	
	/* For some reason, after thread_first load, interrupts are disabled. 
	   That's why timer_test was not working properly. 
	   We should investigate if this is the behaviour expected. 
	   Meanwhile we always make shure that interrupts are on after creating
	   the timer ! */
	
	CPU::int_enable();
	
    cout << "LEDs are flashing now!!!\n";
    unsigned int * led = (unsigned int *)(Traits<Machine>::LEDS_ADDRESS);
    for (unsigned int i = 0; ; i++) {
      *led = i;
      for(unsigned int x = 0; x < 0x0001FFFF; x++);
    }

    return 0;
}
