// EPOS Vector Utility Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <utility/malloc.h>
#include <utility/vector.h>

__USING_SYS;

const int N = 10;

OStream cout;

int main()
{

    cout << "Vector Utility Test\n";

    cout << "\nThis is a vector of integers:\n";
    Vector<int, N> v;
    int o[N];
    Vector<int, N>::Element * e[N];
    cout << "Inserting the following integers into the vector ";
    for(int i = 0; i < N; i++) {
	o[i] = i;
	e[i] = new Vector<int, N>::Element(&o[i]);
	v.insert(e[i], i);
	cout << "[" << i << "]=" << i;
	if(i != N - 1)
	    cout << ", ";
    }
    cout << "\n";

    cout << "The vector has now " << v.size() << " elements:\n";
    for(int i = 0; i < N; i++) {
	cout << "[" << i << "]=" << *v.get(i)->object();
	if(i != N - 1)
	    cout << ", ";
    }
    cout << "\n";

    for(int i = 0; i < N; i++)
	(*v.get(i)->object())++;
    cout << "The vector's elements were incremented and are now:\n";
    for(int i = 0; i < N; i++) {
	cout << "[" << i << "]=" << *v.get(i)->object();
	if(i != N - 1)
	    cout << ", ";
    }
    cout << "\n";

    cout << "Removing the element whose value is " << o[N/2] << " => " 
	 << *v.remove(&o[N/2])->object() << "\n";
    cout << "Removing the second element => " << *v.remove(1)->object()
	 << "\n";
    cout << "Removing the element whose value is " << o[N/4] << " => " 
	 << *v.remove(&o[N/4])->object() << "\n";
    cout << "Removing the last element => " << *v.remove(N - 1)->object()
	 << "\n";
    cout << "Trying to remove an element that is not on the vector => " 
	 << v.remove(&o[N/2]) << "\n";
    cout << "Removing all remaining elements => ";
    for(int i = 0; i < N; i++) {
	cout << *v.remove(i)->object();
	if(i != N - 1)
	    cout << ", ";
    }
    cout << "\n";
    cout << "The vector has now " << v.size() << " elements\n";
    for(int i = 0; i < N; i++)
	delete e[i];

    cout << "\nDone!\n";

    return 0;
}
