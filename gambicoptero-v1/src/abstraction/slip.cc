#include <slip.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <utility/debug.h>

__BEGIN_SYS

int SLIP::send(const Address& dst,const Protocol& prot,
             const char * data,unsigned int size)
{
    if (prot != IP) {
        db<SLIP>(ERR) << "SLIP cannot be used with protocols other than IP\n";
        return -1;
    }
    _tx_lock.lock();
    if (size > MTU)
        size = MTU;

    _uart.put(END);
    _stats.tx_bytes++;

    unsigned len = size;
    while(len--) {
        switch(*data) {
        case END: {
            _uart.put(ESC);
            _uart.put(ESC_END);
            _stats.tx_bytes+=2;
        }
        break;

        case ESC: {
            _uart.put(ESC);
            _uart.put(ESC_ESC);
            _stats.tx_bytes+=2;
        }
        break;

        default: {
            _uart.put(*data);
            _stats.tx_bytes++;
        }
        }
        data++;
    }
    _uart.put(END);
    _stats.tx_bytes++;
    _stats.tx_packets++;

    _tx_lock.unlock();
    return size;
}

int SLIP::receive(Address * src,Protocol * prot,
                char * data,unsigned int size)
{
    _rx_lock.lock();
    char c;
    unsigned int received = 0;

    *src = 0;
    *prot = IP;

    while(1) {
        c = _uart.get();
        _stats.rx_bytes++;
        switch(c) {
        case END:
            if(received) {
                _stats.rx_packets++;
                _rx_lock.unlock();
                return received;
            }
            break;

        case ESC:
            c = _uart.get();
            _stats.rx_bytes++;
            switch(c) {
            case ESC_END:
                c = END;
                break;
            case ESC_ESC:
                c = ESC;
                break;
            }
        default:
            if(received < size)
                data[received++] = c;

        }
    }
    return -1;
}

__END_SYS
