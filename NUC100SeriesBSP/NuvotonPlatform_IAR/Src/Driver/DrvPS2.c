/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "NUC1xx.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "DrvPS2.h"
#include "DrvSYS.h"


/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
static PFN_DRVPS2_CALLBACK *g_pfnPS2callback = NULL;
 	

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPS2_Open                                                                        	   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*               None                                    	                                     		   */
/*                                                                                                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS                                                                          	   */
/*                                                                                                         */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to initialize PS2                                                   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPS2_Open(void)
{
	SYS->IPRSTC2.PS2_RST = 1;
    SYS->IPRSTC2.PS2_RST = 0;

    SYSCLK->APBCLK.PS2_EN = 1;

    _DRVPS2_PS2ENABLE();  
    _DRVPS2_TXFIFO(1);
    
    _DRVPS2_CLRFIFO();

	return E_SUCCESS;
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvPS2_Close                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*					None                                                                                   */
/* RETURN                                                                                                  */
/*                  None                                                                                   */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*                  Disable PS2 IP, disable ISR.                                                  		   */
/*---------------------------------------------------------------------------------------------------------*/

void DrvPS2_Close()
{
	PS2->PS2CON.PS2EN = 0;
	SYSCLK->APBCLK.PS2_EN = 0;
	
	SYS->IPRSTC2.PS2_RST = 1;
    SYS->IPRSTC2.PS2_RST = 0;
    
    PS2->PS2CON.TXFIFO_DEPTH = 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Interrupt Handler                                                                                 	   */
/*---------------------------------------------------------------------------------------------------------*/
void PS2_IRQHandler(void)
{
    uint32_t u32IntStatus;

    u32IntStatus = PS2->u32INTID;
    PS2->u32INTID = 3;
    if(g_pfnPS2callback != NULL)
    {
        g_pfnPS2callback(u32IntStatus);
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPS2_EnableInt                                                                     	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               u32InterruptFlag -[in]	DRVPS2_TXINT/DRVPS2_RXINT                                          */
/* Returns:                                                                                                */
/*                                                                                                         */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to enable PS2 Interrupt and Install the call back function          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t
DrvPS2_EnableInt(
	uint32_t  u32InterruptFlag,
	PFN_DRVPS2_CALLBACK pfncallback
)
{

	PS2->PS2CON.TXINTEN = (u32InterruptFlag & DRVPS2_TXINT)?1:0;
	PS2->PS2CON.RXINTEN = (u32InterruptFlag & DRVPS2_RXINT)?1:0;

	g_pfnPS2callback = pfncallback;
	
	NVIC_SetPriority (PS2_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
	NVIC_EnableIRQ(PS2_IRQn);

	return E_SUCCESS;

}

																  
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPS2_IsIntEnabled                                                                 	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               u32InterruptFlag -[in]	DRVPS2_TXINT/DRVPS2_RXINT                                          */
/* Returns:                                                                                                */
/*                                                                                                         */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to get the interrupt enable status								   */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t 
DrvPS2_IsIntEnabled(
	uint32_t	u32InterruptFlag
)
{
	uint32_t u32Reg = *((uint32_t *)&(PS2->PS2CON));
	return (u32Reg &
		 (u32InterruptFlag & (DRVPS2_RXINT | DRVPS2_TXINT)));
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPS2_DisableInt                                                                   	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               u32InterruptFlag -[in]	DRVPS2_TXINT/DRVPS2_RXINT										   */
/* Returns:                                                                                                */
/*                                                                                                         */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to disable PS2 Interrupt and uninstall the call back function        */
/*---------------------------------------------------------------------------------------------------------*/
 
void DrvPS2_DisableInt(
	uint32_t	u32InterruptFlag
)
{
	if(u32InterruptFlag & DRVPS2_TXINT)
		_DRVPS2_TXINTDISABLE();
	if(u32InterruptFlag & DRVPS2_RXINT)
		_DRVPS2_RXINTDISABLE();

	g_pfnPS2callback = NULL;
    NVIC_DisableIRQ(PS2_IRQn);

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPS2_ClearInt                                                                     	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               u32InterruptFlag     -[in]   DRVPS2_TXINT/DRVPS2_RXINT                                    */
/* Returns:                                                                                                */
/*                                                                                                         */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to clear PS2 Interrupt								        	   */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t
DrvPS2_ClearInt(
	uint32_t	u32InterruptFlag
)
{
	//Write '1' to clear int
	
	if((u32InterruptFlag & DRVPS2_RXINT) == DRVPS2_RXINT)	   
		_DRVPS2_INTCLR(1);

	if((u32InterruptFlag & DRVPS2_TXINT) == DRVPS2_TXINT)
		_DRVPS2_INTCLR(2);

	return E_SUCCESS;


}	


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPS2_GetIntStatus                                                                 	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               u32InterruptFlag     -[in]   DRVPS2_RXINT / DRVPS2_TXINT                                  */
/* Returns:                                                                                                */
/*                                                                                                         */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to get the interrupt status 										   */
/*---------------------------------------------------------------------------------------------------------*/
int8_t
DrvPS2_GetIntStatus(
	uint32_t	u32InterruptFlag
)
{	
	switch(u32InterruptFlag)
	{
		case DRVPS2_RXINT:
			if(PS2->u32INTID & DRVPS2_RXINT)		  	// Rx interrupt
				return TRUE;
			break;
		case DRVPS2_TXINT:
			if(PS2->u32INTID & DRVPS2_TXINT)		    // Tx interrupt
				return TRUE;
			break;
		default:
			break;
	}

	
	return FALSE;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPS2_SetTxFIFODepth                                                                     */
/*                                                                                                         */
/* Parameter:        																					   */	
/*               u32TxFIFODepth   - 0 to 15                                               				   */
/*                                                                                                         */
/* Returns:                                                                                                */
/*                                                                                                         */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to set Tx FIFO Depth                                                 */
/*---------------------------------------------------------------------------------------------------------*/

void DrvPS2_SetTxFIFODepth(
	uint16_t	u32TxFIFODepth
)
{
	_DRVPS2_TXFIFO(u32TxFIFODepth);
}

 
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPS2_Read                                                                               */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               pu8RxBuf       	the pointer of rx buffer                                               */
/* Returns:                                                                                                */
/*               E_SUCCESS                                                                          	   */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to read one Rx byte from RX register                                 */
/*---------------------------------------------------------------------------------------------------------*/
								  
int32_t 
DrvPS2_Read(
	uint8_t		*pu8RxBuf
)
{   
	*pu8RxBuf = _DRVPS2_RXDATA();
	
    return E_SUCCESS;
	
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPS2_Write	                                                                           */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               pu32TxBuf       	the pointer of tx buffer                                               */
/*               u32WriteBytes      the bytes count of send data                                           */
/* Returns:                                                                                                */
/*               E_SUCCESS                                                                          	   */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is to write data to TX fifo and wait Tx finished by ps2                      */
/*               if data count sent is less than 16 bytes, please use macro for speed                      */
/*---------------------------------------------------------------------------------------------------------*/

int32_t 
DrvPS2_Write(
	uint32_t	*pu32TxBuf, 
	uint32_t	u32WriteBytes
)
{
    uint32_t  u32delayno, txcnt, remainder;
    uint8_t i=0;
    
    txcnt = u32WriteBytes/DRVPS2_TXFIFODEPTH;
    remainder = u32WriteBytes % DRVPS2_TXFIFODEPTH;
    if(remainder)
    	txcnt++;
    if(u32WriteBytes >= DRVPS2_TXFIFODEPTH)//Tx fifo is 16 bytes
    {
    	_DRVPS2_TXFIFO(DRVPS2_TXFIFODEPTH);
    }
    else
    {
    	_DRVPS2_TXFIFO(u32WriteBytes);
    }
	
	do
	{
		u32delayno = 0;
		while (_DRVPS2_ISTXEMPTY() != 1)
		{
			u32delayno++;
			if ( u32delayno >= 0x40000000 )     	    
				return E_DRVPS2_ERR_TIMEOUT;     	       
		}
		_DRVPS2_TXDATA0(pu32TxBuf[i++]);
		_DRVPS2_TXDATA1(pu32TxBuf[i++]);
		_DRVPS2_TXDATA2(pu32TxBuf[i++]);
		_DRVPS2_TXDATA3(pu32TxBuf[i++]);
		//memcpy((char*)&PS2->TXDATA[0], pu32TxBuf+i, txcnt);

	}while(--txcnt);

    return E_SUCCESS;
	
  
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPS2_GetVersion                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             None                                                                                 	   */
/* Returns:                                                                                                */
/*               Version Number                                                                            */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to get DrvPS2 Version Number                                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t
DrvPS2_GetVersion(void)
{
	return _DRVPS2_VERSION_NUM;
	
}






