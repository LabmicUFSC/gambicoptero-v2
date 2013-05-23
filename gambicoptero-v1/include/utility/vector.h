// EPOS Vector Utility Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __vector_h
#define	__vector_h

#include "list.h"

__BEGIN_SYS

// Vector
template <typename T, unsigned int SIZE, 
	  typename El = List_Elements::Pointer<T> >
class Vector
{
public:
    typedef T Object_Type;
    typedef El Element;

public:
    Vector(): _size(0) {
	for(unsigned int i = 0; i < SIZE; i++)
	    _vector[i] = 0;
    }

    bool empty() const { return (_size == 0); }
    unsigned int size() const { return _size; }

    Element * get(int i) const { return _vector[i]; }

    bool insert(Element * e, unsigned int i) {
	if(_vector[i])
	    return false;
	
	_vector[i] = e;
	_size++;
	return true;
    }

    Element * remove(unsigned int i) {
	if(_vector[i]) {
	    Element * e = _vector[i];
	    _vector[i] = 0;
	    _size--;
	    return e;
	}
	return 0;
    }
    Element * remove(Element * e) {
	for(int i = 0; i < SIZE; i++)
	    if(_vector[i] == e) {
		_vector[i] = 0;
		_size--;
		return e;
	    }
	return 0;
    }
    Element * remove(const Object_Type * obj) {
	for(int i = 0; i < SIZE; i++)
	    if(_vector[i]->object() == obj) {
		Element * e = _vector[i];
		_vector[i] = 0;
		_size--;
		return e;
	    }
	return 0;
    }
    
    Element * search(const Object_Type * obj) {
	for(int i = 0; i < SIZE; i++)
	    if(_vector[i]->object() == obj)
		return _vector[i];
	return 0;
    }
    
private:
    unsigned int _size;
    Element * _vector[SIZE];
};

__END_SYS
 
#endif
