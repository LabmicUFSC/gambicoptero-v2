// EPOS Network Interface Mediator Common Package

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __nic_h
#define __nic_h

#include <cpu.h>
#include <utility/observer.h>
#include <utility/crc.h>

__BEGIN_SYS

class NIC_Common
{
protected:
    NIC_Common() {}

public:
    // NIC physical address (e.g. MAC)
    template<unsigned int LENGTH>
    class Address {
    public:
	Address() {}

	Address(unsigned char a0, unsigned char a1 = 0,
		unsigned char a2 = 0, unsigned char a3 = 0,
		unsigned char a4 = 0, unsigned char a5 = 0,
		unsigned char a6 = 0, unsigned char a7 = 0)
	{
	    _address[0] =  a0;
	    if(LENGTH > 1) _address[1] = a1;
	    if(LENGTH > 2) _address[2] = a2;
	    if(LENGTH > 3) _address[3] = a3;
	    if(LENGTH > 4) _address[4] = a4;
	    if(LENGTH > 5) _address[5] = a5;
	    if(LENGTH > 6) _address[6] = a6;
	    if(LENGTH > 7) _address[7] = a7;
	}

	operator bool() { 
		unsigned int i;
		for (i=0;i<LENGTH;++i) {
			if (_address[i] != 0)
				return true;
		}
		return false;
	}

	bool operator ==(const Address & a) const { 
		unsigned int i;
		for (i=0;i<LENGTH;++i) {
			if (_address[i] != a._address[i])
				return false;
		}
		return true;
	}

    friend OStream & operator << (OStream & cout, const Address & a) {
	    cout << hex;
	    for(unsigned int i = 0; i < LENGTH; i++) {
		cout  << (unsigned int)(a._address[i]);
		if(i < (LENGTH - 1))
            cout << ":";
	    }
	    cout << dec;
        return cout;
	}


	friend Debug & operator << (Debug & db, const Address & a) {
	    db << hex;
	    for(unsigned int i = 0; i < LENGTH; i++) {
		db << (unsigned int)(a._address[i]);
		if(i < (LENGTH - 1))
		    db << ":";
	    }
	    db << dec;
	    return db;
	}

    private:
	unsigned char _address[LENGTH];
    } __attribute__((packed));

    // NIC protocol id
    typedef unsigned char Protocol;

    // NIC statistics
    struct Statistics {
	Statistics(): rx_packets(0), tx_packets(0), 
		      rx_bytes(0), tx_bytes(0) {}

	unsigned int rx_packets;
	unsigned int tx_packets;
	unsigned int rx_bytes;
	unsigned int tx_bytes;
    };

    // Observer
    typedef Conditional_Observer Observer;
    typedef Conditionally_Observed Observed;

    // Polymorphic (or not) NIC wrapper
    template<typename T>
    class NIC_Base {
    private:
	typedef typename T::Address Address;
	typedef typename T::Protocol Protocol;
	typedef typename T::Statistics Statistics;

    public:
	NIC_Base(unsigned int unit = 0) {}

	virtual ~NIC_Base() {}
    
	virtual int send(const Address & dst, const Protocol & prot, 
			 const void * data, unsigned int size) = 0; 

	virtual int receive(Address * src, Protocol * prot,
			    void * data, unsigned int size) = 0;
    
	virtual void reset() = 0;
    
	virtual unsigned int mtu() = 0;

	virtual const Address & address() = 0;

	virtual const Statistics & statistics() = 0;
    };

    template<typename NIC, bool polymorphic>
    class NIC_Wrapper: public NIC_Base<NIC>, private NIC {
    private:
	typedef typename NIC::Address Address;
	typedef typename NIC::Protocol Protocol;
	typedef typename NIC::Statistics Statistics;

    public:
	NIC_Wrapper(unsigned int unit = 0): NIC(unit) {}

	virtual ~NIC_Wrapper() {}

	virtual int send(const Address & dst, const Protocol & prot, 
			 const void * data, unsigned int size) {
	    return NIC::send(dst, prot, data, size); 
	}

	virtual int receive(Address * src, Protocol * prot,
			    void * data, unsigned int size) {
	    return NIC::receive(src, prot, data, size); 
	}
    
	virtual void reset() { NIC::reset(); }
    
	virtual unsigned int mtu() { return NIC::mtu(); }

	virtual const Address & address() { return NIC::address(); }

	virtual const Statistics & statistics() { return NIC::statistics(); }
    };

    template<typename NIC>
    class NIC_Wrapper<NIC, false>: public NIC {
    public:
	NIC_Wrapper(unsigned int unit = 0): NIC(unit) {}
    };

    // Meta_NIC (efficiently handles polymorphic or monomorphic lists of NICs
    template<typename NICS>
    class Meta_NIC {
    private:
	static const bool polymorphic = NICS::Polymorphic;

	typedef typename NICS::template Get<0>::Result T;

    public:
	typedef	typename IF<polymorphic, NIC_Base<T>, T>::Result Base;

	template<int Index>
	struct Get { 
	    typedef NIC_Wrapper<typename NICS::template Get<Index>::Result,
				polymorphic> Result;
	};
    };
};

__END_SYS

#ifdef __NIC_H
#include __NIC_H
#endif

#endif
