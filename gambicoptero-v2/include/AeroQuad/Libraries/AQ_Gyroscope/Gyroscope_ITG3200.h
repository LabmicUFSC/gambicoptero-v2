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

#ifndef _AEROQUAD_GYROSCOPE_ITG3200_H_
#define _AEROQUAD_GYROSCOPE_ITG3200_H_


#include <AeroQuad/Libraries/AQ_Defines/GlobalDefined.h>
#include <AeroQuad/Libraries/AQ_Defines/SensorsStatus.h>
#include <AeroQuad/Libraries/AQ_Gyroscope/Gyroscope_ITG3200Common.h>

void measureSpecificGyroADC(int *gyroADC, short sample_x, short sample_y, short sample_z) {

  gyroADC[XAXIS] = sample_x  - gyroZero[XAXIS];
  gyroADC[YAXIS] = gyroZero[YAXIS] - sample_y;
  gyroADC[ZAXIS] = gyroZero[ZAXIS] - sample_z;

}

void measureSpecificGyroSum(short sample_x, short sample_y, short sample_z) {
  gyroSample[XAXIS] += sample_x;
  gyroSample[YAXIS] += sample_y;
  gyroSample[ZAXIS] += sample_z;
}

void evaluateSpecificGyroRate(int *gyroADC) {

  gyroADC[XAXIS] = (gyroSample[XAXIS] / gyroSampleCount) - gyroZero[XAXIS];
  gyroADC[YAXIS] = gyroZero[YAXIS] - (gyroSample[YAXIS] / gyroSampleCount);
  gyroADC[ZAXIS] = gyroZero[ZAXIS] - (gyroSample[ZAXIS] / gyroSampleCount);
}

bool calibrateGyro() {
  //Finds gyro drift.
  //Returns false if during calibration there was movement of board. 

  int findZero[3][FINDZERO];
  int diff = 0;

  for (int i=0; i<FINDZERO; i++) {
    gyro->measureGyro();
    findZero[XAXIS][i] = gyro->sample_x();
    findZero[YAXIS][i] = gyro->sample_y();
    findZero[ZAXIS][i] = gyro->sample_z();
    Alarm::delay(10000);
  }

  for(int i = XAXIS; i < ZAXIS; i++) {
    int tmp = findMedianIntWithDiff(findZero[i], FINDZERO, &diff);
    if (diff <= GYRO_CALIBRATION_TRESHOLD) { // 4 = 0.27826087 degrees during 49*10ms measurements (490ms). 0.57deg/s difference between first and last.
      gyroZero[i] = tmp;
    } 
    else {
      return false; //Calibration failed.
    }
  }
  
  return true; //Calibration successfull.
}

#endif
