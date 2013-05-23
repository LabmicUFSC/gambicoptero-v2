
#include<utility/PID.h>

__USING_SYS

int PIDclass::printPid(const PIDdata * pidData){
    db<PIDclass>(TRC) <<  "P: " << (int)pidData->P << endl
                       << "I: " << (int)pidData->I << endl
                       << "D: " << (int)pidData->D << endl
                       << "lastPosition: " << (int)pidData->lastPosition << endl
                       << "previousPIDTime: " << (int)pidData->previousPIDTime << endl
                       << "integratedError: " << (int)pidData->integratedError << endl
                       << " windupGuard: " << (int)pidData->windupGuard << endl;

    return 696969;
}


float PIDclass::updatePID(float targetPosition, float currentPosition,
                int pidIDX) {//struct PIDdata *PIDparameters) {

    db<PIDclass>(TRC) << " --- PID updatePID --- PIdIDX: " << pidIDX << endl;

    PIDdata * PIDparameters = &PID[pidIDX];
    db<PIDclass>(TRC) << " --- PidParameters ---" << printPid(PIDparameters) << endl;

    // AKA PID experiments
    //cout << "current time: " << currentTime;
    //cout << "previous time " << PIDparameters->previousPIDTime << endl;
  const float deltaPIDTime = (AeroQuad::_currentTime - PIDparameters->previousPIDTime) / 1000000.0;
  db<PIDclass>(TRC) << "delta Pid Time: " << deltaPIDTime << endl;
  db<PIDclass>(TRC)<< "target position: " << targetPosition << " --  currentPosition : " << currentPosition << endl;
  PIDparameters->previousPIDTime = AeroQuad::_currentTime;  // AKA PID experiments
  float error = targetPosition - currentPosition;
  db<PIDclass>(TRC) << "Error: " << error << endl;
  PIDparameters->integratedError += error * deltaPIDTime;
  PIDparameters->integratedError = AQMath::constrain(PIDparameters->integratedError, -PIDparameters->windupGuard, PIDparameters->windupGuard);
  float dTerm = PIDparameters->D * (currentPosition - PIDparameters->lastPosition) / (deltaPIDTime * 100); // dT fix from Honk
  PIDparameters->lastPosition = currentPosition;
  return (PIDparameters->P * error) + (PIDparameters->I * (PIDparameters->integratedError)) + dTerm;
}

void PIDclass::zeroIntegralError(){
      for (unsigned char axis = AeroQuad::XAXIS; axis <= ATTITUDE_YAXIS_PID_IDX; axis++) {
        PID[axis].integratedError = 0;
        PID[axis].previousPIDTime = AeroQuad::_currentTime;
    //    db<PIDclass> (TRC) << "PID[" << axis << "].previousPIDTime = " << PID[axis].previousPIDTime << endl;
      }
}
