/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "DrvI2S.h"
#include "DrvSYS.h"


/*---------------------------------------------------------------------------------------------------------*/
/* I2S Interrupt Flag                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#define I2S_I2SRXINT                0x00000002
#define I2S_I2STXINT                0x00000004

#define I2S_RXUDF                   0x00000100
#define I2S_RXOVF                   0x00000200

#define I2S_TXUDF                   0x00010000
#define I2S_TXOVF                   0x00020000
#define I2S_RZCF                    0x00400000
#define I2S_LZCF                    0x00800000


static I2S_CALLBACK_T I2SHandler = {0};

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     <I2S_IRQHandler>                                                                          */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               ISR to handle I2S interrupt event                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void I2S_IRQHandler(void)
{
    uint32_t u32Reg;

    u32Reg = inpw(&(I2S->I2SSTATUS));

    if (u32Reg & I2S_I2STXINT)
    {
        /* Tx underflow */
        if ((u32Reg & I2S_TXUDF) && (I2S->I2SIE.TXUDFIE == 1))
        {
            if (I2SHandler.TxUnderflowFn)
                I2SHandler.TxUnderflowFn(u32Reg);
            
            outp32(&(I2S->I2SSTATUS), I2S_TXUDF);
        }
        /* Tx overflow */
        else if ((u32Reg & I2S_TXOVF) && (I2S->I2SIE.TXOVFIE == 1))
        {
            if (I2SHandler.TxOverflowFn)
                I2SHandler.TxOverflowFn(u32Reg);
            
            outp32(&(I2S->I2SSTATUS), I2S_TXOVF); 
        }
        /* Tx right zero cross */
        else if ((u32Reg & I2S_RZCF) && (I2S->I2SIE.RZCIE == 1))
        {
            if (I2SHandler.TxRightZeroCrossFn)
                I2SHandler.TxRightZeroCrossFn(u32Reg);
            outp32(&(I2S->I2SSTATUS), I2S_RZCF);   
        }
        /* Tx left zero cross */
        else if ((u32Reg & I2S_LZCF) && (I2S->I2SIE.LZCIE == 1))
        {
            if (I2SHandler.TxLeftZeroCrossFn)
                I2SHandler.TxLeftZeroCrossFn(u32Reg);
            outp32(&(I2S->I2SSTATUS), I2S_LZCF);   
        }   
        /* Tx threshold level */
        else if (I2S->I2SIE.TXTHIE == 1)
        {
            if (I2SHandler.TxFifoThresholdFn)
                I2SHandler.TxFifoThresholdFn(u32Reg);  
        }
    }
    else if (u32Reg & I2S_I2SRXINT)
    {
        /* Rx underflow */
        if ((u32Reg & I2S_RXUDF) && (I2S->I2SIE.RXUDFIE == 1))
        {
            if (I2SHandler.RxUnderflowFn)
                I2SHandler.RxUnderflowFn(u32Reg);
            
            outp32(&(I2S->I2SSTATUS), I2S_RXUDF);  
        }
        /* Rx overflow */
        else if ((u32Reg & I2S_RXOVF) && (I2S->I2SIE.RXOVFIE == 1))
        {
            if (I2SHandler.RxOverflowFn)
                I2SHandler.RxOverflowFn(u32Reg);

            outp32(&(I2S->I2SSTATUS), I2S_RXOVF);    
        }
        /* Rx threshold level */
        else if (I2S->I2SIE.RXTHIE == 1)
        {
            if (I2SHandler.RxFifoThresholdFn)
                I2SHandler.RxFifoThresholdFn(u32Reg);  
        }   
    }   
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_Open                                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*          S_DRVI2S_DATA_T *sParam - [in]  Data structure to configure                                    */
/*          u32SampleRate                   Sampling rate.                                                 */
/*                                          The setting takes effect when I2S operates as master mode      */
/*          u8WordWidth                     Data length - DRVI2S_DATABIT_8  :  8-bit Data                  */
/*                                                        DRVI2S_DATABIT_16 : 16-bit Data                  */
/*                                                        DRVI2S_DATABIT_24 : 24-bit Data                  */
/*                                                        DRVI2S_DATABIT_32 : 32-bit Data                  */
/*          u8AudioFormat                   Audio format- DRVI2S_MONO: Mono / DRVI2S_STEREO: Stereo        */
/*          u8DataFormat                    Data format - DRVI2S_FORMAT_I2S : I2S format                   */
/*                                                        DRVI2S_FORMAT_MSB : MSB justified format         */
/*          u8Mode                          I2S operation mode - DRVI2S_MODE_MASTER: master mode           */
/*                                                               DRVI2S_MODE_SLAVE : slave mode            */
/*          u8TxFIFOThreshold               Tx FIFO Threshold Level - DRVI2S_FIFO_LEVEL_WORD_0 : 0 Word    */ 
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_1 : 1 Word    */
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_2 : 2 Word    */
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_3 : 3 Word    */
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_4 : 4 Word    */
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_5 : 5 Word    */
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_6 : 6 Word    */
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_7 : 7 Word    */
/*          u8TxFIFOThreshold               Rx FIFO Threshold Level - DRVI2S_FIFO_LEVEL_WORD_1 : 1 Word    */ 
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_2 : 2 Word    */
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_3 : 3 Word    */
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_4 : 4 Word    */
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_5 : 5 Word    */
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_6 : 6 Word    */
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_7 : 7 Word    */
/*                                                                    DRVI2S_FIFO_LEVEL_WORD_8 : 8 Word    */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               0      Success                                                                            */
/*                                                                                                         */
/* Description:                                                                                            */
/*          This function is used to configure the I2S mode/format/FIFO threshold function/Bit Clock (BCLK)*/       
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2S_Open(S_DRVI2S_DATA_T *sParam)
{
    uint8_t u8Divider;
    uint32_t u32BitRate, u32SrcClk;
    
    SYS->IPRSTC2.I2S_RST = 1;
    SYS->IPRSTC2.I2S_RST = 0;

    SYSCLK->APBCLK.I2S_EN = 1;
    
    I2S->I2SCON.WORDWIDTH  = sParam->u8WordWidth;
    I2S->I2SCON.MONO       = sParam->u8AudioFormat;
    I2S->I2SCON.FORMAT     = sParam->u8DataFormat;
    I2S->I2SCON.SLAVE      = sParam->u8Mode;
    I2S->I2SCON.TXTH       = sParam->u8TxFIFOThreshold;
    I2S->I2SCON.RXTH       = sParam->u8RxFIFOThreshold - 1;

    u32SrcClk = DrvI2S_GetSourceClockFreq();
    
    u32BitRate = sParam->u32SampleRate * (sParam->u8WordWidth + 1) * 16;

    u8Divider = ((u32SrcClk*10)/(u32BitRate * 2) + 5) / 10 - 1;
    
    I2S->I2SCLKDIV.BCLK_DIV = u8Divider;   
    
    I2S->I2SCON.I2SEN = 1; 

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_Close                                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable I2S function and I2S clock                                                        */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_Close(void)
{
    I2S->I2SCON.I2SEN = 0;
    SYS->IPRSTC2.I2S_RST = 1;
    SYS->IPRSTC2.I2S_RST = 0;
    SYSCLK->APBCLK.I2S_EN = 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_EnableInt                                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*           Type        - [in]     I2S_RX_UNDERFLOW / I2S_RX_OVERFLOW / I2S_RX_FIFO_THRESHOLD             */
/*                                  I2S_TX_UNDERFLOW / I2S_TX_OVERFLOW / I2S_TX_FIFO_THRESHOLD             */
/*                                  I2S_TX_RIGHT_ZERO_CROSS / I2S_TX_LEFT_ZERO_CROSS                       */
/*           callbackfn  - [in]     The call back function for specified I2S function                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*           0      Success                                                                                */
/*          <0      Incorrect Argument                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*           This function is used to enable the I2S relative interrupt and install callback function      */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2S_EnableInt(E_I2S_CALLBACK_TYPE Type, I2S_CALLBACK callbackfn)
{
    switch (Type)
    {
        case I2S_RX_UNDERFLOW:
        {
            I2SHandler.RxUnderflowFn = callbackfn;  
            break;
        }
        case I2S_RX_OVERFLOW:
        {
            I2SHandler.RxOverflowFn = callbackfn; 
            break;
        }
        case I2S_RX_FIFO_THRESHOLD:
        {
            I2SHandler.RxFifoThresholdFn = callbackfn;  
            break;
        }        
        case I2S_TX_UNDERFLOW:
        {
            I2SHandler.TxUnderflowFn = callbackfn;  
            break;
        }
        case I2S_TX_OVERFLOW:
        {
            I2SHandler.TxOverflowFn = callbackfn;  
            break;
        }
        case I2S_TX_FIFO_THRESHOLD:
        {
            I2SHandler.TxFifoThresholdFn = callbackfn;  
            break;
        }
        case I2S_TX_RIGHT_ZERO_CROSS:
        {
            I2SHandler.TxRightZeroCrossFn = callbackfn; 
            break;
        }
        case I2S_TX_LEFT_ZERO_CROSS:
        {
            I2SHandler.TxLeftZeroCrossFn = callbackfn; 
            break;
        }        
        default:
            return E_DRVI2S_ERR_ARGUMENT;       
    }
        
    outp32(&(I2S->I2SIE), inp32(&(I2S->I2SIE)) | (1<<Type));    
    NVIC_SetPriority(I2S_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
    NVIC_EnableIRQ(I2S_IRQn);
    
    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_DisableInt                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*           Type - [in] I2S_RX_UNDERFLOW / I2S_RX_OVERFLOW / I2S_RX_FIFO_THRESHOLD                        */
/*                       I2S_TX_UNDERFLOW / I2S_TX_OVERFLOW / I2S_TX_FIFO_THRESHOLD                        */
/*                       I2S_TX_RIGHT_ZERO_CROSS / I2S_TX_LEFT_ZERO_CROSS                                  */
/*                                                                                                         */
/* Returns:                                                                                                */
/*           0      Success                                                                                */
/*          <0      Incorrect Argument                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*           This function is used to disable the I2S relative interrupt and uninstall callback function   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2S_DisableInt(E_I2S_CALLBACK_TYPE Type)
{   
    outp32(&(I2S->I2SIE), inp32(&(I2S->I2SIE)) & ~(1<<Type));
    
    switch(Type)
    {
        case I2S_RX_UNDERFLOW:
        {
            I2SHandler.RxUnderflowFn = NULL;  
            break;
        }
        case I2S_RX_OVERFLOW:
        {
            I2SHandler.RxOverflowFn = NULL; 
            break;
        }
        case I2S_RX_FIFO_THRESHOLD:
        {
            I2SHandler.RxFifoThresholdFn = NULL;  
            break;
        }        
        case I2S_TX_UNDERFLOW:
        {
            I2SHandler.TxUnderflowFn = NULL;  
            break;
        }
        case I2S_TX_OVERFLOW:
        {
            I2SHandler.TxOverflowFn = NULL;  
            break;
        }
        case I2S_TX_FIFO_THRESHOLD:
        {
            I2SHandler.TxFifoThresholdFn = NULL;  
            break;
        }
        case I2S_TX_RIGHT_ZERO_CROSS:
        {
            I2SHandler.TxRightZeroCrossFn = NULL; 
            break;
        }
        case I2S_TX_LEFT_ZERO_CROSS:
        {
            I2SHandler.TxLeftZeroCrossFn = NULL; 
            break;
        }        
        default:
            return E_DRVI2S_ERR_ARGUMENT;    
    }
    
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_GetBCLKFreq                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               I2S BCLK (Bit Clock) frequency                                                            */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Get I2S BCLK (Bit Clock) frequency. BCLK = I2S source clock / (2x(BCLK_DIV + 1))          */       
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvI2S_GetBCLKFreq(void)
{
    uint32_t u32Reg, u32SrcClk;
    
    u32SrcClk = DrvI2S_GetSourceClockFreq();
    
    u32Reg = I2S->I2SCLKDIV.BCLK_DIV;
    
    return ((u32SrcClk >> 1) / (u32Reg + 1));
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_SetBCLKFreq                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32Bclk    - [in]      I2S bit clock frequency (Hz)                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Set bit clock divider if I2S operates in master mode.                                     */   
/*               BCLK = I2S source clock / (2x(BCLK_DIV + 1))                                              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_SetBCLKFreq(uint32_t u32Bclk)
{
    uint8_t u8Divider;
    uint32_t u32SrcClk;

    u32SrcClk = DrvI2S_GetSourceClockFreq(); 
    
    u8Divider = ((u32SrcClk*10)/(u32Bclk * 2) + 5) / 10 - 1;

    I2S->I2SCLKDIV.BCLK_DIV = u8Divider;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_GetMCLKFreq                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               I2S MCLK (Master Clock) frequency                                                         */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Get I2S MCLK (Master Clock) frequency. MCLK = I2S source clock / (2xMCLK_DIV)             */       
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvI2S_GetMCLKFreq(void)
{
    uint32_t u32Reg, u32SrcClk;

    u32Reg = I2S->I2SCLKDIV.MCLK_DIV;

    u32SrcClk = DrvI2S_GetSourceClockFreq();    

    if (u32Reg == 0)
        return u32SrcClk;
    else
        return ((u32SrcClk >> 1) / u32Reg);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_SetMCLKFreq                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32Mclk   - [in]       I2S master clock output ftequency (Hz)                             */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Set master clock divider. MCLK = I2S source clock / (2xMCLK_DIV)                          */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_SetMCLKFreq(uint32_t u32Mclk)
{
    uint8_t u8Divider;
    uint32_t u32SrcClk;

    u32SrcClk = DrvI2S_GetSourceClockFreq(); 

    if (u32Mclk == u32SrcClk)
    {
        u8Divider = 0;
    }
    else
    {
        u8Divider = (u32SrcClk/u32Mclk) >> 1;
    }
    
    I2S->I2SCLKDIV.MCLK_DIV = u8Divider;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_SetChannelZeroCrossDetect                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               channel    - [in]      I2S_LEFT_CHANNEL / I2S_RIGHT_CHANNEL                               */
/*               i32flag    - [out]     1:Enable / 0:Disable                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               0       Success                                                                           */
/*              <0       Incorrect Argument                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable/Disable left/right channel zero cross detect function                              */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2S_SetChannelZeroCrossDetect(E_I2S_CHANNEL channel, int32_t i32flag)
{
    if (channel == I2S_LEFT_CHANNEL)
    {
        I2S->I2SCON.LCHZCEN = i32flag;
    }
    else if (channel == I2S_RIGHT_CHANNEL)
    {
        I2S->I2SCON.RCHZCEN = i32flag;
    }
    else
        return E_DRVI2S_ERR_ARGUMENT;

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_EnableTxDMA                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable I2S Tx DMA function. I2S requests DMA to transfer data to Tx FIFO                  */
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_EnableTxDMA(void)
{   
    I2S->I2SCON.TXDMA = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_DisableTxDMA                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable I2S Tx DMA function.                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_DisableTxDMA(void)
{   
    I2S->I2SCON.TXDMA = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_EnableRxDMA                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable I2S Rx DMA function. I2S requests DMA to transfer data from Rx FIFO                */
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_EnableRxDMA(void)
{
    I2S->I2SCON.RXDMA = 1; 
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_DisableRxDMA                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable I2S Rx DMA function.                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_DisableRxDMA(void)
{
    I2S->I2SCON.RXDMA = 0; 
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_EnableTx                                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable I2S Tx function                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_EnableTx(void)
{
    I2S->I2SCON.TXEN = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_DisableTx                                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable I2S Tx function                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_DisableTx(void)
{
    I2S->I2SCON.TXEN = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_EnableRx                                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable I2S Rx function                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_EnableRx(void)
{
    I2S->I2SCON.RXEN = 1;  
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_DisableRx                                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable I2S Rx function                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_DisableRx(void)
{
    I2S->I2SCON.RXEN = 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_EnableTxMute                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable Tx Mute function                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_EnableTxMute(void)
{
    I2S->I2SCON.MUTE = 1;  
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_DisableTxMute                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable Tx Mute function                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_DisableTxMute(void)
{
    I2S->I2SCON.MUTE = 0;  
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_EnableMCLK                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable MCLK (Master Clock) and output signal                                              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_EnableMCLK(void)
{   
    I2S->I2SCON.MCLKEN = 1;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_DisableMCLK                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable MCLK (Master Clock) and output signal                                             */
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_DisableMCLK(void)
{   
    I2S->I2SCON.MCLKEN = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_ClearTxFIFO                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Clear Tx FIFO. Internal pointer is reset to FIFO start point                              */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_ClearTxFIFO(void)
{
    I2S->I2SCON.CLR_TXFIFO = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_ClearRxFIFO                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Clear Rx FIFO. Internal pointer is reset to FIFO start point                              */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_ClearRxFIFO(void)
{
    I2S->I2SCON.CLR_RXFIFO = 1;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_SelectClockSource                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*           u8ClkSrcSel  - [in] DRVI2S_EXT_12M/DRVI2S_PLL/DRVI2S_HCLK/DRVI2S_INTERNAL_22M                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to select I2S clock source                                          */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2S_SelectClockSource(uint8_t u8ClkSrcSel)
{
    SYSCLK->CLKSEL2.I2S_S = u8ClkSrcSel;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_GetSourceClockFreq                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               I2S source clock frequency (Hz)                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get I2S source clock frequency.                                  */       
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvI2S_GetSourceClockFreq(void)
{
    uint8_t  u8ClkSrcSel;
    uint32_t u32Freq = __XTAL;
     
    
    u8ClkSrcSel = SYSCLK->CLKSEL2.I2S_S;

    switch (u8ClkSrcSel)
    {
        case DRVI2S_EXT_12M:
            u32Freq = __XTAL;
            break;
        case DRVI2S_PLL:
            u32Freq = DrvSYS_GetPLLClockFreq();  
            break;
        case DRVI2S_HCLK:
            u32Freq = SystemCoreClock;
            break; 
        case DRVI2S_INTERNAL_22M:
            u32Freq = __IRC22M;
            break;
    }

    return u32Freq;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2S_GetVersion                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               The DrvI2S version number                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get the DrvI2S version number                                    */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvI2S_GetVersion(void)
{
    return DRVI2S_VERSION_NUM;
}

