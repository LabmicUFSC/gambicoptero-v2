// EPOS Thread Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <thread.h>
#include <alarm.h>

__USING_SYS

const int iterations = 100;

int func_a(void);
int func_b(void);

Thread * a;
Thread * b;
Thread * m;

OStream cout;

int main()
{
    cout << "Thread test\n";

    m = Thread::self();

    cout << "I'm the first thread of the first task created in the system.\n";
    cout << "I'll now create two threads and then wait for them to finish ...\n";

    a = new Thread(&func_a);
    b = new Thread(&func_b);

    int status_a = a->join();
    int status_b = b->join();

    cout << "Thread A exited with status " << status_a 
	 << " and thread B exited with status " << status_b << "\n";

    delete a;
    delete b;
    delete m;
    
    cout << "It should not be shown on the display!\n";

    return 0;
}

int func_a(void)
{
    for(int i = iterations; i > 0; i--) {
	for(int i = 0; i < 79; i++)
	    cout << "a";
	cout << "\n";
	Alarm::delay(500000);
    }

    return 'A';   
}

int func_b(void)
{
    for(int i = iterations; i > 0; i--) {
	for(int i = 0; i < 79; i++)
	    cout << "b";
	cout << "\n";
	Alarm::delay(500000);
    }

    return 'B';   
}
