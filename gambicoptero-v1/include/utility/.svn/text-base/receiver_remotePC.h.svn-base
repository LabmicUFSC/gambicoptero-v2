#ifndef RECEIVER_REMOTEPC_H
#define RECEIVER_REMOTEPC_H
#include <utility/receiver.h>

__BEGIN_SYS
class ReceiverRemotePc : public Receiver {

public:
    ReceiverRemotePc() : Receiver(){}
    int _receiverCommandPC[MAX_NB_CHANNEL];
   virtual void initializeReceiver(int nbChannel);
    const float getReceiverSIData(short channel);

    /*int getRawChannelValue(short channel) {
        db<ReceiverRemotePc>(TRC) << "--- getting raw channel: " << channel << endl;

        return myReceiverCommand[channel];
    }*/
/*
void setChannelValue(short channel,int value) {
    db<ReceiverRemotePc>(TRC) << "--- SETTING___________ raw channel: " << channel << endl;
    myReceiverCommand[channel] = value;
}
*/
};

__END_SYS
#endif // RECEIVER_REMOTEPC_H
