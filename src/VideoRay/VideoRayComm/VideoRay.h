#ifndef _VIDEO_RAY_H_
#define _VIDEO_RAY_H_

#include <string>
#include "syllo/sylloserial/serialib.h"

#define TX_BUF_SIZE 256
#define RX_BUF_SIZE 256


class VideoRay {
public:

     VideoRay();
     ~VideoRay();
     
     void setDevice(std::string device);


     int getDepth();
     int getYaw(); //heading
     int getPitch();
     int getRoll();
     
     int getYawAccel();
     int getPitchAccel();
     int getRollAccel();
     


     int setDesiredHeading(int heading);
     
     

protected:

     char txBuf[TX_BUF_SIZE];
     char rxBuf[RX_BUF_SIZE];

     serialib mSerialPort;
     std::string mDevice;

};

#endif
