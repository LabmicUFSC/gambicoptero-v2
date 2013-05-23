#include<utility/receiver.h>

__USING_SYS
Receiver::Receiver(){
    lastReceiverChannel = 0;
    receiverXmitFactor = 0.0;
    for (int i = 0 ; i < MAX_NB_CHANNEL; i++){
        receiverData[i]      = 0;
      //  _receiverCommand[i]   = 1200;
        _receiverCommand[i] = 1100;
        receiverCommandSmooth[i] = 0;
        receiverSlope[i] = 0;
        receiverOffset[i] = 0;
        receiverSmoothFactor[i] = 0;

    }

    for (int i = 0; i < 3; i++)
        receiverZero[i] = 0;
}

void Receiver::initializeReceiverParam(int nbChannel){

    db<Receiver>(TRC) << " ---  Receiver PAI ---" << endl;
    lastReceiverChannel = nbChannel;

    _receiverCommand[0] = 1150;
    _receiverCommand[1] = 1150;
    _receiverCommand[2] = 1150;
    _receiverCommand[THROTTLE] = 1000;
    _receiverCommand[MODE] = 1000;
    _receiverCommand[AUX] = 1000;
    _receiverCommand[AUX+1] = 1000;
    _receiverCommand[AUX+2] = 1000;

    for (short channel = 0; channel < lastReceiverChannel; channel++) {
        receiverCommandSmooth[channel] = 1.0;
    }
    for (short channel = 0; channel < THROTTLE; channel++) {
        receiverZero[channel] = 1000;
    }

    for (short channel = 0; channel < lastReceiverChannel; channel++) {
        receiverSlope[channel] = 1;
    }
    for (short channel = 0; channel < lastReceiverChannel; channel++) {
        receiverOffset[channel] = 1;
    }
    for (short channel = 0; channel < lastReceiverChannel; channel++) {
        receiverSmoothFactor[channel] = 1;
    }
}

/*
const float Receiver::getReceiverSIData(short channel) {
    db<Receiver>(TRC) << " --- getReceiverSIData --- Channel: " << channel<< endl;
    const float value = ((_receiverCommand[channel] -
                 receiverZero[channel]) *
                (2.5 * PWM2RAD));  // +/- 2.5RPS 50% of full rate
    return value;
}*/

/*void Receiver::readReceiver(){
    db<Receiver>(TRC) << "--- Read Receiver --- " << endl;
    for(short channel = 0; channel < lastReceiverChannel; channel++) {

        // Apply receiver calibration adjustment
        receiverData[channel] = (receiverSlope[channel] * getRawChannelValue(channel)) + receiverOffset[channel];
        // Smooth the flight control receiver inputs
        receiverCommandSmooth[channel] = AQMath::filterSmooth(receiverData[channel], receiverCommandSmooth[channel], receiverSmoothFactor[channel]);
    }

    // Reduce receiver commands using receiverXmitFactor and center around 1500
    for (short channel = 0; channel < THROTTLE; channel++) {
        _receiverCommand[channel] = ((receiverCommandSmooth[channel] - receiverZero[channel]) * receiverXmitFactor) + receiverZero[channel];
    }
    // No xmitFactor reduction applied for throttle, mode and AUX
    for (short channel = THROTTLE; channel < lastReceiverChannel; channel++) {
        _receiverCommand[channel] = receiverCommandSmooth[channel];
    }
}
*/




