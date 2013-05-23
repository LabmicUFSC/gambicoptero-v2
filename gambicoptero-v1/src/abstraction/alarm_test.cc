// EPOS Alarm Abstraction Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <alarm.h>

__USING_SYS

const int iterations = 10;

void func_a(void);
void func_b(void);

OStream cout;

int main()
{
    cout << "Alarm test\n";

    cout << "I'm the first thread of the first task created in the system.\n";
    cout << "I'll now create two alarms and put myself in a delay ...\n";

    Function_Handler handler_a(&func_a);
    Alarm alarm_a(1000000, &handler_a, iterations);
 
    Function_Handler handler_b(&func_b);
    Alarm alarm_b(1000000, &handler_b, iterations);

    // Note that in case of idle-waiting, this thread will go into suspend
    // and the alarm handlers above will trigger the functions in the context
    // of the idle thread!
    Alarm::delay(1000000 * (iterations + 1));

    cout << "I'm done, bye!\n";

    return 0;
}

void func_a()
{
    for(int i = 0; i < 79; i++)
	cout << "a";
    cout << "\n";
}

void func_b(void)
{
    for(int i = 0; i < 79; i++)
	cout << "b";
    cout << "\n";
}
