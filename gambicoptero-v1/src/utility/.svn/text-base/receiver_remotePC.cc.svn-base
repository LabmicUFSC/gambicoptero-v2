#include<utility/receiver_remotePC.h>
#include<utility/AeroQuad.h>

__USING_SYS

void ReceiverRemotePc::initializeReceiver(int nbChannel) {

     //initializeReceiverParam(nbChannel);
     for (short channel = AeroQuad::XAXIS; channel < THROTTLE; channel++) {
         //_receiverCommand[channel] = 1200;
         receiverZero[channel] = 1000;
         _receiverCommandPC[channel] = 1000;
         db<ReceiverRemotePc> (TRC)<< "receiverCommand: " << channel << "-> " << _receiverCommand[channel] << endl;
     }
     //_receiverCommand[THROTTLE] = 690;
     _receiverCommandPC[THROTTLE] = 1000;
     receiverZero[THROTTLE] = 0;
     _receiverCommand[MODE] = 2000;
     receiverZero[MODE] = 0;
     _receiverCommand[AUX] = 2000;
     receiverZero[AUX] = 0;
 }

const float ReceiverRemotePc::getReceiverSIData(short channel){
    db<ReceiverRemotePc>(TRC) << " --- getReceiverSIData --- Channel: " << channel<< endl;
    const float value = ((_receiverCommandPC[channel] -
                 receiverZero[channel]) *
                (2.5 * PWM2RAD));  // +/- 2.5RPS 50% of full rate
    return value;
}

