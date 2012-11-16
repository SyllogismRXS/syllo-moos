#ifndef __PROTOCOL_PRO4_H__
#define __PROTOCOL_PRO4_H__

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
 
#include "syllo/VideoRay/VideoRayComm/types.h"
#include "syllo/VideoRay/VideoRayComm/protocol_pro4_device_types.h"

/** @file   protocol_pro4.h
 *  @brief  Code for the standard pro4 communication protocol
 *
 *  The PRO4 protocol is a robust packetized communication protocol suitable for
 *  low latency control applications on a multi-party physical layer.  Each device
 *  on the physical bus presents a memory mapped CSR (Control Status Register) 
 *  interface.  The protocol also supports the wrapping of non memory mapped 
 *  protocols through writes to a special Custom Command Register.
 *
 *  Responses from devices can either be dumps of consecutive addresses in the 
 *  memory map, or device defined specific response packets. 
 *
 *  All data packets are composed of a 7 byte header, followed by 0 to 254 data 
 *  payload bytes, and ending in a checksum of the total packet.
 *
 *  Request packets may have no data payload.
 *
 *  The first byte in a response packet data payload is ALWAYS a VideoRay 
 *  designated device type. 
 *
 *  The PRO4 protcol is fully compatible with the Futaba RS301CR/RS302CD servos.
 *
 *  Note: when multibyte data is sent it is recommended the LSB is sent first.
 *
 *  Please see the document PRO4_Communication_Protocol.doc for more details.
 *
 *
 *  A device implementor is expected to implement the 'callback' handlers.  The 
 *  handler API definitions are below, in the User supplied packet handler API
 *  section.
 */

/** Definition packet header 
 */
typedef struct Protocol_PRO4_Packet_Header_t {
    uint8_t sync[2];         /* 0xFAAF for request or 0xFDDF for response */
    uint8_t id;
    uint8_t flags;
    uint8_t address;
    uint8_t length;
    uint8_t checksum;       /* XOR checksum of header bytes. */
} Protocol_PRO4_Packet_Header;



/** Explicit size of protocol header
  * Full packet size would be:
  *     PROTOCOL_PRO4_HEADER_SIZE  + Data Payload Length + 1
  */
#define PROTOCOL_PRO4_HEADER_SIZE 7

/**
 *  @name Indexes useful for inplace packet buffer creation 
 */
/*@{*/
/** Start of full data payload */
#define PROTOCOL_PRO4_DATA_PAYLOAD_START_INDEX  7
#define PROTOCOL_PRO4_REQUEST_DATA_PAYLOAD_START_INDEX  7
#define PROTOCOL_PRO4_RESPONSE_DATA_PAYLOAD_START_INDEX 8
/*@}*/


/**
 *  @name Start of packet synchronization bytes 
 */
/*@{*/
#define SYNC_REQUEST    0xFAAF
#define SYNC_REQUEST_1  0xFA 
#define SYNC_REQUEST_2  0xAF 

#define SYNC_RESPONSE   0xFDDF
#define SYNC_RESPONSE_1 0xFD 
#define SYNC_RESPONSE_2 0xDF 
/*@}*/
 
 
/**
 *  @name Macros and defines for Device ID byte 
 */
/*@{*/
#define ID_BROADCAST 0xFF
#define ID_MULTICAST_FLAG     0x80
#define ID_RELAY_REQUEST_FLAG 0x40
#define ID_MULTICAST_MASK   (~ID_MULTICAST_FLAG)
/**Check if ID is mutilcast or broadcast */
#define ID_IS_MULTICAST(x)  (x & ID_MULTICAST_FLAG)
/** Check if ID is a relay request */
#define ID_IS_RELAY(x)      (x & ID_RELAY_REQUEST_FLAG)
/** Get Group ID from a multicast ID */
#define ID_GROUP(x)         (x & ID_MULTICAST_MASK)
/** Set a multicast ID for a specific group ID*/
#define ID_SET_GROUP(x)     (ID_MULTICAST_FLAG | x)
/*@}*/

/**
 *  @name Macros and defines for Flag byte 
 * 
 *  The FLAG byte in a request packet defines the type of response requested.
 */
/*@{*/
#define NO_RESPONSE 0x0
#define RESPONSE_TYPE_FLAG     0x80
#define RESPONSE_CSR_DUMP      0x80
#define RESPONSE_LENGTH_MASK   (~RESPONSE_TYPE_FLAG)
/** Test if FLAG indicates a device specific response */
#define RESPONSE_IS_DEVICE_SPECIFIC(x) (!(x & RESPONSE_TYPE_FLAG))
/** Return length of the desired response, start address is in the Address byte*/
#define RESPONSE_LENGTH(x) (x & RESPONSE_LENGTH_MASK)
/** Helper to extract the device type byte from the data payload */
#define RESPONSE_DEVICE_TYPE(x) (x[0])
/** Helper to extract the actual data payload from a response */
#define RESPONSE_PAYLOAD_DATA(x) (&x[1])
/*@}*/
 
 
#define TOP_OF_CSR 0xF0
#define TOP_OF_FULL_CSR 0x100
 
/**
 *  @name Predefined Registers
 */
/*@{*/
/** Allows for the encapsulation of a custom command protocol */
#define ADDR_CUSTOM_COMMAND    0xF0
#define ADDR_CONFIG_DATA_SIZE  0xF5
#define ADDR_CONFIG_DATA       0xF7
#define ADDR_NODE_ID           0xFB
#define ADDR_GROUP_ID          0xFC
/** The device ID returns the factory programmed unique ID for this device */
#define ADDR_DEVICE_ID         0xFD
/** The Reboot register is 16-bit at addresses 0xFE & 0xFF
    Writing the value 0xDEAD into the reboot register should causes the device 
    to reboot after a delay.
*/
#define ADDR_REBOOT            0xFE
/* Note: 16-bit code is LSB First */
#define REBOOT_CODE            0xADDE
#define REBOOT_CODE_1          0xDE
#define REBOOT_CODE_2          0xAD
/*@}*/

/** The value 0xFF is reserved to indicate a data payload of larger than 254 
  * bytes
  */
#define LENGTH_EXTENDED     0xFF

/** Calculates the checksum
 *  
 *  Calculates an XOR checksum over len bytes in the buffer passed in.  
 *
 *
 *  @param buf Array of data to checksum
 *  @param len Length of buf to calculate checksum on
 *  @return XOR checksum  
 */
uint8_t protocol_pro4_checksum(char* buf, int len);

/** Build a conforming request packet header
 *  
 *  Fills buf with a properly constructed packet header.  
 *  Assumes that enough space has been alloacted in the buffer for the packet 
 *  header. 
 *
 *
 *  @param id Node id of destintion for a request packet
 *  @param flags FLAGS byte
 *  @param addr CSR memory map address
 *  @param len Length of data payload for packet associated with this header
 *  @param buf Buffer for packet header
 *  @return Number of bytes written to buf.  
 */
unsigned int protocol_pro4_build_request_header(uint8_t id, 
                                                uint8_t flags, 
                                                uint8_t addr,
                                                uint8_t len, 
                                                char *buf);


/** Build a conforming response packet header
 *  
 *  Fills buf with a properly constructed packet header. 
 *  Assumes that enough space has been alloacted in the buffer for the packet 
 *  header. 
 *
 *
 *  @param id Node id of responding device (self id)
 *  @param flags FLAGS byte
 *  @param addr CSR memory map address
 *  @param len Length of data payload for packet associated with this header, 
 *             This is just the length of the actual data, and does not include 
 *             the device type byte.  The actual length byte sent will be this value
 *             plus 1.
 *  @param device_type Designated device type for responding device
 *  @param buf Buffer for packet header
 *  @return number of bytes written to buf.  
 */
unsigned int protocol_pro4_build_response_header(uint8_t id, 
                                                 uint8_t flags, 
                                                 uint8_t addr,
                                                 uint8_t len, 
                                                 uint8_t device_type,
                                                 char *buf);                                                


/** Build a conforming response packet header for an extended length response
 *  
 *  Fills buf with a properly constructed packet header. 
 *  Assumes that enough space has been alloacted in the buffer for the extended length 
 *  packet header. 
 *
 *
 *  @param id Node id of responding device (self id)
 *  @param flags FLAGS byte
 *  @param addr CSR memory map address
 *  @param len Length of data payload for packet associated with this header, 
 *             This is just the length of the actual data, and does not include 
 *             the device type byte.  The actual length byte sent will be this value
 *             plus 1.
 *  @param device_type Designated device type for responding device
 *  @param buf Buffer for packet header
 *  @return number of bytes written to buf.  
 */
unsigned int protocol_pro4_build_response_extended_length_header(uint8_t id, 
                                                 uint8_t flags, 
                                                 uint8_t addr,
                                                 uint16_t len, 
                                                 uint8_t device_type,
                                                 char *buf);                                                


/** Build a conforming full request packet 
 *  
 *  Fills buf with a properly constructed packet header and appends the trailing checksum.  
 *  Assumes that enough space has been alloacted in the buffer for the packet 
 *  header, data payload, and trailing checksum.
 * 
 *  This function writes the header to the begining of the buffer, then
 *  calculates the total checksum.  It assumes that the datapayload exists in 
 *  the buffer and starts at the proper position (REQUEST_DATA_PAYLOAD_START_INDEX)
 *
 *
 *  @param id Node id of destintion for a request packet
 *  @param flags FLAGS byte
 *  @param addr CSR memory map address
 *  @param len Length of data payload for packet associated with this header
 *  @param buf Buffer for entire packet
 *  @return Total size of packet.  
 */
unsigned int protocol_pro4_build_request_inplace(uint8_t id, 
                                                 uint8_t flags, 
                                                 uint8_t addr,
                                                 uint8_t len, 
                                                 char *buf);                                                


/** Build a conforming full response packet 
 *  
 *  Fills buf with a properly constructed packet header and appends the trailing checksum.  
 *  Assumes that enough space has been alloacted in the buffer for the packet 
 *  header, data payload, and trailing checksum.
 * 
 *  This function writes the header to the begining of the buffer, then
 *  calculates the total checksum.  It assumes that the datapayload exists in 
 *  the buffer and starts at the proper position (REQUEST_DATA_PAYLOAD_START_INDEX)
 *
 *
 *  @param id Node id of destintion for a request packet
 *  @param flags FLAGS byte
 *  @param addr CSR memory map address
 *  @param len Length of data payload for packet associated with this header, 
 *             This is just the length of the actual data, and does not include 
 *             the device type byte.  The actual length byte sent will be this value
 *             plus 1.
 *  @param device_type Designated device type for responding device
 *  @param buf Buffer for entire packet
 *  @return Total size of packet.  
 */
unsigned int protocol_pro4_build_response_inplace(uint8_t id, 
                                                  uint8_t flags, 
                                                  uint8_t addr,
                                                  uint8_t len, 
                                                  int8_t device_type,
                                                  char *buf);   
                                        

/**
 *  @name The following functions provide a standardized parsing infratructure  
 *
 *  The parse API is intended to be passed an input stream.  The stream
 *  may not have a complete packet.  The parse routine will statically maintain 
 *  state across invocations.
 *
 *  The parse routine will call certain externally defined functions.  It is assumed
 *  that the developer will provide these in the specific application.  Of course
 *  failing to provide them will cause a linker error.
 */
/*@{*/

/** Main parser function
 *
 *  The parse routine maintains static state for the parser.
 *  It uses a total of 4 bytes statically.
 *  1 byte for parser state
 *  2 bytes for an index into the packet_buf
 *  1 byte for a running xsum
 *
 *  It is assumed that packet_buf has AT LEAST enough space for the header.
 *  The parse routine will NOT store payload data for packets addressed to other nodes.
 *
 *  @param  packet_buf Storage for current packet being parsed
 *  @param  max_size of packet_buf
 *  @param  data Input data stream byte
 *  @return Number of errors detected.  
 */
unsigned int protocol_pro4_parse(char data,
                                 char* packet_buf, 
                                 unsigned int max_size);

                                 
/**  @name User supplied packet handler API
 *
 *   Required End-user provided functions    
 *
 *   These functions MUST be implemented in a device specific file.  They 
 *   provide the device level behavior for actions on various received packets.
 *
 *   The id parameter is provided in the packet handle functions so that
 *   behavior can be tailored to device/group/broadcast requests if desired.
 *
 *   Failure to provide these will result in a linker error
 *
 *   protocol_pro3_handler_NOP.c provides a skeleton implementation
 */
/*@{*/

/** Handles request packets sent to the custom command register
 *
 *  This handler API function is called upon reception of a request packet
 *  addressed to the custom command register.
 *
 *  @param id Id of destintion for a request packet
 *  @param len Length of data buf
 *  @param buf data payload buffer
 */
extern void protocol_pro4_handle_custom_command_request(uint8_t id,
                                                        int len, 
                                                        char *buf);

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
extern void protocol_pro4_handle_custom_command_response(uint8_t id,
                                                         int len,
                                                         uint8_t device_type,
                                                         char *buf);

/** Handles the response to a device specific request packet
 *  
 *  This handler API function is called when the device has been requested to 
 *  respond with a device specific response packet.  The device is expected to 
 *  generate and transmit an appropriate response.
 *
 *  @param flags FLAGS byte
 */
extern void protocol_pro4_respond_device_specific(uint8_t flags);

/** Handles the response to a csr read request packet
 *
 *  This handler API function is called when the device has been requested to 
 *  respond with csr read response packet.  The device is expected to 
 *  generate and transmit an appropriate response.
 *
 *  @param addr CSR memory map address
 *  @param len Length of data payload for packet associated with this header
 */
extern void protocol_pro4_respond_csr_read(int addr, int len);

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
extern void protocol_pro4_csr_write(int addr,  int len, char *buf);

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
extern void protocol_pro4_handle_response(uint8_t id, 
                                          uint8_t flags,
                                          int addr, 
                                          int len,
                                          uint8_t device_type,       
                                          char *buf);

/** Allows for selective storage and processing of packets.
 *  
 * @returns True if packet should be stored and parsed.  Typically a non-host
 *          device will only accept packets with it's node or group id or the
 *          broadcast ID.
 */
 
extern char protocol_pro4_accept_id(uint8_t id, char isResponse);

/** Allows for selective relaying of packets.
 *  
 * @returns True if packet was forwarded.  Typically a non-host
 *          device will forward all packets to all other interfaces if
 *          the node id forward bit is set.
 *
 * @param data buffer containing entire packet
 *
 */
extern char protocol_pro4_relay_packet(char* data);
/*@}*/                                        

/*@}*/
#endif
