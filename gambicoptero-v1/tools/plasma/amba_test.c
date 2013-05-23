#include "plasma.h"

#define MemoryRead(A) (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


void print_string(char * p)
{
   while(*p){
     if(*p == '\n') hal_uart_putc('\r');
     hal_uart_putc(*p);
     p++;
   }
}

void printhex_8(char c)
{
    hal_uart_putc("0123456789ABCDEF"[(c >> 4) & 15]);
    hal_uart_putc("0123456789ABCDEF"[c & 15]);
}

void printhex_16(short s){
   printhex_8((char)(s >> 8));
   printhex_8((char)(s & 255));
}

void printhex_32(int i){
   printhex_16((short)(i >> 16));
   printhex_16((short)(i & 0x0000FFFF));
}


unsigned int wait = 0;
void timer_handler(unsigned irq){
    print_string("$TIMER_INT$");
}

int main()
{

    hal_uart_init();
    pic_init();

    print_string("Hello\n");

    unsigned int i = 0;


    print_string("Test external RAM\n");
    for(i = EXT_RAM_BASE; i < EXT_RAM_TOP; i += 4){
        MemoryWrite(i, i);
    }
    for(i = EXT_RAM_BASE; i < EXT_RAM_TOP; i += 4){
        if(MemoryRead(i) != i){
            print_string("Error\n");
            while(1);
        }
    }
    print_string("OK\n");


    print_string("Test Timer\n");
    unsigned int time = timer_regs->time;
    print_string("Time: "); printhex_32(time); print_string("\n");
    for (i = 0; i < 256; ++i);
    time = timer_regs->time - time;
    print_string("Elapsed: "); printhex_32(time); print_string("\n");
    print_string("OK\n");

    print_string("Test Timer interrupt / IC\n");
    pic_register_handler(IRQ_TIMER, timer_handler);
    int_enable();
    unsigned int wait_time = timer_regs->time + time;
    timer_regs->time = wait_time;
    while(timer_regs->time < wait_time);
    print_string("OK\n");


    print_string("Test GPIO\n");
    while(1){
        gpio_base->output = gpio_base->input;
    }

    return 0;
}

