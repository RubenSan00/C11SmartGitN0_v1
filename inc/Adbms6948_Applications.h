/*!
******************************************************************************
Copyright (c) 2022 Analog Devices, Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc. and its
licensors. By using this software you agree to the terms of the associated
Analog Devices License Agreement.
******************************************************************************
 * @file    Adbms6948_Applications.h
 * @brief   This file Application level implementations of BMS library APIs
 * @version $Revision$
 * @date    $Date$
 *
 * Developed by: ADI BMS Applications Team, Bangalore, India
*****************************************************************************/

/*! \addtogroup Adbms6948_AppsExample
*  @{
*/

#ifndef __ADBMS6948_APPLICATIONS_H
#define __ADBMS6948_APPLICATIONS_H

#include "Adbms6948.h"
#include "common.h"

typedef struct
{
    int16_t anCellVoltage[16];        /*!< Cell Voltage Codes for all the cells: 16 bit ADC measurement */
    int32_t nCurrent;                          /*!< Current channel 1 value*/
    uint16_t nVoltageCT;                        /*!< Value of CT bits for Voltage*/
    uint16_t nCurrentCT;                        /*!< Value of CT bits for Current*/
} BMS_BUFFER;

void ExecuteCommand(int cmd);

#endif


/*! @}*/




