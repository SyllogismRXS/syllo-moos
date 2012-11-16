#ifndef VideoRayControl_APP_H 
#define VideoRayControl_APP_H 

#include <iostream>
#include <string>
#include <vector>

#include <MOOSLIB/MOOSApp.h>

class VideoRayControlApp : public CMOOSApp { 
 public:
  VideoRayControlApp(); 
  virtual ~VideoRayControlApp();

 protected: 

  double appTick;
  double commsTick;

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
