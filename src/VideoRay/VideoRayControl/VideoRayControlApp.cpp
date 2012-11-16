#include <iostream>
#include <sstream>
#include <fstream>

#include "syllo/sylloserial/serialib.h"
#include "syllo/VideoRay/VideoRayComm/protocol_pro4.h"

#include "VideoRayControlApp.h"

//default constructor
VideoRayControlApp::VideoRayControlApp()
{    
} 

//default (virtual) destructor 
VideoRayControlApp::~VideoRayControlApp()
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
bool VideoRayControlApp::OnNewMail (MOOSMSG_LIST &NewMail)
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
bool VideoRayControlApp::OnConnectToServer()
{
     DoRegistrations();
     return true;
}

/*
  Called by the base class periodically. This is where you place code 
  which does the work of the application 
*/ 
bool VideoRayControlApp::Iterate()
{
     int status;
     serialib serialPort;
     char rxBuf[256];
     status = ParseSerialStream(serialPort, rxBuf, 512, 5000);
     return true;
}

/* 
   called by the base class before the first :: Iterate is called . Place 
   startup code here − especially code which reads configuration data from the 
   mission file 
*/ 
bool VideoRayControlApp::OnStartUp()
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

void VideoRayControlApp::DoRegistrations()
{
     //M_Comms.Register(It->Moosname,0);
}


/** Handles request packets sent to the custom command register
 *
 *  This handler API function is called upon reception of a request packet
 *  addressed to the custom command register.
 *
 *  @param id Id of destintion for a request packet
 *  @param len Length of data buf
 *  @param buf data payload buffer
 */
void protocol_pro4_handle_custom_command_request(uint8_t id,
                                                        int len, 
                                                        char *buf)
{
}

/** Handles response packets sent to the custom command register
 *
 *  This handler API function is called upon reception of a response packet
 *  addressed to the custom command register.
 *
 *  @param id Id of destintion for a request packet
 *  @param device_type Designated device type for responding device
 *  @param len Length of data buf
 *  @param buf Data payload buffer
*/
void protocol_pro4_handle_custom_command_response(uint8_t id,
                                                         int len,
                                                         uint8_t device_type,
                                                         char *buf)
{
}

/** Handles the response to a device specific request packet
 *  
 *  This handler API function is called when the device has been requested to 
 *  respond with a device specific response packet.  The device is expected to 
 *  generate and transmit an appropriate response.
 *
 *  @param flags FLAGS byte
 */
void protocol_pro4_respond_device_specific(uint8_t flags)
{
}

/** Handles the response to a csr read request packet
 *
 *  This handler API function is called when the device has been requested to 
 *  respond with csr read response packet.  The device is expected to 
 *  generate and transmit an appropriate response.
 *
 *  @param addr CSR memory map address
 *  @param len Length of data payload for packet associated with this header
 */
void protocol_pro4_respond_csr_read(int addr, int len)
{
}

/** Handles request packets which write data into csr registers
 *
 *  This handler API function is called when the device has been requested to 
 *  write data into it's csr file.  In general the devicce should not generate
 *  a response in this handler, as one of the respond_* API functions will be 
 *  called after this call.
 *
 *  @param addr CSR memory map address
 *  @param len Length of data payload for packet associated with this header
 *  @param buf Data payload buffer to write into CSR file
 */
void protocol_pro4_csr_write(int addr,  int len, char *buf)
{
}

/** Handles response packets sent from device id
 *
 *  This handler API function is called when the device receives and accepts 
 *  a response packet.
 *
 *  @param id Node id of device which sent the response packet
 *  @param flags FLAGS byte
 *  @param addr CSR memory map address
 *  @param len Length of data payload for packet associated with this header
 *  @param device_type Designated device type for responding device
 *  @param buf Buffer for entire packet
 *  @return Total size of packet.  
 
 */
void protocol_pro4_handle_response(uint8_t id, 
                                          uint8_t flags,
                                          int addr, 
                                          int len,
                                          uint8_t device_type,       
                                          char *buf)
{
}

/** Allows for selective storage and processing of packets.
 *  
 * @returns True if packet should be stored and parsed.  Typically a non-host
 *          device will only accept packets with it's node or group id or the
 *          broadcast ID.
 */
 
char protocol_pro4_accept_id(uint8_t id, char isResponse)
{
     return 0;
}

/** Allows for selective relaying of packets.
 *  
 * @returns True if packet was forwarded.  Typically a non-host
 *          device will forward all packets to all other interfaces if
 *          the node id forward bit is set.
 *
 * @param data buffer containing entire packet
 *
 */
char protocol_pro4_relay_packet(char* data)
{
     return 0;
}
