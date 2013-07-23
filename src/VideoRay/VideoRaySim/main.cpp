#include <string.h>

#include "VideoRaySimApp.h"

int main(int argc, char * argv[])
{

  std::string sMissionFile = "Mission.moos";
  std::string sMOOSName = argv[0];
  
  switch ( argc ) { 
  case 3:
    //command line says don’t register with default name
    sMOOSName = argv[2]; 
  case 2:
    //command line says don’t use default ”mission.moos” config file
    sMissionFile = argv [1];
  }
  
  //make an application
  VideoRaySimApp TheApp;

  //run forever pasing registration name and mission file parameters
  TheApp.Run(sMOOSName.c_str(), sMissionFile.c_str() ) ;
  
  //probably will never get here ..
  return 0; 
}
