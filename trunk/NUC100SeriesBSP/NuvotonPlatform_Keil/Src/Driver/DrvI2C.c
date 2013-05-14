/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "DrvI2C.h"


#define I2C_STA              0x20
#define I2C_STO              0x10
#define I2C_SI               0x08
#define I2C_AA               0x04

static I2C_CALLBACK_T I2CHandler0 = {0};
static I2C_CALLBACK_T I2CHandler1 = {0};


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     <I2C0_IRQHandler>                                                                         */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               ISR to handle I2C0 interrupt event                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void I2C0_IRQHandler(void)
{
    uint32_t status;

    status  = I2C0->I2CSTATUS;

    if (I2C0->I2CTOC.TIF)
    {
        I2C0->I2CTOC.TIF = 1;  /* Clear TIF */
        
        if (I2CHandler0.TimeoutCallBackFn)
        {
            I2CHandler0.TimeoutCallBackFn(status); 
        }
    }
    else
    {
        switch (status)
        {   
            case 0x38:  /* Arbitration loss */
            {
                if (I2CHandler0.ArbitLossCallBackFn)
                    I2CHandler0.ArbitLossCallBackFn(status); 
                break;
            }
            case 0x00:  /* Bus error */
            {
                if (I2CHandler0.BusErrCallBackFn)
                    I2CHandler0.BusErrCallBackFn(status); 
                break;
            }
            default:
            {
                if (I2CHandler0.I2CCallBackFn)
                    I2CHandler0.I2CCallBackFn(status);
            }       
        }   
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     <I2C1_IRQHandler>                                                                         */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               ISR to handle I2C1 interrupt event                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void I2C1_IRQHandler(void)
{
    uint32_t status;

    status  = I2C1->I2CSTATUS;

    if (I2C1->I2CTOC.TIF)
    {
        I2C1->I2CTOC.TIF = 1;  /* Clear TIF */
        
        if (I2CHandler1.TimeoutCallBackFn)
        {
            I2CHandler1.TimeoutCallBackFn(status); 
        }
    }
    else
    {
        switch (status)
        {   
            case 0x38:  /* Arbitration loss */
            {
                if (I2CHandler1.ArbitLossCallBackFn)
                    I2CHandler1.ArbitLossCallBackFn(status); 
                break;
            }
            case 0x00:  /* Bus error */
            {
                if (I2CHandler1.BusErrCallBackFn)
                    I2CHandler1.BusErrCallBackFn(status); 
                break;
            }
            default:
            {
                if (I2CHandler1.I2CCallBackFn)
                    I2CHandler1.I2CCallBackFn(status);
            }       
        }   
    }  
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_Ctrl                                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port       - [in]      I2C_PORT0 / I2C_PORT1                                              */
/*               start      - [in]      1:Enable / 0:Disable                                               */
/*               stop       - [in]      1:Enable / 0:Disable                                               */
/*               intFlag    - [in]      Wrtie '1' to clear this flag                                       */
/*               ack        - [in]      1:Enable / 0:Disable                                               */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Set I2C control bit                                                                       */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2C_Ctrl(E_I2C_PORT port, uint8_t start, uint8_t stop, uint8_t intFlag, uint8_t ack)
{
    
    uint32_t Reg = 0;
        
    if (start)
        Reg |= I2C_STA;
    if (stop)
        Reg |= I2C_STO;
    if (intFlag)
        Reg |= I2C_SI;
    if (ack)
        Reg |= I2C_AA;

    if (port)
        *((__IO uint32_t *)&I2C1->I2CON) = (*((__IO uint32_t *)&I2C1->I2CON) & ~0x3C) | Reg;
    else
        *((__IO uint32_t *)&I2C0->I2CON) = (*((__IO uint32_t *)&I2C0->I2CON) & ~0x3C) | Reg;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_WriteData                                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port       - [in]      I2C_PORT0 / I2C_PORT1                                              */
/*               u8data     - [in]      Byte Data                                                          */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Write Data into I2C Data register                                                         */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2C_WriteData(E_I2C_PORT port, uint8_t u8data)
{
    if (port)
    {
        I2C1->I2CDAT = u8data;
    }   
    else
    {
        I2C0->I2CDAT = u8data;    
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_ReadData                                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port       - [in]      I2C_PORT0 / I2C_PORT1                                              */
/* Returns:                                                                                                */
/*               Data from I2C Data register                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Read Data from I2C Data register                                                          */       
/*---------------------------------------------------------------------------------------------------------*/
uint8_t DrvI2C_ReadData(E_I2C_PORT port)
{
    if (port)
    {                          
        return I2C1->I2CDAT;
    }   
    else
    {
        return I2C0->I2CDAT;  
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_GetIntFlag                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port       - [in]      I2C_PORT0 / I2C_PORT1                                              */
/* Returns:                                                                                                */
/*               0 : Flag is not set                                                                       */
/*               1 : Flag is set                                                                           */
/* Description:                                                                                            */
/*               Get I2C interrupt flag                                                                    */       
/*---------------------------------------------------------------------------------------------------------*/
uint8_t DrvI2C_GetIntFlag(E_I2C_PORT port)
{
    if (port)
    {
        return I2C1->I2CON.SI;
    }   
    else
    {
        return I2C0->I2CON.SI;    
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_ClearIntFlag                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port       - [in]      I2C_PORT0 / I2C_PORT1                                              */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Clear I2C interrupt flag                                                                  */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2C_ClearIntFlag(E_I2C_PORT port)
{
    if (port)
    { 
        I2C1->I2CON.SI = 1;
    }   
    else
    {
        I2C0->I2CON.SI = 1;   
    }           
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_ClearTimeoutFlag                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port       - [in]      I2C_PORT0 / I2C_PORT1                                              */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Clear I2C time out flag                                                                   */       
/*---------------------------------------------------------------------------------------------------------*/
void DrvI2C_ClearTimeoutFlag(E_I2C_PORT port)
{
    if (port)
    { 
        I2C1->I2CTOC.TIF = 1;
    }   
    else
    {
        I2C0->I2CTOC.TIF = 1;  
    }           
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_GetStatus                                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port       - [in]      I2C_PORT0 / I2C_PORT1                                              */
/* Returns:                                                                                                */
/*               status                                                                                    */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Get I2C status. There are 26 status codes. Please refer to data sheet in detail.          */       
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvI2C_GetStatus(E_I2C_PORT port)
{
    if (port)
    {
        return I2C1->I2CSTATUS;
    }
    else
    {
        return I2C0->I2CSTATUS;
    }   
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_Open                                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port          - [in]       I2C_PORT0 / I2C_PORT1                                          */
/*               u32BusClock   - [in]       I2C bus clock frequency (Hz)                                   */
/* Returns:                                                                                                */
/*               0 : Success                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable I2C function and set clock divider. I2C clock = I2C source clock / (4x(divider+1)) */       
/*               The maximum of I2C bus clock is 1MHz and the minimum of the divider is 4.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_Open(E_I2C_PORT port, uint32_t u32BusClock)
{
    uint32_t divider;     
 
    divider = (uint32_t) (((SystemCoreClock*10)/(u32BusClock * 4) + 5) / 10 - 1);   /* Compute proper divider for I2C clock */
    
    if (divider < 4)
    {
        divider = 4;
    }

    if (port)
    {
        SYSCLK->APBCLK.I2C1_EN = 1;
        SYS->IPRSTC2.I2C1_RST = 1;
        SYS->IPRSTC2.I2C1_RST = 0;
        I2C1->I2CON.ENS1 = 1;
        I2C1->I2CLK = divider;
    }
    else
    {
        SYSCLK->APBCLK.I2C0_EN = 1;
        SYS->IPRSTC2.I2C0_RST = 1;
        SYS->IPRSTC2.I2C0_RST = 0;  
        I2C0->I2CON.ENS1 = 1;
        I2C0->I2CLK = divider;
    }

    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_Close                                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port       - [in]      I2C_PORT0 / I2C_PORT1                                              */
/* Returns:                                                                                                */
/*               0 : Success                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable I2C function and clock source                                                     */       
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_Close(E_I2C_PORT port)
{
    if (port)
    {
        I2C1->I2CON.ENS1 = 0;
        SYS->IPRSTC2.I2C1_RST = 1;
        SYS->IPRSTC2.I2C1_RST = 0;
        SYSCLK->APBCLK.I2C1_EN = 0;
    }
    else
    {
        I2C0->I2CON.ENS1 = 0;
        SYS->IPRSTC2.I2C0_RST = 1;
        SYS->IPRSTC2.I2C0_RST = 0;
        SYSCLK->APBCLK.I2C0_EN = 0;
    }
    
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_SetClockFreq                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port         - [in]        I2C_PORT0 / I2C_PORT1                                          */
/*               u32BusClock  - [in]        I2C Bus Clock Frequency (Hz)                                   */
/* Returns:                                                                                                */
/*               0 : Success                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Set clock divider. I2C clock = I2C source clock / (4x(divider+1))                         */       
/*               The maximum of I2C bus clock is 1MHz and the minimum of the divider is 4.                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_SetClockFreq(E_I2C_PORT port, uint32_t u32BusClock)
{
    uint32_t divider;     
 
    divider = (uint32_t) (((SystemCoreClock*10)/(u32BusClock * 4) + 5) / 10 - 1);

    if (divider < 4)
    {
        divider = 4;
    }

    if (port)
    {
        I2C1->I2CLK = divider;
    }
    else
    {
        I2C0->I2CLK = divider;
    }
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_GetClockFreq                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port          - [in]       I2C_PORT0 / I2C_PORT1                                          */
/* Returns:                                                                                                */
/*               I2C Bus Clock Frequency                                                                   */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Get I2C Bus Clock                                                                         */       
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvI2C_GetClockFreq(E_I2C_PORT port)
{
    uint32_t divider;     
 
    divider = ( (port)? I2C1->I2CLK:I2C0->I2CLK );
    return ( SystemCoreClock / ((divider+1)<<2) );
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_EnableInt                                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port          - [in]       I2C_PORT0 / I2C_PORT1                                          */
/* Returns:                                                                                                */
/*               0 : Success                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable I2C interrupt and NVIC corresponding to I2C                                        */       
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_EnableInt(E_I2C_PORT port)
{
    if (port)
    {
        I2C1->I2CON.EI = 1;
        NVIC_SetPriority(I2C1_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(I2C1_IRQn);
    }
    else
    {
        I2C0->I2CON.EI = 1;
        NVIC_SetPriority(I2C0_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(I2C0_IRQn);
    }

    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_DisableInt                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port          - [in]       I2C_PORT0 / I2C_PORT1                                          */
/* Returns:                                                                                                */
/*               0 : Success                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable I2C interrupt and NVIC corresponding to I2C                                       */       
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_DisableInt(E_I2C_PORT port)
{
    if (port)
    {
        I2C1->I2CON.EI = 0;
        NVIC_DisableIRQ(I2C1_IRQn);
    }
    else
    {
        I2C0->I2CON.EI = 0;
        NVIC_DisableIRQ(I2C0_IRQn);
    }

    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_SetTimeoutCounter                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port          - [in]       I2C_PORT0 / I2C_PORT1                                          */
/*               i32enable     - [in]       1:Enable / 0:Disable                                           */
/*               u8div4        - [in]       1:Enable / 0:Disable                                           */
/* Returns:                                                                                                */
/*               0 : Success                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable/Disable I2C 14-bit timeout counter and set div4 bit of timeout counter             */       
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_SetTimeoutCounter(E_I2C_PORT port, int32_t i32enable, uint8_t u8div4)
{
    if (port)
    {
        I2C1->I2CTOC.DIV4 = u8div4;
        I2C1->I2CTOC.ENTI = i32enable;
    }
    else
    {
        I2C0->I2CTOC.DIV4 = u8div4;
        I2C0->I2CTOC.ENTI = i32enable;
    }
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_SetAddress                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port          - [in]       I2C_PORT0 / I2C_PORT1                                          */
/*               slaveNo       - [in]       There are 4 slave addrss. The slaveNo is 0 ~ 3.                */
/*               slave_addr    - [in]       7-bit physical slave address                                   */
/*               GC_Flag       - [in]       1:Enable / 0:Disable                                           */
/* Returns:                                                                                                */
/*               0 : Success                                                                               */
/*              <0 : Failed                                                                                */
/* Description:                                                                                            */
/*               Set 4 7-bit slave addresses and enable General Call function                              */       
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_SetAddress(E_I2C_PORT port, uint8_t slaveNo, uint8_t slave_addr, uint8_t GC_Flag)
{
    if (port)
    {
        switch (slaveNo)
        {
            case 0:
                I2C1->I2CADDR0.I2CADDR = slave_addr;
                I2C1->I2CADDR0.GC      = GC_Flag;
                break;
            case 1:
                I2C1->I2CADDR1.I2CADDR = slave_addr;
                I2C1->I2CADDR1.GC      = GC_Flag;
                break;
            case 2:
                I2C1->I2CADDR2.I2CADDR = slave_addr;
                I2C1->I2CADDR2.GC      = GC_Flag;
                break;
            case 3:
                I2C1->I2CADDR3.I2CADDR = slave_addr;
                I2C1->I2CADDR3.GC      = GC_Flag;
                break;
            default:
                return E_DRVI2C_ERR_ARGUMENT;
        }
    }
    else
    {
        switch (slaveNo)
        {
            case 0:
                I2C0->I2CADDR0.I2CADDR  = slave_addr;
                I2C0->I2CADDR0.GC       = GC_Flag;
                break;
            case 1:
                I2C0->I2CADDR1.I2CADDR = slave_addr;
                I2C0->I2CADDR1.GC      = GC_Flag;
                break;
            case 2:
                I2C0->I2CADDR2.I2CADDR = slave_addr;
                I2C0->I2CADDR2.GC      = GC_Flag;
                break;
            case 3:
                I2C0->I2CADDR3.I2CADDR = slave_addr;
                I2C0->I2CADDR3.GC      = GC_Flag;
                break;
            default:
                return E_DRVI2C_ERR_ARGUMENT;
        }   
    }
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_SetAddressMask                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port          - [in]       I2C_PORT0 / I2C_PORT1                                          */
/*               slaveNo       - [in]       There are 4 slave addrss. The slaveNo is 0 ~ 3.                */
/*               slaveAddrMask - [in]       7-bit physical slave address mask                              */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               0 : Success                                                                               */
/*              <0 : Failed                                                                                */
/* Description:                                                                                            */
/*               Set 4 7-bit slave address mask. The corresponding address bit is "Don't Care".            */       
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_SetAddressMask(E_I2C_PORT port, uint8_t slaveNo, uint8_t slaveAddrMask)
{
    slaveAddrMask <<= 1;
    if (port)
    {
        switch (slaveNo)
        {
            case 0:
                I2C1->I2CADM0.I2CADM = slaveAddrMask;
                break;
            case 1:
                I2C1->I2CADM1.I2CADM = slaveAddrMask;
                break;
            case 2:
                I2C1->I2CADM2.I2CADM = slaveAddrMask;
                break;
            case 3:
                I2C1->I2CADM3.I2CADM = slaveAddrMask;
                break;
            default:
                return E_DRVI2C_ERR_ARGUMENT;
        }
    }
    else
    {
        switch (slaveNo)
        {
            case 0:
                I2C0->I2CADM0.I2CADM = slaveAddrMask;
                break;
            case 1:
                I2C0->I2CADM1.I2CADM = slaveAddrMask;
                break;
            case 2:
                I2C0->I2CADM2.I2CADM = slaveAddrMask;
                break;
            case 3:
                I2C0->I2CADM3.I2CADM = slaveAddrMask;
                break;
            default:
                return E_DRVI2C_ERR_ARGUMENT;
        }   
    }
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_InstallCallback                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port          - [in]       I2C_PORT0 / I2C_PORT1                                          */
/*               Type          - [in]       I2CFUNC / ARBITLOSS / BUSERROR / TIMEOUT                       */
/*               callbackfn    - [in]       Call back function                                             */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               0       Success                                                                           */
/*               <0      Failed                                                                            */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Install I2C call back function for I2C normal function, Arbitration loss, Bus error       */
/*                                                  and Counter timeout .                                  */       
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_InstallCallback(E_I2C_PORT port, E_I2C_CALLBACK_TYPE Type, I2C_CALLBACK callbackfn)
{
    if (port == I2C_PORT0)
    {   
        switch(Type)
        {
            case I2CFUNC:
            {
                I2CHandler0.I2CCallBackFn = callbackfn;  
                break;
            }
            case ARBITLOSS:
            {
                I2CHandler0.ArbitLossCallBackFn = callbackfn;  
                break;
            }
            case BUSERROR:
            {
                I2CHandler0.BusErrCallBackFn = callbackfn;  
                break;
            }        
            case TIMEOUT:
            {
                I2CHandler0.TimeoutCallBackFn = callbackfn;  
                break;
            }
            default:
                return E_DRVI2C_ERR_ARGUMENT;   
        }
    }
    else if (port == I2C_PORT1)
    {   
        switch(Type)
        {
            case I2CFUNC:
            {
                I2CHandler1.I2CCallBackFn = callbackfn;  
                break;
            }
            case ARBITLOSS:
            {
                I2CHandler1.ArbitLossCallBackFn = callbackfn;  
                break;
            }
            case BUSERROR:
            {
                I2CHandler1.BusErrCallBackFn = callbackfn;  
                break;
            }        
            case TIMEOUT:
            {
                I2CHandler1.TimeoutCallBackFn = callbackfn;  
                break;
            }
            default:
                return E_DRVI2C_ERR_ARGUMENT;  
        }
    }       
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_UninstallCallBack                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               port          - [in]       I2C_PORT0 / I2C_PORT1                                          */
/*               Type          - [in]       I2CFUNC / ARBITLOSS / BUSERROR / TIMEOUT                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               0       Success                                                                           */
/*               <0      Failed                                                                            */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Uninstall I2C call back function for I2C normal function, Arbitration loss, Bus error     */
/*                                                  and Counter timeout .                                  */       
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvI2C_UninstallCallBack(E_I2C_PORT port, E_I2C_CALLBACK_TYPE Type)
{
    if (port == I2C_PORT0)
    { 
        switch(Type)
        {
            case I2CFUNC:
            {
                I2CHandler0.I2CCallBackFn = 0;  
                break;
            }
            case ARBITLOSS:
            {
                I2CHandler0.ArbitLossCallBackFn = 0;  
                break;
            }
            case BUSERROR:
            {
                I2CHandler0.BusErrCallBackFn = 0;  
                break;
            }        
            case TIMEOUT:
            {
                I2CHandler0.TimeoutCallBackFn = 0;  
                break;
            }
            default:
                return E_DRVI2C_ERR_ARGUMENT;   
        }  
    }
    else if (port == I2C_PORT1)
    { 
        switch(Type)
        {
            case I2CFUNC:
            {
                I2CHandler1.I2CCallBackFn = 0;  
                break;
            }
            case ARBITLOSS:
            {
                I2CHandler1.ArbitLossCallBackFn = 0;  
                break;
            }
            case BUSERROR:
            {
                I2CHandler1.BusErrCallBackFn = 0;  
                break;
            }        
            case TIMEOUT:
            {
                I2CHandler1.TimeoutCallBackFn = 0;  
                break;
            }
            default:
                return E_DRVI2C_ERR_ARGUMENT;   
        }   
    }       
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvI2C_GetVersion                                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               The DrvI2C version number                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get the DrvI2C version number                                    */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvI2C_GetVersion(void)
{
    return DRVI2C_VERSION_NUM;
}

