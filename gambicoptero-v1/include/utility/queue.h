// EPOS Queue Utility Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

// Queue is a traditional queue, with insertions at the tail
// and removals either from the head or from specific objects 

// Ordered_Queue is an ordered queue, i.e. objects are inserted
// in-order based on the integral value of "element.rank". Note that "rank"
// implies an order, but does not necessarily need to be "the absolute order"
// in the queue; it could, for instance, be a priority information or a 
// time-out specification. Insertions must first tag "element" with "rank".
// Removals are just like in the traditional queue. Elements of both Queues
// may be exchanged.
// Example: insert(B,7);insert(C,9);insert(A,4)
// +---+		+---+	+---+	+---+
// |obj|		| A |-->| B |-->| C |
// + - +       head -->	+ - +	+ - +	+ - + <-- tail
// |ord|		| 4 |<--| 7 |<--| 9 |
// +---+ 		+---+	+---+	+---+

// Relative Queue is an ordered queue, i.e. objects are inserted
// in-order based on the integral value of "element.rank" just like above.
// But differently from that, a Relative Queue handles "rank" as relative 
// offsets. This is very useful for alarm queues. Elements of Relative Queue
// cannot be exchanged with elements of the other queues described earlier.
// Example: insert(B,7);insert(C,9);insert(A,4)
// +---+		+---+	+---+	+---+
// |obj|		| A |-->| B |-->| C |
// + - +       head -->	+ - +	+ - +	+ - + <-- tail
// |ord|		| 4 |<--| 3 |<--| 2 |
// +---+ 		+---+	+---+	+---+

// Scheduling Queue is an ordered queue whose ordering criterion is externally
// definable and for which selecting methods are defined (e.g. choose). This
// utility is most useful for schedulers, such as CPU or I/O.

#ifndef __queue_h
#define	__queue_h

#include <cpu.h>
#include "list.h"
#include "spin.h"

__BEGIN_SYS

// Wrapper for non-atomic queues  
template <typename List, bool atomic>
class Queue_Wrapper: private List
{
public:
    typedef typename List::Object_Type Object_Type;
    typedef typename List::Element Element;

public:
    void lock() { }
    void unlock() { }

    bool empty() { return List::empty(); }
    unsigned int size() { return List::size(); }

    Element * head() { return List::head(); }
    Element * tail() { return List::tail(); }

    void insert(Element * e) { List::insert(e); }

    Element * remove() { return List::remove(); }
    Element * remove(Element * e) { return List::remove(e); }
    Element * remove(const Object_Type * obj) { return List::remove(obj); }

    Element * search(const Object_Type * obj) {	return List::search(obj); }

    Element * volatile & chosen() { return List::chosen(); }

    Element * choose() { return List::choose(); }
    Element * choose_another() { return List::choose_another(); }
    Element * choose(Element * e) { return List::choose(e); }
    Element * choose(const Object_Type * obj) {	return List::choose(obj); }
};

// Wrapper for atomic queues  
template <typename List>
class Queue_Wrapper<List, true>: private List
{
private:
    static const bool smp = Traits<Thread>::smp;

public:
    typedef typename List::Object_Type Object_Type;
    typedef typename List::Element Element;

public:
    void lock() { _lock.acquire(); }
    void unlock() { _lock.release(); }

    bool empty() {
	enter();
	bool tmp = List::empty();
	leave();
	return tmp;
    }

    unsigned int size() {
	enter(); 
	unsigned int tmp = List::size();
	leave();
	return tmp;
    }

    Element * head() { 
	enter(); 
	Element * tmp = List::head();
	leave();
	return tmp;
    }

    Element * tail() { 
	enter(); 
	Element * tmp = List::tail();
	leave();
	return tmp;
    }

    void insert(Element * e) { 
	enter(); 
	List::insert(e);
	leave();
    }

    Element * remove() { 
	enter(); 
	Element * tmp = List::remove();
	leave();
	return tmp;
    }

    Element * remove(const Object_Type * obj) {
	enter(); 
	Element * tmp = List::remove(obj); 
	leave();
	return tmp;
    }

    Element * search(const Object_Type * obj) {
	enter(); 
	Element * tmp = List::search(obj);
	leave();
	return tmp;
    }

    Element * volatile & chosen() { 
	enter(); 
	Element * volatile & tmp = List::chosen();
	leave();
	return tmp;
    }

    Element * choose() { 
	enter(); 
	Element * tmp = List::choose();
	leave();
	return tmp;
    }

    Element * choose_another() { 
	enter(); 
	Element * tmp = List::choose_another();
	leave();
	return tmp;
    }
    Element * choose(Element * e) {
	enter(); 
	Element * tmp = List::choose(e);
	leave();
	return tmp;
    }

    Element * choose(const Object_Type * obj) {
	enter(); 
	Element * tmp = List::choose(obj);
	leave();
	return tmp;
    }

private:
    void enter() {
	CPU::int_disable();
	if(smp) _lock.acquire(); 
    }

    void leave() {
	if(smp) _lock.release();
	CPU::int_disable();
    }

private:
    Spin _lock;
};


// Queue
template <typename T,
	  bool atomic = false,
	  typename El = List_Elements::Doubly_Linked<T> >
class Queue: public Queue_Wrapper<List<T, El>, atomic> {};


// Ordered Queue
template <typename T, 
	  typename R = List_Element_Rank, 
	  bool atomic = false,
	  typename El = List_Elements::Doubly_Linked_Ordered<T, R> >
class Ordered_Queue:
    public Queue_Wrapper<Ordered_List<T, R, El>, atomic> {};


// Relatively-Ordered Queue
template <typename T, 
	  typename R = List_Element_Rank, 
	  bool atomic = false,
	  typename El = List_Elements::Doubly_Linked_Ordered<T, R> >
class Relative_Queue:
    public Queue_Wrapper<Relative_List<T, R, El>, atomic> {};

__END_SYS
 
#endif
