/*!
******************************************************************************
Copyright (c) 2022 Analog Devices, Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc. and its
licensors. By using this software you agree to the terms of the associated
Analog Devices License Agreement.
******************************************************************************
* @file   : Std_Types.h
* @brief  : This file contains all necessary Standard Type definitions.
* @version: $Revision$
* @date   : $Date$
* Developed by: BMS Applications Team, Bangalore, India
*****************************************************************************/

#ifndef STD_TYPES_H
#define STD_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#ifndef E_OK
#define E_OK 			(Std_ReturnType)0
#endif
#ifndef E_NOT_OK
#define E_NOT_OK 		(Std_ReturnType)1
#endif


#ifndef NULL
#define NULL            0
#endif

#ifndef NULL_PTR
#define NULL_PTR        0
#endif

#ifndef TRUE
#define TRUE            true
#endif
#ifndef FALSE
#define FALSE           false
#endif

#define STD_HIGH		0x01
#define STD_LOW			0x00

#define STD_ACTIVE		0x01
#define STD_IDLE		0x00

#define STD_ON			0x01
#define STD_OFF			0x00

typedef uint8_t         Std_ReturnType;
typedef unsigned char   boolean;

typedef int8_t          sint8;
typedef uint8_t         uint8;
typedef int16_t         sint16;
typedef uint16_t        uint16;
typedef int32_t         sint32;
typedef uint32_t        uint32;
typedef int64_t         sint64;
typedef uint64_t        uint64;

typedef float           float32;
typedef double          float64;

typedef struct {
    uint16 vendorID;
    uint16 moduleID;
    uint8  instanceID;

    uint8 sw_major_version;    /**< Vendor numbers */
    uint8 sw_minor_version;    /**< Vendor numbers */
    uint8 sw_patch_version;    /**< Vendor numbers */

    uint8 ar_major_version;    /**< Autosar specification numbers */
    uint8 ar_minor_version;    /**< Autosar specification numbers */
    uint8 ar_patch_version;    /**< Autosar specification numbers */
} Std_VersionInfoType;

#endif
