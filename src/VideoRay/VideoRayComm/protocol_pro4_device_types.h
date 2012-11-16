#ifndef __PROTOCOL_PRO4_DEVICE_TYPES_H__
#define __PROTOCOL_PRO4_DEVICE_TYPES_H__

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

/** @file   protocol_pro4_device_types.h
 *  @brief  Defines for VideoRay designated device types
 *
 *  These defines specify the type of device that is sending a pro4 protocol 
 *  reply message.  The device type is ALWAYS the first byte in the payload data
 *  of a reply message
 * 
 */
 
 #define DEVICE_HOST_COMPUTER           0x0
 #define DEVICE_PRO4_ROV                0x1
 #define DEVICE_MANIPULATOR             0x2
 #define DEVICE_CAMERA                  0x3
 #define DEVICE_THRUSTER_MODULE         0x4
 #define DEVICE_RADIATION_SENSOR        0x5
 #define DEVICE_CP_PROBE                0x6
 #define DEVICE_LIGHT                   0x7
 #define DEVICE_GENERIC_SENSOR_MODULE   0x8
 #define DEVICE_PROTOCOL_ADAPTER_MUX    0x10
 #define DEVICE_KCF_SMART_TETHER_NODE   0x50
 #endif
