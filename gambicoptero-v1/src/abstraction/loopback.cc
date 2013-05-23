#include <loopback.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <utility/string.h>


__BEGIN_SYS

int Loopback::send(const Address& dst,const Protocol& prot,
                   const char * data,unsigned int size)
{
    _lock.lock();
    if (size > MTU)
        size = MTU;
    memcpy(_data,data,size);
    _stats.tx_bytes += size;
    _stats.tx_packets++;
    _prot = prot;
    _size = size;
    _lock.unlock();
    return size;
}

int Loopback::receive(Address * src,Protocol * prot,
                      char * data,unsigned int size)
{
    _lock.lock();
    while (_size == 0) {
        _lock.unlock();
        Thread::yield();
        _lock.lock();
    }
    _lock.lock();
    if (_size < size) size = _size;
    memcpy(data,_data,size);
    _stats.rx_bytes += size;
    _stats.rx_packets++;
    *src = _unit;
    *prot = _prot;
    _size = 0;
    _lock.unlock();
    return size;
}

void Loopback::reset() {
    _lock.lock();
    _stats.rx_bytes = 0;
    _stats.rx_packets = 0;
    _stats.tx_bytes = 0;
    _stats.tx_packets = 0;
    _size = 0;
    _lock.unlock();
}


__END_SYS
