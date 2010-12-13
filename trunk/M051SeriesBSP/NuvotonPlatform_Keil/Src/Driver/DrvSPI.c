/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include "M051Series.h"
#include "core_cm0.h"
#include "DrvSPI.h"
#include "DrvSYS.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    uint8_t bBusy;
    PFN_DRVSPI_CALLBACK pfncallback;
    uint32_t u32userData;
} S_DRVSPI_HANDLE;

static S_DRVSPI_HANDLE g_sSpiHandler[2];

static SPI_T * SPI_PORT[2]={SPI0, SPI1};

/*---------------------------------------------------------------------------------------------------------*/
/* Interrupt Handler                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Function: SPI0_IRQHandler                                                                               */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    SPI0 interrupt handler.                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void SPI0_IRQHandler(void)
{
	// write '1' to clear SPI0 interrupt flag
	SPI0->CNTRL.IF = 1;
	
	if(g_sSpiHandler[0].pfncallback != NULL)
	{
		g_sSpiHandler[0].pfncallback(g_sSpiHandler[0].u32userData);
	}
	
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: SPI1_IRQHandler                                                                                 */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Description:                                                                                              */
/*    SPI1 interrupt handler.                                                                                */
/*-----------------------------------------------------------------------------------------------------------*/
void SPI1_IRQHandler(void)
{
	// write '1' to clear SPI1 interrupt flag
	SPI1->CNTRL.IF = 1;
	
	if(g_sSpiHandler[1].pfncallback != NULL)
	{
		g_sSpiHandler[1].pfncallback(g_sSpiHandler[1].u32userData);
	}
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_Open                                                                                        */
/*                                                                                                              */
/* Parameters:        	                                                                                        */
/*         eSpiPort     [in]: Specify the SPI port                                                              */
/*         eMode        [in]: Specify the operation mode (eDRVSPI_MASTER/eDRVSPI_SLAVE)                         */
/*         eType        [in]: Specify the transfer type (eDRVSPI_TYPE0 ~ eDRVSPI_TYPE7)                         */
/*         i32BitLength [in]: Specify the bit length in a transaction (1~32)                                    */
/*                                                                                                              */
/* Returns:                                                                                                     */
/*         E_DRVSPI_ERR_INIT: The specified SPI port has been opened before.                                    */
/*         E_DRVSPI_ERR_BUSY: The specified SPI port is in busy status.                                         */
/*         E_DRVSPI_ERR_BIT_LENGTH: The specified bit length is out of range.                                   */
/*         E_SUCCESS: Success.                                                                                  */
/*                                                                                                              */
/* Description:                                                                                                 */
/*       Configure the operation mode, transfer type and bit length of a transaction of the specified SPI port. */
/*       The timing waveform types:                                                                             */
/*
DRVSPI_TYPE0:          

    CS    --|          Active state           |---
               _   _   _   _   _   _   _   _  
    CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
              
    Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---
          
    Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----


DRVSPI_TYPE1:          

    CS    --|          Active state           |---
               _   _   _   _   _   _   _   _  
    CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
              
    Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----
          
    Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----


DRVSPI_TYPE2:          

    CS    --|          Active state           |---
               _   _   _   _   _   _   _   _  
    CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
              
    Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---
          
    Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---


DRVSPI_TYPE3:          

    CS    --|          Active state           |---
               _   _   _   _   _   _   _   _  
    CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
              
    Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----
          
    Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---


DRVSPI_TYPE4:          

    CS    --|          Active state           |---
           ___   _   _   _   _   _   _   _   ______ 
    CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
              
    Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |------
          
    Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----


DRVSPI_TYPE5:

    CS    --|          Active state           |---
           ___   _   _   _   _   _   _   _   ______ 
    CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
              
    Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----
          
    Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----


DRVSPI_TYPE6:

    CS    --|          Active state           |---
           ___   _   _   _   _   _   _   _   ______ 
    CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
              
    Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |------
          
    Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |------


DRVSPI_TYPE7:

    CS    --|          Active state           |---
           ___   _   _   _   _   _   _   _   ______ 
    CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
              
    Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----
          
    Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----


Master / Slave Transfer Type Matching Table

    DRVSPI_TYPE0 <==> DRVSPI_TYPE3
    DRVSPI_TYPE1 <==> DRVSPI_TYPE1
    DRVSPI_TYPE2 <==> DRVSPI_TYPE2
    DRVSPI_TYPE3 <==> DRVSPI_TYPE0
    DRVSPI_TYPE4 <==> DRVSPI_TYPE7
    DRVSPI_TYPE5 <==> DRVSPI_TYPE5
    DRVSPI_TYPE6 <==> DRVSPI_TYPE6
    DRVSPI_TYPE7 <==> DRVSPI_TYPE4
*/
/*--------------------------------------------------------------------------------------------------------------*/
int32_t DrvSPI_Open(E_DRVSPI_PORT eSpiPort, E_DRVSPI_MODE eMode, E_DRVSPI_TRANS_TYPE eType, int32_t i32BitLength)
{
	int32_t i32TimeOut;
	
	if(g_sSpiHandler[eSpiPort].bBusy)
	{
		return E_DRVSPI_ERR_INIT;
	}
	
	if(eSpiPort == eDRVSPI_PORT0)
	{
		SYSCLK->APBCLK.SPI0_EN        =1;
		SYS->IPRSTC2.SPI0_RST         =1;
		SYS->IPRSTC2.SPI0_RST         =0;
		SYS->P1_MFP.MFP4_AIN4_SS0     =0;
		SYS->P1_MFP.ALT4_AIN4_SS0     =1;
		SYS->P1_MFP.MFP5_AIN5_MOSI0   =0;
		SYS->P1_MFP.ALT5_AIN5_MOSI0   =1;
		SYS->P1_MFP.MFP6_AIN6_MISO0   =0;
		SYS->P1_MFP.ALT6_AIN6_MISO0   =1;
		SYS->P1_MFP.MFP7_AIN7_SCLK0   =0;
		SYS->P1_MFP.ALT7_AIN7_SCLK0   =1;
	}
	else
	{
		SYSCLK->APBCLK.SPI1_EN       =1;
		SYS->IPRSTC2.SPI1_RST        =1;
		SYS->IPRSTC2.SPI1_RST        =0;
		SYS->P0_MFP.MFP4_AD4_SS1     =0;
		SYS->P0_MFP.ALT4_AD4_SS1     =1;
		SYS->P0_MFP.MFP5_AD5_MOSI1   =0;
		SYS->P0_MFP.ALT5_AD5_MOSI1   =1;
		SYS->P0_MFP.MFP6_AD6_MISO1   =0;
		SYS->P0_MFP.ALT6_AD6_MISO1   =1;
		SYS->P0_MFP.MFP7_AD7_SCLK1   =0;
		SYS->P0_MFP.ALT7_AD7_SCLK1   =1;
	}
	
	/* Check busy*/
	i32TimeOut = 0x10000;
	while(SPI_PORT[eSpiPort]->CNTRL.GO_BUSY == 1)
	{
		if(i32TimeOut-- <= 0)
			return E_DRVSPI_ERR_BUSY;
	}
	
	g_sSpiHandler[eSpiPort].bBusy = TRUE;
	g_sSpiHandler[eSpiPort].pfncallback = NULL;
	g_sSpiHandler[eSpiPort].u32userData = 0;
	
	/* Bit length 1 ~ 32 */
	if((i32BitLength <= 0) || (i32BitLength > 32))
	{
		return E_DRVSPI_ERR_BIT_LENGTH;
	}
   
	/* "i32BitLength = 0" means 32 bits */
	if(i32BitLength == 32)
	{
		i32BitLength = 0;
	}
	SPI_PORT[eSpiPort]->CNTRL.TX_BIT_LEN = i32BitLength;
	
	if(eMode == eDRVSPI_SLAVE)
		SPI_PORT[eSpiPort]->CNTRL.SLAVE = 1;
	else
		SPI_PORT[eSpiPort]->CNTRL.SLAVE = 0;
	
	/* Automatic slave select and low active */
	SPI_PORT[eSpiPort]->SSR.AUTOSS = 1;
	
	/* Transition types */
	if(eType==eDRVSPI_TYPE0)
	{
		SPI_PORT[eSpiPort]->CNTRL.CLKP = 0;
		SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 0;
		SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 0;
	}
	else if(eType==eDRVSPI_TYPE1)
	{
		SPI_PORT[eSpiPort]->CNTRL.CLKP = 0;
		SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 1;
		SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 0;
	}
	else if(eType==eDRVSPI_TYPE2)
	{
		SPI_PORT[eSpiPort]->CNTRL.CLKP = 0;
		SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 0;
		SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 1;
	}
	else if(eType==eDRVSPI_TYPE3)
	{
		SPI_PORT[eSpiPort]->CNTRL.CLKP = 0;
		SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 1;
		SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 1;
	}
	else if(eType==eDRVSPI_TYPE4)
	{
		SPI_PORT[eSpiPort]->CNTRL.CLKP = 1;
		SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 0;
		SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 0;
	}
	else if(eType==eDRVSPI_TYPE5)
	{
		SPI_PORT[eSpiPort]->CNTRL.CLKP = 1;
		SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 1;
		SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 0;
	}
	else if(eType==eDRVSPI_TYPE6)
	{
		SPI_PORT[eSpiPort]->CNTRL.CLKP = 1;
		SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 0;
		SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 1;
	}
	else
	{
		SPI_PORT[eSpiPort]->CNTRL.CLKP = 1;
		SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 1;
		SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 1;
	}
	

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_Close                                                                                  */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    eSpiPort [in]:  Specify the SPI port.                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Close the specified SPI module and disable the SPI interrupt.                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_Close(E_DRVSPI_PORT eSpiPort)
{
	int32_t i32TimeOut;
	
	g_sSpiHandler[eSpiPort].bBusy = FALSE;
	g_sSpiHandler[eSpiPort].pfncallback = NULL;
	g_sSpiHandler[eSpiPort].u32userData = 0;

	/* Check SPI state */
	i32TimeOut = 0x10000;
	while(SPI_PORT[eSpiPort]->CNTRL.GO_BUSY == 1)
	{
		if(i32TimeOut-- <= 0)
			break;
	}
   
   if(eSpiPort == eDRVSPI_PORT0)
	{
		NVIC_DisableIRQ(SPI0_IRQn);
		SYS->IPRSTC2.SPI0_RST=1;
		SYS->IPRSTC2.SPI0_RST=0;
		SYSCLK->APBCLK.SPI0_EN=0;
	}
	else
	{
		NVIC_DisableIRQ(SPI1_IRQn);
		SYS->IPRSTC2.SPI1_RST=1;
		SYS->IPRSTC2.SPI1_RST=0;
		SYSCLK->APBCLK.SPI1_EN=0;
	}
   
}

/*------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetEndian                                                                                       */
/*                                                                                                                  */
/* Parameters:        	                                                                                            */
/*    eSpiPort [in]: Specify the SPI port                                                                           */
/*    eEndian  [in]: Specify LSB first or MSB first                                                                 */
/*                                                                                                                  */
/* Returns:                                                                                                         */
/*    None.                                                                                                         */
/*                                                                                                                  */
/* Description:                                                                                                     */
/*    Dertermine to transfer data with LSB first or MSB first                                                       */
/*------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_SetEndian(E_DRVSPI_PORT eSpiPort, E_DRVSPI_ENDIAN eEndian)
{

	if(eEndian == eDRVSPI_LSB_FIRST)
		SPI_PORT[eSpiPort]->CNTRL.LSB = 1;
	else
		SPI_PORT[eSpiPort]->CNTRL.LSB = 0;

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetBitLength                                                                           */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    eSpiPort     [in]: Specify the SPI port                                                              */
/*    i32BitLength [in]: Specify the bit length (1~32 bits)                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    E_SUCCESS              : Success.                                                                    */
/*    E_DRVSPI_ERR_BIT_LENGTH: The bit length is out of range.                                             */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the bit length of SPI transfer.                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvSPI_SetBitLength(E_DRVSPI_PORT eSpiPort, int32_t i32BitLength)
{

    if((i32BitLength < 1) || (i32BitLength > 32))
    {
        return E_DRVSPI_ERR_BIT_LENGTH;
    }
    if(i32BitLength == 32)
        i32BitLength = 0;

	SPI_PORT[eSpiPort]->CNTRL.TX_BIT_LEN = i32BitLength;
	
    return E_SUCCESS;
}

/*----------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetByteReorder                                                                          */
/*                                                                                                          */
/* Parameters:        	                                                                                    */
/*    eSpiPort     [in]: Specify the SPI port                                                               */
/*    eOption      [in]: the options of Byte Reorder function and Byte Suspend function.                    */
/*                       eDRVSPI_BYTE_REORDER_SUSPEND_DISABLE: both Byte Reorder function and Byte Suspend  */
/*                                                             function are disabled.                       */
/*                       eDRVSPI_BYTE_REORDER_SUSPEND        : both Byte Reorder function and Byte Suspend  */
/*                                                             function are enabled.                        */
/*                       eDRVSPI_BYTE_REORDER                : only enable the Byte Reorder function.       */
/*                       eDRVSPI_BYTE_SUSPEND                : only enable the Byte Suspend function.       */
/*                       The Byte Suspend function is only available in 32-bit transaction.                 */
/*                                                                                                          */
/* Returns:                                                                                                 */
/*    E_SUCCESS              : Success.                                                                     */
/*    E_DRVSPI_ERR_BIT_LENGTH: The bit length is not 16-, 24- or 32-bit.                                    */
/*                                                                                                          */
/* Description:                                                                                             */
/*    Enable/disable Byte Reorder function.                                                                 */
/*    The Byte Reorder function is supported only in 16-, 24- and 32-bit transaction mode.                  */
/*----------------------------------------------------------------------------------------------------------*/
int32_t DrvSPI_SetByteReorder(E_DRVSPI_PORT eSpiPort, E_DRVSPI_BYTE_REORDER eOption)
{
	if( (SPI_PORT[eSpiPort]->CNTRL.TX_BIT_LEN) % 8 )
		return E_DRVSPI_ERR_BIT_LENGTH;

	SPI_PORT[eSpiPort]->CNTRL.BYTE_REORDER = eOption;

    return E_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetSuspendCycle                                                                          */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    eSpiPort    [in]: Specify the SPI port.                                                                */
/*    i32Interval [in]: In burst transfer mode, this value specified the delay clocks between successive     */
/*                      transactions. If the Byte Suspend function is enabled, it specified the delay clocks */
/*                      among each byte. It could be 2~17.                                                   */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    E_DRVSPI_ERR_SUSPEND_INTERVAL: The suspend interval setting is out of range.                           */
/*    E_SUCCESS: Success.                                                                                    */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Set the number of clock cycle of the suspend interval.                                                 */
/*-----------------------------------------------------------------------------------------------------------*/
int32_t DrvSPI_SetSuspendCycle(E_DRVSPI_PORT eSpiPort, int32_t i32Interval)
{
    if((i32Interval < 2) || (i32Interval > 17))
	{
		return E_DRVSPI_ERR_SUSPEND_INTERVAL;
	}

	SPI_PORT[eSpiPort]->CNTRL.SP_CYCLE = i32Interval-2;

	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetTriggerMode                                                                         */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    eSpiPort       [in]: Specify the SPI port                                                            */
/*    eSSTriggerMode [in]: Specify the trigger mode. (eDRVSPI_EDGE_TRIGGER or eDRVSPI_LEVEL_TRIGGER)       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the trigger mode of slave select pin.                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_SetTriggerMode(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SSLTRIG eSSTriggerMode)
{
    SPI_PORT[eSpiPort]->SSR.SS_LTRIG = eSSTriggerMode;
}

/*----------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetSlaveSelectActiveLevel                                                               */
/*                                                                                                          */
/* Parameters:        	                                                                                    */
/*    eSpiPort   [in]: Specify the SPI port                                                                 */
/*    eSSActType [in]: Select the active type of slave select pin.                                          */
/*                     eDRVSPI_ACTIVE_LOW_FALLING: Slave select pin is active low in level-trigger mode;    */
/*                                                 or falling-edge trigger in edge-trigger mode.            */
/*                     eDRVSPI_ACTIVE_HIGH_RISING: Slave select pin is active high in level-trigger mode;   */
/*                                                 or rising-edge trigger in edge-trigger mode.             */
/*                                                                                                          */
/* Returns:                                                                                                 */
/*    None.                                                                                                 */
/*                                                                                                          */
/* Description:                                                                                             */
/*    Set the active level of slave select.                                                                 */
/*----------------------------------------------------------------------------------------------------------*/
void DrvSPI_SetSlaveSelectActiveLevel(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SS_ACT_TYPE eSSActType)
{
    SPI_PORT[eSpiPort]->SSR.SS_LVL = eSSActType;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_GetLevelTriggerStatus                                                                            */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    TRUE : The transaction number and the transferred bit length met the requirements which defines in TX_NUM      */
/*           and TX_BIT_LEN among one transfer.                                                                      */
/*    FALSE: The transaction number or the transferred bit length of one transaction doesn't meet the requirements.  */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Get the level-trigger transmission status.                                                                     */
/*-------------------------------------------------------------------------------------------------------------------*/
uint8_t DrvSPI_GetLevelTriggerStatus(E_DRVSPI_PORT eSpiPort)
{
	if(SPI_PORT[eSpiPort]->SSR.LTRIG_FLAG==1)
		return TRUE;
	else
		return FALSE;
}

/*----------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_EnableAutoSS                                                                            */
/*                                                                                                          */
/* Parameters:        	                                                                                    */
/*    eSpiPort   [in]: Specify the SPI port                                                                 */
/*                                                                                                          */
/* Returns:                                                                                                 */
/*    None.                                                                                                 */
/*                                                                                                          */
/* Description:                                                                                             */
/*    Enable the automatic slave select function.                                                           */
/*----------------------------------------------------------------------------------------------------------*/
void DrvSPI_EnableAutoSS(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->SSR.AUTOSS = 1;
    SPI_PORT[eSpiPort]->SSR.SSR = 1;
}

/*----------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_DisableAutoSS                                                                           */
/*                                                                                                          */
/* Parameters:        	                                                                                    */
/*    eSpiPort   [in]: Specify the SPI port                                                                 */
/*                                                                                                          */
/* Returns:                                                                                                 */
/*    None.                                                                                                 */
/*                                                                                                          */
/* Description:                                                                                             */
/*    Disable the Automatic Slave Slect function.                                                           */
/*----------------------------------------------------------------------------------------------------------*/
void DrvSPI_DisableAutoSS(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->SSR.AUTOSS = 0;
    SPI_PORT[eSpiPort]->SSR.SSR = 0;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetSS                                                                                            */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*   eSpiPort   [in]: Specify the SPI port                                                                           */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*   None.                                                                                                           */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*   Configure the slave select pin.                                                                                 */
/*   In automatic slave select operation, call this function to select the slave select pin. The state of slave      */
/*   select pin will be controlled by hardware.                                                                      */
/*   In manual slave select operation, the slave select pin will be set to active state.                             */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_SetSS(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->SSR.SSR = 1;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_ClrSS                                                                                            */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*   eSpiPort  [in]: Specify the SPI port                                                                            */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*   None.                                                                                                           */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*   In automatic slave select operation, call this function to deselect the slave select pin. The slave select pin  */
/*   will be set to inactive state.                                                                                  */
/*   In manual slave select operation, the slave select pin will be set to inactive state.                           */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_ClrSS(E_DRVSPI_PORT eSpiPort)
{
	SPI_PORT[eSpiPort]->SSR.SSR = 0;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_IsBusy                                                                                           */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    TRUE:  The SPI port is in busy.                                                                                */
/*    FALSE: The SPI port is not in busy.                                                                            */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Check the busy status of the specified SPI port.                                                               */
/*-------------------------------------------------------------------------------------------------------------------*/
uint8_t DrvSPI_IsBusy(E_DRVSPI_PORT eSpiPort)
{
    return ((SPI_PORT[eSpiPort]->CNTRL.GO_BUSY)?TRUE:FALSE);
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_BurstTransfer                                                                                    */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort    [in]: Specify the SPI port                                                                         */
/*    i32BurstCnt [in]: Specify the transaction number in one transfer. It could be 1 or 2.                          */
/*    i32Interval [in]: Specify the delay clocks between successive transactions. It could be 2~17.                  */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    E_DRVSPI_ERR_BURST_CNT        : The transaction number is out of range.                                        */
/*    E_DRVSPI_ERR_SUSPEND_INTERVAL : The suspend interval setting is out of range.                                  */
/*    E_SUCCESS                     : Success.                                                                       */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Configure the burst transfer settings.                                                                         */
/*-------------------------------------------------------------------------------------------------------------------*/
int32_t DrvSPI_BurstTransfer(E_DRVSPI_PORT eSpiPort, int32_t i32BurstCnt, int32_t i32Interval)
{

	if((i32BurstCnt < 1) || (i32BurstCnt > 2))
	{
		return E_DRVSPI_ERR_BURST_CNT;
	}
	
	if((i32Interval < 2) || (i32Interval > 17))
	{
		return E_DRVSPI_ERR_SUSPEND_INTERVAL;
	}

	SPI_PORT[eSpiPort]->CNTRL.TX_NUM = i32BurstCnt-1;
	SPI_PORT[eSpiPort]->CNTRL.SP_CYCLE = i32Interval-2;

	return E_SUCCESS;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetClockFreq                                                                                     */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort  [in]: Specify the SPI port                                                                           */
/*    u32Clock1 [in]: Specify the SPI clock rate in Hz. It's the clock rate of SPI base clock or variable clock 1.   */
/*    u32Clock2 [in]: Specify the SPI clock rate in Hz. It's the clock rate of variable clock 2.                     */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    The actual clock rate of SPI engine clock is returned.                                                         */
/*    SPI engine clock rate = APB clock rate / ((DIVIDER + 1) * 2)                                                   */
/*    The actual clock may different to the target SPI clock due to hardware limitation.                             */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Configure the SPI clock.                                                                                       */
/*-------------------------------------------------------------------------------------------------------------------*/
uint32_t DrvSPI_SetClockFreq(E_DRVSPI_PORT eSpiPort, uint32_t u32Clock1, uint32_t u32Clock2)
{
    uint32_t u32Div;
    uint32_t u32Pclk;
    
    u32Pclk = DrvSYS_GetHCLKFreq();
    
	if(u32Clock2!=0)
	{
		u32Div = (((u32Pclk / u32Clock2) + 1) >> 1) - 1;
		if(u32Div > 65535)
		    u32Div = 65535;
		SPI_PORT[eSpiPort]->DIVIDER.DIVIDER2 = u32Div;
	}
	else
		SPI_PORT[eSpiPort]->DIVIDER.DIVIDER2 = 0xFFFF;
	
	if(u32Clock1!=0)
	{
		u32Div = (((u32Pclk / u32Clock1) + 1) >> 1) - 1;
		if(u32Div > 0xFFFF)
		    u32Div = 0xFFFF;
		SPI_PORT[eSpiPort]->DIVIDER.DIVIDER = u32Div;
		
	}
	else
	{
		u32Div = 0xFFFF;
		SPI_PORT[eSpiPort]->DIVIDER.DIVIDER = u32Div;
	}
	return ( u32Pclk / ((u32Div+1)*2) );
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_GetClock1Freq                                                                                    */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    The current SPI bus clock frequency in Hz.                                                                     */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Get the SPI engine clock rate in Hz.                                                                           */
/*-------------------------------------------------------------------------------------------------------------------*/
uint32_t DrvSPI_GetClock1Freq(E_DRVSPI_PORT eSpiPort)
{
	uint32_t u32Div;
	uint32_t u32ApbClock;

	u32ApbClock = DrvSYS_GetHCLKFreq();
	u32Div = SPI_PORT[eSpiPort]->DIVIDER.DIVIDER;
	return ((u32ApbClock >> 1) / (u32Div + 1));   /* SPI_CLK = APB_CLK / ((Divider + 1) * 2) */
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_GetClock2Freq                                                                                    */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    The frequency of variable clock 2 in Hz.                                                                       */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Get the clock rate of variable clock 2 in Hz.                                                                  */
/*-------------------------------------------------------------------------------------------------------------------*/
uint32_t DrvSPI_GetClock2Freq(E_DRVSPI_PORT eSpiPort)
{
	uint32_t u32Div;
	uint32_t u32ApbClock;

	u32ApbClock = DrvSYS_GetHCLKFreq();
	u32Div = SPI_PORT[eSpiPort]->DIVIDER.DIVIDER2;
	return ((u32ApbClock >> 1) / (u32Div + 1));   /* SPI_CLK = APB_CLK / ((Divider + 1) * 2) */
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetVariableClockFunction                                                                         */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*    bEnable    [in]: TRUE  -- Enable variable clock function                                                       */
/*                     FALSE -- Disable variable clock function                                                      */
/*    u32Pattern [in]: Specify the variable clock pattern                                                            */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Set the variable clock function.                                                                               */
/*    If the bit pattern of VARCLK is ．0・, the output frequency of SPICLK is according to the value of DIVIDER.      */
/*    If the bit pattern of VARCLK is ．1・, the output frequency of SPICLK is according to the value of DIVIDER2.     */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_SetVariableClockFunction(E_DRVSPI_PORT eSpiPort, uint8_t bEnable, uint32_t u32Pattern)
{
	if(bEnable)
	{
		SPI_PORT[eSpiPort]->CNTRL.VARCLK_EN = 1;
		SPI_PORT[eSpiPort]->VARCLK = u32Pattern;
	}
	else
		SPI_PORT[eSpiPort]->CNTRL.VARCLK_EN = 0;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_EnableInt                                                                                        */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort    [in]: Specify the SPI port                                                                         */
/*    pfnCallback [in]: The callback function of SPI interrupt.                                                      */
/*    u32UserData [in]: The parameter which will be passed to the callback function.                                 */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Enable the SPI interrupt of the specified SPI port and install the callback function.                          */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_EnableInt(E_DRVSPI_PORT eSpiPort, PFN_DRVSPI_CALLBACK pfnCallback, uint32_t u32UserData)
{
	
	if(pfnCallback != NULL)
	{
		g_sSpiHandler[eSpiPort].pfncallback = pfnCallback;
		g_sSpiHandler[eSpiPort].u32userData = u32UserData;
	}
	
	SPI_PORT[eSpiPort]->CNTRL.IE = 1;
	
	if(eSpiPort == eDRVSPI_PORT0)
	{
	    NVIC_EnableIRQ(SPI0_IRQn);
	}
	else
	{
	    NVIC_EnableIRQ(SPI1_IRQn);
	}
}	

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_DisableInt                                                                                       */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Disable the SPI interrupt.                                                                                     */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_DisableInt(E_DRVSPI_PORT eSpiPort)
{
	g_sSpiHandler[eSpiPort].pfncallback = NULL;
	g_sSpiHandler[eSpiPort].u32userData = 0;
	
	SPI_PORT[eSpiPort]->CNTRL.IE = 0;

	if(eSpiPort == eDRVSPI_PORT0)
	{
	    NVIC_DisableIRQ(SPI0_IRQn);
	}
	else
	{
	    NVIC_DisableIRQ(SPI1_IRQn);
	}
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_GetIntFlag                                                                                       */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    0: SPI interrupt doesn't occur                                                                                 */
/*    1: SPI interrupt occurs                                                                                        */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Return the SPI interrupt flag                                                                                  */
/*-------------------------------------------------------------------------------------------------------------------*/
uint32_t DrvSPI_GetIntFlag(E_DRVSPI_PORT eSpiPort)
{
	return (SPI_PORT[eSpiPort]->CNTRL.IF);
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_ClrIntFlag                                                                                       */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None                                                                                                           */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Clear the SPI interrupt flag                                                                                   */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_ClrIntFlag(E_DRVSPI_PORT eSpiPort)
{
	SPI_PORT[eSpiPort]->CNTRL.IF = 1;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SingleRead                                                                                       */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]:  Specify the SPI port                                                                         */
/*    pu32Data   [out]: Store the data got from the SPI bus.                                                         */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    TRUE:  The data stored in pu32Data is valid.                                                                   */
/*    FALSE: The data stored in pu32Data is invalid.                                                                 */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Read data from SPI Rx registers and trigger SPI for next transfer.                                             */
/*-------------------------------------------------------------------------------------------------------------------*/
uint8_t DrvSPI_SingleRead(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Data)
{
	if(SPI_PORT[eSpiPort]->CNTRL.GO_BUSY==1)
		return FALSE;

	*pu32Data = SPI_PORT[eSpiPort]->RX[0];
	SPI_PORT[eSpiPort]->CNTRL.GO_BUSY = 1;
	return TRUE;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SingleWrite                                                                                      */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort  [in]:  Specify the SPI port                                                                          */
/*    pu32Data  [in]:  Store the data which will be transmitted through the SPI bus.                                 */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    TRUE:  The data stored in pu32Data has been transferred.                                                       */
/*    FALSE: The SPI is in busy. The data stored in pu32Data has not been transferred.                               */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Write data to SPI bus and trigger SPI to start transfer.                                                       */
/*-------------------------------------------------------------------------------------------------------------------*/
uint8_t DrvSPI_SingleWrite(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Data)
{
	if(SPI_PORT[eSpiPort]->CNTRL.GO_BUSY==1)
		return FALSE;

	SPI_PORT[eSpiPort]->TX[0] = *pu32Data;
	SPI_PORT[eSpiPort]->CNTRL.GO_BUSY = 1;
	return TRUE;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_BurstRead                                                                                        */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort  [in]:  Specify the SPI port                                                                          */
/*    pu32Buf   [out]: Store the data got from the SPI bus.                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    TRUE:  The data stored in pu32Buf is valid.                                                                    */
/*    FALSE: The data stored in pu32Buf is invalid.                                                                  */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Read two words of data from SPI Rx registers and then trigger SPI for next transfer.                           */
/*-------------------------------------------------------------------------------------------------------------------*/
uint8_t DrvSPI_BurstRead(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Buf)
{
	if(SPI_PORT[eSpiPort]->CNTRL.GO_BUSY==1)
		return FALSE;

	pu32Buf[0] = SPI_PORT[eSpiPort]->RX[0];
	pu32Buf[1] = SPI_PORT[eSpiPort]->RX[1];
	SPI_PORT[eSpiPort]->CNTRL.GO_BUSY = 1;

	return TRUE;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_BurstWrite                                                                                       */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort  [in]:  Specify the SPI port                                                                          */
/*    pu32Buf   [in]:  Store the data which will be transmitted through the SPI bus.                                 */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    TRUE:  The data stored in pu32Buf has been transferred.                                                        */
/*    FALSE: The SPI is in busy. The data stored in pu32Buf has not been transferred.                                */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Write two words of data to SPI bus and then trigger SPI to start transfer.                                     */
/*-------------------------------------------------------------------------------------------------------------------*/
uint8_t DrvSPI_BurstWrite(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Buf)
{
	if(SPI_PORT[eSpiPort]->CNTRL.GO_BUSY==1)
		return FALSE;

	SPI_PORT[eSpiPort]->TX[0] = pu32Buf[0];
	SPI_PORT[eSpiPort]->TX[1] = pu32Buf[1];
	SPI_PORT[eSpiPort]->CNTRL.GO_BUSY = 1;

	return TRUE;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_DumpRxRegister                                                                                   */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort     [in]:  Specify the SPI port                                                                       */
/*    pu32Buf      [out]: Store the data got from Rx registers.                                                      */
/*    u32DataCount [in]:  The count of data read from Rx registers.                                                  */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    The count of data actually read from Rx registers.                                                             */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Read data from Rx registers. This function will not trigger a SPI data transfer.                               */
/*-------------------------------------------------------------------------------------------------------------------*/
uint32_t DrvSPI_DumpRxRegister(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Buf, uint32_t u32DataCount)
{
	uint32_t i;
	
	if(u32DataCount>2)
		u32DataCount = 2;
	
	for(i=0; i<u32DataCount; i++)
	{
		pu32Buf[i] = SPI_PORT[eSpiPort]->RX[i];
	}

    return u32DataCount;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetTxRegister                                                                                    */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort     [in]:  Specify the SPI port                                                                       */
/*    pu32Buf      [in]:  Store the data which will be written to Tx registers.                                      */
/*    u32DataCount [in]:  The count of data write to Tx registers.                                                   */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    The count of data actually written to Tx registers.                                                            */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Write data to Tx registers. This function will not trigger a SPI data transfer.                                */
/*-------------------------------------------------------------------------------------------------------------------*/
uint32_t DrvSPI_SetTxRegister(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Buf, uint32_t u32DataCount)
{
	uint32_t i;

	if(u32DataCount>2)
		u32DataCount = 2;
	
	for(i=0; i<u32DataCount; i++)
	{
		SPI_PORT[eSpiPort]->TX[i] = pu32Buf[i];
	}

    return u32DataCount;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetGo                                                                                            */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort     [in]:  Specify the SPI port                                                                       */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Set the GO_BUSY bit to trigger a SPI data trasfer.                                                             */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_SetGo(E_DRVSPI_PORT eSpiPort)
{
	SPI_PORT[eSpiPort]->CNTRL.GO_BUSY = 1;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_ClrGo                                                                                            */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort     [in]:  Specify the SPI port                                                                       */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Clear the GO_BUSY bit.                                                                                         */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_ClrGo(E_DRVSPI_PORT eSpiPort)
{
	SPI_PORT[eSpiPort]->CNTRL.GO_BUSY = 0;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_GetVersion                                                                                       */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    Version number.                                                                                                */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Get the version number of NUC100 SPI driver.                                                                   */
/*-------------------------------------------------------------------------------------------------------------------*/
uint32_t DrvSPI_GetVersion(void)
{
	return DRVSPI_VERSION_NUM;
}
