// EPOS List Utility Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <utility/malloc.h>
#include <utility/list.h>

__USING_SYS;

const int N = 10;

void test_simple_list();
void test_list();
void test_ordered_list();
void test_relative_list();
void test_scheduling_list();
void test_grouping_list();
void test_simple_grouping_list();

OStream cout;

int main()
{

    cout << "List Utility Test\n";

    test_simple_list();
    test_simple_grouping_list();
    test_list();
    test_ordered_list();
    test_relative_list();
    test_scheduling_list();
    test_grouping_list();

    cout << "\nDone!\n";

    return 0;
}

void test_simple_list ()
{
    cout << "\nThis is a singly-linked list of integers:\n";
    Simple_List<int> l;
    int o[N];
    Simple_List<int>::Element * e[N];
    cout << "Inserting the following integers into the list ";
    for(int i = 0; i < N; i++) {
	o[i] = i;
	e[i] = new Simple_List<int>::Element(&o[i]);
	l.insert(e[i]);
	cout << i;
	if(i != N - 1)
	    cout << ", ";
    }
    cout << "\n";
    cout << "The list has now " << l.size() << " elements\n";
    cout << "They are: ";
    for(Simple_List<int>::Iterator i = l.begin(); i != l.end(); i++) {
	cout << *i->object();
	if(Simple_List<int>::Iterator(i->next()) != l.end())
	    cout << ", ";
    }
    cout << "\n";
    cout << "Removing the element whose value is " << o[N/2] << " => " 
	 << *l.remove(&o[N/2])->object() << "\n";
    cout << "Removing the list's head => " << *l.remove_head()->object()
	 << "\n";
    cout << "Removing the element whose value is " << o[N/4] << " => " 
	 << *l.remove(&o[N/4])->object() << "\n";
    cout << "Removing the list's tail => " << *l.remove_tail()->object()
	 << "\n";
    cout << "Trying to remove an element that is not on the list => " 
	 << l.remove(&o[N+1]) << "\n";
    cout << "Removing all remaining elements => ";
    while(l.size() > 0) {
	cout << *l.remove()->object();
	if(l.size() > 0)
	    cout << ", ";
    }
    cout << "\n";
    cout << "The list has now " << l.size() << " elements\n";
    for(int i = 0; i < N; i++)
	delete e[i];
}

void test_simple_grouping_list() 
{
    cout << "\nThis is a simple grouping list of integers:\n";
    Simple_Grouping_List<int> l;
    int o[N * 2];
    Simple_Grouping_List<int>::Element * e[N * 2];
    Simple_Grouping_List<int>::Element * d1 = 0, * d2 = 0;
    cout << "Inserting the following group of integers into the list ";
    for(int i = 0; i < N * 2; i += 4) {
	o[i] = i;
	o[i + 1] = i + 1;
	e[i] = new Simple_Grouping_List<int>::Element(&o[i], 2);
	l.insert_merging(e[i], &d1, &d2);
	cout << i << "(2), ";
	if(d1) {
	    cout << "[nm]"; // next merged
	    delete d1;
	}
	if(d2) {
	    cout << "[tm]"; // this merged
	    delete d2;
	}
    }
    for(int i = 2; i < N * 2; i += 4) {
	o[i] = i;
	o[i + 1] = i + 1;
	e[i] = new Simple_Grouping_List<int>::Element(&o[i], 2);
	l.insert_merging(e[i], &d1, &d2);
	cout << i << "(2)";
	if(d1) {
	    cout << "[nm]"; // next merged
	    delete d1;
	}
	if(d2) {
	    cout << "[tm]"; // this merged
	    delete d2;
	}
	if(i < (N - 1) * 2)
	    cout << ", ";
    }
    cout << "\n";
    cout << "The list has now " << l.size() << " elements that group " 
	 << l.grouped_size() << " elements in total\n";
    cout << "They are: ";
    for(Simple_Grouping_List<int>::Iterator i = l.begin(); i != l.end(); i++) {
	cout << *i->object();
	if(Simple_Grouping_List<int>::Iterator(i->next()) != l.end())
	    cout << ", ";
    }
    cout << "\n";
    cout << "Allocating 1 element from the list => "; 
    d1 = l.search_decrementing(1);
    if(d1) {
	cout << *(d1->object() + d1->size()) << "\n";
	if(!d1->size()) {
	    cout << "[rm]"; // removed
	    delete d1;
	}
    } else
	cout << "failed!\n";
    cout << "Allocating 6 more elements from the list => "; 
    d1 = l.search_decrementing(6);
    if(d1) {
	cout << *(d1->object() + d1->size());
	if(!d1->size()) {
	    cout << "[rm]"; // removed
	    delete d1;
	}
	cout << "\n";
    } else
	cout << "failed!\n";
    cout << "Allocating " << N * 2 << " more elements from the list => "; 
    d1 = l.search_decrementing(N * 2);
    if(d1) {
	cout << *(d1->object() + d1->size());
	if(!d1->size()) {
	    cout << "[rm]"; // removed
	    delete d1;
	}
	cout << "\n";
    } else
	cout << "failed!\n";
    cout << "Allocating " << (N * 2)-7 << " more elements from the list => "; 
    d1 = l.search_decrementing((N * 2) - 7);
    if(d1) {
	cout << *(d1->object() + d1->size());
	if(!d1->size()) {
	    cout << "[r]"; // removed
	    delete d1;
	}
	cout << "\n";
    } else
	cout << "failed!\n";
    cout << "The list has now " << l.size() << " elements that group " 
	 << l.grouped_size() << " elements in total\n";
}

void test_list ()
{
    cout << "\nThis is a doubly-linked list of integers:\n";
    List<int> l;
    int o[N];
    List<int>::Element * e[N];
    cout << "Inserting the following integers into the list ";
    for(int i = 0; i < N; i++) {
	o[i] = i;
	e[i] = new List<int>::Element(&o[i]);
	l.insert(e[i]);
	cout << i;
	if(i != N - 1)
	    cout << ", ";
    }
    cout << "\n";
    cout << "The list has now " << l.size() << " elements\n";
    cout << "They are: ";
    for(List<int>::Iterator i = l.begin(); i != l.end(); i++) {
	cout << *i->object();
	if(List<int>::Iterator(i->next()) != l.end())
	    cout << ", ";
    }
    cout << "\n";
    cout << "Removing the element whose value is " << o[N/2] << " => " 
	 << *l.remove(&o[N/2])->object() << "\n";
    cout << "Removing the list's head => " << *l.remove_head()->object()
	 << "\n";
    cout << "Removing the element whose value is " << o[N/4] << " => " 
	 << *l.remove(&o[N/4])->object() << "\n";
    cout << "Removing the list's tail => " << *l.remove_tail()->object()
	 << "\n";
    cout << "Trying to remove an element that is not on the list => " 
	 << l.remove(&o[N+1]) << "\n";
    cout << "Removing all remaining elements => ";
    while(l.size() > 0) {
	cout << *l.remove()->object();
	if(l.size() > 0)
	    cout << ", ";
    }
    cout << "\n";
    cout << "The list has now " << l.size() << " elements\n";
    for(int i = 0; i < N; i++)
	delete e[i];
}

void test_ordered_list ()
{
    cout << "\nThis is an ordered, linked list of integers:\n";
    Ordered_List<int> l;
    int o[N];
    Ordered_List<int>::Element * e[N];
    cout << "Inserting the following integers into the list ";
    for(int i = 0; i < N; i++) {
	o[i] = i;
	e[i] = new Ordered_List<int>::Element(&o[i], N - i - 1);
	l.insert(e[i]);
	cout << i << "(" << N - i - 1 << ")";
	if(i != N - 1)
	    cout << ", ";
    }
    cout << "\n";
    cout << "The list has now " << l.size() << " elements\n";
    cout << "They are: ";
    for(Ordered_List<int>::Iterator i = l.begin(); i != l.end(); i++) {
	cout << *i->object();
	if(Ordered_List<int>::Iterator(i->next()) != l.end())
	    cout << ", ";
    }
    cout << "\n";
    cout << "Removing the element whose value is " << o[N/2] << " => " 
	 << *l.remove(&o[N/2])->object() << "\n";
    cout << "Removing the list's head => " << *l.remove_head()->object()
	 << "\n";
    cout << "Removing the element whose value is " << o[N/4] << " => " 
	 << *l.remove(&o[N/4])->object() << "\n";
    cout << "Removing the list's tail => " << *l.remove_tail()->object()
	 << "\n";
    cout << "Trying to remove an element that is not on the list => " 
	 << l.remove(&o[N+1]) << "\n";
    cout << "Removing all remaining elements => ";
    while(l.size() > 0) {
	cout << *l.remove()->object();
	if(l.size() > 0)
	    cout << ", ";
    }
    cout << "\n";
    cout << "The list has now " << l.size() << " elements\n";
    for(int i = 0; i < N; i++)
	delete e[i];
}

void test_relative_list ()
{
    cout << "\nThis is a realtive ordered, linked list of integers:\n";
    Relative_List<int> l;
    int o[N];
    Relative_List<int>::Element * e[N];
    cout << "Inserting the following integers into the list ";
    for(int i = 0; i < N; i++) {
	o[i] = i;
	e[i] = new Relative_List<int>::Element(&o[i], N - i - 1);
	l.insert(e[i]);
	cout << i << "(" << N - i - 1 << ")";
	if(i != N - 1)
	    cout << ", ";
    }
    cout << "\n";
    cout << "The list has now " << l.size() << " elements\n";
    cout << "They are: ";
    for(Relative_List<int>::Iterator i = l.begin(); i != l.end(); i++) {
	cout << *i->object();
	if(Relative_List<int>::Iterator(i->next()) != l.end())
	    cout << ", ";
    }
    cout << "\n";
    cout << "Removing the element whose value is " << o[N/2] << " => " 
	 << *l.remove(&o[N/2])->object() << "\n";
    cout << "Removing the list's head => " << *l.remove_head()->object()
	 << "\n";
    cout << "Removing the element whose value is " << o[N/4] << " => " 
	 << *l.remove(&o[N/4])->object() << "\n";
    cout << "Removing the list's tail => " << *l.remove_tail()->object()
	 << "\n";
    cout << "Trying to remove an element that is not on the list => " 
	 << l.remove(&o[N+1]) << "\n";
    cout << "Removing all remaining elements => ";
    while(l.size() > 0) {
	cout << *l.remove()->object();
	if(l.size() > 0)
	    cout << ", ";
    }
    cout << "\n";
    cout << "The list has now " << l.size() << " elements\n";
    for(int i = 0; i < N; i++)
	delete e[i];
}

void test_scheduling_list ()
{
    cout << "\nThis is scheduling list of integers:\n";
    Scheduling_List<int> l;
    int o[N];
    Scheduling_List<int>::Element * e[N];
    cout << "Inserting the following integers into the list ";
    for(int i = 0; i < N; i++) {
	o[i] = i;
	e[i] = new Scheduling_List<int>::Element(&o[i], N - i - 1);
	l.insert(e[i]);
	cout << i << "(" << N - i - 1 << ")";
	if(i != N - 1)
	    cout << ", ";
    }
    cout << "\n";
    cout << "The list has now " << l.size() << " elements\n";
    cout << "They are: ";
    for(Scheduling_List<int>::Iterator i = l.begin(); i != l.end(); i++) {
	cout << *i->object();
	if(Scheduling_List<int>::Iterator(i->next()) != l.end())
	    cout << ", ";
    }
    cout << "\n";
    cout << "Scheduling the list => " << *l.choose()->object() << "\n";
    cout << "They are: ";
    for(Scheduling_List<int>::Iterator i = l.begin(); i != l.end(); i++) {
	cout << *i->object();
	if(Scheduling_List<int>::Iterator(i->next()) != l.end())
	    cout << ", ";
    }
    cout << "\n";
    cout << "Forcing scheduling of antorher element => " <<
	*l.choose_another()->object() << "\n";
    cout << "They are: ";
    for(Scheduling_List<int>::Iterator i = l.begin(); i != l.end(); i++) {
	cout << *i->object();
	if(Scheduling_List<int>::Iterator(i->next()) != l.end())
	    cout << ", ";
    }
    cout << "\n";
    cout << "Forcing scheduling of element whose value is " << o[N/2] << " => " 
 	 << *l.choose(e[N/2])->object() << "\n";
    cout << "They are: ";
    for(Scheduling_List<int>::Iterator i = l.begin(); i != l.end(); i++) {
	cout << *i->object();
	if(Scheduling_List<int>::Iterator(i->next()) != l.end())
	    cout << ", ";
    }
    cout << "\n";
    cout << "Removing the list's head => " << *l.remove(l.choose())->object()
	 << "\n";
    cout << "Removing the element whose value is " << o[N/4] << " => " 
	 << *l.remove(e[N/4])->object() << "\n";
    cout << "Removing all remaining elements => ";
    while(l.size() > 0) {
	cout << *l.remove(l.choose())->object();
	if(l.size() > 0)
	    cout << ", ";
    }
    cout << "\n";
    cout << "The list has now " << l.size() << " elements\n";
    for(int i = 0; i < N; i++)
	delete e[i];
}

void test_grouping_list() 
{
    cout << "\nThis is a grouping list of integers:\n";
    Grouping_List<int> l;
    int o[N * 2];
    Grouping_List<int>::Element * e[N * 2];
    Grouping_List<int>::Element * d1 = 0, * d2 = 0;
    cout << "Inserting the following group of integers into the list ";
    for(int i = 0; i < N * 2; i += 4) {
	o[i] = i;
	o[i + 1] = i + 1;
	e[i] = new Grouping_List<int>::Element(&o[i], 2);
	l.insert_merging(e[i], &d1, &d2);
	cout << i << "(2), ";
	if(d1) {
	    cout << "[nm]"; // next merged
	    delete d1;
	}
	if(d2) {
	    cout << "[tm]"; // this merged
	    delete d2;
	}
    }
    for(int i = 2; i < N * 2; i += 4) {
	o[i] = i;
	o[i + 1] = i + 1;
	e[i] = new Grouping_List<int>::Element(&o[i], 2);
	l.insert_merging(e[i], &d1, &d2);
	cout << i << "(2)";
	if(d1) {
	    cout << "[nm]"; // next merged
	    delete d1;
	}
	if(d2) {
	    cout << "[tm]"; // this merged
	    delete d2;
	}
	if(i < (N - 1) * 2)
	    cout << ", ";
    }
    cout << "\n";
    cout << "The list has now " << l.size() << " elements that group " 
	 << l.grouped_size() << " elements in total\n";
    cout << "They are: ";
    for(Grouping_List<int>::Iterator i = l.begin(); i != l.end(); i++) {
	cout << *i->object();
	if(Grouping_List<int>::Iterator(i->next()) != l.end())
	    cout << ", ";
    }
    cout << "\n";
    cout << "Allocating 1 element from the list => "; 
    d1 = l.search_decrementing(1);
    if(d1) {
	cout << *(d1->object() + d1->size()) << "\n";
	if(!d1->size()) {
	    cout << "[rm]"; // removed
	    delete d1;
	}
    } else
	cout << "failed!\n";
    cout << "Allocating 6 more elements from the list => "; 
    d1 = l.search_decrementing(6);
    if(d1) {
	cout << *(d1->object() + d1->size());
	if(!d1->size()) {
	    cout << "[rm]"; // removed
	    delete d1;
	}
	cout << "\n";
    } else
	cout << "failed!\n";
    cout << "Allocating " << N * 2 << " more elements from the list => "; 
    d1 = l.search_decrementing(N * 2);
    if(d1) {
	cout << *(d1->object() + d1->size());
	if(!d1->size()) {
	    cout << "[rm]"; // removed
	    delete d1;
	}
	cout << "\n";
    } else
	cout << "failed!\n";
    cout << "Allocating " << (N * 2)-7 << " more elements from the list => "; 
    d1 = l.search_decrementing((N * 2) - 7);
    if(d1) {
	cout << *(d1->object() + d1->size());
	if(!d1->size()) {
	    cout << "[r]"; // removed
	    delete d1;
	}
	cout << "\n";
    } else
	cout << "failed!\n";
    cout << "The list has now " << l.size() << " elements that group " 
	 << l.grouped_size() << " elements in total\n";
}
