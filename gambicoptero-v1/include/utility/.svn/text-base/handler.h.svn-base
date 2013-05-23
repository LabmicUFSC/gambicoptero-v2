// EPOS Handler Utility Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __handler_h
#define __handler_h

#include <system/config.h>

__BEGIN_SYS

class Handler
{
public:
    // A handler function
    typedef void (Function)();

public:
    Handler() {}
    virtual ~Handler() {}

    virtual void operator()() = 0;
    void operator delete(void * object) {}
};

class Function_Handler: public Handler
{
public:
    Function_Handler(Function * h): _handler(h) {}
    ~Function_Handler() {}

    void operator()() { _handler(); }

private:
    Function * _handler;
};

template<typename T>
class Functor_Handler: public Handler
{
public:
    typedef void (Functor)(T *);

	Functor_Handler(Functor * h, T * p): _handler(h), _ptr(p) {}
    ~Functor_Handler() {}

    void operator()() { _handler(_ptr); }

private:
    Functor * _handler;
    T * _ptr;
};

__END_SYS

#endif
