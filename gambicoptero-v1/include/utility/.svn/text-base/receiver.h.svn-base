#ifndef RECEIVER_H
#define RECEIVER_H


// Receiver variables
//#include<utility/AeroQuad.h>
#include<utility/AQMath.h>
#include<machine.h>
__BEGIN_SYS
class Receiver {

public:


    static const int MAX_NB_CHANNEL  = 8;
    static const int PWM2RAD = 0.002; //  Based upon 5RAD for full stick movement, you take this times the RAD to get the PWM conversion factor
    static const int LASTCHANNEL = 6;
  //  static const int TIMEOUT  = 25000;
    static const int MINCOMMAND = 1000;
    static const int MIDCOMMAND = 1500;
    static const int MAXCOMMAND = 2000;
    static const int MINDELTA = 200;
    static const int MINCHECK = MINCOMMAND + 5;//100
    static const int MAXCHECK = MAXCOMMAND - 100;
    static const int MINTHROTTLE = MINCOMMAND + 5;//100;
    static const int LEVELOFF = 100;
    static const int THROTTLE  = 3;
    static const int MODE  = 4;
    static const int AUX = 5;
    static const int AUX2  = 6;
    static const int AUX3 = 7;

    Receiver();
    int lastReceiverChannel;
    float receiverXmitFactor;
    int receiverData[MAX_NB_CHANNEL];
    int receiverZero[3];
    int _receiverCommand[MAX_NB_CHANNEL] ;
  //  int myReceiverCommand[MAX_NB_CHANNEL];

    int receiverCommandSmooth[MAX_NB_CHANNEL] ;
    float receiverSlope[MAX_NB_CHANNEL];
    float receiverOffset[MAX_NB_CHANNEL];
    float receiverSmoothFactor[MAX_NB_CHANNEL];
    int channelCal;

    virtual void initializeReceiver(int nbChannel) = 0;
    void initializeReceiverParam(int nbChannel = 6);
    int getRawChannelValue(short channel);
//    void readReceiver();
    void readReceiver();

    void setChannelValue(short channel,int value);

    // return the smoothed & scaled number of radians/sec in stick movement - zero centered
    const float getReceiverSIData(short channel);

};
__END_SYS
#endif // RECEIVER_H
