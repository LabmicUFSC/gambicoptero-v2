#ifndef ___DummyUartDevice___
#define ___DummyUartDevice___

class DummyUartDevice 
{
public:
  DummyUartDevice(char* port_name) {}

  ~DummyUartDevice() {}

  int send( unsigned char *buffer, int size ) { return 0; }

  int receive( unsigned char *buffer, int size ) { return 0; }

};

#endif
