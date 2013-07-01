// AXI4LITE Dummy Application
//
// Author: Hugo
// Documentation: $EPOS/doc/loader			Date: 23 May 2011

#include <utility/ostream.h>
#include <display.h>
#include <machine.h>

__USING_SYS

OStream cout;

void gpio_handler(unsigned int i){
	cout << "Button(" << i << ") Pressed!\n";
}

void precision_test(){

    volatile unsigned int * timer_reg = reinterpret_cast<volatile unsigned int *>(Traits<AXI4LITE_Timer>::BASE_ADDRESS);
    unsigned int tmp = 0;
    unsigned int mem_w=0,mem_r=0,mem_wr=0,int_loop=0,float_loop=0,print=0;

    cout << "Precision test\n";

    unsigned int HEAP_SIZE = 256;

    int *memory = new int[HEAP_SIZE];

    cout << "Heap - Write\n";
    tmp = *timer_reg;
    for(int i = 0; i < HEAP_SIZE; i += 1){
        memory[i] = i;
    }
    mem_w = *timer_reg - tmp;
    cout << "Heap - Read\n";
    tmp = *timer_reg;
    for(int i = 0; i < HEAP_SIZE; i += 1){
        if(memory[i] != i){
            cout << "Heap - Read - Error\n";
            while(1);
        }
    }
    mem_r = *timer_reg - tmp;
    cout << "Heap - Read after write\n";
    tmp = *timer_reg;
    for(int i = 0; i < HEAP_SIZE; i += 1){
        memory[i] = i;
        if(memory[i] != i){
            cout << "Heap - Read after write - Error\n";
            while(1);
        }
    }
    mem_wr = *timer_reg - tmp;
    cout << "OK\n";


    cout << "Integer loop\n";
    tmp = *timer_reg;
    unsigned int result = 1;
    for(int i = 0; i < HEAP_SIZE; i += 1){
        result += (unsigned int) ((int)result * i);
        result /= (unsigned int)i + 1;
    }
    int_loop = *timer_reg - tmp;
    cout << "OK: " << (int)result << "\n";


    /*
    cout << "Float loop\n";
    tmp = *timer_reg;
    float f_result = 1;
    for(int i = 0; i < HEAP_SIZE; i += 1){
        float f_i = (float)i + 1.0;
        f_result += f_result * f_i;
        f_result /= f_i;
    }
    float_loop = *timer_reg - tmp;
    cout << "OK: " << (int)f_result << "\n";
    */

    cout << "Print test\n";
    tmp = *timer_reg;
    cout << "aaa" << "bbbb" << "ccccccccccccccccc" << result << "|"<< 1937 << "|1234567890\n";
    print = *timer_reg - tmp;
    cout << "OK\n";

    cout << "Precision test end\n";
    cout << "mem_w: " << mem_w << "\n";
    cout << "mem_r: " << mem_r << "\n";
    cout << "mem_wr: " << mem_wr << "\n";
    cout << "int_loop: " << int_loop << "\n";
    cout << "float_loop: " << float_loop << "\n";
    cout << "print: " << print << "\n";
    cout << "total: " << mem_w+mem_r+mem_wr+int_loop+float_loop+print << "\n\n";
    while(true);
}


int main()
{
  cout << "EPOS LOADED\n";

  volatile unsigned int * timer_reg = reinterpret_cast<volatile unsigned int *>(Traits<AXI4LITE_Timer>::BASE_ADDRESS);
  unsigned int ticks = 5000*5000*2;
  unsigned int next_time = 0;
  cout << "1s tick - " << ticks << "\n";
  while(true){
      next_time = ticks + *timer_reg;
      while(next_time > *timer_reg);
      cout << "tick " << *timer_reg << "\n";
  }

  //precision_test();

  CPU::int_disable();
  IC::int_vector(IC::IRQ_GPIO_0, &gpio_handler);	
  IC::enable(IC::IRQ_GPIO_0);
  IC::int_vector(IC::IRQ_GPIO_1, &gpio_handler);	
  IC::enable(IC::IRQ_GPIO_1);
  IC::int_vector(IC::IRQ_GPIO_2, &gpio_handler);	
  IC::enable(IC::IRQ_GPIO_2);
  IC::int_vector(IC::IRQ_GPIO_3, &gpio_handler);	
  IC::enable(IC::IRQ_GPIO_3);
  CPU::int_enable();

  cout << "LEDs are flashing now!!!\n";
  unsigned int * led = (unsigned int *)(Traits<Machine>::LEDS_ADDRESS);
  for (unsigned int i = 0; i < 0x100 ; i++) {
    *led = i;
    for(unsigned int x = 0; x < 0x0007FFFF; x++);
  }
  cout << "I'm done ... CPU should halt now!\n";

  while(1);
	
  return 0;
}

