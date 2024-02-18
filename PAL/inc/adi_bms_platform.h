/*!
******************************************************************************
Copyright (c) 2022 Analog Devices, Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc. and its
licensors. By using this software you agree to the terms of the associated
Analog Devices License Agreement.
******************************************************************************
* @file   : adi_bms_platform.h
* @brief  : This file contains all necessary platform function definitions for BMS.
* @version: $Revision$
* @date   : $Date$
* Developed by: BMS Applications Team, Bangalore, India
*****************************************************************************/

#ifndef ADI_BMS_PLATFORM_H_
#define ADI_BMS_PLATFORM_H_

#include "Std_Types.h"
#include "common.h"
#include "mbed.h"
#include "SPI.h"
#include "Timer.h"
#include "bms_configuration.h"

/*! Number of bytes of data per register group*/
#define PAL_MUL_SPI_BUFF_SIZE 	(4+(8 * ADBMS6948_MAX_NO_OF_DEVICES_IN_DAISY_CHAIN))
/*! Delay of 8 ms in microseconds*/
#define ADI_PAL_DELAY_8MS 		((uint32_t)8000u)
/*! Delay of 1 ms in microseconds*/
#define ADI_PAL_DELAY_1MS 		((uint32_t)1000u)

#define PLATFORM_SPI_INIT()                                             adi_pal_SPI_Init()
#define ADI_PAL_MEMSET(pAddr, nVal, nSize)								adi_pal_Memset(pAddr, nVal, nSize)
#define ADI_PAL_MEMCPY(pDestAddr, pSrcAddr, nSize)						adi_pal_Memcpy(pDestAddr, pSrcAddr, nSize)
#define ADI_PAL_REPORT_DEVELOPMENT_ERROR(ModId, InstId, ApiId, ErrCode) adi_pal_ReportDevelopmentError(ModId, InstId, ApiId, ErrCode)
#define ADI_PAL_REPORT_RUNTIME_ERROR(ErrorId, Status)					adi_pal_ReportRuntimeError(ErrorId, Status)
#define ADI_PAL_CRITICAL_SECTION_START
#define ADI_PAL_CRITICAL_SECTION_STOP
#define ADI_PAL_SPIWRITE(pData, nDataLen, nChainID)					    adi_pal_SpiWrite(pData, nDataLen, nChainID)
#define ADI_PAL_SPIWRITEREADS(pTxBuf, pRxBuf, DataCfg, nChainID)		adi_pal_SpiWriteReads(pTxBuf, pRxBuf, DataCfg, nChainID)
#define ADI_PAL_SPIWRITEREADALL(pTxBuf, pRxBuf, nDataLen, nChainID)     adi_pal_SpiWriteReadAll(pTxBuf, pRxBuf, nDataLen, nChainID)
#define ADI_PAL_TIMERDELAY(uS, nChainID)								adi_pal_TimerDelay(uS, nChainID)
#define ADI_PAL_ISERROR(nChainID)										adi_pal_IsError(nChainID)
#define ADI_PAL_GET_TIMEOUT_STATUS(nChainID)                            adi_pal_GetTimeoutStatus(nChainID)
#define ADI_PAL_TIMER_START(nDelay, nChainID)                           adi_pal_TimerStart(nDelay, nChainID)

#define ADI_PAL_CS_Pin 

extern mbed::SPI spi;
extern Timer timer;
extern DigitalOut chip_select;
extern Timeout timeout;

void adi_pal_Init
(
void
);

void adi_pal_SPI_Init
(
void
);

void adi_pal_ReportDevelopmentError
(
uint16  ModId,
uint8  InstId,
uint8  ApiId,
uint8  ErrCode
);

void adi_pal_ReportRuntimeError
(
uint16  ErrorId,
uint8  Status
);

void adi_pal_Memset
(
uint8 *  pAddr,
uint8  nVal,
uint32  nSize
);

void adi_pal_Memcpy
(
uint8 *  pDstAddr,
uint8 *  pSrcAddr,
uint32  nSize
);

void adi_pal_SpiWrite
(
uint8 * pData,
uint16 nDataLen,
uint8  nChainID
);

void adi_pal_SpiWriteReads
(
uint8 pTxBuf[8][PAL_MUL_SPI_BUFF_SIZE],
uint8 pRxBuf[8][PAL_MUL_SPI_BUFF_SIZE],
uint32 DataCfg,
uint8 nChainId
);

void adi_pal_SpiWriteReadAll
(
uint8 *pTxBuf,
uint8 *pRxBuf,
uint32 nRegGrps,
uint8 nChainID
);

boolean adi_pal_IsError
(
uint8 nChainID
);

void adi_pal_TimerDelay
(
uint32 nPeriodInUS,
uint8  nChainID
);

void adi_pal_DeInit
(
void
);

boolean adi_pal_GetTimeoutStatus
(
uint8 nChainID
);

void adi_pal_TimerStart
(
uint32  nDelay,
uint8  nChainID
);

#endif /* ADI_BMS_PLATFORM_H */

