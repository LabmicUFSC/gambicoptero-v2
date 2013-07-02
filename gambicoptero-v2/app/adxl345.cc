#include <machine.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <alarm.h>
#include <mach/mc13224v/adxl345.h>
__USING_SYS


int main() {
	OStream cout;
	MC13224V_ADXL345 accel;
	int x, y, z;

	cout << "ADXL345 accelerometer test\n";

	accel.enable_x();
	accel.enable_y();
	accel.enable_z();

	while(1) {
		x = accel.sample_x();
		y = accel.sample_y();
		z = accel.sample_z();

		cout << "x = " << x << ", ";
		cout << "y = " << y << ", ";
		cout << "z = " << z << "\n";

		Alarm::delay(100);
	}

	return 0;
}
