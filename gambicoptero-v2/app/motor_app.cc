#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <mach/mc13224v/adxl345_i2c.h>
#include <periodic_thread.h>

__USING_SYS

OStream cout;

void maca_handler(MACA_Transceiver::Event event)
 {
      if(event == MACA_Transceiver::LOST_COMMUNICATION)
          cout<<"Lost communication! Abort!\n";
      else if(event == MACA_Transceiver::COORDINATOR_DATA_READY)
          cout<<"Network received data from coordinator!\n";
      else if(event == MACA_Transceiver::FEEDBACK_DATA_READY)
          cout<<"Network received feedback data!\n";
 }

int periodic_func() {
    ADXL345_Proxy accel;

    init(maca_handler);
    
    short x,y,z;
    for(;;) {
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

