// EPOS GPIO Pin Mediator Common Package

#ifndef __gpio_pin_h
#define __gpio_pin_h

#include <system/config.h>

__BEGIN_SYS

class GPIO_Pin_Common
{
protected:
    GPIO_Pin_Common() {}
};

__END_SYS

#ifdef __GPIO_PIN_H
#include __GPIO_PIN_H
#endif

#endif
