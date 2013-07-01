#ifndef __epos_wrappers
#define __epos_wrappers

#include <rtc.h>

typedef unsigned int uint32;

void srandom(int seed);

void exit(int s);

typedef System::RTC::Second time_t;
time_t time ( time_t * timer );

typedef System::RTC::Microsecond clock_t;
clock_t clock ( void );

#define UCHAR_MAX 255
#define endl "\n"
#define NULL 0

#ifndef DEBUG
      #define assert(x)
#else
      #define assert(x) \
          if (! (x)) \
          { \
              cout << "ERROR!! Assert " << #x << " failed\n"; \
              cout << " on line " << __LINE__  << "\n"; \
              cout << " in file " << __FILE__ << "\n";  \
          }
#endif //ifndef DEBUG

#endif

