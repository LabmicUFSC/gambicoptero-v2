// EPOS Semaphore Abstraction Test Program 2

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <utility/random.h>
#include <thread.h>
#include <semaphore.h>

__USING_SYS

const int ITERATIONS = 1000;
const int THREADS = 100;
const int SEMAPHORES = 10;

Semaphore sem_display;

Thread * threads[THREADS];
Semaphore * semaphores[SEMAPHORES];

OStream cout;

int thread(int t)
{
    for(int i = 0; i < ITERATIONS; i++) {
	int s = Pseudo_Random::random() % SEMAPHORES;

	if(t % 2)
	    semaphores[s]->p();
	else
	    semaphores[s]->v();

	sem_display.p();
	cout << "t(" << t << ")->s(" << s << ")\n";
	sem_display.v();

	if(t % 2)
	    semaphores[s]->v();
	else
	    semaphores[s]->p();
    }
}

int main()
{
    cout << "Semaphore Stress Test:\n";

    cout << "  Creating " << SEMAPHORES << "semaphores ...";
    for(int i = 0; i < SEMAPHORES; i++)
	semaphores[i] = new Semaphore;
    cout << " done!\n";

    cout << "  Creating " << THREADS << "threads ...";
    sem_display.p();
    for(int i = 0; i < THREADS; i++)
	threads[i] = new Thread(&thread, i);
    cout << " done!\n";

    cout << "  Threads will now coordinately interact with each other "
	    << ITERATIONS << " times in a ramdom order ...\n";
    sem_display.v();
    for(int i = 0; i < THREADS; i++)
	threads[i]->join();
    cout << " done!\n";

    cout << "The end!\n";

    return 0;
}
