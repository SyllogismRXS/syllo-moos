#ifndef  __PRO4_SPECIFIC_RESPONSE_H__
#define  __PRO4_SPECIFIC_RESPONSE_H__

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

/** @file   pro4_specific_response.h
 *  @brief  Defines structures for the response packets, and a helper API
 *
 *          Device specific responses are sent in reply to a request where the 
 *          FLAG byte is < 0x80.  
 *      
 *          These responses allow for the tailoring of the protocol to minimize
 *          bandwidth, as sequential reads are not required.  Also, data not 
 *          available in the CSR can be transmitted as well. 
 */

/** Define PRO4 Devices specific response packets */
typedef enum {
               /** A one byte reply, just the device type in the payload */ 
               RESPONSE_ACK=1, 
               /** Send Full 3-D orientation and Depth data */
               RESPONSE_FULL_ATTITUDE_DEPTH,
               /** Send Full 3-D orientation, Depth, and Water Temp data */
               RESPONSE_FULL_ATTITUDE_DEPTH_WATER_TEMP,
               /** Send Full 3-D orientation, Depth, and Internal Sensor Status*/
               RESPONSE_FULL_ATTITUDE_DEPTH_HEALTH,
               RESPONSE_FULL_CSR
             } PRO4_ROV_RESPONSES;

/** @name Response Structures
 *  
 *  Structures defining the response packets.
 */
//@{


/** Yaw, Pitch, Roll and Depth response */
typedef struct Response_Attitude_tag {
    /** Heading in degrees */
    int16_t heading;
    /** Pitch in degrees */
    int16_t pitch;
    /** Roll in degrees */
    int16_t roll;
    /** depth in mpsi */
    int16_t depth;
} Response_Attitude;         

/** Attitude depth plus water temp */
typedef struct Response_Attitude_WaterTemp_tag {
    /** full attitude */
    Response_Attitude attitude;
    /** water temp mC*/
    int16_t temp;
} Response_Attitude_WaterTemp;            

/** Attitude depth plus water temp */
typedef struct Response_Attitude_Health_tag {
    /** full attitude */
    Response_Attitude attitude;
    /** internal temp */
    int16_t temp;
    /** internal humidity */
    int16_t rh;    
} Response_Attitude_Health_tag;            

//@}
#endif
