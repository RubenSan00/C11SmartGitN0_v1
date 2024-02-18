/*******************************************************************************
Copyright (c) 2022 Analog Devices, Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc. and its
licensors. By using this software you agree to the terms of the associated
Analog Devices License Agreement.
******************************************************************************
* @file:
* @brief: BMS configuration
* @version: $Revision$
* @date:    $Date$
* Developed by: BMS Apps - Plano, TX
*****************************************************************************/

#ifndef BMS_CONFIGURATION_H_
#define BMS_CONFIGURATION_H_

#include <stdint.h>
#include "Adbms6948_Cfg.h"


#define SYS_CELL_MON_CHAIN_ID                   (0)
#define SYS_CM_DEVICES_PRESENT                  (1)
#define SYS_MAX_CELL_MON_CELLS                  (16u)
#define SYS_CM_CELL_UPPER_RANGE		            (60000) /* 6.0V*/
#define SYS_CM_CELL_LOWER_RANGE		            (1000) /* 0.1 V*/


/* SYSTEM_CONFIG_WORD definition*/
#define ENABLE_CELL_BALANCING           (1<<0)
#define CELL_BALANCING_MODE             (1<<1)
#define ENABLE_SOC_CELL_BALANCING       (1<<2)
#define CFG_LPCM_ENABLE                 (1<<9)
#define PACK_SENS_PRESENT               (1<<24)
#define CELL_MON_PRESENT                (1<<25)
/* SYSTEM_CONFIG_WORD definition*/

/* SYSTEM_CONFIG_WORD Configuration*/
#define SYSTEM_CONFIG_WORD				(CELL_MON_PRESENT) /* Devices, subsytems present*/
#if LPCM_MODE
  #define SYSTEM_CONFIG_WORD			SYSTEM_CONFIG_WORD|CFG_LPCM_ENABLE) /* Devices, subsytems present*/
#endif
#define SYS_SYSTEM_CONFIG_WORD			(SYSTEM_CONFIG_WORD)


#define SYS_FW_VERSION			(0x0400) /*(MSB:Major, LSB:minor)*/


#define SYS_NUM_OF_CELLS            SYS_MAX_CELL_MON_CELLS
#define SYS_MAX_INT_DEV_PARAMS      (8)
#define SYS_MAX_GPIO_INPUTS         (10)
#define SYS_NUM_OF_GPIOS            (10)
#define SYS_MAX_CADC_CONV_CNT       (256)

#define SYS_CELL_UV_LIMIT						(-1400000) /*-1.4 V *(10^6)*/
#define SYS_CELL_OV_LIMIT						(4100000) /*4.1 V *(10^6)*/

#define SYS_GPIO_UV_LIMIT						(-1400000) /*-1.4 V *(10^6)*/
#define SYS_GPIO_OV_LIMIT						(5000000) /*5.0 V *(10^6)*/

#define SYS_MAX_DIE_TEMP						(125)

#define SYS_DIAG_MASK_CELL_MEASURE  (0x9BF0)
#define SYS_MAX_DIGITAL_SUPPLY      (3600000) /*3.6 V *(10^6)*/
#define SYS_MIN_DIGITAL_SUPPLY      (2100000) /*2.1 V *(10^6)*/

#define SYS_MAX_ANALOG_SUPPLY       (5500000) /*5.5 V *(10^6)*/
#define SYS_MIN_ANALOG_SUPPLY       (4500000) /*4.5 V *(10^6)*/


#define SYS_MAX_VREF2_VOLT      (3007000) /*3.007 V *(10^6)*/
#define SYS_MIN_VREF2_VOLT      (2993000) /*2.993 V *(10^6)*/


#endif /*BMS_CONFIGURATION_H_*/




