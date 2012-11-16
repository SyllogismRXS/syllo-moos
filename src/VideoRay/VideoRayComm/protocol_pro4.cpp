/*
 *    (C) Copyright 2010 VideoRay LLC.
 *    Author: Andy Goldstein <andy.goldstein@videoray.com>
 *
 *    This file is part of the VideoRay C sample.
 *
 *    This code is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *      This code is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with thise code.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "syllo/VideoRay/VideoRayComm/protocol_pro4.h"
#include "syllo/sylloserial/serialib.h"

/** @file   protocol_pro4.c
 *  @brief  PRO4 Protocol Communication
 *
 *  PRO4 Communication Protocol infrastructure
 *
 *
 *  The functions implemented here are used for the parsing and handling of 
 *  PRO4 communication packets.  
 *
 *  The API function protocol_pro4_parse() is called for each data byte received.
 *  This function will parse the incoming stream, validate, and accept packets.
 *  Accepted packets will be passed to the protocol_pro4_handler() function which
 *  will call the appropriate User supplied packet handler API functions. 
 *
 */

uint8_t protocol_pro4_checksum(char* buf, int len) 
{
     uint8_t xsum=0;
     int i=0;
     for (i=0;i<len;i++) {
	  xsum ^= buf[i];
     }
     return xsum;
}


unsigned int protocol_pro4_build_request_header(uint8_t id, 
                                                uint8_t flags, 
                                                uint8_t addr, 
                                                uint8_t len, 
                                                char *buf) 
{

     buf[0]=SYNC_REQUEST_1;
     buf[1]=SYNC_REQUEST_2;
     buf[2]=id;
     buf[3]=flags;
     buf[4]=addr;
     buf[5]=len;
     buf[6]=protocol_pro4_checksum(buf,6);
     return 7;
}


unsigned int protocol_pro4_build_response_header(uint8_t id, 
                                                 uint8_t flags, 
                                                 uint8_t addr, 
                                                 uint8_t len, 
                                                 uint8_t device_type,
                                                 char *buf) 
{

     if (len!=LENGTH_EXTENDED) { //account for device type byte
	  len++;
     }

     buf[0]=SYNC_RESPONSE_1;
     buf[1]=SYNC_RESPONSE_2;
     buf[2]=id;
     buf[3]=flags;
     buf[4]=addr;
     buf[5]=len;
     buf[6]=protocol_pro4_checksum(buf,6);
     buf[7]=device_type;
     return 8;
    
}


unsigned int protocol_pro4_build_response_extended_length_header(uint8_t id, 
								 uint8_t flags, 
								 uint8_t addr,
								 uint16_t len, 
								 uint8_t device_type,
								 char *buf) 
{
     int index=0;
     int len_checksum=0;
     len++;
     unsigned char *p = (unsigned char*) &len;

     //the extended length and extended length checksum goes before the data payload, including the device type, hence the -1
     index = protocol_pro4_build_response_header(id, flags, addr, LENGTH_EXTENDED, device_type,buf) - 1;
	
     //added extended length, extended length checksum and device type
     len_checksum ^= buf[index++] = *p++;
     len_checksum ^= buf[index++] = *p++;
     buf[index++] =  len_checksum;
     buf[index++] =  device_type;

     return index;

}	

unsigned int protocol_pro4_build_request_inplace(uint8_t id, 
                                                 uint8_t flags, 
                                                 uint8_t addr,
                                                 uint8_t len, 
                                                 char *buf) 
{
     int xsum_index;
     xsum_index=PROTOCOL_PRO4_REQUEST_DATA_PAYLOAD_START_INDEX+len; 
     protocol_pro4_build_request_header(id,flags,addr,len,buf);
     buf[xsum_index] = protocol_pro4_checksum(buf,xsum_index); 
    
     return xsum_index+1;
                                                            
}                                                 


unsigned int
protocol_pro4_build_response_inplace(uint8_t id, 
                                     uint8_t flags, 
                                     uint8_t addr,
                                     uint8_t len, 
                                     int8_t device_type,
                                     char *buf) 
{
     int xsum_index;
  
     xsum_index=PROTOCOL_PRO4_RESPONSE_DATA_PAYLOAD_START_INDEX + len; 
     protocol_pro4_build_response_header(id,flags,addr,len,device_type,buf);
     buf[xsum_index] = protocol_pro4_checksum(buf,xsum_index); 
    
     return xsum_index+1;
}


/** PRO4 Protocol standard parser */
typedef enum {UNSYNC=0, 
              PRO4_SYNC,
              PRO4_HEADER,
              PRO4_PAYLOAD,
              PRO4_XSUM} Parser_State; 
              
/* Forward ref to completed packet handler */              
void protocol_pro4_handler(Protocol_PRO4_Packet_Header *header, char* data);
              
#include <stdio.h>              
unsigned int protocol_pro4_parse(char data,
                                 char* packet_buf, 
                                 unsigned int max_size) {

     static unsigned char parse_state=UNSYNC;
     static unsigned int index=0;
     static uint8_t checksum=0;
     Protocol_PRO4_Packet_Header *header;
     int err_cnt=0;
    
     /* map header struct to buffer.  Should be alignment safe since every element
      * in the header is a byte.
      */
     header = (Protocol_PRO4_Packet_Header *) packet_buf; 

     switch(parse_state) {

     case UNSYNC:
	  if ((data==(char) SYNC_REQUEST_1) || (data== (char) SYNC_RESPONSE_1)) {
	       /* got 1st sync byte */
	       index=0;
	       checksum=data;
	       packet_buf[index++]=data;
	       parse_state=PRO4_SYNC;
	  }
	  else {
	       printf("%d %d %d\r\n",data, SYNC_REQUEST_1, (data==SYNC_REQUEST_1));
	       err_cnt++;
	  }
          break;
    
     case PRO4_SYNC:
	  if ((data==(char) SYNC_REQUEST_2) || (data== (char) SYNC_RESPONSE_2)) {
	       /* got 2nd sync byte */
	       parse_state=PRO4_HEADER;
	       checksum^=data;
	       packet_buf[index++]=data;
	  }
	  else {
	       /* Error, go back to unsync state */
	       err_cnt++;
	       parse_state=UNSYNC;
	  }
          break;
          
     case PRO4_HEADER:
	  packet_buf[index++]=data;
	  if (index==PROTOCOL_PRO4_HEADER_SIZE) {
	       /* Check the header checksum */
	       if (data==checksum) {
		    if (header->length>0)
			 parse_state=PRO4_PAYLOAD;
		    else 
			 parse_state=PRO4_XSUM;
	       }
	       else {
                    err_cnt++;
                    parse_state=UNSYNC;
	       }
	  }
	  checksum^=data;
          break;
          
     case PRO4_PAYLOAD:
	  if (protocol_pro4_accept_id(header->id, 
				      header->sync[0]==SYNC_RESPONSE_1) 
	      || ID_IS_RELAY(header->id)) {
	       packet_buf[index++]=data;
	  }
	  else {
	       index++;
	  }
	  checksum^=data;

	  /* make sure we dont blow the buffer */
	  if (index>=max_size) {
	       err_cnt++;
	       parse_state=UNSYNC;
	  }

	  /* check to see if we have gotten the entire packet */
	  if (index==PROTOCOL_PRO4_HEADER_SIZE+header->length) 
               parse_state=PRO4_XSUM;
          break;
          
     case PRO4_XSUM:
	  if (data==checksum) {
	       /* It's a good complete packet */
	       if (protocol_pro4_accept_id(header->id, header->sync[0]==SYNC_RESPONSE_1)) {
		    /* it's a packet I can use */
		    protocol_pro4_handler(header,
					  &packet_buf[PROTOCOL_PRO4_DATA_PAYLOAD_START_INDEX]);
	       } 
	       else {
                    if (ID_IS_RELAY(header->id))
			 protocol_pro4_relay_packet(packet_buf);
	       }
	  }
	  /* got back to UNSYNC state after processing a packet */
	  parse_state=UNSYNC;
          break;

     default: 
	  parse_state=UNSYNC;
          break;

     }
     return err_cnt;
}


/** Handles the processing of a validated packet, and calls user supplied API */
void 
protocol_pro4_handler(Protocol_PRO4_Packet_Header *header, char* data) {

     if (header->address==ADDR_CUSTOM_COMMAND) {
	  /* It's a custom command packet */
	  if (header->sync[0]==SYNC_REQUEST_1) {
	       /* It's a custom command request */
	       protocol_pro4_handle_custom_command_request(header->id, 
							   header->length,
							   data);
	  }
	  else {
	       /* It's a custom command response */
	       protocol_pro4_handle_custom_command_response(header->id, 
							    header->length,
							    RESPONSE_DEVICE_TYPE(data),
							    RESPONSE_PAYLOAD_DATA(data));
	  }
     }
     else if (header->sync[0]==SYNC_REQUEST_1) {
	  /*It's a request packet */
	  if (header->length) {
	       /* Packet contains data to be writen into CSR map,
		* Always perform the write into the csr prior to any
		* response.
		*/
	       protocol_pro4_csr_write(header->address,  header->length, data);
	  }
	  if (header->flags && !(ID_IS_MULTICAST(header->id))) {
	       /* a response is expected, and the request is not
		  not multicast
	       */
	       if (RESPONSE_IS_DEVICE_SPECIFIC(header->flags)) {
		    protocol_pro4_respond_device_specific(header->flags);
	       }    
	       else {
		    /* A CSR read response is requested */
		    if (header->flags==RESPONSE_CSR_DUMP) 
			 protocol_pro4_respond_csr_read(header->address,
							TOP_OF_CSR-header->address);
		    else
			 protocol_pro4_respond_csr_read(header->address,
							RESPONSE_LENGTH(header->flags));
	       }
	  }     
     }
     else {
	  /* It's a response packet from some device */
	  protocol_pro4_handle_response(header->id,
					header->flags,
					header->address,
					header->length,
					RESPONSE_DEVICE_TYPE(data),
					RESPONSE_PAYLOAD_DATA(data)
	       );
     }
}

/*
 * Author: Kevin DeMarco
 */

#define IDLE_STATE             0
#define SYNC_STATE             1
#define NETWORK_ID_STATE       3
#define HDR_FLAGS_STATE        4
#define CSR_ADDR_STATE         5
#define DATA_LENGTH_STATE      6
#define HDR_CHECKSUM_STATE     7
#define EXT_LENGTH_0_STATE     8
#define EXT_LENGTH_1_STATE     9
#define EXT_LEN_CHECKSUM_STATE 10
#define DATA_STATE             11
#define CHECKSUM_STATE         12

int ParseSerialStream(serialib &serialPort, char *buf, int maxNumBytes, unsigned int TimeOut_ms)
{
     unsigned int bytes = 0;
     char ret;   
     int state = IDLE_STATE;
     int dataLength = 0;
     int isExtLength = 0;
     char rxChar;

     printf("Receiving...\n");
     while (bytes < maxNumBytes) {
	  ret = serialPort.ReadChar(&rxChar, TimeOut_ms);
	  if (ret == 1) {
	       printf("0x%02X \t State: %d\n", (unsigned char)rxChar, state);
	       switch (state) {
	       case IDLE_STATE:
		    if ((unsigned char)rxChar == (unsigned char)SYNC_RESPONSE_1) {
			 state = SYNC_STATE;
		    }
		    break;

	       case SYNC_STATE:
		    if ((unsigned char)rxChar == (unsigned char)SYNC_RESPONSE_2) {
			 state = NETWORK_ID_STATE;
		    } else {
			 return -4;
		    }
		    break;
		    
	       case NETWORK_ID_STATE:
		    state = HDR_FLAGS_STATE;
		    break;

	       case HDR_FLAGS_STATE:
		    state = CSR_ADDR_STATE;
		    break;

	       case CSR_ADDR_STATE:
		    state = DATA_LENGTH_STATE;
		    break;

	       case DATA_LENGTH_STATE:
		    dataLength = rxChar;
		    state = HDR_CHECKSUM_STATE;
		    break;
	
	       case HDR_CHECKSUM_STATE:
		    if ((unsigned char)dataLength == (unsigned char)LENGTH_EXTENDED) {
			 state = EXT_LENGTH_0_STATE;
		    } else {
			 state = DATA_STATE;
		    }
		    break;
	
	       case EXT_LENGTH_0_STATE:
		    dataLength = rxChar;
		    state = EXT_LENGTH_1_STATE;
		    break;
	
	       case EXT_LENGTH_1_STATE:
		    dataLength += (((int)rxChar) << 8) & 0xFF00;
		    state = EXT_LEN_CHECKSUM_STATE;
		    break;
		    
	       case EXT_LEN_CHECKSUM_STATE:
		    state = DATA_STATE;
		    break;
	
	       case DATA_STATE:
		    buf[bytes++] = rxChar;
		    
		    if (bytes >= dataLength) {
			 state = CHECKSUM_STATE;
		    }
		    
		    break;
	
	       case CHECKSUM_STATE:
		    state = IDLE_STATE;
		    return bytes;
		    break;
	
	       default:
		    break;
	       }
		    
	  }
	  
	  if (ret < 0) return ret;                                      // Error while reading : return the error number
     
     }
     return -3;                                                          // Buffer is full : return -3
}


////////////////
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
