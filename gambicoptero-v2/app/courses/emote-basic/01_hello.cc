// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <system/config.h>
#include <utility/ostream.h>
#include <sensor.h>
#include <alarm.h>

__USING_SYS

int temperatura() {
       Temperature_Sensor sensor;
       cout << "Temperatura = " << sensor.sample() << "\n";
}

int main() {
    OStream cout;
    cout << "Hello EPOS!\n";

    Handler_Function handler(temperatura);
    Alarm alarm(1000000,handler);

    while(1);
}

