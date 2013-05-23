// EPOS Observer Utility Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/debug.h>
#include <utility/malloc.h>
#include <utility/observer.h>

__BEGIN_SYS

void Observed::attach(Observer * o)
{
    db<Observed>(TRC) << "Observed::attach(obs=" << o << ")\n";

    _observers.insert(&o->_link);
} 

void Observed::detach(Observer * o)
{
    db<Observed>(TRC) << "Observed::detach(obs=" << o << ")\n";

    _observers.remove(&o->_link); 
}

void Observed::notify()
{
    db<Observed>(TRC) << "Observed::notify()\n";

    for(Element * e = _observers.head(); e; e = e->next()) {
	db<Observed>(INF) << "Observed::notify(this=" << this
			  << ",obs=" << e->object() << ")\n";
	
	e->object()->update(this);
    }
} 

void Conditionally_Observed::attach(Conditional_Observer * o, int c)
{
    db<Observed>(TRC) << "Observed::attach(o=" << o << ",c=" << c << ")\n";

    o->_link = Element(o, c);
    _observers.insert(&o->_link);
} 

void Conditionally_Observed::detach(Conditional_Observer * o, int c)
{
    db<Observed>(TRC) << "Observed::detach(obs=" << o << ",c=" << c << ")\n";

    _observers.remove(&o->_link); 
}

void Conditionally_Observed::notify(int c)
{
    db<Observed>(TRC) << "Observed::notify(cond=" << c << ")\n";
    
    for(Element * e = _observers.head(); e; e = e->next()) {
	db<Observed>(INF) << "Observed::notify(this=" << this
			  << ",obs=" << e->object() << ")\n";
	
	if(e->rank() == c)
	    e->object()->update(this, c);
    }
}

__END_SYS
 
