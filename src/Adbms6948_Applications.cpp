/*!
******************************************************************************
Copyright (c) 2022 Analog Devices, Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc. and its
licensors. By using this software you agree to the terms of the associated
Analog Devices License Agreement.
******************************************************************************
 * @file    Adbms6948_Applications.cpp
 * @brief   This file Application level implementations of BMS library APIs
 * @version $Revision$
 * @date    $Date$
 *
 * Developed by: ADI BMS Applications Team, Bangalore, India
*****************************************************************************/

/*! \addtogroup Adbms6948_AppsExample
*  @{
*/

#include "Adbms6948_Applications.h"
#include "print_result.h"

const uint32_t LOOP_MEASUREMENT_COUNT = 10;      /*! Loop measurment count */

const uint8_t knChainId = 0u;
uint8_t nNumCells = 16u;

Adbms6948_DaisyChainCfgType oDaisyChainConfig = {
    TRUE,       /* Enable reference ON */
    TRUE,       /* Init Enable */
    1,          /* No of devices */
    &nNumCells, /* Cells per device */
    0x07FFu,    /* GPIO Pin control Selection */
    0x4E2u,     /* Over voltage threshold (val * 16 * 100uV) */
    0x271u,     /* Under voltage threshold (val * 16 * 100uV) */
    ADBMS6948_FILTER_DISABLED,
    ADBMS6948_CTH_VOLT_22_5mv,
    ADBMS6948_GPIO11_GPIO_MODE, /*GPIO11 mode*/
};

Adbms6948_RunTimeErrorCfgType oRuntimeErrConfig = {
    /* Adbms6948_E_Invalid_PEC */
    {
        (uint8_t)STD_ON,
        (uint16_t)0x14U
    },

    /* Adbms6948_E_CommandCount */
    {
        (uint8_t)STD_ON,
        (uint16_t)0x15U
    },

    /* Adbms6948_E_Platform */
    {
        (uint8_t)STD_ON,
        (uint16_t)0x16U
    }
};

Adbms6948_CfgType koConfig = {
    &oDaisyChainConfig,
    &oRuntimeErrConfig
};

BMS_BUFFER oBmsBuffer[LOOP_MEASUREMENT_COUNT] = {0};

void Adbms6948_read_config(const uint8_t knChainId, int nRet);
void Adbms6948_measure_cells(const uint8_t knChainId, int nRet);
void Adbms6948_measure_current(const uint8_t knChainId, int nRet);
void Adbms6948_measure_schannel(const uint8_t knChainId, int nRet);
void Adbms6948_measure_average_cell(const uint8_t knChainId, int nRet);
void Adbms6948_trig_cont_measurement(const uint8_t knChainId, int nRet);
void Adbms6948_read_cont_measurement(const uint8_t knChainId, int nRet, uint8_t loop);
void Adbms6948_measure_aux_voltage(const uint8_t knChainId, int nRet);
void Adbms6948_measure_status_voltage(const uint8_t knChainId, int nRet);
void Adbms6948_configure_read_cell_ovuv_flags(const uint8_t knChainId, int nRet);
void Adbms6948_write_read_always_on_memory(const uint8_t knChainId, int nRet);

void parseCellOVUVStatus(const uint32_t nCellOVUVStatus, uint16_t *const pnCellOVStatus, uint16_t * const pnCellUVStatus);

/*!
    @brief  Execute command

    @param[in]  cmd User Command

    @return None
*/
void ExecuteCommand(int cmd)
{
    int nRet = 0;
    uint8_t loop_count = 0;
    nRet = Adbms6948_Init(&koConfig);

    switch(cmd) {
        case 0:
            PrintMenu();
            break;

        case 1:
            Adbms6948_read_config(knChainId, nRet);
            break;

        case 2:
            Adbms6948_measure_cells(knChainId, nRet);
            break;

        case 3:
            Adbms6948_measure_current(knChainId, nRet);
            break;

        case 4:
            Adbms6948_measure_schannel(knChainId, nRet);
            break;

        case 5:
            Adbms6948_measure_average_cell(knChainId, nRet);
            break;

        case 6:
            Adbms6948_measure_aux_voltage(knChainId, nRet);
            break;

        case 7:
            Adbms6948_measure_status_voltage(knChainId, nRet);
            break;

        case 8:
            Adbms6948_trig_cont_measurement(knChainId, nRet);
            while(loop_count < LOOP_MEASUREMENT_COUNT) {
                Adbms6948_read_cont_measurement(knChainId, nRet, loop_count);
                loop_count++;
            }
            loop_count = 0;
            while(loop_count < LOOP_MEASUREMENT_COUNT) {
                PrintCellVoltages(&oBmsBuffer[loop_count].anCellVoltage[0]);
                PrintAverageCurrent(&oBmsBuffer[loop_count].nCurrent);
                printf("Voltage CT: %d, Current CT: %d\n", oBmsBuffer[loop_count].nVoltageCT, oBmsBuffer[loop_count].nCurrentCT);
                loop_count++;
            }
            break;
        case 9:
            Adbms6948_configure_read_cell_ovuv_flags(knChainId, nRet);
            break;

        case 10:
            Adbms6948_write_read_always_on_memory(knChainId, nRet);
            break;

        default:
            printf("Invalid Command Entered \n");
            break;

    }
    Adbms6948_DeInit();
}

void Adbms6948_read_config(const uint8_t knChainId, int nRet)
{
    if(nRet == 0) {
        printf("Printing Configuration Values\n");
        PrintConfig(&oDaisyChainConfig);
    } else {
        printf("Write Failed\n");
    }
}

void Adbms6948_measure_cells(const uint8_t knChainId, int nRet)
{
    int16_t anCellVolt[16];

    Adbms6948_TrigCADCInputs oCADCInputs{
        0,
        0,
        0,
        ADBMS6948_CELL_OW_NONE};
    if(nRet == 0) {

        nRet = Adbms6948_TrigCADC(&oCADCInputs, knChainId);
        wait_us(1100); //1.1ms

        nRet = Adbms6948_ReadCellVolt(ADBMS6948_CELL_MEAS_DATA,
                                      ADBMS6948_CELL_GRP_SEL_ALL,
                                      &anCellVolt[0],
                                      ADBMS6948_SEND_NONE,
                                      knChainId);
        printf("%d\n", nRet);
        printf("Printing Cell Voltages\n");
        PrintCellVoltages(&anCellVolt[0]);
    }

    else {
        printf("Init Failed\n");
    }
}


void Adbms6948_measure_current(const uint8_t knChainId, int nRet)
{
    int32_t nCurrent;

    Adbms6948_TrigI1ADCInputs oI1ADCInputs{
        0,
        0,
        ADBMS6948_CURRENT_OW_NONE};
    if(nRet == 0) {

        Adbms6948_TrigI1ADC(&oI1ADCInputs, knChainId);
        wait_us(1100); //1.1ms
        Adbms6948_ReadCurrent(ADBMS6948_CURRENT_MEAS_DATA,
                              ADBMS6948_CURRENT_I1,
                              &nCurrent,
                              ADBMS6948_SEND_NONE,
                              knChainId);
        PrintCurrent(&nCurrent);
    }

    else {
        printf("Init Failed\n");
    }
}

void Adbms6948_measure_schannel(const uint8_t knChainId, int nRet)
{
    int16_t anSPinVolt[16];

    Adbms6948_TrigCADCInputs oCADCInputs{
        0,
        0,
        0,
        ADBMS6948_CELL_OW_NONE};
    if(nRet == 0) {

        Adbms6948_TrigSADC(
            0,
            0,
            ADBMS6948_CELL_OW_NONE,
            knChainId);
        wait_us(8800); //8.8ms
        Adbms6948_ReadCellVolt(ADBMS6948_CELL_MEAS_DATA_REDUNDANT,
                               ADBMS6948_CELL_GRP_SEL_ALL,
                               &anSPinVolt[0],
                               ADBMS6948_SEND_NONE,
                               knChainId);
        printf("Printing Spin Voltages\n");
        PrintCellVoltages(&anSPinVolt[0]);
    }

    else {
        printf("Init Failed\n");
    }
}

void Adbms6948_measure_average_cell(const uint8_t knChainId, int nRet)
{
    int16_t anAvgCellVolt[16];

    Adbms6948_TrigCADCInputs oCADCInputs{
        1,
        0,
        0,
        ADBMS6948_CELL_OW_NONE};
    if(nRet == 0) {

        Adbms6948_TrigCADC(&oCADCInputs, knChainId);
        wait_us(8800); //8.8ms
        Adbms6948_ReadCellVolt(ADBMS6948_CELL_MEAS_DATA_AVERAGED,
                               ADBMS6948_CELL_GRP_SEL_ALL,
                               &anAvgCellVolt[0],
                               ADBMS6948_SEND_NONE,
                               knChainId);
        printf("Printing Average Cell Voltages\n");
        PrintCellVoltages(&anAvgCellVolt[0]);
        Adbms6948_SendSoftReset(knChainId);
    }

    else {
        printf("Init Failed\n");
    }
}

void Adbms6948_trig_cont_measurement(const uint8_t knChainId, int nRet)
{
    /*Trigger C-ADC and I1-ADC simultaneously in Continuous mode*/
    Adbms6948_TrigIVADCInputs oTrigIVADCInputs = {
        TRUE,                   /*Adbms6948_bContMode*/
        TRUE,                   /*Adbms6948_bEnRedun*/
        FALSE,                  /*Adbms6948_bEnSSDP*/
        FALSE,                  /*Adbms6948_bRstIIRFltr*/
        ADBMS6948_CELL_OW_NONE  /*Adbms6948_eOWSel*/
    };

    if(nRet==0) {
        nRet |= Adbms6948_TrigIVADC(&oTrigIVADCInputs,  knChainId);

    } else {
        printf("Init Failed\n");
    }
}

void Adbms6948_read_cont_measurement(const uint8_t knChainId, int nRet, uint8_t loop)
{
    if(nRet==0) {
        wait_us(1100); //1.1ms
        nRet = Adbms6948_ReadCellVoltCurrentAverage(
                   &oBmsBuffer[loop].anCellVoltage[0],
                   &oBmsBuffer[loop].nCurrent,
                   TRUE,
                   &oBmsBuffer[loop].nVoltageCT,
                   &oBmsBuffer[loop].nCurrentCT,
                   knChainId);
    } else {
        printf("Init Failed\n");
    }
}

void Adbms6948_measure_aux_voltage(const uint8_t knChainId, int nRet)
{
    int16_t anGpioVoltage[11];

    Adbms6948_TrigAuxADC(
        0,
        0,
        ADBMS6948_AUX_CH_ALL,
        knChainId);
    wait_us(20000); //20ms

    Adbms6948_ReadGPIOInputVolt(
        ADBMS6948_GPIO_MEAS_DATA,
        ADBMS6948_GPIO_GRP_ALL,
        &anGpioVoltage[0],
        knChainId);
    PrintGpioVoltage(&anGpioVoltage[0]);
}

void Adbms6948_measure_status_voltage(const uint8_t knChainId, int nRet)
{
    int16_t anStatVoltage[8];

    Adbms6948_TrigAuxADC(
        0,
        0,
        ADBMS6948_AUX_CH_ALL,
        knChainId);
    wait_us(20000); //20ms

    Adbms6948_ReadDevParam(
        ADBMS6948_DEV_PARAMS_ALL,
        &anStatVoltage[0],
        knChainId);
    PrintStatusVoltage(&anStatVoltage[0]);
}

void Adbms6948_configure_read_cell_ovuv_flags(const uint8_t knChainId, int nRet)
{
    /*VUV (in V) = (16 * 150e-6 * VUV[15:0]) + 1.5*/
    /*0xD1 corresponds to VUV[15:0] bitfield in Status D Register*/
    Adbms6948_UVThresCfgType uvThresh = 0xD1;
    uint32_t nCellOVUVStatus = 0x0;
    uint16_t nCellOVStatus = 0x0;
    uint16_t nCellUVStatus = 0x0;
    int16_t anCellVolt[16];

    if (E_OK == nRet) {
        /*Configure the Cell UV Threshold*/
        nRet |= Adbms6948_CfgUVThreshold(&uvThresh, knChainId);
        Adbms6948_TrigCADCInputs oCADCInputs{
            0,
            0,
            0,
            ADBMS6948_CELL_OW_NONE};

        /*Trigger C-ADC conversions*/
        nRet |= Adbms6948_TrigCADC(&oCADCInputs, knChainId);
        wait_us(1100); //1.1ms
        /*Read the Cell Voltages*/
        nRet |= Adbms6948_ReadCellVolt(ADBMS6948_CELL_MEAS_DATA,
                                    ADBMS6948_CELL_GRP_SEL_ALL,
                                    &anCellVolt[0],
                                    ADBMS6948_SEND_NONE,
                                    knChainId);
        /*Read Cell OV, UV Status*/
        nRet |= Adbms6948_ReadCellOVUVStatus(&nCellOVUVStatus, ADBMS6948_SEND_NONE, knChainId);

        if (E_OK == nRet) {
            printf("Printing Cell Voltages\n");
            PrintCellVoltages(&anCellVolt[0]);
            parseCellOVUVStatus(nCellOVUVStatus, &nCellOVStatus, &nCellUVStatus);
            PrintCellOVUVFlags(nCellUVStatus, nCellOVStatus);
        }
        else {
            printf("Error\n");
        }
    }
    else {
        printf("Init Failed\n");
    }
}

void Adbms6948_write_read_always_on_memory(const uint8_t knChainId, int nRet)
{
    uint8_t anAlwaysOnMemDataWrite[8] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint8_t anAlwaysOnMemReadData[8] = {0};
    bool result = false;
    if (E_OK != nRet) {
        printf("Init Failed\n");
    }
    else {
        /*Read from Always On Memory in anAlwaysOnMemReadData[]*/
        /*TODO: Add code to read from Always On Memory*/
        /*===============User Code Start ================*/


        /*===============User Code End ==================*/

        printf("Data in the memory before writing: \n");
        PrintAlwaysOnData(&anAlwaysOnMemReadData[0]);


        /*Write contents of anAlwaysOnMemDataWrite[] to Always On Memory*/
        /*TODO: Add code to write to Always On Memory*/
        /*===============User Code Start ================*/


        /*===============User Code End ==================*/

        printf("Data to write: \n");
        PrintAlwaysOnData(&anAlwaysOnMemDataWrite[0]);


        /*Read Data from Always On Memory*/
        /*TODO: Add code to read from Always On Memory*/
        /*===============User Code Start ================*/


        /*===============User Code End ==================*/

        printf("Data in the memory after writing: \n");
        PrintAlwaysOnData(&anAlwaysOnMemReadData[0]);

        /*Compare the data read with data written of 6 bytes*/
        /*TODO: Compare the data written and read and store in result variable*/
        /*===============User Code Start ================*/


        /*===============User Code End ==================*/

        printf("Data comparison result: \n");
        if ((true == result) && (E_OK == nRet)) {
            printf("Success\n");
        }
        else {
            printf("Error: Data read is different from data written\n");
            printf("Data Written was: ");
            PrintAlwaysOnData(&anAlwaysOnMemDataWrite[0]);
            printf("Data Read is: ");
            PrintAlwaysOnData(&anAlwaysOnMemReadData[0]);
        }

        /*TODO: (Optional) Write below data to Always ON Memory
        so that in the next iteration different data is read than that in the IC originally*/
        memset(&anAlwaysOnMemDataWrite[0], 0x55, 6);
        /*===============User Code Start ================*/


        /*===============User Code End ==================*/
    }
}

void parseCellOVUVStatus(const uint32_t nCellOVUVStatus, uint16_t *const pnCellOVStatus, uint16_t * const pnCellUVStatus)
{
    uint8_t nBit = 0;
    *pnCellOVStatus = 0;
    *pnCellUVStatus = 0;
    /*Parse data for Odd numbered indices from the STATD content*/
    for (int i = 0; i < 31; i+=2)
    {
        nBit = (((nCellOVUVStatus & (1 << (i + 1))) >> (i+1)) & 0x01);
        *pnCellOVStatus |= (nBit << (i/2));
    }

    /*Parse data for Even numbered indices from the STATD content*/
    for (int i = 0; i < 31; i+=2)
    {
        nBit = (((nCellOVUVStatus & (1 << i)) >> i) & 0x01);
        *pnCellUVStatus |= (nBit << (i/2));
    }
    return;
}

/*! @}*/