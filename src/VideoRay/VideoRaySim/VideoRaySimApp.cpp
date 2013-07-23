#include <iostream>
#include <sstream>
#include <fstream>

#include "VideoRaySimApp.h"

//default constructor
VideoRaySimApp::VideoRaySimApp()
{    
} 

//default (virtual) destructor 
VideoRaySimApp::~VideoRaySimApp()
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
bool VideoRaySimApp::OnNewMail (MOOSMSG_LIST &NewMail)
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
bool VideoRaySimApp::OnConnectToServer()
{
  DoRegistrations();
  return true;
}

/*
  Called by the base class periodically. This is where you place code 
  which does the work of the application 
*/ 
bool VideoRaySimApp::Iterate()
{
  return true;
}

/* 
   called by the base class before the first :: Iterate is called . Place 
   startup code here − especially code which reads configuration data from the 
   mission file 
*/ 
bool VideoRaySimApp::OnStartUp()
{
  appTick = 5;
  commsTick = 5;

  if(!m_MissionReader.GetConfigurationParam("AppTick",appTick)){
    MOOSTrace("Warning, AppTick not set.\n");
  }
  
  if(!m_MissionReader.GetConfigurationParam("CommsTick",commsTick)){
    MOOSTrace("Warning, CommsTick not set.\n");
  }

  SetAppFreq(appTick);
  SetCommsFreq(commsTick);
  
  DoRegistrations();
  return true;
}

void VideoRaySimApp::DoRegistrations()
{
  //M_Comms.Register(It->Moosname,0);
}
