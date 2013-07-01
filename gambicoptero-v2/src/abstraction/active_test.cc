// EPOS Active Object Abstraction Test Program

#include <utility/ostream.h>
#include <active.h>

__USING_SYS

const int iterations = 100;

OStream cout;

class A: public Active
{
public:
    int run() {
	for(int i = iterations; i > 0; i--) {
	    for(int i = 0; i < 79; i++)
		cout << "a";
	    cout << "\n";
	}

	return 'A';   
    }
};

class B: public Active
{
public:
    int run() {
	for(int i = iterations; i > 0; i--) {
	    for(int i = 0; i < 79; i++)
		cout << "b";
	    cout << "\n";
	}

	return 'B';   
    }
};

int main()
{
    cout << "Active Object Test\n";

    A * a = new A;
    B * b = new B;

    a->start();
    b->start();

    cout << "Both threads are now done and have suspended themselves. I'll now wake them up so they can exit ...\n";

    int status_a = a->join();
    int status_b = b->join();

    cout << "Thread A exited with status " << status_a 
 	 << " and thread B exited with status " << status_b << "\n";

    delete a;
    delete b;
    
    cout << "I'm also done, bye!\n";

    return 0;
}
