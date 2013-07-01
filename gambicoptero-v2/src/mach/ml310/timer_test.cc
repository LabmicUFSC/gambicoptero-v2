// EPOS ML310_Timer Test Program

#include <utility/ostream.h>
#include <machine.h>
#include <ic.h>
#include <timer.h>

__USING_SYS

OStream cout;

void timer_handler(unsigned int interrupt){
  static unsigned int seconds = 0;
  static unsigned int minutes = 0;

  cout << interrupt << "=>" << minutes << ":" << seconds << "\n";
  seconds++;

  if(seconds == 60){
     minutes++;
     seconds = 0;
  }
}

int main()
{
    cout << "ML310_Timer test\n";

    Timer _timer;
    CPU::int_disable();

    Machine::int_vector(IC::IRQ_TIMER, &timer_handler);
    _timer.frequency(1);

    CPU::int_enable();

    cout << "LEDs are flashing now!!!\n";
    unsigned int * led = (unsigned int *)(Traits<Machine>::LEDS_BASEADDR);
    for (unsigned int i = 0; ; i++) {
      *led = ~i;
      for(unsigned int x = 0; x < 0x007FFFFF; x++);
    }

    return 0;
}
