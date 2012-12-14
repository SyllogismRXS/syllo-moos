#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "VideoRay.h"
#include "syllo/VideoRay/VideoRayComm/protocol_pro4.h"

VideoRay::VideoRay() {}

VideoRay::~VideoRay()
{
     mSerialPort.Close();
}


void VideoRay::setDevice(std::string device)
{
     mDevice = device;

     int status;
     status = mSerialPort.Open(mDevice.c_str(), 115200);
     if (status!=1) {
     	  printf ("Error while opening port. Permission problem ?\n");
     	  exit(-1);
     }
     mSerialPort.FlushReceiver();
}

int VideoRay::setDesiredHeading(int heading)
{
     txBuf[0] = 0x00;
     txBuf[1] = 0x00;
     txBuf[2] = 0x00;
     txBuf[3] = 0x00;
     txBuf[4] = 0x00;
     txBuf[5] = 0x00;
     txBuf[6] = 0x00;

     // Turn on Auto heading
     //txBuf[7] = 0x55;
     //txBuf[8] = 0x55;

     //Turn off Auto heading
     txBuf[7] = 0xFF;
     txBuf[8] = 0xFF;

     txBuf[9] = 0x00;
 
     int bytes = protocol_pro4_build_request_inplace(0x01, 0x00, 0x0D, 2, txBuf);
     //protocol_pro4_build_request_inplace(0x01, uint8_t flags, uint8_t addr, uint8_t len, char *buf);
     
     for(int i = 0; i < bytes; i++) {
     	  printf("%d: 0x%02X\n", i, (unsigned char)txBuf[i]);
     }
     
     int status = mSerialPort.Write((const void *)txBuf, bytes);
     if (status!=1) {
     	  printf ("Error writing: reboot\n");
     	  return status;
     }
     
     bytes = ParseSerialStream(mSerialPort, rxBuf, 256, 5000);
     if (bytes < 0) {
     	  printf("Serial Rx Error: %d\n", bytes);
     }
     
     printf("Received %d data bytes\n", bytes);

    
     return 0;
}

int VideoRay::getHeading()
{
     int bytes = protocol_pro4_build_request_inplace(0x01, 0x82, 0x68, 0, txBuf);
     //protocol_pro4_build_request_inplace(0x01, uint8_t flags, uint8_t addr, uint8_t len, char *buf);
     
     for(int i = 0; i < bytes; i++) {
     	  printf("%d: 0x%02X\n", i, (unsigned char)txBuf[i]);
     }
     
     int status = mSerialPort.Write((const void *)txBuf, bytes);
     if (status!=1) {
     	  printf ("Error writing: reboot\n");
     	  return status;
     }
     
     bytes = ParseSerialStream(mSerialPort, rxBuf, 256, 5000);
     if (bytes < 0) {
     	  printf("Serial Rx Error: %d\n", bytes);
     }
     
     printf("Received %d data bytes\n", bytes);

     return 0;
}
