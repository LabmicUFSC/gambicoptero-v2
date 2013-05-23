// EPOS Debug Utility Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __debug_h
#define __debug_h 

#include <utility/ostream.h>

__BEGIN_SYS

class Debug
{
public:
    template<typename T>
    Debug & operator<<(T p){
    	kerr << p; return *this;
    }
}; 

class Null_Debug
{
public:
    template<class T>
    Null_Debug & operator<<(const T & o) { return *this; }

    template<class T>
    Null_Debug & operator<<(const T * o) { return *this; }
};

template <bool debugged>
class Select_Debug: public Debug {};
template <>
class Select_Debug<false>: public Null_Debug {};

// Error
enum Debug_Error {ERR = 1};

template <typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::error)> 
db(Debug_Error l)
{
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::error)>(); 
}

template <typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged)
		     && Traits<Debug>::error)> 
db(Debug_Error l)
{
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged)
			 && Traits<Debug>::error)>(); 
}

// Warning
enum Debug_Warning {WRN = 2};

template <typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::warning)> 
db(Debug_Warning l)
{
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::warning)>(); 
}

template <typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged)
		     && Traits<Debug>::warning)> 
db(Debug_Warning l)
{
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged)
			 && Traits<Debug>::warning)>(); 
}

// Info
enum Debug_Info {INF = 3};

template <typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::info)> 
db(Debug_Info l)
{
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::info)>(); 
}

template <typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged)
		     && Traits<Debug>::info)> 
db(Debug_Info l)
{
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged)
			 && Traits<Debug>::info)>(); 
}

// Trace
enum Debug_Trace {TRC = 4};

template <typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::trace)> 
db(Debug_Trace l)
{
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::trace)>(); 
}

template <typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged)
		     && Traits<Debug>::trace)> 
db(Debug_Trace l)
{
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged)
			 && Traits<Debug>::trace)>(); 
}

union Debug_Level
{
    Debug_Error err;
    Debug_Warning wrn;
    Debug_Info inf;
    Debug_Trace trc;
};
__END_SYS

#endif

