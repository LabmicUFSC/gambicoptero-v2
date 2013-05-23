/**
    PWM Motors class 
*/

#ifndef _AQUAD_MOTORS_PWM_H_
#define _AQUAD_MOTORS_PWM_H_

#include "Motors.h"

template <unsigned int N_MOTORS>
class Motors_PWM_Timer : public Motors<N_MOTORS> {
public:
    void initialize_motors() {
        /* Should initialize ports as output here.
           Is it necessary? */

        command_all_motors(MIN_COMMAND);

        /* Should initialize timers here. */
    }

    void write_motors() {
        /* TODO */    
    }

    void command_all_motors() {
        /* TODO */
    }
};

#endif
