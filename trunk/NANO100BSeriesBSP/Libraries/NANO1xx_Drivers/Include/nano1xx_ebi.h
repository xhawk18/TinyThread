/******************************************************************************
 * @file     nano1xx_ebi.h
 * @brief    NANO1xx EBI driver header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef _NANO1XX_EBI_H_
#define _NANO1XX_EBI_H_

#include "nano1xx.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_EBI_Driver EBI Driver
  @{
*/


/** @addtogroup NANO1xx_EBI_EXPORTED_CONSTANTS EBI Exported Constants
  @{
*/

#define EBI_BASE_ADDR				0x60000000	/* 0x60000000 ~ 0x6001FFFF */
#define EBI_MAX_SIZE				0x20000
#define EBI_TIMEOUT_COUNT			0x10000

#define EBI_READ_DATA8(Addr)			*((volatile unsigned char *)(EBI_BASE_ADDR+Addr))
#define EBI_WRITE_DATA8(Addr, Data)		*((volatile unsigned char *)(EBI_BASE_ADDR+Addr))=Data
#define EBI_READ_DATA16(Addr)			*((volatile unsigned short *)(EBI_BASE_ADDR+Addr))
#define EBI_WRITE_DATA16(Addr, Data)	*((volatile unsigned short *)(EBI_BASE_ADDR+Addr))=Data
#define EBI_READ_DATA32(Addr)			*((volatile unsigned int *)(EBI_BASE_ADDR+Addr))
#define EBI_WRITE_DATA32(Addr, Data)	*((volatile unsigned int *)(EBI_BASE_ADDR+Addr))=Data


#define E_EBI_MCLKDIV_1			0
#define E_EBI_MCLKDIV_2			1
#define E_EBI_MCLKDIV_4			2
#define E_EBI_MCLKDIV_8			3
#define E_EBI_MCLKDIV_16		4
#define E_EBI_MCLKDIV_32		5
#define E_EBI_MCLKDIV_DEFAULT	6


/*@}*/ /* end of group NANO1xx_EBI_EXPORTED_CONSTANTS */


/** @addtogroup NANO1xx_EBI_EXPORTED_STRUCTS EBI Exported Structs
  @{
*/
typedef struct 
{
    uint8_t				eMCLKDIV;
    uint8_t				u8ExttALE;
    uint8_t				u8ExtIR2R;
    uint8_t				u8ExtIR2W;
    uint8_t				u8ExtIW2X;
    uint8_t				u8ExttAHD;
    uint8_t				u8ExttACC;
} EBI_TIMING_T;

/*@}*/ /* end of group NANO1xx_EBI_EXPORTED_STRUCTS */


/** @addtogroup NANO1xx_EBI_EXPORTED_FUNCTIONS EBI Exported Functions
  @{
*/
void EBI_Init(int data_width_is_8_bit);
void EBI_DeInit(void);
void EBI_SetBusTiming(EBI_TIMING_T sEBITiming);
void EBI_GetBusTiming(EBI_TIMING_T *psEBITiming);

/*@}*/ /* end of group NANO1xx_EBI_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_EBI_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/







