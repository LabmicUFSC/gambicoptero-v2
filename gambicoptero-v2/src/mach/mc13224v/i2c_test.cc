// EPOS MC13224V_I2C Test Program

#include <i2c.h>
#include <utility/ostream.h>

__USING_SYS

int main() {
    OStream cout;

    I2C * i2c = new I2C();

    cout << "I2C Test App for MC13224V." << endl;
    cout << "This test reads the DEV_ID registers of these two devices:" << endl;
    cout << " - ADXL345 accelerometer (device-id 0x53): should answer 0xE5." << endl;
    cout << " - ITS3200 gyroscope (device-id 0x68): should answer 0x69." << endl << endl;

    unsigned char acc_id = 0x53;
    unsigned char gyr_id = 0x68;
    unsigned char response = 0x00;

    i2c->set_timeout(40000); // IT23200 only supports Fast-I2C, MC13224V does not support it - it is unstable
    if(i2c->read_register(gyr_id, 0x00, 1, &response))
        cout << "ITS3200 ID: 0x" << hex << response << endl;
    else
        cout << "ITS3200 ID: 0x" << hex << response << " (result is unreliable)" << endl;

    i2c->set_timeout(0); // no timeout => device works propperly
    if(i2c->read_register(acc_id, 0x00, 1, &response))
        cout << "ADXL345 ID: 0x" << hex << response << endl;
    else
        cout << "ADXL345 ID: 0x" << hex << response << " (result is unreliable)" << endl;

    cout << endl << endl << "I'm done!" << endl;

    while(true);

    return 0;
}
