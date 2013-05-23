#include <utility/ostream.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <thread.h>
#include <semaphore.h>
#include <alarm.h>

__USING_SYS

const int iterations = 100;

OStream cout;

const int BUF_SIZE = 16;
char buffer[BUF_SIZE];
Semaphore empty(BUF_SIZE);
Semaphore full(0);

int consumer()
{
    int out = 0;
    for(int i = 0; i < iterations; i++) {
	full.p();
	cout << buffer[out];
	out = (out + 1) % BUF_SIZE;
 	Alarm::delay(1000 * out);
	empty.v();
    }
}

int main()
{
    Thread * cons = new Thread(&consumer);

    // producer
    int in = 0;
    for(int i = 0; i < iterations; i++) {
	empty.p();
 	cout << "P";
 	Alarm::delay(1000 * in);
	buffer[in] = 'a' + in;
	in = (in + 1) % BUF_SIZE;
	full.v();
    }
    cons->join();

    delete cons;

    return 0;
}
