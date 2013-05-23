#include "plasma.h"

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


int main()
{

    hal_uart_init();

    print_string("A test application\n");

    print_string("Test GPIO\n");
    while(1){
        gpio_base->output = gpio_base->input;
    }

    return 0;
}

