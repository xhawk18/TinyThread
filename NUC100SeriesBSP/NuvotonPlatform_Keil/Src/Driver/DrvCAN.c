/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of headers                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#include "DrvCAN.h"
#include "DrvSYS.h"
#include "NUC1xx.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static CAN_CALLBACK_T CANHandler = {0};

#define CAN_DEBUG(...)
/*---------------------------------------------------------------------------------------------------------*/
/* CAN0 ISR                                                                                         	   */
/*---------------------------------------------------------------------------------------------------------*/
void CAN0_IRQHandler(void)
{
    uint32_t status;
	status  = inpw(&CAN0->INTR);
	 	
	if(CAN0->INTR.BEI ==1)								/* Bus Error Interrupt */
	{
		CAN0->INTR.BEI =1 ; 							/* Clear Bus Error Interrupt Flag */			

    	if (CANHandler.BusErrorCallBackFn)				/* Bus Error Interrupt Callback Function */ 
		{
			CANHandler.BusErrorCallBackFn(status); 
		}

		CAN_DEBUG("Bus Error Interrupt\n");
 	}
				   
	if(CAN0->INTR.ALI ==1)		/* Arbitration Lost Interrupt */
	{
		CAN0->INTR.ALI =1 ; 						   	/* Clear Arbitration Lost Interrupt Flag */

		if (CANHandler.ArbitLostCallBackFn)				/* Arbitration Lost Interrupt Callback Function */
		{
			CANHandler.ArbitLostCallBackFn(status); 
		}
		CAN_DEBUG("Arbitration Lost Interrupt\n");

  	}			   
	
	if(CAN0->INTR.WUI ==1)		/* Wake-Up Interrupt */
	{												  	
		CAN0->CMD.WAKEUP_EN = 0; 						/* When the system being wakeup, this bit must be
														   clear before the user clears the interrupt flag.*/
		CAN0->INTR.WUI =1 ; 						  	/* Clear Wake-Up Interrupt Flag */

        if (CANHandler.WakeUpCallBackFn)				/* Wake-Up Interrupt Callback Function */
		{
			CANHandler.WakeUpCallBackFn(status); 
		}
		CAN_DEBUG("Wake up Interrupt\n");

  	}			   
	if(CAN0->INTR.TI ==1)	 	/* Transmit Interrupt */
	{
		CAN0->INTR.TI =1 ; 							   	/* Clear Transmit Interrupt Flag */

		if (CANHandler.TransmitCallBackFn)				/* Transmit Interrupt Callback Function */
		{
			CANHandler.TransmitCallBackFn(status); 
		}
		CAN_DEBUG("Transmit Interrupt\n");

  	}			   
	if(CAN0->INTR.RI ==1)		/* Receive Interrupt */
	{
		CAN0->INTR.RI =1 ; 							  	/* Clear Receive Interrupt Flag */

		if (CANHandler.ReceiveCallBackFn)				/* Receive Interrupt Callback Function */
		{
			CANHandler.ReceiveCallBackFn(status); 
		}
		CAN_DEBUG("Receive Interrupt\n");
  	}			   


}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_Init		                                                                   	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to initialize CAN. It consists of reset IP and enable CAN clock.     */
/*---------------------------------------------------------------------------------------------------------*/
void DrvCAN_Init(void)
{
	SYS->IPRSTC2.CAN0_RST =1;  /* Reset CAN0 */
	SYS->IPRSTC2.CAN0_RST =0;

	SYSCLK->APBCLK.CAN0_EN 	= 1;

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_Open		                                                                   	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               i32CANKiloBitRate  	-[in]    Desird CAN BUS BitRate                                    */
/*                                                                                                         */
/*               i16SamplePointPos  	-[in]    Desird Sample Point Persentage                            */
/*                                               ex. Set 875 means 87.5% position                          */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_DRVCAN_ERR_BITRATE    Can not compute the suitable value                                */
/*               E_SUCCESS               Set suitable value complete                                       */
/* Description:                                                                                            */
/*               The function is used to set CAN bit rate according current system setting,                */
/*               It is used to change CAN divider and using current system clock to compute                */
/*               the most near setting value                                                               */
/*---------------------------------------------------------------------------------------------------------*/

/*
						Fcclk
   CANBitRate = ------------------------------------
			   (Tseg1+1)+(Tseg2+1)+1) *  CAN_DIV

	Where 
		Fclk: CAN clock source value
		CAN_DIV : CAN clock divider

	It can change Tseg1 and Tseg2 to adjust Sample Point postion
*/


int32_t DrvCAN_Open(int32_t i32CANKiloBitRate,int16_t i16SamplePointPos)
{
 	int32_t i32FinClk,i32Div,i32Sum,i32TSEG1,i32TSEG2;
	
	CAN0->CMD.CAN_EN 		= 1;
	
	if(i16SamplePointPos >1000)
		return E_DRVCAN_ERR_BITRATE;

	/* According CLK Source to compute */
	i32FinClk = DrvCAN_GetClockFreq()*1000;

	i32Div = 1;

STR:		
	/* Get the sum of TSEG1 and TSEG2 */
	i32Sum = i32FinClk / (i32CANKiloBitRate*i32Div) -3;
	
	if(i32Sum < 0)
		return E_DRVCAN_ERR_BITRATE;
	/* The setting is not exist*/
	i32TSEG1 = (i32Sum + 1)* i16SamplePointPos /1000;

	if((i32Sum > 48)||(i32TSEG1>32))
	{
		i32Div++;
		if( i32Div > 1024)			
			return E_DRVCAN_ERR_BITRATE;
		goto STR;
	}

	i32TSEG2 = i32Sum - i32TSEG1;
		
	CAN0->BTIMR.TSEG1 = i32TSEG1;

	CAN0->BTIMR.TSEG2 = i32TSEG2;

	DrvSYS_SetClockDivider(E_SYS_CAN_DIV,i32Div-1);

	return E_SUCCESS;
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_Close                                                                   	       */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               E_SUCCESS   				Success                                                        */
/* Description:                                                                                            */
/*               To disable the CAN APB Clock and BUS Enable function                                      */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvCAN_Close()
{
	/*Disable APB CLK & CAN BUS*/
	SYSCLK->APBCLK.CAN0_EN =0;
	
	CAN0->CMD.CAN_EN =0;

	return E_SUCCESS;

}

				
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_Write                                                                   	       */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               Msg     	-[in]   	    Data Sturcture of CAN                                          */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS   				Success                                                        */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is set CAN information and send to CAN BUS                                   */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvCAN_Write(STR_CAN_T *Msg)
{
	
	/* Configure Setting */		
	CAN0->TXFINFO.TXFF 		= Msg->u8cFormat;

	/* Standard and extended format */
	if(Msg->u8cFormat ==1 )  
		CAN0->TXIDR.TXID 	= Msg->id;							
	else
		CAN0->TXIDR.TXID 	= (Msg->id)<<18;							
		
	CAN0->TXFINFO.TXDLC		= Msg->u8cLen;
	
	CAN0->TXDATA[0] 		= Msg->u32cData[0];
		
	CAN0->TXDATA[1] 		= Msg->u32cData[1];
		
	CAN0->TXFINFO.TXRTR 	= Msg->u8cType;

	/* Trigger and Send */
	CAN0->CMD.TXREQ = 1; 
	
	return E_SUCCESS;
}		  

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_Read                                                                   	       	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               STR_CAN_T  -[out]           The data stucture of CAN                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to get CAN RX information and store in structure                     */
/*---------------------------------------------------------------------------------------------------------*/

STR_CAN_T DrvCAN_Read(void)
{
	STR_CAN_T recCAN;

	recCAN.u8cLen 		= CAN0->RXFINFO.RXDLC; 
	recCAN.u8cFormat	= CAN0->RXFINFO.RXIDE; 
	if(recCAN.u8cFormat==1)   
		recCAN.id 		= CAN0->RXIDR.RXID;
	else
		recCAN.id 		= (CAN0->RXIDR.RXID)>>18;
	
	recCAN.u8cType		= CAN0->RXFINFO.RXRTR; 

	recCAN.u32cData[0] 	= CAN0->RX_DATA[0] ; 
	recCAN.u32cData[1] 	= CAN0->RX_DATA[1] ; 

 	return recCAN;
}		  

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_EnableInt                                                                   	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               u32InterruptSrc -[in]  Interrupt Source:It could be DRVCAN_INT_BEI/DRVCAN_INT_ALI/        */
/*                                                       DRVCAN_INT_WUI/DRVCAN_INT_TI/DRVCAN_INT_RI        */
/*                                                 DRVCAN_INT_BEI:Bus Error Interrupt                      */
/*                                                 DRVCAN_INT_ALI:Arbitration Lost Interrupt               */
/*                                                 DRVCAN_INT_WUI:Wake-Up Interrupt                        */
/*                                                 DRVCAN_INT_TI :Transmit Interrupt                       */
/*                                                 DRVCAN_INT_RI :Receive Interrupt                        */
/*                                                                                                         */
/*               pfncallback                  Callback function pointer                                    */
/*                                                                                                         */
/* Returns:                                                                                                */
/*                                                                                                         */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to enable specified CAN interrupt                                    */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_EnableInt(uint32_t u32InterruptSrc)
{
	
	CAN0->INTEN.BEIE		=(u32InterruptSrc & DRVCAN_INT_BEI)?1:0 ;
	CAN0->INTEN.ALIE		=(u32InterruptSrc & DRVCAN_INT_ALI)?1:0;
	CAN0->INTEN.WUIE		=(u32InterruptSrc & DRVCAN_INT_WUI)?1:0;
	CAN0->INTEN.TIE			=(u32InterruptSrc & DRVCAN_INT_TI)?1:0;
	CAN0->INTEN.RIE			=(u32InterruptSrc & DRVCAN_INT_RI)?1:0;

	if(u32InterruptSrc&DRVCAN_INT_WUI)				/* Wakeup Function Enable when wakeup INT Enable */
	 	CAN0->CMD.WAKEUP_EN = 1; 

	NVIC_EnableIRQ(CAN0_IRQn); 
	return TRUE;

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_InstallCallback                                                                	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               E_CAN_CALLBACK_TYPE -[in]   Callback type:It could be INT_BEI/INT_ALI/INT_WUI/INT_TI/INT_RI */
/*                                                 TYPE_BEI:Bus Error                                      */
/*                                                 TYPE_ALI:Arbitration Lost                               */
/*                                                 TYPE_WUI:Wake-Up                                        */
/*                                                 TYPE_TI :Transmit                                       */
/*                                                 TYPE_RI :Receive                                        */
/*                                                                                                         */
/*               pfncallback                  Callback function pointer                                    */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Side effects:                                                                                           */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to install the call back function                                    */
/*---------------------------------------------------------------------------------------------------------*/

void DrvCAN_InstallCallback(E_DRVCAN_CALLBACK_TYPE Type, PFN_DRVCAN_CALLBACK callbackfn)
{
	switch(Type)
   	{
       	case TYPE_BEI:
       	{
			CANHandler.BusErrorCallBackFn = callbackfn;
       		break;
		}
       	case TYPE_ALI:
       	{
			CANHandler.ArbitLostCallBackFn = callbackfn;
       		break;
		}
       	case TYPE_WUI:
       	{
			CANHandler.WakeUpCallBackFn = callbackfn;
       		break;
		}
       	case TYPE_TI:
       	{
			CANHandler.TransmitCallBackFn = callbackfn;
       		break;
		}
       	case TYPE_RI:
       	{
			CANHandler.ReceiveCallBackFn = callbackfn;
       		break;
		}
	}
	return ;
	
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_UninstallCallback                                                              	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               E_CAN_CALLBACK_TYPE -[in]   Callback type:It could be DRVCAN_INT_BEI/DRVCAN_INT_ALI/      */
/*                                                                     DRVCAN_INT_WUI/DRVCAN_INT_TI/       */
/*                                                                     DRVCAN_INT_RI                       */
/*                                                                                                         */
/*                                                 TYPE_BEI:Bus Error                                      */
/*                                                 TYPE_ALI:Arbitration Lost                               */
/*                                                 TYPE_WUI:Wake-Up                                        */
/*                                                 TYPE_TI :Transmit                                       */
/*                                                 TYPE_RI :Receive                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Side effects:                                                                                           */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to  uninstall the call back function                                 */
/*---------------------------------------------------------------------------------------------------------*/
void DrvCAN_UninstallCallback(E_DRVCAN_CALLBACK_TYPE Type)
{
	switch(Type)
   	{
       	case TYPE_BEI:
       	{
			CANHandler.BusErrorCallBackFn = NULL;
       		break;
		}
       	case TYPE_ALI:
       	{
			CANHandler.ArbitLostCallBackFn = NULL;
       		break;
		}
       	case TYPE_WUI:
       	{
			CANHandler.WakeUpCallBackFn = NULL;
       		break;
		}
       	case TYPE_TI:
       	{
			CANHandler.TransmitCallBackFn = NULL;
       		break;
		}
       	case TYPE_RI:
       	{
			CANHandler.ReceiveCallBackFn = NULL;
       		break;
		}
	}
	return ;

}
 
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvUART_DisableInt                                                                   	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               u32InterruptSrc[in]   Interrupt Source : DRVCAN_INT_BEI/DRVCAN_INT_ALI/DRVCAN_INT_WUI     */
/*                                                        /DRVCAN_INT_TI/DRVCAN_INT_RI                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS                    Success                                                      */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to disable CAN Interrupt and uninstall the call back function        */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_DisableInt(uint32_t u32InterruptSrc)
{
 	if(u32InterruptSrc & DRVCAN_INT_BEI)
		CANHandler.BusErrorCallBackFn 	= NULL;
	
	if(u32InterruptSrc & DRVCAN_INT_ALI)
		CANHandler.ArbitLostCallBackFn 	= NULL;

	if(u32InterruptSrc & DRVCAN_INT_TI)
		CANHandler.TransmitCallBackFn	= NULL;
	
	if(u32InterruptSrc & DRVCAN_INT_WUI)
		CANHandler.WakeUpCallBackFn		= NULL;
	
	if(u32InterruptSrc & DRVCAN_INT_RI)
		CANHandler.ReceiveCallBackFn 	= NULL;


	outpw(&CAN0->INTEN,inpw(&CAN0->INTEN)&~u32InterruptSrc);
    NVIC_DisableIRQ(CAN0_IRQn);   

	return E_SUCCESS;

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_GetErrorStatus                                                                	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               DRVCAN_ERRFLAG   -[in]   Bus Error Flag - It includes of           			           */
/*                                            DRVCAN_ERRSTUFF :  Stuff Error                               */
/*                                            DRVCAN_ERRFORM  :  Form Error                                */
/*                                            DRVCAN_ERRCRC   :  CRC Error                                 */
/*                                            DRVCAN_ERRACK   :  Acknowledge Error                         */
/*                                            DRVCAN_ERRBIT   :  Bit Error                                 */
/* Returns:                                                                                                */
/*               TRUE  : The Error Flag is set                                                             */
/*               E_SUCCESS : The Error Flag is clear                                                       */
/*                                                                                                         */
/* Side effects:                                                                                           */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to Get specified CAN Error Status occurs or not    				   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_GetErrorStatus(E_DRVCAN_ERRFLAG u32ErrorFlag)
{
	
	switch(u32ErrorFlag)
	{
		case DRVCAN_ERRSTUFF:
			if(CAN0->ERRCR.STUFF_ERR)	
				return TRUE;
			break;
	
		case DRVCAN_ERRFORM:
			if(CAN0->ERRCR.FORM_ERR)	
				return TRUE;
			break;

		case DRVCAN_ERRCRC:
			if(CAN0->ERRCR.CRC_ERR)	
				return TRUE;
			break;
			  
		case DRVCAN_ERRACK:
			if(CAN0->ERRCR.ACK_ERR)	
				return TRUE;
			break;
			
		case DRVCAN_ERRBIT:
			if(CAN0->ERRCR.BIT_ERR)	
				return TRUE;
			break;

		default:
			break;
	}
	
	return E_SUCCESS;
}




/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_SetAcceptanceFilter                                                            	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               id_Filter  -[in]            The data to write to the specified ID Filter                  */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS                   SUCCSSS                                                       */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to Set a Accept ID filter                                            */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_SetAcceptanceFilter(int32_t id_Filter )
{
	CAN0->ACR.ACRID =id_Filter;	

	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_SetMaskFilter                                                                 	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               id_Filter  -[in]            The data to write to the specified Mask Filter                */
/* Returns:                                                                                                */
/*               E_SUCCESS                   SUCCSSS                                                       */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to set mask ID filter                                                */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_SetMaskFilter(int32_t id_Filter )
{
	CAN0->AMR.AMRID =id_Filter;	

	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_SetBusTiming                                                                   	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               i8SynJumpWidth   -[in]   Synchronization Jump Width :It could be 0~3                      */
/*               i16TimeSeg1      -[in]   Time Segment 1:It could be 0~31                                  */
/*               i8TimeSeg2       -[in]   Time Segment 2 :It could be 0~15                                 */
/*               SampPtNo         -[in]   Sampling Point Number :It could be 0~1                           */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS                                                                                 */
/* Side effects:                                                                                           */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to Set CAN Bus Timming Parameter according user's thinking           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_SetBusTiming(
	int8_t i8SynJumpWidth,
	int16_t i16TimeSeg1,
	int8_t i8TimeSeg2,
	int8_t i8SampPtNo
)
{
	if((i8SynJumpWidth>3) || (i16TimeSeg1 >31 || (i8TimeSeg2 > 15) || (i8SampPtNo >1)))
	{
	   return E_DRVCAN_ERR_ARGUMENT;
	}

	CAN0->BTIMR.SJW 	= i8SynJumpWidth;
	CAN0->BTIMR.TSEG1 	= i16TimeSeg1;
	CAN0->BTIMR.TSEG2 	= i8TimeSeg2;
	CAN0->BTIMR.SAMP 	= i8SampPtNo;
	
	return E_SUCCESS;

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_GetClock                                                                 	       */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               Current CAN Clock  (KHz)                                                                  */
/* Description:                                                                                            */
/*               The function is used to Get Current CAN Clock Source Clock                                */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_GetClockFreq(void)
{
	uint32_t clk =0 ;
											/* Check UART Clock Source Setting */
	if(SYSCLK->CLKSEL1.CAN_S == 0)			
	{
		clk = DrvSYS_GetExtClockFreq();			/* Get External Clock From DrvSYS Setting */
	}
	else if(SYSCLK->CLKSEL1.CAN_S == 1)
	{
		clk = DrvSYS_GetPLLClockFreq();         /* According PLL Clock */
	}
	else
		clk = __IRC22M;						/* Clock 22Mhz  */

	return clk;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_GetTxErrorCount                                                              	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None																					   */
/* Returns:                                                                                                */
/*               Transmit Error Count                                                                      */
/* Description:                                                                                            */
/*               The function is used to get transmit error count value                                    */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_GetTxErrorCount(void)
{
	return CAN0->TECNTR.TECNT;
  
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_GetRxErrorCount                                                              	   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               Receive Error Count                                                                       */
/* Description:                                                                                            */
/*               The function is used to get receive error count value                                     */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_GetRxErrorCount(void)
{
	return CAN0->RECNTR.RECNT;

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_ReTransmission                                                            	       */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               bIsEnable     Enable/Disable Automatic re-transmission when a message transmission fail   */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to Enable or Disable automatic re-transmission                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvCAN_ReTransmission(int32_t bIsEnable)
{
	CAN0->CMD.ABRT = bIsEnable?1:0 ;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_GetVersion                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             None                                                                                 	   */
/* Returns:                                                                                                */
/*               Version Number                                                                            */
/* Description:                                                                                            */
/*               The function is used to get DrvCAN Version Number                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_GetVersion(void)
{
	return DRVCAN_VERSION_NUM;
	
}



