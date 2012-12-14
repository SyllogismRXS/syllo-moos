#include <iostream>
#include <sstream>
#include <fstream>

//#include "syllo/VideoRay/VideoRayComm/protocol_pro4.h"

#include "VideoRayApp.h"

//default constructor
VideoRayApp::VideoRayApp()
{    
} 

//default (virtual) destructor 
VideoRayApp::~VideoRayApp()
{
}

/* 
   Called by base class whenever new mail has arrived. 
   Place your code for handling mail (notifications that something 
   has changed in the MOOSDB in this function

   Parameters:
   -NewMail : std::list<CMOOSMsg> reference
   
   Return values : 
   -return true if everything went OK 
   -return false if there was problem
*/ 
bool VideoRayApp::OnNewMail (MOOSMSG_LIST &NewMail)
{
     MOOSMSG_LIST::iterator p;
     for( p = NewMail.begin() ; p != NewMail.end() ; p++ ){
    
	  CMOOSMsg & rMsg = *p;

	  /*
	    if(MOOSStrCmp(rMsg.GetKey(), it->moosName)){
	    //if(rMsg.IsDouble() && (it->moosType == moosDouble)) {
	    int value = (int)rMsg.GetDouble();
	    }

	    if(rMsg.IsString() && (it->moosType == moosString)){
	    //spawnControl.setField(it->xmosName, rMsg.GetString());
	    }
    
	    }//end Mailbox for
	  */
     }
     return true;
}

/* 
   called by the base class when the application has made contact with 
   the MOOSDB and a channel has been opened . Place code to specify what 
   notifications you want to receive here . 
*/ 
bool VideoRayApp::OnConnectToServer()
{
     DoRegistrations();
     return true;
}

/*
  Called by the base class periodically. This is where you place code 
  which does the work of the application 
*/ 
bool VideoRayApp::Iterate()
{
     int status;
     char rxBuf[256];
     
     //status = ParseSerialStream(serialPort, rxBuf, 512, 5000);
     return true;
}

/* 
   called by the base class before the first :: Iterate is called . Place 
   startup code here − especially code which reads configuration data from the 
   mission file 
*/ 
bool VideoRayApp::OnStartUp()
{
     appTick = 5;
     commsTick = 5;
     device = "/dev/ttyUSB0";

     if(!m_MissionReader.GetConfigurationParam("AppTick",appTick)){
	  MOOSTrace("Warning, AppTick not set.\n");
     }
  
     if(!m_MissionReader.GetConfigurationParam("CommsTick",commsTick)){
	  MOOSTrace("Warning, CommsTick not set.\n");
     }

     if(!m_MissionReader.GetConfigurationParam("Device",device)){
	  MOOSTrace("Warning, Device not set.\n");
     }

     SetAppFreq(appTick);
     SetCommsFreq(commsTick);
  
     DoRegistrations();

     videoRay.setDevice(device);
     
     int heading = videoRay.getHeading();

     printf("heading: %d\n", heading);

     videoRay.setDesiredHeading(0);

     //// Open serial port
     //int status;
     //status = serialPort.Open(device.c_str(), 115200);
     //if (status!=1) {
     //	  printf ("Error while opening port. Permission problem ?\n");
     //	  return status;
     //}
     //serialPort.FlushReceiver();
     //
     //char txBuf[256];
     //char rxBuf[256];

     ////// REBOOT COMMAND...
     //txBuf[0] = 0x00;
     //txBuf[1] = 0x00;
     //txBuf[2] = 0x00;
     //txBuf[3] = 0x00;
     //txBuf[4] = 0x00;
     //txBuf[5] = 0x00;
     //txBuf[6] = 0x00;
     //txBuf[7] = 0xDE;
     //txBuf[8] = 0xAD;
     //txBuf[9] = 0x00;
     //int bytes = protocol_pro4_build_request_inplace(0x01, 0x00, 0xFE, 2, txBuf);
     //////protocol_pro4_build_request_inplace(0x01, uint8_t flags, uint8_t addr, uint8_t len, char *buf);
     //status = serialPort.Write((const void *)txBuf, bytes);
     //if (status!=1) {
     //	  printf ("Error writing: reboot\n");
     //	  return status;
     //}
     //
     //bytes = ParseSerialStream(serialPort, rxBuf, 256, 5000);
     //if (bytes < 0) {
     //	  printf("Serial Rx Error: %d\n", bytes);
     //}

     //int bytes = protocol_pro4_build_request_inplace(0x01, 0x82, 0x6C, 0, txBuf);
     //protocol_pro4_build_request_inplace(0x01, uint8_t flags, uint8_t addr, uint8_t len, char *buf);
     
     //for(int i = 0; i < bytes; i++) {
     //	  printf("%d: 0x%02X\n", i, (unsigned char)txBuf[i]);
     //}
     //
     //status = serialPort.Write((const void *)txBuf, bytes);
     //if (status!=1) {
     //	  printf ("Error writing: reboot\n");
     //	  return status;
     //}
     //
     //bytes = ParseSerialStream(serialPort, rxBuf, 256, 5000);
     //if (bytes < 0) {
     //	  printf("Serial Rx Error: %d\n", bytes);
     //}
     //
     //printf("Received %d data bytes\n", bytes);

     // REBOOT COMMAND...
     //txBuf[0] = 0xFA;
     //txBuf[1] = 0xAF;
     //txBuf[2] = 0x01;
     //txBuf[3] = 0x00;
     //txBuf[4] = 0xFE;
     //txBuf[5] = 0x02;
     //txBuf[6] = 0xA8;
     //txBuf[7] = 0xDE;
     //txBuf[8] = 0xAD;
     //txBuf[9] = 0x73;
     //
     //status = serialPort.Write((const void *)txBuf, 10);
     //if (status!=1) {
     //	  printf ("Error writing: reboot\n");
     //	  return status;
     //}

     //serialPort.Close();

     return true;
}

void VideoRayApp::DoRegistrations()
{
     //M_Comms.Register(It->Moosname,0);
}
