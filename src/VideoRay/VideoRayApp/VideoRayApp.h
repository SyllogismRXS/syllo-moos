#ifndef VideoRayControl_APP_H 
#define VideoRayControl_APP_H 

#include <iostream>
#include <string>
#include <vector>

#include <MOOSLIB/MOOSApp.h>
#include "syllo/VideoRay/VideoRayComm/VideoRay.h"


class VideoRayApp : public CMOOSApp { 
 public:
  VideoRayApp(); 
  virtual ~VideoRayApp();

 protected: 

  double appTick;
  double commsTick;
  
  std::string device;

  VideoRay videoRay;

  //where we handle new mail
  bool OnNewMail(MOOSMSG_LIST &NewMail); 

  //where we do the work 
  bool Iterate(); 

  //called when we connect to the server 
  bool OnConnectToServer();
		
  //called when we are starting up..
  bool OnStartUp();

  void DoRegistrations();

};
 
#endif
