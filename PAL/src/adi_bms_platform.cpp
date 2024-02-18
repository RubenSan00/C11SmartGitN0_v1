/*!
******************************************************************************
Copyright (c) 2022 Analog Devices, Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc. and its
licensors. By using this software you agree to the terms of the associated
Analog Devices License Agreement.
******************************************************************************
* @file   : adi_bms_platform.cpp
* @brief  : This file contains all necessary platform function definitions for BMS.
* @version: $Revision$
* @date   : $Date$
* Developed by: BMS Applications Team, Bangalore, India
*****************************************************************************/

/*! \addtogroup PAL Platform Abstraction Layer
 *  @{
 */

/*! \addtogroup BMS_PAL BMS Platform Abstraction Layer
 *  @{
 */

/*============= I N C L U D E S =============*/
#include "adi_bms_platform.h"
#include "mbed.h"


/*============== D E F I N E S ===============*/
/*This macro is used from BMS drivers to calculate the data length*/
/*! Register Data length with PEC  */
#define ADBMS6948_CMD_DATA_LEN                  ((uint8_t)0x04u)

/*! Register Data length without PEC per device  */
#define ADBMS6948_REG_DATA_LEN_WITHOUT_PEC      ((uint8_t)0x06u)

/*! PEC length */
#define ADBMS6948_PEC_LEN                       ((uint8_t)0x02u)

/*Uncomment below macro to print each SPI transaction data*/
/*! Macro to enable printing of SPI values sent/received from BMS IC*/
//#define _ADI_BMS_DEBUG_

/******************************************************************************/
/****************************** PLATFORM DATA *********************************/
/******************************************************************************/
/* Globals */
uint32 ErrorCounter = 0;
uint32 timer_period = 0;

void Det_ReportError(uint16 ModId,uint8 InstId,uint8 ApiId,uint8 ErrCode);

/*!
    @brief          Initializes the PAL layer.

 */
void adi_pal_Init(void)
{
}

/*!
    @brief          Initializes the SPI module for BMS IC.

 */
void adi_pal_SPI_Init(void)
{
}

/*!
    @brief          Reports the development error.

    @param[in]      ModId   ID of the module.
    @param[in]      InstId  ID of the instance.
    @param[in]      ApiId   ID of the API.
    @param[in]      ErrCode Error code returned by the API.
 */
void adi_pal_ReportDevelopmentError
(
    uint16  ModId,
    uint8  InstId,
    uint8  ApiId,
    uint8  ErrCode
)
{
    Det_ReportError(ModId, InstId, ApiId, ErrCode);
}

/*!
    @brief          Reports runtime error with Error ID and status

    @param[in]      ErrorId Error ID.
    @param[in]      Status  Status of the error.
 */
void adi_pal_ReportRuntimeError
(
    uint16  ErrorId,
    uint8  Status
)
{
#ifdef _ADI_DEBUG_
    printf("Runtime error- ErrID: 0x%04X, Status: 0x%02X", ErrorId, Status);
#endif
}

/*!
    @brief          Platform layer implementation of memset function.

    Sets @c nVal as the value for @c nSize number of elements starting from the
    location pointed to by @c pAddr.

    @param[in]      pAddr   Pointer to the location to start setting values.
    @param[in]      nVal    Value to set.
    @param[in]      nSize   Number of elements to set.
 */
void adi_pal_Memset
(
    uint8 *  pAddr,
    uint8  nVal,
    uint32  nSize
)
{
    (void) memset(pAddr, nVal, nSize);
}

/*!
    @brief          Platform layer implementation of memcpy function.

    Copies @c nSize number of bytes from location pointed to by
    @c pSrcAddr to @c pDstAddr.

    @param[in]      pDstAddr    Pointer to the destination address.
    @param[in]      pSrcAddr    Pointer to the source address.
    @param[in]      nSize       Number of elements to copy.
 */
void adi_pal_Memcpy
(
    uint8 *  pDstAddr,
    uint8 *  pSrcAddr,
    uint32  nSize
)
{
    memcpy(pDstAddr, pSrcAddr, nSize);
}

/*!
    @brief          Transmits data pointed to by @c pData to BMS IC over SPI.

    @param[in]      pData       Pointer to the data to transmit.
    @param[in]      nDataLen    Number of bytes of data to transmit.
    @param[in]      nChainID    Daisy chain ID to perform the operation.
 */
void adi_pal_SpiWrite
(
    uint8 *pData,
    uint16 nDataLen,
    uint8  nChainID
)
{
    uint8_t rx_data[nDataLen];

    spi.lock();
    chip_select = 0;
    spi.write((char *)pData, nDataLen, (char *)rx_data, nDataLen);
    chip_select = 1;
    spi.unlock();
#ifdef _ADI_BMS_DEBUG_
    printf("Data TX: \n");
    for(uint8 nIdx = 0; nIdx < nDataLen; ++nIdx) {
        if (nIdx != nDataLen-1) {
            printf("0x%02X, ", pData[nIdx]);
        } else {
            printf("0x%02X\n", pData[nIdx]);
        }
    }
#endif
}

/*!
    @brief          Sends data and then receives the response data from BMS IC
                    over SPI.

    @param[in]      pTxBuf      Pointer to the buffer to transmit.
    @param[in]      pRxBuf      Pointer to the buffer to receive data in.
    @param[in]      DataCfg     Specifies the data buffers.
                                DataCfg[15:0]: Specifies length of all data buffer.
                                DataCfg[31:16]: Specifies number of data buffer.
    @param[in]      knChainId   Daisy chain ID to perform the operation.
 */
void adi_pal_SpiWriteReads (uint8 pTxBuf[8][PAL_MUL_SPI_BUFF_SIZE], uint8 pRxBuf[8][PAL_MUL_SPI_BUFF_SIZE],  uint32 DataCfg, uint8 knChainId )
{
    uint8_t reg_group_cnt;
    uint8_t reg_group_total = (DataCfg >> 16u);
    uint16_t nGroupDataLen = (uint16_t)(((uint16_t)DataCfg) - ADBMS6948_CMD_DATA_LEN);
    uint16_t data_size = (ADBMS6948_CMD_DATA_LEN + nGroupDataLen);
    uint8_t cmd[data_size];

    for (reg_group_cnt = 0; reg_group_cnt < reg_group_total ; reg_group_cnt++) {
        spi.lock();
        chip_select = 0;

        memcpy(&cmd[0], &pTxBuf[reg_group_cnt][0], ADBMS6948_CMD_DATA_LEN); /* dst, src, size */
        spi.write((char *)cmd, data_size,(char *)cmd, data_size);
        memcpy(&pRxBuf[reg_group_cnt][ADBMS6948_CMD_DATA_LEN], &cmd[ADBMS6948_CMD_DATA_LEN], nGroupDataLen); /* dst, src, size */

        chip_select = 1;
        spi.unlock();
#ifdef _ADI_BMS_DEBUG_
        printf("Data TX: \n");
        uint8_t nDataLen = ADBMS6948_CMD_DATA_LEN;
        for(uint8_t nIdx = 0; nIdx < nDataLen; ++nIdx) {
            if (nIdx != nDataLen-1) {
                printf("0x%02X, ", pTxBuf[reg_group_cnt][nIdx]);
            } else {
                printf("0x%02X\n", pTxBuf[reg_group_cnt][nIdx]);
            }
        }

        printf("Data RX: \n");
        nDataLen = ADBMS6948_CMD_DATA_LEN + nGroupDataLen;
        for(uint8_t nIdx = ADBMS6948_CMD_DATA_LEN; nIdx < nDataLen; ++nIdx) {
            if (nIdx != nDataLen-1) {
                printf("0x%02X, ", pRxBuf[reg_group_cnt][nIdx]);
            } else {
                printf("0x%02X\n", pRxBuf[reg_group_cnt][nIdx]);
            }
        }
#endif
    }
}

/*!
    @brief          Write "Read All" command and receive the data over SPI.

    @param[in]      pTxBuf      Pointer to the transmit buffer.
    @param[in]      pRxBuf      Pointer to the receive buffer.
    @param[in]      nDataLen    Length of data to receive.
    @param[in]      nChainID    Daisy chain ID to perform the operation.
 */
void adi_pal_SpiWriteReadAll(uint8 *pTxBuf, uint8 *pRxBuf, uint32 nDataLen, uint8 nChainID)
{
    uint16_t data_size = (ADBMS6948_CMD_DATA_LEN + nDataLen);
    uint8_t cmd[data_size];

    spi.lock();
    chip_select = 0;

    memcpy(&cmd[0], pTxBuf, ADBMS6948_CMD_DATA_LEN); /* dst, src, size */
    spi.write((char *)cmd, data_size,(char *)cmd, data_size);
    memcpy(pRxBuf, &cmd[ADBMS6948_CMD_DATA_LEN], nDataLen); /* dst, src, size */

    chip_select = 1;
    spi.unlock();

}

/*!
    @brief          Returns status to indicate whether the platform layer has error.

    @param[in]      nChainID    Daisy chain ID to perform the operation.
    @return         @c TRUE  - If Platform error is there.  \n
                    @c FALSE - If Platform error is not there.
 */
boolean adi_pal_IsError
(
    uint8 nChainID
)
{
    boolean bRet = FALSE;

    return bRet;
}

/*!
    @brief          Starts a blocking timer with @c nPeriodInUS number of
                    microseconds as the delay.

    @param[in]      nPeriodInUS     Delay in microseconds.
    @param[in]      nChainID        Daisy chain ID to perform the operation.
 */
void adi_pal_TimerDelay
(
    uint32 nPeriodInUS,
    uint8  nChainID
)
{
    wait_us(nPeriodInUS);

}

/*!
    @brief          De-initializes the PAL layer.

 */
void adi_pal_DeInit(void)
{

}

/*!
    @brief          Returns the timeout status of non-blocking timer.

    @param[in]      nChainID    Daisy chain ID to perform the operation.
    @return         @c TRUE  - If the timer has expired.    \n
                    @c FALSE - If the timer has not expired.
 */
boolean adi_pal_GetTimeoutStatus
(
    uint8 nChainID
)
{
    boolean bRet = 1;
    if(timer_period > timer.elapsed_time().count()) {
        bRet = 0;
    }
    return bRet;
}

/*!
    @brief          Starts a non-blocking timer with the delay provided

    @param[in]      nDelay      Delay in microseconds.
    @param[in]      nChainID    Daisy chain ID to perform the operation.
 */
void adi_pal_TimerStart
(
    uint32  nDelay,
    uint8  nChainID
)
{
    timer_period = nDelay;
    timer.start();
}

/*!
    @brief          This function reports the error reported by a BMS service

    @param[in]      ModId   Module ID
    @param[in]      InstId  Instance ID
    @param[in]      ApiId   API ID
    @param[in]      ErrCode Error code
 */
void Det_ReportError(uint16 ModId, uint8 InstId, uint8 ApiId, uint8 ErrCode)
{
#ifdef _ADI_DEBUG_
    printf("ModId: %d, InstId: %d, ApiId: %d, Errcode: %d\n", ModId, InstId, ApiId, ErrCode);
#endif
}

/*! @} */ /*BMS Platform Abstraction Layer*/
/*! @} */ /*Platform Abstraction Layer*/

/*
 *
 * EOF: $URL$
 *
*/

