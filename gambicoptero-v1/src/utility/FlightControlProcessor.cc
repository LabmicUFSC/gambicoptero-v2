#include<utility/FlightControlProcessor.h>

__USING_SYS
void  FlightControlProcessor::processFlightControl(){

    if(_receiver->_receiverCommandPC[AeroQuad::XAXIS] < 1100
            && AeroQuad::frameCounter % 2 == 0){
        _receiver->_receiverCommandPC[AeroQuad::XAXIS]++;
        _receiver->_receiverCommandPC[AeroQuad::YAXIS]++;
        _receiver->_receiverCommandPC[AeroQuad::ZAXIS]++;
        _receiver->_receiverCommandPC[Receiver::THROTTLE]++;
        db<FlightControlProcessor>(TRC) << "INCREMENT XAXIS" << endl;

    } else { }//Motors::shutDown();}

    db<FlightControlProcessor>(TRC) << " ---- Process Flight Control  ----" << endl;
    // ********************** Calculate Flight Error ***************************
    calculateFlightError();


    db<FlightControlProcessor> (TRC) << "// ********************** Update Yaw ***************************************" << endl;

    headingHoldProcessor->processHeading(_gyro, _fcQuadPlus);

    if (AeroQuad::frameCounter % AeroQuad::THROTTLE_ADJUST_TASK_SPEED == 0) {  // 50hz task
        // ********************** Process Altitude hold **************************
        //sem barometro nao precisa
        //processAltitudeHold(); //throttle = receiverCommand[THROTTLE]; só aparentemente
        // ********************** Process Battery monitor hold **************************
#if defined BattMonitor && defined BattMonitorAutoDescent
        //    processBatteryMonitorThrottleAdjustment();
#endif
        // ********************** Process throttle correction ********************
        processThrottleCorrection();
    }

    // ********************** Calculate Motor Commands *************************
    if (motorArmed && AeroQuad::_safetyCheck) {
        _fcQuadPlus->applyMotorCommand(throttle);
    }

    processMinMaxCommand();

    if (_receiver->_receiverCommandPC[Receiver::THROTTLE] < Receiver::MINCHECK) {
       for (short motor = 0; motor < 4 /*LASTMOTOR*/; motor++) {
         _fcQuadPlus->motorMaxCommand[motor] = AeroQuad::minArmedThrottle;
       }
     }

    // Apply limits to motor commands
    //TEm que arrumar o processMinMaxCommand
    for (short motor = 0; motor < 4; motor++) {
        _fcQuadPlus->motorCommand[motor] =  AQMath::constrain(
                    _fcQuadPlus->motorCommand[motor], 1000, 1150);/*AQMath::constrain(
                    _fcQuadPlus->motorCommand[motor],
                    _fcQuadPlus->motorMinCommand[motor],
                    _fcQuadPlus->motorMaxCommand[motor]); */
    db<FlightControlProcessor>(TRC) << "MOTOR "<< motor << ": "
                                    << _fcQuadPlus->motorCommand[motor] << endl;
    }
    //  Alarm::delay(1000000);
    /*  db<FlightControlProcessor>(TRC) << "MOTOR FRONT: " << _fcQuadPlus->motorCommand[Motors::MOTOR1] << endl;
      db<FlightControlProcessor>(TRC) << "MOTOR REAR: " << _fcQuadPlus->motorCommand[Motors::MOTOR2] << endl;
      db<FlightControlProcessor>(TRC) << "MOTOR RIGHT: " << _fcQuadPlus->motorCommand[Motors::MOTOR3] << endl;
      db<FlightControlProcessor>(TRC) << "MOTOR LEFT: " << _fcQuadPlus->motorCommand[Motors::MOTOR4] << endl;
*/

      // ESC Calibration
      if (motorArmed == AeroQuad::OFF || AeroQuad::_safetyCheck == AeroQuad::OFF) {
        processCalibrateESC();
        AeroQuad::calibrateESC= 5;
      }

      // *********************** Command Motors **********************
      if (motorArmed == AeroQuad::ON && AeroQuad::_safetyCheck == AeroQuad::ON) {
          db<FlightControlProcessor> (TRC) << "--- Write Motors ---" << endl;
          _fcQuadPlus->write();
      }


}

void FlightControlProcessor::calculateFlightError()
{
    db<FlightControlProcessor>(TRC) << "--- Calculate Flight Error ---" << endl;
    if (flightMode == ATITUDE_FLIGHT_MODE ) {
        db<FlightControlProcessor>(TRC) << "--- ATTITUDE FLIGHT MODE---" << endl;

        db<FlightControlProcessor>(TRC) << "ReceiverCommand X: "
                                        << _receiver->_receiverCommandPC[AeroQuad::XAXIS] << endl;

        db<FlightControlProcessor>(TRC) << "ReceiverZero:X "
                                        << _receiver->receiverZero[AeroQuad::XAXIS] << endl;

        float rollAttitudeCmd  = _pid->updatePID((_receiver->_receiverCommandPC[AeroQuad::XAXIS] -
                                                  _receiver->receiverZero[AeroQuad::XAXIS]) *
                                                 ATTITUDE_SCALING,
                                                 _kinematics->kinematicsAngle[AeroQuad::XAXIS],
                                                 PIDclass::ATTITUDE_XAXIS_PID_IDX);// &PID[ATTITUDE_XAXIS_PID_IDX]);


        float pitchAttitudeCmd = _pid->updatePID((_receiver->_receiverCommandPC[AeroQuad::YAXIS] -
                                                  _receiver->receiverZero[AeroQuad::YAXIS]) *
                                                 ATTITUDE_SCALING,
                                                 -_kinematics->kinematicsAngle[AeroQuad::YAXIS],
                                                 PIDclass::ATTITUDE_YAXIS_PID_IDX);//&PID[ATTITUDE_YAXIS_PID_IDX]);


        db<FlightControlProcessor>(TRC) << "ReceiverCommandPC Y: "
                                        << _receiver->_receiverCommandPC[AeroQuad::YAXIS] << endl;

        db<FlightControlProcessor>(TRC) << "ReceiverZero Y: "
                                        << _receiver->receiverZero[AeroQuad::YAXIS] << endl;

        db<FlightControlProcessor>(TRC) <<  "kinematics angle: X: " << _kinematics->kinematicsAngle[AeroQuad::XAXIS] << endl;
        db<FlightControlProcessor>(TRC) <<  "kinematics angle: Y: " << _kinematics->kinematicsAngle[AeroQuad::YAXIS] << endl;
        db<FlightControlProcessor>(TRC) << "RollAttitudeCmd: " << rollAttitudeCmd<< endl;
        db<FlightControlProcessor>(TRC) << "pitchAttitudeCmd: " << pitchAttitudeCmd << endl;


        _fcQuadPlus->motorAxisCommandRoll   = _pid->updatePID(rollAttitudeCmd,
                                                              _gyro->gyroRate[AeroQuad::XAXIS]*1.2,
                                                              PIDclass::ATTITUDE_GYRO_XAXIS_PID_IDX);//&PID[ATTITUDE_GYRO_XAXIS_PID_IDX]);

        _fcQuadPlus->motorAxisCommandPitch  = _pid->updatePID(pitchAttitudeCmd,
                                                              -_gyro->gyroRate[AeroQuad::YAXIS]*1.2,
                                                              PIDclass::ATTITUDE_GYRO_YAXIS_PID_IDX);//&PID[ATTITUDE_GYRO_YAXIS_PID_IDX]);


    }
    else {
        db<FlightControlProcessor>(TRC) << "--- ACRO MODE ---" << endl;

        //o receiverCommand está com 1500, se foi inicializado
        _fcQuadPlus->motorAxisCommandRoll = _pid->updatePID(_receiver->getReceiverSIData(AeroQuad::XAXIS),
                                                            (_gyro->gyroRate[AeroQuad::XAXIS])*0.8,
                                                            PIDclass::RATE_XAXIS_PID_IDX);//&PID[RATE_XAXIS_PID_IDX]);

        _fcQuadPlus->motorAxisCommandPitch = _pid->updatePID(_receiver->getReceiverSIData(AeroQuad::YAXIS),
                                                             -(_gyro->gyroRate[AeroQuad::YAXIS])*0.8,
                                                             PIDclass::RATE_YAXIS_PID_IDX);//&PID[RATE_YAXIS_PID_IDX]);


    }
    db<FlightControlProcessor>(TRC) << "motorAxisCommandRoll: " << _fcQuadPlus->motorAxisCommandRoll << endl;
    db<FlightControlProcessor>(TRC) << "motorAxisCommandPitch: " << _fcQuadPlus->motorAxisCommandPitch << endl;
}

int FlightControlProcessor::processThrottleCorrection()
{

    int throttleAsjust = throttle / (Math::cosineTaylor(
                                         Math::radians (
                                             _kinematics->kinematicsAngle[AeroQuad::XAXIS]
                                             )
                                         ) *
                                     Math::cosineTaylor (
                                         Math::radians (
                                             _kinematics->kinematicsAngle[AeroQuad::YAXIS]
                                             )
                                         )
                                     );

    throttleAsjust = AQMath::constrain ((throttleAsjust - throttle), 0, 160); //compensate max  +/- 25 deg XAXIS or YAXIS or  +/- 18 ( 18(XAXIS) + 18(YAXIS))
    throttle = throttle + throttleAsjust + 0;//(int)batteyMonitorThrottleCorrection;

    throttle = AQMath::constrain(throttle, Receiver::MINCOMMAND, Receiver::MAXCOMMAND-150);  // limmit throttle to leave some space for motor correction in max throttle manuever
    db<FlightControlProcessor>(TRC) << "Throttle correction: " << throttle << endl;
}

void FlightControlProcessor::processMinMaxCommand(){
    /*for (byte motor = 0; motor < LASTMOTOR; motor++)
      {
        motorMinCommand[motor] = minArmedThrottle;
        motorMaxCommand[motor] = MAXCOMMAND;
      }

      int maxMotor = motorCommand[0];

      for (byte motor=1; motor < LASTMOTOR; motor++) {
        if (motorCommand[motor] > maxMotor) {
          maxMotor = motorCommand[motor];
        }
      }

      for (byte motor = 0; motor < LASTMOTOR; motor++) {
        if (maxMotor > MAXCOMMAND) {
          motorCommand[motor] =  motorCommand[motor] - (maxMotor - MAXCOMMAND);
        }
      }*/

    // Force motors to be equally distant from throttle value for balanced motor output during hard yaw
    short motorMaxCheck = AeroQuad::OFF;
    short motorMinCheck = AeroQuad::OFF;

    // Check if everything within motor limits
    for (short motor = 0; motor < 4/*LASTMOTOR*/; motor++) {
        motorMaxCheck = motorMaxCheck | (_fcQuadPlus->motorCommand[motor] >= Receiver::MAXCOMMAND);
        motorMinCheck = motorMinCheck | (_fcQuadPlus->motorCommand[motor] <= AeroQuad::minArmedThrottle);
    }

    // If everything within limits, turn flags off and reset max/mins to default
    if (!motorMaxCheck) {
        if (maxLimit) { // only reset if flag was on
            for (short motor = 0; motor < 4/*LASTMOTOR*/; motor++)
                motorMinCommand[motor] = AeroQuad::minArmedThrottle;
            maxLimit =AeroQuad::OFF;
        }
    }
    if (!motorMinCheck) {
        if (minLimit) { // only reset if flag was on
            for (short motor = 0; motor < 4/*LASTMOTOR*/; motor++)
                motorMaxCommand[motor] = Receiver::MAXCOMMAND;
            minLimit = AeroQuad::OFF;
        }
    }

    // If any limits reached, freeze current min/max values and turn limit flag on
    // In future iterations, if limit still exceeded again, use only first frozen values
    for (short motor = 0; motor < 4/*LASTMOTOR*/; motor++) {
        if ((_fcQuadPlus->motorCommand[motor] >= Receiver::MAXCOMMAND) && maxLimit == AeroQuad::OFF) {
            for (short motorLimit = 0; motorLimit < 4/*LASTMOTOR*/; motorLimit++)
                _fcQuadPlus->motorMinCommand[motorLimit] = _fcQuadPlus->motorCommand[motorLimit];
            maxLimit = AeroQuad::ON;
        }
        if ((_fcQuadPlus->motorCommand[motor] <= AeroQuad::minArmedThrottle) && minLimit == AeroQuad::OFF) {
            for (short motorLimit = 0; motorLimit < 4/*LASTMOTOR*/; motorLimit++)
                _fcQuadPlus->motorMaxCommand[motorLimit] = _fcQuadPlus->motorCommand[motorLimit];
            minLimit = AeroQuad::ON;
        }
    }
}

void FlightControlProcessor::processCalibrateESC()
{

    db<FlightControlProcessor>(TRC) << "---processCalibrateESC --- " << endl;
    //calibrateESC vem do serial/ configuração
 switch (AeroQuad::calibrateESC) { // used for calibrating ESC's
 /* case 1:
    for (byte motor = 0; motor < LASTMOTOR; motor++)
      motorCommand[motor] = MAXCOMMAND;
    break;
  case 3:
    for (byte motor = 0; motor < LASTMOTOR; motor++)
      motorCommand[motor] = constrain(testCommand, 1000, 1200);
    break;*/
  case 5:
    for (short motor = 0; motor < 4 /*LASTMOTOR*/; motor++)
      _fcQuadPlus->motorCommand[motor] = AQMath::constrain(
                  _fcQuadPlus->motorConfiguratorCommand[motor],
                  1000, 1200);
    AeroQuad::_safetyCheck = AeroQuad::ON;
    break;
  default:
   for (short motor = 0; motor < 4; motor++){
       _fcQuadPlus->motorCommand[motor] = Receiver::MINCOMMAND + 10 ;
        motorArmed = AeroQuad::ON;

    }


 }
 _fcQuadPlus->write(); // Defined in Motors.h

}
//   Send calibration commands to motors



