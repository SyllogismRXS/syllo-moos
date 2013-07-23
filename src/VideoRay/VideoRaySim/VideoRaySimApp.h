#ifndef VideoRaySim_APP_H 
#define VideoRaySim_APP_H 

#include <iostream>
#include <string>
#include <vector>

#include <MOOS/libMOOS/App/MOOSApp.h>

class VideoRaySimApp : public CMOOSApp { 
 public:
  VideoRaySimApp(); 
  virtual ~VideoRaySimApp();

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
