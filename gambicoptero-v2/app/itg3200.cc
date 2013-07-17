#include <machine.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <alarm.h>
#include <mach/mc13224v/itg3200_i2c.h>
__USING_SYS


int main() {
	OStream cout;
	ITG3200_I2C gyro;
	short x, y, z;
	Alarm::delay(1000000);

	cout << "ITG3200 test\n";

	gyro.initGyro();

	while(1) {
		gyro.measureGyro();
		x = gyro.sample_x();
		y = gyro.sample_y();
		z = gyro.sample_z();

		cout << "x = " << x << ",\t ";
		cout << "y = " << y << ",\t ";
		cout << "z = " << z << "\n";

		Alarm::delay(10000);
	}

	return 0;
}
