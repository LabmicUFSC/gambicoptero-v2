// EPOS Task Test Program

#include <utility/ostream.h>
#include <alarm.h>
#include <task.h>

__USING_SYS

const int iterations = 10;

int func_a(void);
int func_b(void);

Thread * a;
Thread * b;
Thread * m;

OStream cout;

int main()
{
    cout << "Task test\n";

    m = Thread::self();

    cout << "I'll try to clone myself:\n";

    cout << "My address space's page directory is located at "
	 << (void *)CPU::pdp() << "\n";
    Address_Space self(Address_Space::SELF);

    cout << "Creating and attaching segments:\n";
    Segment cs(100000);
    Segment ds(100000);
    CPU::Log_Addr * code = self.attach(cs);
    CPU::Log_Addr * data = self.attach(ds);
    cout << "  code => " << code << " done!\n";
    cout << "  data => " << data << " done!\n";

    cout << "Copying segments:";
    memcpy(code, 0, 100000);
    memcpy(data, (void *)0x00400000, 100000);
    cout << " done!\n";

    cout << "Detaching segments:";
    self.detach(cs);
    self.detach(ds);
    cout << " done!\n";

    Task * task = new Task(cs, ds);

    a = task->create_thread(&func_a);
    b = new Thread(&func_b);

    m->suspend();

    cout << "Both threads are now done and have suspended themselves. I'll now wait for 1 second and then wake them up so they can exit ...\n";

    Alarm::delay(1000000);

    a->resume();
    b->resume();

    int status_a = a->join();
    int status_b = b->join();

    cout << "Thread A exited with status " << status_a 
	 << " and thread B exited with status " << status_b << "\n";

    delete a;
    delete b;
    delete m;
    
    cout << "I'm also done, bye!\n";

    return 0;
}

int func_a(void)
{
    for(int i = iterations; i > 0; i--) {
	for(int i = 0; i < 79; i++)
	    cout << "a";
	cout << "\n";
	Thread::yield();
    }

    Thread::self()->suspend();

    return 'A';   
}

int func_b(void)
{
    for(int i = iterations; i > 0; i--) {
	for(int i = 0; i < 79; i++)
	    cout << "b";
	cout << "\n";
	Thread::yield();
    }

    m->resume();

    Thread::self()->suspend();

    return 'B';   
}
