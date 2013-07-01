// EPOS Application Scaffold and Application Abstraction Implementation

#include <utility/ostream.h>
#include <application.h>

__BEGIN_SYS

// Global objects
OStream cout;
OStream cerr;

// Application class attributes
Heap Application::_heap[IF_INT<Traits<Heap>::priority_alloc, 2, 1>::Result];

__END_SYS
