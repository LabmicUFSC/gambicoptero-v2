// EPOS Event Handler Utility Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <utility/handler.h>
#include <alarm.h>
#include <thread.h>
#include <semaphore.h>
#include <chronometer.h>

__USING_SYS

const int iterations = 100;
const int period_a = 100;
const int period_b = 200;
const int period_c = 300;

void func_a(void);
int func_b(void);
int func_c(void);
int max(int a, int b, int c) { return ((a >= b) && (a >= c)) ? a : ((b >= a) && (b >= c) ? b : c); }

OStream cout;

Semaphore sem_c(0);
Thread * thread_b;
Thread * thread_c;

int main()
{
    cout << "Event Handler Utility Test\n";

    cout << "\nThis test consists in creating three event handlers as follows:\n";
    cout << "  Handler 1 triggers a \"function\" that prints an \"a\" every " << period_a << " ms;\n";
    cout << "  Handler 2 triggers a \"thread\" that prints a \"b\" every " << period_b << " ms;\n";
    cout << "  Handler 3 triggers a \"v\" on a \"semaphore\" that controls another thread that prints a \"c\" every " << period_c << "ms.\n";

    thread_b = new Thread(&func_b);
    thread_c = new Thread(&func_c);

    cout << "Threads B and C have been created!\n";

    Function_Handler handler_a(&func_a);
    Thread_Handler handler_b(thread_b);
    Semaphore_Handler handler_c(&sem_c);
    
    cout << "Now the alarms will be created, along with a chronometer to keep track of the total execution time. I'll then wait for the threads to finish...\n\n";

    Chronometer chrono;
    chrono.start();

    Alarm alarm_a(period_a * 1000, &handler_a, iterations);
    Alarm alarm_b(period_b * 1000, &handler_b, iterations);
    Alarm alarm_c(period_c * 1000, &handler_c, iterations);

    int status_b = thread_b->join();
    int status_c = thread_c->join();

    chrono.stop();

    cout << "\n\nThread B exited with status " << status_b 
  	 << " and thread C exited with status " << status_c << "\n";

    cout << "\nThe estimated time to run the test was " << max(period_a, period_b, period_c) * iterations << " ms. The measured time was " << chrono.read() / 1000 <<" ms!\n";

    delete thread_b;
    delete thread_c;
    
    cout << "I'm also done, bye!\n";

    return 0;
}

void func_a()
{
    cout << "a";
    return;   
}

int func_b(void)
{
    cout << "B";
    for(int i = 0; i < iterations; i++) {
	thread_b->suspend();
	cout << "b";
    }
    cout << "B";
    return 'B';   
}

int func_c(void)
{
    cout << "C";
    for(int i = 0; i < iterations; i++) {
	sem_c.p();
	cout << "c";
    }
    cout << "C";
    return 'C';   
}
