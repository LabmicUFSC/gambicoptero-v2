// EPOS EMote2_Startup Test Program

#include <system/config.h>
#include <mach/mc13224v/emote2_startup.h>

__USING_SYS


EMote2_Startup::EMote2_Startup() {
    if(Traits<Machine>::emote_version == 0x2f) {
        _led1_r = GPIO_Pin(10, true);
        _led1_g = GPIO_Pin(9, true);
        _led1_b = GPIO_Pin(11, true);
        _led2 = GPIO_Pin(8, true);
        _led3 = GPIO_Pin(23, true);
        _but1 = GPIO_Pin(24, false);
        _but2 = GPIO_Pin(22, false);
    }
    else {
        _led1_r = GPIO_Pin(23, true);
        _led1_g = GPIO_Pin(24, true);
        _led1_b = GPIO_Pin(25, true);
        _led2 = GPIO_Pin(26, true);
        _led3 = GPIO_Pin(27, true);
        _but1 = GPIO_Pin(11, false);
        _but2 = GPIO_Pin(10, false);
    }
}
