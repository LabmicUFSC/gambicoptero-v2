// EPOS Observer Utility Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __observer_h
#define	__observer_h

#include <utility/list.h>

__BEGIN_SYS

// Observer
class Observer;

class Observed // Subject
{ 
    friend class Observer;

private:
    typedef Simple_List<Observer>::Element Element;

protected: 
    Observed() {}

public: 
    virtual ~Observed() {}

    virtual void attach(Observer * o);
    virtual void detach(Observer * o);
    virtual void notify();

private: 
    Simple_List<Observer> _observers;
}; 

class Observer
{ 
    friend class Observed;

protected: 
    Observer(): _link(this) {} 

public: 
    virtual ~Observer() {}
    
    virtual void update(Observed * o) = 0;

private:
    Observed::Element _link;
};


// Conditionally Observed
class Conditional_Observer;

class Conditionally_Observed // Subject
{
    friend class Conditional_Observer;

private:
    typedef
    List_Elements::Singly_Linked_Ordered<Conditional_Observer> Element;

public: 
    Conditionally_Observed() {
	db<Observed>(TRC) << "Observed() => " << this << "\n";
    }

    virtual ~Conditionally_Observed() {
	db<Observed>(TRC) << "~Observed(this=" << this << ")\n";
    }

    virtual void attach(Conditional_Observer * o, int c);
    virtual void detach(Conditional_Observer * o, int c);
    virtual void notify(int c);

private: 
    Simple_List<Conditional_Observer, Element> _observers;
}; 

class Conditional_Observer
{
    friend class Conditionally_Observed;

public: 
    Conditional_Observer(): _link(this) {
	db<Observer>(TRC) << "Observer() => " << this << "\n";
    } 

    virtual ~Conditional_Observer() {
	db<Observer>(TRC) << "~Observer(this=" << this << ")\n";
    }
    
    virtual void update(Conditionally_Observed * o, int c) = 0;

private:
    Conditionally_Observed::Element _link;
};

// Conditional Data Observer
// An Observer that receives useful data
template<typename T>
class Data_Observer;

template<typename T>
class Data_Observed // Subject
{
    friend class Data_Observer<T>;

public:
    typedef Data_Observer<T> Observer;
    typedef List_Elements::Singly_Linked_Ordered<Observer> Element;

    Data_Observed() {
	db<Observed>(TRC) << "Observed() => " << this << "\n";
    }

    virtual ~Data_Observed() {
	db<Observed>(TRC) << "~Observed(this=" << this << ")\n";
    }

    virtual void attach(Observer * o, long c) {
    	db<Observed>(TRC) << "Observed::attach(o=" << o << ",c=" << c << ")\n";

		o->_link = Element(o, c);
		_observers.insert(&o->_link);
    }
    virtual void detach(Observer * o, long c) {
    	db<Observed>(TRC) << "Observed::detach(obs=" << o << ",c=" << c << ")\n";

    	_observers.remove(&o->_link);
    }
    virtual void notify(T src,T dst,long c,void * data,unsigned int size) {
        db<Observed>(TRC) << "Observed::notify(cond=" << c << ")\n";

        for(Element * e = _observers.head(); e; e = e->next()) {
		if(e->rank() == c) {
			db<Observed>(INF) << "Observed::notify(this=" << this
				  << ",obs=" << e->object() << ")\n";
			e->object()->update(this, c, src, dst, data, size);
		}
        }

    }

private:
    Simple_List<Observer, Element> _observers;
};

template<typename T>
class Data_Observer
{
public:
    friend class Data_Observed<T>;
    typedef Data_Observed<T> Observed;

    Data_Observer(): _link(this) {
	db<Observer>(TRC) << "Observer() => " << this << "\n";
    }

    virtual ~Data_Observer() {
    	db<Observer>(TRC) << "~Observer(this=" << this << ")\n";
    }

    virtual void update(Observed * o, long c,T src,T dst,void * data,unsigned int size) = 0;

private:
    typename Observed::Element _link;
};

__END_SYS
 
#endif
