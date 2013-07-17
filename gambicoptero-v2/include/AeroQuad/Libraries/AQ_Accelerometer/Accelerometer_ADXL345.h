/*
  AeroQuad v3.0.1 - February 2012
  www.AeroQuad.com
  Copyright (c) 2012 Ted Carancho.  All rights reserved.
  An Open Source Arduino based multicopter.
 
  This program is free software: you can redistribute it and/or modify 
  it under the terms of the GNU General Public License as published by 
  the Free Software Foundation, either version 3 of the License, or 
  (at your option) any later version. 

  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
  GNU General Public License for more details. 

  You should have received a copy of the GNU General Public License 
  along with this program. If not, see <http://www.gnu.org/licenses/>. 
*/

#ifndef _AEROQUAD_ACCELEROMETER_ADXL345_H_
#define _AEROQUAD_ACCELEROMETER_ADXL345_H_

#include <AeroQuad/Libraries/AQ_Accelerometer/Accelerometer.h>
#include <AeroQuad/Libraries/AQ_Defines/SensorsStatus.h>
#include <AeroQuad/Libraries/AQ_Defines/GlobalDefined.h>
#include <alarm.h>
#include <mach/mc13224v/adxl345_i2c.h>

#define ACCEL_ADDRESS 0x53

ADXL345_I2C *accel;


void initializeAccel() {
  accel = new ADXL345_I2C();
  accel->initAccel();
  Alarm::delay(10*1000); 
}
  
void measureAccel() {
  accel->measureAccel();
  accelSample[XAXIS] = accel->sample_x();
  accelSample[YAXIS] = accel->sample_y();
  accelSample[ZAXIS] = accel->sample_z();
  meterPerSecSec[XAXIS] = accel->sample_x() * accelScaleFactor[XAXIS] + runTimeAccelBias[XAXIS];
  meterPerSecSec[YAXIS] = accel->sample_y() * accelScaleFactor[YAXIS] + runTimeAccelBias[YAXIS];
  meterPerSecSec[ZAXIS] = accel->sample_z() * accelScaleFactor[ZAXIS] + runTimeAccelBias[ZAXIS];
  

}

void measureAccelSum() {
  accel->measureAccel();

  accelSample[XAXIS] += accel->sample_x();
  accelSample[YAXIS] += accel->sample_y();
  accelSample[ZAXIS] += accel->sample_z();
  accelSampleCount++;
}

void evaluateMetersPerSec() {
	
  for (byte axis = XAXIS; axis <= ZAXIS; axis++) {
    meterPerSecSec[axis] = (accelSample[axis] / accelSampleCount) * accelScaleFactor[axis] + runTimeAccelBias[axis];
	  accelSample[axis] = 0;
  }
  accelSampleCount = 0;		
}

void computeAccelBias() {
  
  for (int samples = 0; samples < SAMPLECOUNT; samples++) {
    measureAccelSum();
    Alarm::delay(2500);
  }

  for (byte axis = 0; axis < 3; axis++) {
    meterPerSecSec[axis] = (float(accelSample[axis])/SAMPLECOUNT) * accelScaleFactor[axis];
    accelSample[axis] = 0;
  }
  accelSampleCount = 0;

  runTimeAccelBias[XAXIS] = -meterPerSecSec[XAXIS];
  runTimeAccelBias[YAXIS] = -meterPerSecSec[YAXIS];
  runTimeAccelBias[ZAXIS] = -9.8065 - meterPerSecSec[ZAXIS];

  accelOneG = fabs(meterPerSecSec[ZAXIS] + runTimeAccelBias[ZAXIS]);
}



#endif
