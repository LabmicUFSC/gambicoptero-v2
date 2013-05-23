#include "plasma.h"

int putchar(int value);
int puts(const char *string);

char *itoa10(unsigned long num)
{
   static char buf[12];
   int i;
   buf[10] = 0;
   for (i = 9; i >= 0; --i)
   {
      buf[i] = (char)((num % 10) + '0');
      num /= 10;
   }
   return buf;
}

char *xtoa(unsigned long num)
{
   static char buf[12];
   int i, digit;
   buf[8] = 0;
   for (i = 7; i >= 0; --i)
   {
      digit = num & 0xf;
      buf[i] = digit + (digit < 10 ? '0' : 'A' - 10);
      num >>= 4;
   }
   return buf;
}

#define MemoryRead(A) (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


void test(unsigned int diff){
    unsigned errors = 0;

    unsigned int i = 0;
    for(i = 0; ; ++i){

        unsigned int addr = RAM_EXTERNAL_BASE+(i*4);

        unsigned int value = i + diff;

        if(addr >= (RAM_EXTERNAL_BASE+RAM_EXTERNAL_SIZE-1)) break;

        MemoryWrite(addr, value);
        unsigned int read_value = MemoryRead(addr);

        if((i % 10000) == 0){
            puts("Currently At address: ");
            puts(xtoa(addr));
            puts("\n");

            puts("Write value: ");
            puts(xtoa(value));
            puts("\n");

            puts("Read value: ");
            puts(xtoa(read_value));
            puts("\n");
        }

        if (value != read_value){
            puts("Error at address: ");
            puts(xtoa(addr));
            puts("\n");
            errors += 1;
        }

        if (value != MemoryRead(addr)){
            puts("Error at address: ");
            puts(xtoa(addr));
            puts("\n");
            errors += 1;
        }

        if (value != MemoryRead(addr)){
            puts("Error at address: ");
            puts(xtoa(addr));
            puts("\n");
            errors += 1;
        }

    }

    puts("\nFinished\nNumber of errors: ");
    puts(xtoa(errors));
    puts("\n");
}


int main()
{
    puts("\nStarting at 0x10000000, writes and reads in the memory until it gets an error\n");
    test(0);
    puts("Do it again\n");
    test(10);

    while(1);
}

