
//#include "mach/mc13224v/motor.h"
//#include <sensor.h>
#include <mach/mc13224v/adxl345.h>
#include<mach/mc13224v/itg3200.h>
#include<utility/FlightControlProcessor.h>
#include <mach/mc13224v/i2c.h>
#include <utility/PID.h>
#include<utility/Kinematics_DCM.h>
#include<utility/DataStorage.h>
#include<utility/FourtOrderFilter.h>
#include<utility/receiver_remotePC.h>
#include<utility/motors.h>

__USING_SYS



void loop(MC13224V_ADXL345 *ac, ITG3200 *gyro, FlightControlProcessor * fcp,
          ARM7_TSC * tsc, FourthOrderFilter* fourthOrderFilter,
          Kinematics *kinematics);

int main() {


#ifdef CHANGE_YAW_DIRECTION
  YAW_DIRECTION = -1;
#endif

  //Motors::startMotors();
  Alarm::delay(2000000);

  MC13224V_I2c::i2c_init();
  MC13224V_ADXL345 * ac             = new MC13224V_ADXL345;
  ITG3200 * gyro                    = new ITG3200;
  ReceiverRemotePc * receiver               = new ReceiverRemotePc;
  DataStorage * dataStorage         = new DataStorage(receiver);
  PIDclass * pid                    = new PIDclass;
  Kinematics * kinematics = new KinematicsDCM;
  FlightControlQuadType<FlightControlQuadPlus> * fcqp =
                                      new FlightControlQuadType<FlightControlQuadPlus>;

  FlightControlProcessor * fcp      = new FlightControlProcessor(kinematics,pid, receiver, fcqp, gyro);
  FourthOrderFilter * fourthOrderFilter = new FourthOrderFilter;
  ARM7_TSC * tsc = new ARM7_TSC;
  //Global initializations here, flightMode, and other from class Aeroquad
  dataStorage->initializeEEPROM(ac, gyro, pid); //passed
  // writeEEPROM();

  //initPlatform(); Inicializa Giroscopio IDG_IDZ500, logo nao precisamos

  for (short motor = 0; motor < 4; motor++) {
      fcp->motorMinCommand[motor] = dataStorage->_minArmedThrottle; //Guess should be minArmedThrolle from data Storage
      fcp->motorMaxCommand[motor] = Receiver::MAXCOMMAND;
  }


  // Setup receiver pins for pin change interrupts
  receiver->initializeReceiver(Receiver::LASTCHANNEL);
  //EEPROM HERE,  not needed for now.

  ac->initializeAccel(MC13224V_ADXL345::FULLRANGE, MC13224V_ADXL345::SAMPLER_RATE_200HZ); // defined in Accel.
  //initSensorsZeroFromEEPROM();
  //gyro_init no construtor;
  ac->computeAccelBias();
  gyro->calibrate();
  ARM7_TSC::init();
  AeroQuad::_currentTime = tsc->time_stamp();
  pid->zeroIntegralError();
  // Flight angle estimation
/*  #ifdef HeadingMagHold
    vehicleState |= HEADINGHOLD_ENABLED;
    initializeMagnetometer();
    KinitializeKinematics(getHdgXY(XAXIS), getHdgXY(YAXIS));
  *///#else

  //Quem deveria ter esse método é o FlightControl, e delegaria a função para o Kinematics
  //O kinematics deveria ser construido dentro do flihgtControlProcessor
  kinematics->initializeKinematics(1.0, 0.0);  // with no compass, DCM matrix initalizes to a heading of 0 degrees
    pid->PID[PIDclass::ATTITUDE_XAXIS_PID_IDX].windupGuard = 0.375;
    pid->PID[PIDclass::ATTITUDE_YAXIS_PID_IDX].windupGuard = 0.375;
    fourthOrderFilter->setupFourthOrder();
    AeroQuad::_previousTime =  tsc->time_stamp();
    Motors::startMotors();
    AeroQuad::_safetyCheck = 0;
    for (int  i = 0;  i <= 500 ; ++i){ //(true){
             loop(ac, gyro, fcp, tsc, fourthOrderFilter, kinematics);
    }

    //Alarm::delay(10000000);
    Motors::shutDown();
}

/*******************************************************************
  // tasks (microseconds of interval)
  ReadGyro        readGyro      (   5000); // 200hz
  ReadAccel       readAccel     (   5000); // 200hz
  RunDCM          runDCM        (  10000); // 100hz
  FlightControls  flightControls(  10000); // 100hz
  ReadReceiver    readReceiver  (  20000); //  50hz
  ReadBaro        readBaro      (  40000); //  25hz
  ReadCompass     readCompass   ( 100000); //  10Hz
  ProcessTelem    processTelem  ( 100000); //  10Hz
  ReadBattery     readBattery   ( 100000); //  10Hz

  Task *tasks[] = {&readGyro, &readAccel, &runDCM, &flightControls,   \
                   &readReceiver, &readBaro, &readCompass,            \
                   &processTelem, &readBattery};

  TaskScheduler sched(tasks, NUM_TASKS(tasks));

  sched.run();
*******************************************************************/

void loop (MC13224V_ADXL345 *ac, ITG3200 *gyro,
           FlightControlProcessor * flightControlProcessor,
           ARM7_TSC * tsc, FourthOrderFilter * fourthOrderFilter,
           Kinematics* kinematics) {

    AeroQuad::_currentTime = tsc->time_stamp();
    AeroQuad::_deltaTime = AeroQuad::_currentTime - AeroQuad::_previousTime;
    gyro->measureGyroSum();
    ac->measureAccelSum();
  /*  db<AeroQuad>(TRC) << " Current time: "<< AeroQuad::_currentTime << endl;
  db<AeroQuad>(TRC) << " Previous time: "<< AeroQuad::_previousTime << endl;
    db<AeroQuad>(TRC) << " Delta time: "<< AeroQuad::_deltaTime << endl;
    ///Alarm::delay(1000);
*/
    // Main scheduler loop set for 100hz
  if (AeroQuad::_deltaTime >= 10000) {

     AeroQuad::frameCounter++;
/*
    // ================================================================
    // 100hz task loop
    // ================================================================
 */
   if (AeroQuad::frameCounter % AeroQuad::TASK_100HZ == 0) {  //  100 Hz tasks

      AeroQuad::_G_Dt = (AeroQuad::_currentTime - AeroQuad::hundredHZpreviousTime) / 1000000.0;
      AeroQuad::hundredHZpreviousTime = AeroQuad::_currentTime;
      db<AeroQuad> (TRC) << "G_dt: " << AeroQuad::_G_Dt << endl;
      ac->evaluateMetersPerSec();
      gyro-> evaluateGyroRate();

      for (int axis = AeroQuad::XAXIS; axis <= AeroQuad::ZAXIS; axis++) {
        ac->filteredAccel[axis] =
                fourthOrderFilter->computeFourthOrder(ac->meterPerSecSec[axis], axis);//&fourthOrder[axis]);
        db<AeroQuad>(TRC) << " Filtered Accel[" << axis << "] -> " << ac->filteredAccel[axis] << endl;
    //    db<AeroQuad>(TRC) << " meterPerSecSec[" << axis << "] -> " <<  ac->meterPerSecSec[axis] << endl;
      }

/*
//}}}
      //NAO SEI QUAL USAR
      // ****************** Calculate Absolute Angle *****************
  //    #if defined FlightAngleNewARG
        calculateKinematics(gyro->gyroRate[XAXIS],
                            gyro->gyroRate[YAXIS],
                            gyro->gyroRate[ZAXIS],
                            ac->filteredAccel[XAXIS],
                            ac->filteredAccel[YAXIS],
                            ac->filteredAccel[ZAXIS],
                            0.0,
                            0.0,
                            0.0,
                            G_Dt);
/*
      #elif defined HeadingMagHold && defined FlightAngleMARG
        calculateKinematics(gyroRate[XAXIS],
                            gyroRate[YAXIS],
                            gyroRate[ZAXIS],
                            filteredAccel[XAXIS],
                            filteredAccel[YAXIS],
                            filteredAccel[ZAXIS],
                            getMagnetometerRawData(XAXIS),
                            getMagnetometerRawData(YAXIS),
                            getMagnetometerRawData(ZAXIS),
                            G_Dt);
      #elif defined FlightAngleARG
        calculateKinematics(gyroRate[XAXIS],
                            gyroRate[YAXIS],
                            gyroRate[ZAXIS],
                            filteredAccel[XAXIS],
                            filteredAccel[YAXIS],
                            filteredAccel[ZAXIS],
                            0.0,
                            0.0,
                            0.0,
                            G_Dt);
      #elif defined HeadingMagHold && !defined FlightAngleMARG && !defined FlightAngleARG
        calculateKinematics(gyroRate[XAXIS],
                            gyroRate[YAXIS],
                            gyroRate[ZAXIS],
                            filteredAccel[XAXIS],
                            filteredAccel[YAXIS],
                            filteredAccel[ZAXIS],
                            accelOneG,
                            getHdgXY(XAXIS),
                            getHdgXY(YAXIS),
                            G_Dt);
      #elif !defined HeadingMagHold && !defined FlightAngleMARG && !defined FlightAngleARG
*/        kinematics->calculateKinematics(gyro->gyroRate[AeroQuad::XAXIS],
                            gyro->gyroRate[AeroQuad::YAXIS],
                            gyro->gyroRate[AeroQuad::ZAXIS],
                            ac->filteredAccel[AeroQuad::XAXIS],
                            ac->filteredAccel[AeroQuad::YAXIS],
                            ac->filteredAccel[AeroQuad::ZAXIS],
                            ac->accelOneG, //deve ser esse
                            0.0,
                            0.0,
                            AeroQuad::_G_Dt);
//      #endif

      flightControlProcessor->processFlightControl();

}}

//}
  /*
          }

    // ================================================================
    // 50hz task loop
    // ================================================================
    if (frameCounter % TASK_50HZ == 0) {  //  50 Hz tasks

      G_Dt = (currentTime - fiftyHZpreviousTime) / 1000000.0;
      fiftyHZpreviousTime = currentTime;

      // Reads external pilot commands and performs functions based on stick configuration
      readPilotCommands(); // defined in FlightCommand.pde


    // ================================================================
    // 10hz task loop
    // ================================================================
    if (frameCounter % TASK_10HZ == 0) {  //   10 Hz tasks

      G_Dt = (currentTime - tenHZpreviousTime) / 1000000.0;
      tenHZpreviousTime = currentTime;


      // Listen for configuration commands and reports telemetry
      readSerialCommand(); // defined in SerialCom.pde
      sendSerialTelemetry(); // defined in SerialCom.pde


    }
*/
    AeroQuad::_previousTime = AeroQuad::_currentTime;

  if (AeroQuad::frameCounter >= 100) {
      AeroQuad::frameCounter = 0;
  }
}


