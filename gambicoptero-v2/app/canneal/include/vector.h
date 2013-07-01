/* This is a simple STD vector interface that uses EPOS Vector class.
 * Only the methods used by canneal are implemented.
 * Author: Giovani Gracioli <giovani@lisha.ufsc.br>
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <utility/vector.h>
#include <utility/malloc.h>
#include "epos_wrappers.h"
#include <thread.h>

#define VECTOR_SIZE 100

extern System::OStream cout;

// vector
template <typename T>
class vector
{
public:
    typedef System::Vector<T,VECTOR_SIZE> Object_Type;
    typedef typename Object_Type::Element Element;
public:
    vector(): _current_index(0) {}
    
    vector(unsigned int n): _current_index(0) {
        //T* v;
        //Element *e;
        if(n > VECTOR_SIZE) {
            cout << "new vector size (" << n << ") is greater than the old one (" << VECTOR_SIZE << ")\n";
            return;         
        }
        /*for(unsigned int i = 0; i < n; i++) {
            v = (T *) malloc(sizeof(T));
            e = new Element(v);
            _v.insert(e, i);
        }*/
    }
    
    T& operator[](const unsigned int i) { 
        return at(i); 
    }
    
    T& at(const unsigned int i) {
        T *tmp;
        
        if(i > _v.size() || i >= VECTOR_SIZE) {
            cout << "********PROBLEM vector AT [" << i << "] size = " << _v.size() << " Thread =  " << System::Thread::self() << "\n";
            return *(_v.get(i)->object());
        }
        
        Element *e = _v.get(i);
        if(e == 0) {
            //cout << "no element found in " << i << endl;
            //v = (T *) malloc(sizeof(T));
            //cout << "vector AT [" << i << "] new -> ";
            tmp = new T();
            e = new Element(tmp);
            _v.insert(e, i);         
            //cout << "vector at(" << i << ") inserted new size = " << _v.size() << endl;
        } else { 
            //cout << "vector at(" << i << ") retrieved size = " << _v.size() << endl;
            tmp = e->object();
        }
        T &value = *tmp;
        //cout << "VECTOR reference = " << (void *) v << endl;
        return value;
    }
    
    void push_back ( T x ) {
        if(_v.size() >= VECTOR_SIZE-1) {
            cout << "push_back() vector is FULL\n";
        }  
        T *value = new T();
        *value = x;
        Element *e = new Element(value);
        //cout << "vector inserting at [" << _v.size() << "] elem = " << x->item_name << endl;
        _v.insert(e, _v.size());
    }
    
    void resize ( unsigned int s, T c = T()) {
        cout << "vector::resize(): current vector size = " << VECTOR_SIZE << " new size = " << s << "\n";
    }
    
    int size() { return _v.size(); }
    
private:
    int _current_index;
    System::Vector<T,VECTOR_SIZE> _v;
};

#endif
