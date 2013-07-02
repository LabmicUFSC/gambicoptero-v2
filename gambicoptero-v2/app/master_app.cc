#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <mach/mc13224v/adxl345_i2c.h>
#include <periodic_thread.h>

__USING_SYS

OStream cout;

int periodic_func() {
    ADXL345_I2C accel;

    accel.initAccel();
    
    short x,y,z;
    for(;;) {
        accel.measureAccel();
        x = accel.sample_x();
        y = accel.sample_y();
        z = accel.sample_z();
        cout << "X: " << (float)(x*0.0078) 
            << "\tY: " << (float)(y*0.0078) 
            << "\tZ: " << (float)(z*0.0078) << "\n";
        Periodic_Thread::wait_next();
    }
}

int main() {
    Periodic_Thread thread(&periodic_func, 10000);
    thread.join();
    for(;;);

//    sensor(1);
}

