/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef _DRVI2S_H
#define _DRVI2S_H

#include "NUC1xx.h"


/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVI2S_MAJOR_NUM    1
#define DRVI2S_MINOR_NUM    02
#define DRVI2S_BUILD_NUM    2

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVI2S_VERSION_NUM     _SYSINFRA_VERSION(DRVI2S_MAJOR_NUM, DRVI2S_MINOR_NUM, DRVI2S_BUILD_NUM)
                               
/*---------------------------------------------------------------------------------------------------------*/
/*  Define Error Code                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
// E_DRVI2S_ERR_ARGUMENT        Incorrect Argument
#define E_DRVI2S_ERR_ARGUMENT   _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVI2S, 1)

/*---------------------------------------------------------------------------------------------------------*/
/* Define parameter                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVI2S_DATABIT_8            0x00
#define DRVI2S_DATABIT_16           0x01
#define DRVI2S_DATABIT_24           0x02
#define DRVI2S_DATABIT_32           0x03

/* Audio Format */
#define DRVI2S_MONO                 0x01
#define DRVI2S_STEREO               0x00

/* I2S Data Format */
#define DRVI2S_FORMAT_MSB           0x01
#define DRVI2S_FORMAT_I2S           0x00

/* I2S Operation mode */
#define DRVI2S_MODE_SLAVE           0x01
#define DRVI2S_MODE_MASTER          0x00

/* I2S FIFO Threshold */
#define DRVI2S_FIFO_LEVEL_WORD_0    0x00
#define DRVI2S_FIFO_LEVEL_WORD_1    0x01
#define DRVI2S_FIFO_LEVEL_WORD_2    0x02
#define DRVI2S_FIFO_LEVEL_WORD_3    0x03
#define DRVI2S_FIFO_LEVEL_WORD_4    0x04
#define DRVI2S_FIFO_LEVEL_WORD_5    0x05
#define DRVI2S_FIFO_LEVEL_WORD_6    0x06
#define DRVI2S_FIFO_LEVEL_WORD_7    0x07
#define DRVI2S_FIFO_LEVEL_WORD_8    0x08


/*---------------------------------------------------------------------------------------------------------*/
/*  I2S Clcok Source Selector                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVI2S_EXT_12M        0
#define DRVI2S_PLL            1
#define DRVI2S_HCLK           2
#define DRVI2S_INTERNAL_22M   3



/* Write data to Tx FIFO */
#define _DRVI2S_WRITE_TX_FIFO(u32Data)  I2S->I2STXFIFO = u32Data

/* Write data from Rx FIFO */
#define _DRVI2S_READ_RX_FIFO()          I2S->I2SRXFIFO

/* Read word data number in Tx FIFO */
#define _DRVI2S_READ_TX_FIFO_LEVEL()    I2S->I2SSTATUS.TX_LEVEL

/* Read word data number in Rx FIFO */
#define _DRVI2S_READ_RX_FIFO_LEVEL()    I2S->I2SSTATUS.RX_LEVEL


/*---------------------------------------------------------------------------------------------------------*/
/*  I2S Data Structure                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    uint32_t    u32SampleRate;
    uint8_t     u8WordWidth;
    uint8_t     u8AudioFormat;
    uint8_t     u8DataFormat;
    uint8_t     u8Mode;
    uint8_t     u8TxFIFOThreshold;
    uint8_t     u8RxFIFOThreshold;

}S_DRVI2S_DATA_T;

typedef enum
{
    I2S_RX_UNDERFLOW        =  0, 
    I2S_RX_OVERFLOW         =  1,
    I2S_RX_FIFO_THRESHOLD   =  2,

    I2S_TX_UNDERFLOW        =  8, 
    I2S_TX_OVERFLOW         =  9,
    I2S_TX_FIFO_THRESHOLD   = 10,
    I2S_TX_RIGHT_ZERO_CROSS = 11,
    I2S_TX_LEFT_ZERO_CROSS  = 12
} E_I2S_CALLBACK_TYPE;

typedef enum
{
    I2S_LEFT_CHANNEL  =  0, 
    I2S_RIGHT_CHANNEL =  1
} E_I2S_CHANNEL;


/*---------------------------------------------------------------------------------------------------------*/
/*  I2S Callback function                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
typedef void (*I2S_CALLBACK)(uint32_t status);

typedef struct
{
    I2S_CALLBACK    RxUnderflowFn;
    I2S_CALLBACK    RxOverflowFn;    
    I2S_CALLBACK    RxFifoThresholdFn;
    
    I2S_CALLBACK    TxUnderflowFn;
    I2S_CALLBACK    TxOverflowFn;
    I2S_CALLBACK    TxFifoThresholdFn;    
    I2S_CALLBACK    TxRightZeroCrossFn;
    I2S_CALLBACK    TxLeftZeroCrossFn;
}I2S_CALLBACK_T;

/*---------------------------------------------------------------------------------------------------------*/
/* Define I2S functions prototype                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void     DrvI2S_ClearTxFIFO(void);
void     DrvI2S_ClearRxFIFO(void);
void     DrvI2S_Close(void);

int32_t  DrvI2S_DisableInt(E_I2S_CALLBACK_TYPE Type);
void     DrvI2S_DisableMCLK(void);
void     DrvI2S_DisableRx(void);
void     DrvI2S_DisableTxDMA(void);
void     DrvI2S_DisableTx(void);
void     DrvI2S_DisableRxDMA(void);
void     DrvI2S_DisableTxMute(void);

int32_t  DrvI2S_EnableInt(E_I2S_CALLBACK_TYPE Type, I2S_CALLBACK callbackfn);
void     DrvI2S_EnableMCLK(void);
void     DrvI2S_EnableRx(void);
void     DrvI2S_EnableRxDMA(void);
void     DrvI2S_EnableTx(void);
void     DrvI2S_EnableTxDMA(void);
void     DrvI2S_EnableTxMute(void);

uint32_t DrvI2S_GetBCLKFreq(void);
uint32_t DrvI2S_GetMCLKFreq(void);
uint32_t DrvI2S_GetSourceClockFreq(void);
uint32_t DrvI2S_GetVersion(void);

int32_t  DrvI2S_Open(S_DRVI2S_DATA_T *sParam);

void     DrvI2S_SelectClockSource(uint8_t u8ClkSrcSel);
void     DrvI2S_SetBCLKFreq(uint32_t u32Bclk);
int32_t  DrvI2S_SetChannelZeroCrossDetect(E_I2S_CHANNEL channel, int32_t i32flag);
void     DrvI2S_SetMCLKFreq(uint32_t u32Mclk);

#endif

