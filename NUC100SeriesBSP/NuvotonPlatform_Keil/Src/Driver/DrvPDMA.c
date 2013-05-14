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
#include "DrvPDMA.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

#define MAX_CHANNEL_NUM   			9
#define CHANNEL_OFFSET    			0x100
#define MAX_TRANSFER_BYTE_COUNT     0xFFFF


static PFN_DRVPDMA_CALLBACK  g_pfnPDMACallback[MAX_CHANNEL_NUM][3]  = {
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0}};								

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_Init				                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to enable AHB PDMA engine clock                                      */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPDMA_Init(void)
{
	/* Enable PDMA Clock */
	SYSCLK->AHBCLK.PDMA_EN  =1;		 	
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_Close				                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to disable all PDMA channel clock and AHB PDMA clock                 */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPDMA_Close(void)
{
   	/* Disable All PDMA clock */
	outpw(&PDMA_GCR->GCRCSR,0x0);
	
	/* Disable PDMA clock */
	SYSCLK->AHBCLK.PDMA_EN  =0;	
	
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_Open				                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel	-[in]       PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                                */
/*               sParam	   	-[in]	    - the struct parameter to configure PDMA   	         			   */
/*                         It consists of                                                                  */
/*                            sSrcCtrl.u32Addr 			- Source Address 					        	   */									        
/*                            sSrcCtrl.eAddrDirection 	- Source Direction                  			   */									        
/*                            sDestCtrl.u32Addr 		- Destination Address 					           */									        
/*                            sDestCtrl.eAddrDirection 	- Destination Direction                  		   */									        
/*                            u8TransWidth 				- Transfer Width                        		   */
/*                            u8Mode 					- Operation Mode   			                       */
/*                            i32ByteCnt   				- Byte Count                                       */
/* Returns:                                                                                                */
/*               E_SUCCESS   					Sueccess                                                   */
/*               E_DRVPDMA_ERR_PORT_INVALID     Invalid port number                                        */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function configures PDMA settin     	                                               */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_Open(E_DRVPDMA_CHANNEL_INDEX eChannel,STR_PDMA_T *sParam)
{						  
	PDMA_T * tPDMA;

	SYSCLK->AHBCLK.PDMA_EN  =1;	
    /*-----------------------------------------------------------------------------------------------------*/
    /* Check PDMA channel                                                                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    if (eChannel > eDRVPDMA_CHANNEL_8)
    {
		return E_DRVPDMA_ERR_PORT_INVALID;
    }

	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 

	/*-----------------------------------------------------------------------------------------------------*/
    /* Set PDMA settings                                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
	outpw(&PDMA_GCR->GCRCSR,inpw(&PDMA_GCR->GCRCSR) | 1<<(eChannel+8) );     	/* Enable Channel Clock */

 
 	tPDMA->CSR.PDMACEN 	= 1;   													/* Enable PDMA Channel */
	tPDMA->SAR	 		= sParam->sSrcCtrl.u32Addr;								/* Set Source Address */
	tPDMA->DAR 			= sParam->sDestCtrl.u32Addr;	    					/* Set Destination Address */
	tPDMA->CSR.SAD_SEL 	= sParam->sSrcCtrl.eAddrDirection; 
	tPDMA->CSR.DAD_SEL 	= sParam->sDestCtrl.eAddrDirection;
	tPDMA->CSR.APB_TWS 	= sParam->u8TransWidth;
	tPDMA->CSR.MODE_SEL = sParam->u8Mode;										/* Set Control Register */
	tPDMA->u32BCR 			= sParam->i32ByteCnt;								    /* Set Byte Count Register */
	return E_SUCCESS;    
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_IsCHBusy   		                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/* Returns:                                                                                                */
/*               TRUE:  The Channel is busy.                                                               */
/*               FALSE: The Channel is usable.                                                             */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to Get Channel Enable/Disable status                                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_IsCHBusy(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
  	PDMA_T * tPDMA;
    
    if (eChannel > MAX_CHANNEL_NUM  )	    						       /* Check Channel is valid */
        return E_DRVPDMA_ERR_PORT_INVALID;    
 
 	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 

	return (tPDMA->CSR.TRIG_EN)?TRUE:FALSE;								   /* Check Channel is triggering or not */
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetTransferLength	                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               pu32TransferLength     -[out]    The data pointer to save transfer length                 */
/* Returns:                                                                                                */
/*                                                                                                         */
/* Side effects:                                                                                           */
/*               E_SUCCESS                      Success                                                    */
/*               E_DRVPDMA_ERR_PORT_INVALID     Invalid port number                                        */
/* Description:                                                                                            */
/*               The function is used to  get channel transfer length setting                              */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_GetTransferLength(E_DRVPDMA_CHANNEL_INDEX eChannel, uint32_t* pu32TransferLength)
{
	PDMA_T * tPDMA;
    
    if (eChannel > MAX_CHANNEL_NUM  )	    						   /* Check Channel is valid */
        return E_DRVPDMA_ERR_PORT_INVALID;    
    
	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 
	
	*pu32TransferLength = tPDMA->u32BCR;      								    /* Get Transfer Length */

    return E_SUCCESS;       
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_SetAPBTransferWidth				                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/*               eTransferWidth	   	-[in]   Transfer width.It could be                                     */
/*                                              eDRVPDMA_WIDTH_32BITS                                      */
/*                                              eDRVPDMA_WIDTH_8BITS                                       */
/*                                              eDRVPDMA_WIDTH_16BITS                                      */
/* Returns:                                                                                                */
/*               E_SUCCESS                  	Success                                                    */
/*               E_DRVPDMA_ERR_PORT_INVALID     Invalid port number                                        */
/* Description:                                                                                            */
/*               The function is used to set APB transfer width for channelx                               */
/*---------------------------------------------------------------------------------------------------------*/
int32_t	DrvPDMA_SetAPBTransferWidth(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_TRANSFER_WIDTH eTransferWidth)
{
	PDMA_T * tPDMA;
	    
    if (eChannel > MAX_CHANNEL_NUM  )	    						   /* Check Channel is valid */
        return E_DRVPDMA_ERR_PORT_INVALID;    
    
	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 
																			
	tPDMA->CSR.APB_TWS = eTransferWidth;	        						/* Set PDMA Transfer Length */

    return E_SUCCESS;     													
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetAPBTransferWidth                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/* Returns:                                                                                                */
/*               0: One word (32 bits) is transferred for every PDMA operation.                            */
/*               1: One byte (8 bits) is transferred for every PDMA operation.                             */
/*               2: One half-word (16 bits) is transferred for every PDMA operation.                       */
/*               E_DRVPDMA_ERR_PORT_INVALID: invalid port number                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to get peripheral transfer width from channelx                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_GetAPBTransferWidth(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
 	PDMA_T * tPDMA;
	   																		/* Check Channel is valid */
    if (eChannel > MAX_CHANNEL_NUM  )	    						   /* Check Channel is valid */
        return E_DRVPDMA_ERR_PORT_INVALID;    

	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 

	return (int32_t)(tPDMA->CSR.APB_TWS);            					    /* Get Transfer Length */   
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_SetCHForAPBDevice                                                                 */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/*               eDevice	   		-[in]   APB device   					          	         		   */
/*                                          It includes eDRVPDMA_SPI0~3,eDRVPDMA_UART0~1,                  */
/*                                                eDRVPDMA_ADC ,eDRVPDMA_I2S                               */
/*               eRWAPB             -[in]   Access Direction                                               */
/*                                          It includes eDRVPDMA_WRITE_APB/eDRVPDMA_READ_APB               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS            			Success                 								   */
/*               E_DRVPDMA_ERR_PORT_INVALID     Invalid port number        								   */
/*               E_DRVPDMA_FALSE_INPUT          Invalid APB device                                         */
/* Description:                                                                                            */
/*               The function is used to select PDMA channel for APB device                                */
/*---------------------------------------------------------------------------------------------------------*/
int32_t	DrvPDMA_SetCHForAPBDevice(
    E_DRVPDMA_CHANNEL_INDEX eChannel, 
    E_DRVPDMA_APB_DEVICE 	eDevice,
    E_DRVPDMA_APB_RW 		eRWAPB    
)
{  
    if (eChannel > MAX_CHANNEL_NUM  )	    						   /* Check Channel is valid */
        return E_DRVPDMA_ERR_PORT_INVALID;    
   
	switch(eDevice)
	{
		case eDRVPDMA_SPI0:												    /* Set SPI0 PDMA Channel */
			if(eRWAPB)	  
				PDMA_GCR->PDSSR0.SPI0_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR0.SPI0_RXSEL	=	eChannel;
	   		break;

	   	case eDRVPDMA_SPI1:													/* Set SPI1 PDMA Channel */
			if(eRWAPB)	  
				PDMA_GCR->PDSSR0.SPI1_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR0.SPI1_RXSEL	=	eChannel;
	   		break;
																			
		case eDRVPDMA_SPI2:													/* Set SPI2 PDMA Channel */
			if(eRWAPB)	
				PDMA_GCR->PDSSR0.SPI2_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR0.SPI2_RXSEL	=	eChannel;
	   		break;
																		   	
		case eDRVPDMA_SPI3:													/* Set SPI3 PDMA Channel */
			if(eRWAPB)	
				PDMA_GCR->PDSSR0.SPI3_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR0.SPI3_RXSEL	=	eChannel;
	   		break;

		case eDRVPDMA_UART0:											   	/* Set UART0 PDMA Channel */
			if(eRWAPB)	
				PDMA_GCR->PDSSR1.UART0_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR1.UART0_RXSEL	=	eChannel;
	   		break;
		case eDRVPDMA_UART1:												/* Set UART1 PDMA Channel */
			if(eRWAPB)	
				PDMA_GCR->PDSSR1.UART1_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR1.UART1_RXSEL	=	eChannel;
	   		break;
		case eDRVPDMA_ADC:													/* Set ADC PDMA Channel */
			if(eRWAPB)	
				return 	E_DRVPDMA_FALSE_INPUT;
			else
				PDMA_GCR->PDSSR1.ADC_RXSEL	=	eChannel;
	   		break;
		case eDRVPDMA_I2S:													/* Set I2S PDMA Channel */
			if(eRWAPB)	
				PDMA_GCR->PDSSR2.I2S_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR2.I2S_RXSEL	=	eChannel;
	   		break;
		default:
			return 	E_DRVPDMA_FALSE_INPUT;
	}
 
    return E_SUCCESS;  
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetCHForAPBDevice	                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/*               eDevice	   		-[in]   CH for APB device 			          	         			   */
/*                                          It includes eDRVPDMA_SPI0~3,eDRVPDMA_UART0~1,                  */
/*                                                eDRVPDMA_ADC,eDRVPDMA_I2S                                */
/* Returns:                                                                                                */
/*               0~8 : Channel 0~8                                                                         */
/*               E_DRVPDMA_FALSE_INPUT:   Wrong parameter                                                  */
/*               Others: Reserved                                                                          */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to get PDMA channel for specified APB device                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_GetCHForAPBDevice(E_DRVPDMA_APB_DEVICE eDevice,E_DRVPDMA_APB_RW eRWAPB)
{
  
	switch(eDevice)
	{
		case eDRVPDMA_SPI0:
			if(eRWAPB)	  
				return PDMA_GCR->PDSSR0.SPI0_TXSEL;						 	/* Get SPI0 PDMA Channel */
			else
				return PDMA_GCR->PDSSR0.SPI0_RXSEL;
	   		
	   	case eDRVPDMA_SPI1:												 	/* Get SPI1 PDMA Channel */
			if(eRWAPB)	  
				return PDMA_GCR->PDSSR0.SPI1_TXSEL;
			else
				return PDMA_GCR->PDSSR0.SPI1_RXSEL;
	   		
		case eDRVPDMA_SPI2:												 	/* Get SPI2 PDMA Channel */
			if(eRWAPB)	
				return PDMA_GCR->PDSSR0.SPI2_TXSEL;
			else
				return PDMA_GCR->PDSSR0.SPI2_RXSEL;
	   																	 	
		case eDRVPDMA_SPI3:													/* Get SPI3 PDMA Channel */
			if(eRWAPB)	
				return PDMA_GCR->PDSSR0.SPI3_TXSEL;
			else
				return PDMA_GCR->PDSSR0.SPI3_RXSEL;
	   		
		case eDRVPDMA_UART0:											 	/* Get UART0 PDMA Channel */
			if(eRWAPB)	
				return PDMA_GCR->PDSSR1.UART0_TXSEL;
			else
				return PDMA_GCR->PDSSR1.UART0_RXSEL;
	   		
		case eDRVPDMA_UART1:											 	/* Get UART1 PDMA Channel */
			if(eRWAPB)	
				return PDMA_GCR->PDSSR1.UART1_TXSEL;
			else
				return PDMA_GCR->PDSSR1.UART1_RXSEL;
	   		
		case eDRVPDMA_ADC:													/* Get ADC PDMA Channel */
			if(eRWAPB)	
				return E_DRVPDMA_FALSE_INPUT;
			else
				return PDMA_GCR->PDSSR1.ADC_RXSEL;

		case eDRVPDMA_I2S:													/* Set I2S PDMA Channel */
			if(eRWAPB)	
				return PDMA_GCR->PDSSR2.I2S_TXSEL;
			else								 
				return PDMA_GCR->PDSSR2.I2S_RXSEL;
	   		
		default:
			return 	E_DRVPDMA_FALSE_INPUT;
	}
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_CHSoftwareReset                                                                   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/* Returns:                                                                                                */
/*               E_SUCCESS               		Success                                                    */
/*               E_DRVPDMA_ERR_PORT_INVALID     Invalid port number                                        */
/* Description:                                                                                            */
/*               The function is used to do software reset Channelx                                        */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_CHSoftwareReset(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
	PDMA_T * tPDMA;
	    
    if (eChannel > MAX_CHANNEL_NUM  )	    						   /* Check Channel is valid */
        return E_DRVPDMA_ERR_PORT_INVALID;    

	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 
 
   	tPDMA->CSR.SW_RST = 1;											   /* Set S/W Reset Bit */
 	
    return E_SUCCESS;     
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_CHEnablelTransfer                                                                 */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/* Returns:                                                                                                */
/*               E_SUCCESS               		Success                                                    */
/*               E_DRVPDMA_ERR_PORT_INVALID     Invalid port number                                        */
/* Description:                                                                                            */
/*               The function is used to enable PDMA channelx and enable channelx                          */
/*               data read or write transfer                                                               */
/*---------------------------------------------------------------------------------------------------------*/

int32_t	DrvPDMA_CHEnableTransfer(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;
	PDMA_T * tPDMA;
    																		/* Check PDMA Channel is Valid */
    if (eChannel > MAX_CHANNEL_NUM  )	    						        /* Check Channel is valid */
        return E_DRVPDMA_ERR_PORT_INVALID;    
            
   	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET);

    tPDMA->CSR.PDMACEN = 1;
    tPDMA->CSR.TRIG_EN = 1;												   /* PDMA Channl & Trigger Enable */

    return E_SUCCESS;      
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_EnableInt			                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               eIntFlag	   			-[in]     Intterrupt source: eDRVPDMA_TABORT/eDRVPDMA_BLKD         */
/* Returns:                                                                                                */
/*               E_SUCCESS               		Success                                                    */
/*               E_DRVPDMA_ERR_PORT_INVALID     Invalid port number                                        */
/* Description:                                                                                            */
/*               The function is used to enable Interrupt for channelx                                     */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_EnableInt(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_INT_ENABLE eIntSource)
{
    PDMA_T * tPDMA;

    if (eChannel > MAX_CHANNEL_NUM  )	    						   /* Check Channel is valid */
        return E_DRVPDMA_ERR_PORT_INVALID;    
			
	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 
	
	if((eIntSource&eDRVPDMA_TABORT)!=0)
		tPDMA->IER.TABORT_IE = 1;

	if((eIntSource&eDRVPDMA_BLKD)!=0)
		tPDMA->IER.BLKD_IE = 1;
	    
    /* Enable Specified Interrupt */
	NVIC_EnableIRQ(PDMA_IRQn);  
   
    return E_SUCCESS;      
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_DisableInt  		                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               eIntFlag	   			-[in]     Intterrupt source: eDRVPDMA_TABORT/eDRVPDMA_BLKD         */
/* Returns:                                                                                                */
/*               E_SUCCESS               		Success                                                    */
/*               E_DRVPDMA_ERR_PORT_INVALID     Invalid port number                                        */
/* Description:                                                                                            */
/*               The function is used to disable Interrupt for channelx                                    */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvPDMA_DisableInt(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_INT_ENABLE eIntSource)
{
    PDMA_T * tPDMA;
	
    if (eChannel > MAX_CHANNEL_NUM  )	    						   /* Check Channel is valid */
        return E_DRVPDMA_ERR_PORT_INVALID;    

	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 
	
	if((eIntSource&eDRVPDMA_TABORT)!=0)					 					/* Disable Specified Interrupt */
		tPDMA->IER.TABORT_IE = 0;

	if((eIntSource&eDRVPDMA_BLKD)!=0)
		tPDMA->IER.BLKD_IE = 0;

	return E_SUCCESS; 
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_IsIntEnabled   	                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               eIntFlag	   			-[in]     Intterrupt source: eDRVPDMA_TABORT/eDRVPDMA_BLKD         */
/* Returns:                                                                                                */
/*               TRUE:     The Specified Source Interrupt is enable                                        */
/*               FASLE:    The Specified Source Interrupt is disable                                       */
/* Description:                                                                                            */
/*               The function is used to check if the specified interrupt source is enabled in Channelx    */
/*---------------------------------------------------------------------------------------------------------*/
int32_t	DrvPDMA_IsIntEnabled(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_INT_ENABLE eIntSource )
{
	PDMA_T * tPDMA;

	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 
	
	if(eIntSource==eDRVPDMA_TABORT)
	{
	  if(tPDMA->IER.TABORT_IE==1)
        return (tPDMA->IER.TABORT_IE)?TRUE:FALSE;
	}
	if(eIntSource==eDRVPDMA_BLKD)		   
	{
	  if(tPDMA->IER.BLKD_IE==1)
	    return (tPDMA->IER.BLKD_IE)?TRUE:FALSE;
	}
	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_ClearIntFlag		                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               eIntFlag	   			-[in]     Intterrupt source: eDRVPDMA_TABORT/eDRVPDMA_BLKD         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to clear interrupt status for channelx                               */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPDMA_ClearIntFlag(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_INT_FLAG eIntFlag)
{
    volatile uint32_t u32SFR;
	
	u32SFR = (uint32_t)&PDMA0->ISR + eChannel * CHANNEL_OFFSET; 
    
    outpw(u32SFR, eIntFlag);
}
						  

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_PollInt    		                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               eIntFlag	   			-[in]     Intterrupt source: eDRVPDMA_TABORT/eDRVPDMA_BLKD         */
/* Returns:                                                                                                */
/*               TRUE:     The Source Interrupt Flag is set                                                */
/*               FASLE:    The Source Interrupt Flag is clear                                              */
/* Description:                                                                                            */
/*               The function is used to polling channel interrupt status                                  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t	DrvPDMA_PollInt(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_INT_FLAG eIntFlag)
{
    volatile uint32_t u32SFR;
    
    u32SFR = (uint32_t) & PDMA0->ISR + eChannel * CHANNEL_OFFSET; 
    return inpw(u32SFR) & eIntFlag;
}
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetInternalBufPointer				                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/* Returns:                                                                                                */
/*               Internal Buffer Pointer                                                                   */
/* Description:                                                                                            */
/*               The function is used to  Get Internal Buffer Pointer                                      */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMA_GetInternalBufPointer(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
 	PDMA_T * tPDMA;
    
    if (eChannel > MAX_CHANNEL_NUM  )	    						   /* Check Channel is valid */
        return E_DRVPDMA_ERR_PORT_INVALID;    
   
	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 

	return (uint32_t)(tPDMA->u32POINT);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetSharedBufData                                                                  */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/* Returns:                                                                                                */
/*               Shard Buffer Content register                                                             */
/* Description:                                                                                            */
/*               The function is used to get shared buffer content from Channelx                           */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMA_GetSharedBufData(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;
    
    u32SFR = (uint32_t)&PDMA0->SBUF + eChannel * CHANNEL_OFFSET;  
    
    return inpw(u32SFR);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     PDMA_IRQHandler     		                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 None 												                                       */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to  PDMA ISR                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void PDMA_IRQHandler(void)
{
    volatile uint32_t u32IntStatus;
	volatile uint32_t u32SFR;
    volatile int32_t  i32Channel= 0 ;
	volatile uint32_t u32ISR;
	PDMA_T * tPDMA;
		
	u32SFR = inpw(&PDMA_GCR->GCRISR);

	for(i32Channel=0;i32Channel<MAX_CHANNEL_NUM;i32Channel++)  
	{
		tPDMA = (PDMA_T *)((uint32_t)PDMA0 + i32Channel * CHANNEL_OFFSET); 

		if(u32SFR & (1<<i32Channel))  										 		/* Check INT Channel */	
		{
			if(tPDMA->ISR.TABORT_IF==1)	 											/* Check Target Abort INT and clear */
			{
			  tPDMA->ISR.TABORT_IF=1;
			   if (g_pfnPDMACallback[i32Channel][0] != 0)    		
		    		(*g_pfnPDMACallback[i32Channel][0])(0);							/* Target Abort callback funtion */
			}
			else if(tPDMA->ISR.BLKD_IF==1)		
			{																		/* Transfer Done INT and clear */
			  tPDMA->ISR.BLKD_IF=1;
			   if (g_pfnPDMACallback[i32Channel][1] != 0)    		
		    		(*g_pfnPDMACallback[i32Channel][1])(0);							
			}
		}
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_InstallCallBack                                                                   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               E_DRVPDMA_INT_ENABLE	-[in]     Intterrupt source :eDRVPDMA_TABORT/eDRVPDMA_BLKD         */
/*               pfncallback            -[in]     The callback function pointer                            */
/* Returns:                                                                                                */
/*               E_SUCCESS                Success                                                          */
/*               E_DRVPDMA_FALSE_INPUT    invalid Argument                                                 */
/* Description:                                                                                            */
/*               The function is used to install call back function for Channelx & Interrupt source        */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_InstallCallBack(
	E_DRVPDMA_CHANNEL_INDEX eChannel, 
    E_DRVPDMA_INT_ENABLE eIntSource,
	PFN_DRVPDMA_CALLBACK pfncallback   
)
{
  
    if (eChannel > MAX_CHANNEL_NUM  )	    						   /* Check Channel is valid */
        return E_DRVPDMA_ERR_PORT_INVALID;    

	g_pfnPDMACallback[eChannel][eIntSource>>1] = pfncallback;	       /* Install Callback funtion */
	
    return E_SUCCESS;      
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetCurrentSourceAddr                                                              */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/* Returns:                                                                                                */
/*               Current Source Address from channelx                                                      */
/* Description:                                                                                            */
/*               The function is used to get current source address from channelx                          */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMA_GetCurrentSourceAddr(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;
    
    u32SFR = (uint32_t)&PDMA0->CSAR + eChannel * CHANNEL_OFFSET;    

    return inpw(u32SFR);      
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetCurrentDestAddr                                                                */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/* Returns:                                                                                                */
/*               Current destination address form channelx                                                 */
/* Description:                                                                                            */
/*               The function is used to get current destination address from channelx                     */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMA_GetCurrentDestAddr(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;
    
	u32SFR = (uint32_t)&PDMA0->CDAR + eChannel * CHANNEL_OFFSET;
    
    return inpw(u32SFR);      
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetRemainTransferCount			    	                                           */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/* Returns:                                                                                                */
/*               Current remained byte count.                                                              */
/* Description:                                                                                            */
/*               The function is used to get current remained byte count of channelx PDMA                  */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMA_GetRemainTransferCount(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;
    
    u32SFR = (uint32_t)&PDMA0->CBCR + eChannel * CHANNEL_OFFSET;
    
	return inpw(u32SFR);   
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_SetSourceAddress				                                                   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/*               u32SourceAddr	   	-[in]   Source address                                                 */
/* Returns:                                                                                                */
/*               E_SUCCESS                  	Success                                                    */
/*               E_DRVPDMA_ERR_PORT_INVALID     Invalid port number                                        */
/* Description:                                                                                            */
/*               The function is used to set source address for channelx                                   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t	DrvPDMA_SetSourceAddress(E_DRVPDMA_CHANNEL_INDEX eChannel,uint32_t u32SourceAddr)
{
	PDMA_T * tPDMA;
	    
    if (eChannel > MAX_CHANNEL_NUM  )	    						   /* Check Channel is valid */
        return E_DRVPDMA_ERR_PORT_INVALID;    
    
	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 
																			
	tPDMA->SAR = u32SourceAddr;	        						/* Set PDMA Channelx Source Address */

    return E_SUCCESS;     													
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetSourceAddress                                                                  */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/* Returns:                                                                                                */
/*               Source address for channelx.                                                              */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to get source address for channelx                                   */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMA_GetSourceAddress(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
 	PDMA_T * tPDMA;
	   																/* Check Channel is valid */
    if (eChannel > MAX_CHANNEL_NUM  )	    						
        return E_DRVPDMA_ERR_PORT_INVALID;    

	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 

	return (uint32_t)(tPDMA->SAR);            					    /* Get PDMA Channelx Source Address  */   
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_SetDestAddress				                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/*               u32DestAddr	   	-[in]   Destination address                                            */
/* Returns:                                                                                                */
/*               E_SUCCESS                  	Success                                                    */
/*               E_DRVPDMA_ERR_PORT_INVALID     Invalid port number                                        */
/* Description:                                                                                            */
/*               The function is used to set destination address for channelx                              */
/*---------------------------------------------------------------------------------------------------------*/
int32_t	DrvPDMA_SetDestAddress(E_DRVPDMA_CHANNEL_INDEX eChannel,uint32_t u32DestAddr)
{
	PDMA_T * tPDMA;
	    
    if (eChannel > MAX_CHANNEL_NUM  )	    						   /* Check Channel is valid */
        return E_DRVPDMA_ERR_PORT_INVALID;    
    
	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 
																			
	tPDMA->DAR = u32DestAddr;	        						/* Set PDMA Channelx Destination Address */

    return E_SUCCESS;     													
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetDestAddress                                                                    */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/* Returns:                                                                                                */
/*               Destination address for channelx.                                                         */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to get destination address for channelx                              */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMA_GetDestAddress(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
 	PDMA_T * tPDMA;
	   																/* Check Channel is valid */
    if (eChannel > MAX_CHANNEL_NUM  )	    						
        return E_DRVPDMA_ERR_PORT_INVALID;    

	tPDMA = (PDMA_T *)((uint32_t)PDMA0 + eChannel * CHANNEL_OFFSET); 

	return (uint32_t)(tPDMA->DAR);            					    /* Get PDMA Channelx Destination Address  */   
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetVersion                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             None                                                                                 	   */
/* Returns:                                                                                                */
/*               Version Number                                                                            */
/* Description:                                                                                            */
/*               The function is used to get DrvPDMA Version Number                                        */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_GetVersion(void)
{
	return DRVPDMA_VERSION_NUM;
}


