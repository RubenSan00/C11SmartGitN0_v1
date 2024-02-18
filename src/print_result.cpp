/*!
******************************************************************************
Copyright (c) 2022 Analog Devices, Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc. and its
licensors. By using this software you agree to the terms of the associated
Analog Devices License Agreement.
******************************************************************************
 * @file    print_result.cpp
 * @brief   This file has serial print functions
 * @version $Revision$
 * @date    $Date$
 *
 * Developed by: ADI BMS Applications Team, Bangalore, India
*****************************************************************************/

/*! \addtogroup Print_Results
*  @{
*/

#include "common.h"
#include "print_result.h"
#include "BufferedSerial.h"

extern mbed::BufferedSerial serial_port;

/*!
    @brief  Print Command Menu
    @return None
*/
void PrintMenu()
{
    printf("List of ADBMS6948 commands and their numbers:\n");
    printf("Command No. : Command description\n");
    printf("1           : Write and read configuration\n");
    printf("2           : Measure cell voltages\n");
    printf("3           : Measure current\n");
    printf("4           : Measure s-channel voltages\n");
    printf("5           : Measure average cell voltages\n");
    printf("6           : Measure gpio pin voltages\n");
    printf("7           : Measure status register voltages\n");
    printf("8           : Measure continuous synchronized voltage and current\n");
    printf("9           : Configure Cell UV Threshold and observe Cell OV UV Flags\n");
    printf("10          : Write and Read from Always ON Memory\n");
    printf("\n");
    printf("Enter '0' for menu\n");
    printf("Please enter command: \n");
}

void PrintConfig(Adbms6948_DaisyChainCfgType *poConfig)
{
    printf("Refon State: %d \n", poConfig->Adbms6948_bEnableRefOn);
    printf("GPIO Pin Config: %d \n", (uint16_t)poConfig->Adbms6948_nGPIOPinCtrlValue);
    printf("OV Threshold Config: %d \n", (uint16_t)poConfig->Adbms6948_nOverVoltThreshold);
    printf("UV Threshold Config: %d \n", (uint16_t)poConfig->Adbms6948_nUnderVoltThreshold);
    printf("IIR Corner Frequency: %d \n", (uint16_t)poConfig->Adbms6948_eIIRCornerFreq);
    printf("CnS Comparison Threshold: %d \n", (uint16_t)poConfig->Adbms6948_eCSThresholdVolt);
}

void PrintCellVoltages(int16_t *paCellVoltages)
{
    for (int i=0; i<16; i++)
    {
        printf("C%d: %1.3f ", (i+1), ADBMS6948_CONVERT_CELLVOLTAGE_HEX_TO_VOLT(*paCellVoltages));
        paCellVoltages++;
    }
    printf("\n");
}

void PrintGpioVoltage(int16_t *paVoltages)
{
    for (int i=0; i<10; i++)
    {
        printf("G%d: %1.3f  ", (i+1), ADBMS6948_CONVERT_GPIO_HEX_TO_VOLT(*paVoltages));
        paVoltages++;
    }
    printf("\n");
}

void PrintCurrent(int32_t *raw_current)
{
    printf("Current Channel Voltage: %f\n", ADBMS6948_CONVERT_CURRENT_HEX_TO_VOLT(*raw_current));
}

void PrintAverageCurrent(int32_t *raw_current)
{
    printf("Avg Current Channel Voltage: %f\n", ADBMS6948_CONVERT_AVG_CURRENT_HEX_TO_VOLT(*raw_current));
}

void PrintStatusVoltage(int16_t *praw_voltages)
{
    printf("VMV: %1.4f  ", ADBMS6948_CONVERT_VMV_HEX_TO_VOLTAGE(*praw_voltages));
    praw_voltages++;
    printf("VP: %1.4f  ", ADBMS6948_CONVERT_VP_HEX_TO_VOLTAGE(*praw_voltages));
    praw_voltages++;
    printf("VREF2: %1.4f  ", ADBMS6948_CONVERT_AUX_VREF2_HEX_TO_VOLTAGE(*praw_voltages));
    praw_voltages++;
    printf("ITMP: %1.4f  ", ADBMS6948_CONVERT_AUX_ITMP_HEX_TO_CELSIUS(*praw_voltages));
    praw_voltages++;
    printf("VREF3: %1.4f  ", ADBMS6948_CONVERT_AUX_VREF3_HEX_TO_VOLTAGE(*praw_voltages));
    praw_voltages++;
    printf("VD: %1.4f  ", ADBMS6948_CONVERT_AUX_VD_HEX_TO_VOLTAGE(*praw_voltages));
    praw_voltages++;
    printf("VA: %1.4f  ", ADBMS6948_CONVERT_AUX_VA_HEX_TO_VOLTAGE(*praw_voltages));
    praw_voltages++;
    printf("VR4K: %1.4f  ", ADBMS6948_CONVERT_AUX_VR4K_HEX_TO_VOLTAGE(*praw_voltages));
    printf("\n");
}

void PrintCellOVUVFlags(uint32_t nCellUVStatus, uint32_t nCellOVStatus)
{
    printf("Cell UV Status: 0x%04X\nCell OV Status: 0x%04X\n", nCellUVStatus, nCellOVStatus);
}

void PrintAlwaysOnData(uint8_t *panDataArray)
{
    if (NULL != panDataArray)
    {
        for (int i = 0; i < 6; i++)
        {
            if (i != 5)
            {
                printf("0x%02X, ", panDataArray[i]);
            }
            else
            {
                printf("0x%02X\n", panDataArray[i]);
            }
        }
    }
    else
    {
        printf("Error!!\n");
    }
}

/*! @}*/