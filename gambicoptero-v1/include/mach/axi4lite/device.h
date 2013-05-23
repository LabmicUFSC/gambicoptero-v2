// EPOS AXI4LITE Device Common Package Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __axi4lite_device_h
#define __axi4lite_device_h

#include <system/config.h>
#include <utility/list.h>

__BEGIN_SYS

class AXI4LITE_Device 
{
private:
    typedef Simple_List<AXI4LITE_Device> List;

public:
    AXI4LITE_Device(const Type_Id & type, unsigned int unit, void * dev,
              unsigned int interrupt = ~0U)
        : _type(type), _unit(unit), _object(dev), _interrupt(interrupt),
          _busy(false), _link(this) { 
        _devices.insert(&_link);
    }

    ~AXI4LITE_Device() { _devices.remove(&_link); }

    void * object() { return _object; }

    static void * seize(const Type_Id & type, unsigned int unit) {
        AXI4LITE_Device * dev = get(type, unit);
        if(!dev) {
            db<AXI4LITE>(WRN) << "AXI4LITE_Device::seize: device not found\n";
            return 0;
        }
        if(dev->_busy) {
            db<AXI4LITE>(WRN) << "AXI4LITE_Device::seize: device busy\n";
            return 0;
        }
        dev->_busy = true;

        db<AXI4LITE>(TRC) << "AXI4LITE_Device::seize(type=" << type << ",unit=" << unit 
                    << ") => " << dev << "\n";

        return dev->_object;
    }

    static void release(const Type_Id & type, unsigned int unit) {
        AXI4LITE_Device * dev = get(type, unit);
        if(!dev)
            db<AXI4LITE>(WRN) << "AXI4LITE_Device::release: device not found\n";
        dev->_busy = false; 
    }

    static AXI4LITE_Device * get(const Type_Id & type, unsigned int unit) {
        List::Element * e = _devices.head();
        for(; e && ((e->object()->_type != type) ||
                    (e->object()->_unit != unit)); e = e->next());
        if(!e)
            return 0;
        return e->object();
    }

    static AXI4LITE_Device * get(unsigned int interrupt) {
        List::Element * e = _devices.head();
        for(; e && (e->object()->_interrupt != interrupt); e = e->next());
        if(!e)
            return 0;
        return e->object();
    }

    static void install_handler(unsigned int interrupt);

private:
    Type_Id _type;
    unsigned int _unit;
    void * _object;
    unsigned int _interrupt;
    bool _busy;
    List::Element _link;

    static List _devices;
};

__END_SYS

#endif
