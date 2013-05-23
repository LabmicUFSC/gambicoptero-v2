// EPOS Queue Utility Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <utility/queue.h>

__USING_SYS;

struct Integer1 {
    Integer1(int _i) : i(_i), e(this) {}
    
    int i;
    Queue<Integer1>::Element e;
};

struct Integer2 {
    Integer2(int _i, int _r) : i(_i), e(this, _r) {}
    
    int i;
    Ordered_Queue<Integer2>::Element e;
};

struct Integer3 {
    Integer3(int _i, int _r) : i(_i), e(this, _r) {}
    
    int i;
    Relative_Queue<Integer3>::Element e;
};

int main()
{
    OStream cout;

    cout << "Queue Utility Test\n";

    cout << "\nThis is an integer queue:\n";
    Integer1 i1(1), i2(2), i3(3), i4(4);
    Queue<Integer1> q1;
    cout << "Inserting the integer " << i1.i << "\n";
    q1.insert(&i1.e);
    cout << "Inserting the integer " << i2.i << "\n";
    q1.insert(&i2.e);
    cout << "Inserting the integer " << i3.i << "\n";
    q1.insert(&i3.e);
    cout << "Inserting the integer " << i4.i << "\n";
    q1.insert(&i4.e);
    cout << "The queue has now " << q1.size() << " elements.\n";
    cout << "Removing the element whose value is " << i2.i << " => " 
	 << q1.remove(&i2)->object()->i << "\n";
    cout << "Removing the queue's head => " << q1.remove()->object()->i 
	 << "\n";
    cout << "Removing the element whose value is " << i4.i << " => " 
	 << q1.remove(&i4)->object()->i << "\n";
    cout << "Removing the queue's head =>" << q1.remove()->object()->i
	 << "\n";
    cout << "The queue has now " << q1.size() << " elements.\n";


    cout << "\nThis is an ordered integer queue:\n";
    Integer2 j1(1, 2), j2(2, 3), j3(3, 4), j4(4, 1);
    Ordered_Queue<Integer2> q2;
    cout << "Inserting the integer " << j1.i 
	 << " with rank " << j1.e.rank() << ".\n";
    q2.insert(&j1.e);
    cout << "Inserting the integer " << j2.i
	 << " with rank " << j2.e.rank() << ".\n";
    q2.insert(&j2.e);
    cout << "Inserting the integer " << j3.i
	 << " with rank " << j3.e.rank() << ".\n";
    q2.insert(&j3.e);
    cout << "Inserting the integer " << j4.i
	 << " with rank " << j4.e.rank() << ".\n";
    q2.insert(&j4.e);
    cout << "The queue has now " << q2.size() << " elements.\n";
    cout << "Removing the element whose value is " << j2.i << " => " 
	 << q2.remove(&j2)->object()->i << "\n";
    cout << "Removing the queue's head => " << q2.remove()->object()->i 
	 << "\n";
    cout << "Removing the queue's head => " << q2.remove()->object()->i
	 << "\n";
    cout << "Removing the queue's head => " << q2.remove()->object()->i 
	 << "\n";
    cout << "The queue has now " << q2.size() << " elements.\n";


    cout << "\nThis is an integer queue with relative ordering:\n";
    Integer3 k1(1, 2), k2(2, 3), k3(3, 4), k4(4, 1);
    Relative_Queue<Integer3> q3;
    cout << "Inserting the integer " << k1.i 
	 << " with relative order " << k1.e.rank() << ".\n";
    q3.insert(&k1.e);
    cout << "Inserting the integer " << k2.i 
	 << " with relative order " << k2.e.rank() << ".\n";
    q3.insert(&k2.e);
    cout << "Inserting the integer " << k3.i
	 << " with relative order " << k3.e.rank() << ".\n";
    q3.insert(&k3.e);
    cout << "Inserting the integer " << k4.i
	 << " with relative order " << k4.e.rank() << ".\n";
    q3.insert(&k4.e);
    cout << "The queue has now " << q3.size() << " elements.\n";
    cout << "Removing the element whose value is " << j2.i << " => " 
	 << q3.remove(&k2)->object()->i << "\n";
    cout << "Removing the queue's head => " << q3.remove()->object()->i
	 << "\n";
    cout << "Removing the queue's head => " << q3.remove()->object()->i
	 << "\n";
    cout << "Removing the queue's head => " << q3.remove()->object()->i
	 << "\n";
    cout << "The queue has now " << q3.size() << " elements.\n";

    return 0;
}
