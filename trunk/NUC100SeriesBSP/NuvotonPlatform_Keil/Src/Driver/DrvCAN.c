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
                                                     

/* Private function prototypes -----------------------------------------------*/
static uint32_t GetFreeIF(void);

/* Private functions ---------------------------------------------------------*/
static STR_CAN_CALLBACK_T CANHandler = {0};

//#define DEBUG_PRINTF printf
#define DEBUG_PRINTF(...)
/*---------------------------------------------------------------------------------------------------------*/
/* CAN0 ISR                                                                                           	   */
/*---------------------------------------------------------------------------------------------------------*/
void CAN0_IRQHandler(void)
{
	uint8_t u8IIDRstatus;

    if(CAN->u32IIDR == 0x8000)     	  /* Check Status Interrupt Flag (Error status Int and Status change Int) */
	{
		/**************************/
		/* Status Change interrupt*/
		/**************************/
		
        if(CAN->STATUS.RXOK == 1)
        {
            CAN->STATUS.RXOK = 0;    /* Clear Rx Ok status*/
            
            DEBUG_PRINTF("RX OK INT\n") ;   		

            if (CANHandler.RxOkCallBackFn)
	    	{
		    	CANHandler.RxOkCallBackFn(0); 
		    }
        }

        if(CAN->STATUS.TXOK == 1)
		{
            CAN->STATUS.TXOK = 0;    /* Clear Tx Ok status*/
            
			DEBUG_PRINTF("TX OK INT\n") ; 

            if (CANHandler.TxOkCallBackFn)
	    	{
		    	CANHandler.TxOkCallBackFn(0); 
		    }
        }
		/**************************/
		/* Error Status interrupt */
		/**************************/
        if(CAN->STATUS.EWARN==1)
        {
            DEBUG_PRINTF("EWARN INT\n") ; 

            if (CANHandler.ErrorWarningCallBackFn)
	    	{
		    	CANHandler.ErrorWarningCallBackFn(0); 
		    }
        }

        if(CAN->STATUS.BOFF==1)
        {
            DEBUG_PRINTF("BOFF INT\n") ; 
        
            if (CANHandler.BusOffCallBackFn)
	    	{
		    	CANHandler.BusOffCallBackFn(0); 
		    }
        }
	}

	else if ((CAN->u32IIDR)!=0)
	{                       
        DEBUG_PRINTF("=> Interrupt Pointer = %d\n",CAN->u32IIDR -1);

		u8IIDRstatus = CAN->u32IIDR;
		
        if (CANHandler.MessageCallBackFn)
  	    {
		    CANHandler.MessageCallBackFn(u8IIDRstatus); 
		}    

		DrvCAN_ClrIntPnd((CAN->u32IIDR) -1 );	   /* Clear Interrupt Pending */

	}

    else if(CAN->u32WU_STATUS==1)
    {
        DEBUG_PRINTF("Wake up\n");

        CAN->u32WU_STATUS = 0;     				   /* Write '0' to clear */

        if (CANHandler.WakeupCallBackFn)
	    {
		    CANHandler.WakeupCallBackFn(0); 
		}    
    }

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    GetFreeIF           																	   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              None                                                                                       */
/* Returns:                                                                                                */
/*              0: IF0 is free  1: IF1 is free   2: No IF is free                                          */
/* Description:                                                                                            */
/*              Searchs the first free message interface, starting from 0.                                 */
/*---------------------------------------------------------------------------------------------------------*/

static uint32_t GetFreeIF(void)
{
    if (CAN->sMsgObj[0].CREQ.BUSY == 0)
        return 0;
    else if (CAN->sMsgObj[1].CREQ.BUSY == 0)
        return 1;
    else
   	    return 2;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_GetCANBitRate                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              None                                                                                       */
/* Returns:                                                                                                */
/*              Current Bit-Rate (kilo bit per second)                                                     */
/* Description:                                                                                            */
/*              Return current CAN bitrate according to the user bit-timing parameter settings             */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvCAN_GetCANBitRate(void)
{
    uint8_t u8Tseg1,u8Tseg2;    
    uint32_t u32Bpr;

    u8Tseg1 = CAN->BTIME.TSEG1;
    u8Tseg2 = CAN->BTIME.TSEG2;
    u32Bpr  = (CAN->BTIME.BRP) | (CAN->BRPE.BPRE <<6);
    return (DrvSYS_GetHCLKFreq()/(u32Bpr+1)/(u8Tseg1 + u8Tseg2 + 3))/1000;
}

                                                                                                            
/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_Init                                                                                */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              None                                                                                       */
/* Returns:                                                                                                */
/*              None                                                                                       */
/* Description:                                                                                            */
/*              The function is used to reset and Initializes CAN IP                                       */
/*---------------------------------------------------------------------------------------------------------*/

void DrvCAN_Init(void)
{
    /* Enable CAN0 Clock and Reset it */
    SYSCLK->APBCLK.CAN0_EN  =1;
	SYS->IPRSTC2.CAN0_RST   =1;  
	SYS->IPRSTC2.CAN0_RST   =0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_Close                                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              None                                                                                       */
/* Returns:                                                                                                */
/*              None                                                                                       */
/* Description:                                                                                            */
/*              Reset and clear all CAN control and disable CAN IP                                         */
/*---------------------------------------------------------------------------------------------------------*/

void DrvCAN_Close(void)
{
	/* Clear Install callback function pointer */
	CANHandler.MessageCallBackFn            = NULL; 
    CANHandler.RxOkCallBackFn               = NULL;    
    CANHandler.TxOkCallBackFn               = NULL;    
    CANHandler.ErrorWarningCallBackFn       = NULL;    
    CANHandler.BusOffCallBackFn             = NULL;    
    CANHandler.WakeupCallBackFn             = NULL;    
      
    /* Disable CAN0 Clock and Reset it */
 	SYS->IPRSTC2.CAN0_RST   =1;  
	SYS->IPRSTC2.CAN0_RST   =0;  
    SYSCLK->APBCLK.CAN0_EN  =0;

}
     

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_Open                                                                                */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              u32kbps: The target CAN kilo bit rate per second.                                          */
/*                       The range of u32kbps is 1~1000KHz                                                 */
/* Returns:                                                                                                */
/*              E_DRVCAN_ERR_BITRATE : Set target bit-rate fail                                            */
/*              E_SUCCESS            : Set target bit-rate successful                                      */
/* Description:                                                                                            */
/*              The function is used to set bus timing parameter according current clock and               */
/*              target bit-rate.                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvCAN_Open(uint32_t u32kbps)
{
    uint8_t u8Tseg1,u8Tseg2;    
    uint32_t u32Bpr;
    uint32_t u32value;

    DrvCAN_EnterInitMode();

    u8Tseg1 = 2;
    u8Tseg2 = 3;

    u32value = DrvSYS_GetHCLKFreq()/1000;
    if(u32kbps*8 < (u32value/2))
        u32Bpr  = DrvSYS_GetHCLKFreq()/(u32kbps*1000) / (u8Tseg1 + u8Tseg2 + 3) -1;
    else
    {
        u8Tseg1 = 2;
        u8Tseg2 = 1;
        u32Bpr  = DrvSYS_GetHCLKFreq()/(u32kbps*1000) / (u8Tseg1 + u8Tseg2 + 3) -1;
    }

    CAN->BTIME.TSEG1 = u8Tseg1;  
    CAN->BTIME.TSEG2 = u8Tseg2;  
    CAN->BTIME.BRP   = u32Bpr & 0x3F ;  
    CAN->u32BRPE     = (u32Bpr >> 6) & 0x0F;
    
    /* Check the result is OK */
    if ((u32kbps < (DrvCAN_GetCANBitRate()*95/100)) || (u32kbps > (DrvCAN_GetCANBitRate()*105/100)))
    {
        DEBUG_PRINTF("\nCurrent CAN bitrate = %d\n",DrvCAN_GetCANBitRate());
        return E_DRVCAN_ERR_BITRATE;        
    }

	DrvCAN_LeaveInitMode();
    return E_SUCCESS;

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_SetTiming       																	   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*            -u8Tseg1: specifies Time Segment before the sample point.			    			           */
/*                      This parameter must be a number between 1 and 16.                                  */
/*            -u8Tseg2: Time Segment after the sample point. This parameter                                */
/*                      must be a number between 1 and 8.												   */
/*            -u8Sjw:   Synchronisation Jump Width. This parameter must be							       */
/*                      a number between 1 and 4.														   */
/*            -u32Brp:  Baud Rate Prescaler. This parameter must be a number							   */
/*                      between 1 and 1024.																   */
/* Returns:                                                                                                */
/*              None                                                                                       */
/* Description:                                                                                            */
/*              Setups the CAN timing with specific parameters                                             */
/* Note:                                                                                                   */
/*      Shown CAN bit-rate calculation equation as below:                                                  */
/*                                                                                                         */
/*                                             Fin                                                         */
/*              Bit-Rate (kbps) = ______________________________________                                   */
/*                                 (u32Brp + 1) x (u8Tseg1 + u8Tseg1 +3)                                   */
/*                                                                                                         */
/*                   where: Fin   : System clock freq. (Hz)                                                */
/*                          u32Brp: the baud rate prescale .You can set brp bit field of BTIME register    */
/*                                   BRPE bit feild in BRPE reigster                                       */
/*                          u8Tseg1: Time Segment before the sample point.                                 */
/*                                   You can set tseg1 bit field of BTIME register                         */
/*                          u8Tseg2: Time Segment ater the sample point.                                   */
/*                                   You can set tseg2 bit field of BTIME register                         */
/*                          u32Brp:  The baud rate prescale.                                               */
/*                                   It is composed of BRP in CAN_BTIME register and BRPE                  */
/*                                   in CAN_BRPE register.                                                 */
/*      Example:                                                                                           */
/*              Assume System clock (Fin) = 16MHz, and set u32Brp=1,u8Tseg1=2,u8Tseg2=3                    */
/*                                                                                                         */
/*              According above equations:                                                                 */
/*                                         16000000                                                        */
/*                    CAN bit-rate = _________________________  =  1MHz                                    */
/*                                    (1 + 1) x ( 2 + 3 + 3)                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

void DrvCAN_SetTiming(uint8_t u8Tseg2, uint8_t u8Tseg1, uint8_t u8Sjw, uint32_t u32Brp)
{
    CAN->BTIME.TSEG1 = u8Tseg1;  
    CAN->BTIME.TSEG2 = u8Tseg2;  
    CAN->BTIME.SJW   = u8Sjw;  
    CAN->BTIME.BRP   = u32Brp & 0x3F ;  
    CAN->u32BRPE     = (u32Brp >> 6) & 0x0F;
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_ResetMsgObj 							     										   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              u8MsgObj: specifies the Message object number, from 0 to 31.                               */
/* Returns:                                                                                                */
/*               - E_SUCCESS: SUCCESS                                                                      */
/*               - E_DRVCAN_NO_USEFUL_INTERFACE: No useful interface                                       */
/* Description:                                                                                            */
/*              Configures the target message object as default                                            */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvCAN_ResetMsgObj(uint8_t u8MsgObj)
{
    uint8_t u8MsgIfNum=0;

    if ((u8MsgIfNum = GetFreeIF()) == 2)
    {
        return E_DRVCAN_NO_USEFUL_INTERFACE;
    }

    CAN->sMsgObj[u8MsgIfNum].u32CMASK  = 0xF3;  /*CAN_CMASK_WRRD| CAN_CMASK_MASK | CAN_CMASK_ARB 
                                              | CAN_CMASK_CONTROL | CAN_CMASK_DATAA  | CAN_CMASK_DATAB; */
    CAN->sMsgObj[u8MsgIfNum].u32MASK1  = 0;
    CAN->sMsgObj[u8MsgIfNum].u32MASK2  = 0;

    CAN->sMsgObj[u8MsgIfNum].u32ARB1   = 0;
    CAN->sMsgObj[u8MsgIfNum].u32ARB2   = 0;

    CAN->sMsgObj[u8MsgIfNum].u32MCON   = 0;
    CAN->sMsgObj[u8MsgIfNum].u32DAT_A1 = 0;
    CAN->sMsgObj[u8MsgIfNum].u32DAT_A2 = 0;
    CAN->sMsgObj[u8MsgIfNum].u32DAT_B1 = 0;
    CAN->sMsgObj[u8MsgIfNum].u32DAT_B2 = 0;

    CAN->sMsgObj[u8MsgIfNum].u32CREQ   = 1 + u8MsgObj;

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_ResetAllMsgObj                                                                      */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              None                                                                                       */
/* Returns:                                                                                                */
/*              None                                                                                       */
/* Description:                                                                                            */
/*              Configures all of the message objects as default.                                          */
/*---------------------------------------------------------------------------------------------------------*/

void DrvCAN_ResetAllMsgObj(void)
{
    uint32_t i=0;
    for (i = 0; i < 32; i++)
        DrvCAN_ResetMsgObj(i);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_SetTxMsgObj   																	   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              - u8MsgObj: specifies the Message object number, from 0 to 31.                             */
/*              - pCanMsg:  pointer to the message structure where received data is copied.				   */
/* Returns:                                                                                                */
/*               - E_SUCCESS: SUCCESS                                                                      */
/*               - E_DRVCAN_NO_USEFUL_INTERFACE :No useful interface                                       */
/* Description:                                                                                            */
/*              The function is used to configure a transmit object .                                      */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvCAN_SetTxMsgObj(uint8_t u8MsgObj, STR_CANMSG_T* pCanMsg)
{
    uint8_t u8MsgIfNum=0;

    if ((u8MsgIfNum = GetFreeIF()) == 2)                        /* Check Free Interface for configure */
    {
        return E_DRVCAN_NO_USEFUL_INTERFACE;
    }

    /* update the contents needed for transmission*/
    CAN->sMsgObj[u8MsgIfNum].u32CMASK = 0xF3;  /*CAN_CMASK_WRRD| CAN_CMASK_MASK | CAN_CMASK_ARB 
                                           | CAN_CMASK_CONTROL | CAN_CMASK_DATAA  | CAN_CMASK_DATAB; */

    if (pCanMsg->IdType == CAN_STD_ID)
    {
        /* standard ID*/
        CAN->sMsgObj[u8MsgIfNum].u32ARB1 = 0;
		CAN->sMsgObj[u8MsgIfNum].u32ARB2 =  (((pCanMsg->Id)&0x7FF)<<2) | CAN_ARB2_DIR | CAN_ARB2_MSGVAL; 
    }
    else
    {
        /* extended ID*/
        CAN->sMsgObj[u8MsgIfNum].u32ARB1 = (pCanMsg->Id)&0xFFFF;
        CAN->sMsgObj[u8MsgIfNum].u32ARB2 = ((pCanMsg->Id)&0x1FF0000)>>16 | CAN_ARB2_DIR | CAN_ARB2_XTD | CAN_ARB2_MSGVAL;
    }

    CAN->sMsgObj[u8MsgIfNum].ARB2.DIR =pCanMsg->FrameType?1:0;

    CAN->sMsgObj[u8MsgIfNum].u32DAT_A1 = ((uint16_t)pCanMsg->Data[1]<<8) | pCanMsg->Data[0];
    CAN->sMsgObj[u8MsgIfNum].u32DAT_A2 = ((uint16_t)pCanMsg->Data[3]<<8) | pCanMsg->Data[2];
    CAN->sMsgObj[u8MsgIfNum].u32DAT_B1 = ((uint16_t)pCanMsg->Data[5]<<8) | pCanMsg->Data[4];
    CAN->sMsgObj[u8MsgIfNum].u32DAT_B2 = ((uint16_t)pCanMsg->Data[7]<<8) | pCanMsg->Data[6];

	CAN->sMsgObj[u8MsgIfNum].u32MCON   =  CAN_MCON_NEWDAT | pCanMsg->DLC |CAN_MCON_TXIE | CAN_MCON_EOB;
    CAN->sMsgObj[u8MsgIfNum].u32CREQ   = 1 + u8MsgObj;

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_SetMsgObjMask   																	   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              - u8MsgObj: specifies the Message object number, from 0 to 31.                             */
/*              - MaskMsg: specifies the mask structure as message object                                  */
/* 		               The structure is including                                                          */
/*                          - u8Xtd ( Mask IDE bit)                                                        */
/*                          - u8Dir ( Mask Direction)                                                      */
/*                          - u32Id ( Mask ID bit)                                                         */
/*                          - u8IdType ( Mask ID Type)                                                     */
/*                            Set '0' means DON'T CARE , disable accpetance filter                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               - E_SUCCESS: SUCCESS                                                                      */
/*               - E_DRVCAN_NO_USEFUL_INTERFACE: No useful interface                                       */
/* Description:                                                                                            */
/*              Configures Mask as the message object                                                      */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvCAN_SetMsgObjMask(uint8_t u8MsgObj,STR_CANMASK_T* MaskMsg)
{
    uint8_t u8MsgIfNum=0;

    if ((u8MsgIfNum = GetFreeIF()) == 2)                /* Check Free Interface for configure */
    {
        return E_DRVCAN_NO_USEFUL_INTERFACE;
    }
			 
    if (MaskMsg->u8IdType == CAN_STD_ID)	        /* standard ID*/
    {
        CAN->sMsgObj[u8MsgIfNum].u32MASK1 =  0;
		CAN->sMsgObj[u8MsgIfNum].u32MASK2 =  ((MaskMsg->u32Id&0x7FF)<<2) ; 
    }
    else										    /* extended ID*/
    {
        CAN->sMsgObj[u8MsgIfNum].u32MASK1 = (MaskMsg->u32Id) &0xFFFF;
        CAN->sMsgObj[u8MsgIfNum].u32MASK2 = ((MaskMsg->u32Id)&0x1FF0000)>>16 ;
    }

	CAN->sMsgObj[u8MsgIfNum].MASK2.MXTD = MaskMsg->u8Xtd;
	CAN->sMsgObj[u8MsgIfNum].MASK2.MDIR = MaskMsg->u8Dir;
    
	CAN->sMsgObj[u8MsgIfNum].MCON.UMASK = 1;

	/* update the contents needed for transmission*/
    CAN->sMsgObj[u8MsgIfNum].u32CMASK = CAN_CMASK_WRRD| CAN_CMASK_MASK;
    CAN->sMsgObj[u8MsgIfNum].u32CREQ  = 1 + u8MsgObj;

    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_SetRxMsgObj                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              - u8MsgObj: specifies the Message object number, from 0 to 31.                             */
/*              - u8idType: specifies the identifier type of the frames that will be transmitted           */
/* 		               using this message object. This parameter can be one of the following values:       */
/*                          - CAN_STD_ID (standard ID, 11-bit)                                             */
/*                          - CAN_EXT_ID (extended ID, 29-bit)                                             */
/*              - u32id: specifies the identifier used for acceptance filtering.                           */
/*              - u8singleOrFifoLast: specifies the end-of-buffer indicator.                               */
/*                    This parameter can be one of the following values:								   */
/*                          - TRUE: for a single receive object or a FIFO receive						   */
/*                            object that is the last one of the FIFO.									   */
/*                          - FALSE: for a FIFO receive object that is not the last one.				   */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               - E_SUCCESS: SUCCESS                                                                      */
/*               - E_DRVCAN_NO_USEFUL_INTERFACE: No useful interface                                       */
/* Description:                                                                                            */
/*              The function is used to configure a receive message object.                                */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvCAN_SetRxMsgObj(uint8_t u8MsgObj, uint8_t u8idType, uint32_t u32id, uint8_t u8singleOrFifoLast)
{
    uint8_t u8MsgIfNum=0;

    if ((u8MsgIfNum = GetFreeIF()) == 2)                        /* Check Free Interface for configure */
    {
        return E_DRVCAN_NO_USEFUL_INTERFACE;
    }
                                                            /* Command Setting */
    CAN->sMsgObj[u8MsgIfNum].u32CMASK = CAN_CMASK_WRRD| CAN_CMASK_MASK | CAN_CMASK_ARB | 
	                             CAN_CMASK_CONTROL | CAN_CMASK_DATAA  | CAN_CMASK_DATAB;

    if (u8idType == CAN_STD_ID) /* According STD/EXT ID format,Configure Mask and Aribration register */
    {
        CAN->sMsgObj[u8MsgIfNum].u32ARB1   = 0;
        CAN->sMsgObj[u8MsgIfNum].u32ARB2   = CAN_ARB2_MSGVAL | (u32id & 0x7FF)<< 2;
    }
    else
    {
        CAN->sMsgObj[u8MsgIfNum].u32ARB1   = u32id & 0xFFFF;
        CAN->sMsgObj[u8MsgIfNum].u32ARB2   = CAN_ARB2_MSGVAL | CAN_ARB2_XTD | (u32id & 0x1FF0000)>>16;
    }

    CAN->sMsgObj[u8MsgIfNum].MCON.UMASK = 1;
    CAN->sMsgObj[u8MsgIfNum].MCON.RXIE  = 1;
	CAN->sMsgObj[u8MsgIfNum].MCON.EOB   = u8singleOrFifoLast?1:0;

    CAN->sMsgObj[u8MsgIfNum].u32DAT_A1  = 0;
    CAN->sMsgObj[u8MsgIfNum].u32DAT_A2  = 0;
    CAN->sMsgObj[u8MsgIfNum].u32DAT_B1  = 0;
    CAN->sMsgObj[u8MsgIfNum].u32DAT_B2  = 0;

    CAN->sMsgObj[u8MsgIfNum].u32CREQ = 1 + u8MsgObj;

    return E_SUCCESS;
}

 
/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_ClrIntPn                                                                            */
/*                                                                                                         */
/* Parameter:        																					   */	
/*               - u8MsgObj: specifies the Message object number, from 0 to 31.                            */
/* Returns:                                                                                                */
/*               - E_SUCCESS: Transmission ended                                                           */
/*               - E_DRVCAN_NO_USEFUL_INTERFACE: No useful interface                                       */
/* Description:                                                                                            */
/*              The function is used to reset IntPnd and TXRQSTNEWDAT bit in a Message Object.             */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_ClrIntPnd(uint8_t u8MsgObj)
{
    uint8_t u8MsgIfNum=0;

    if ((u8MsgIfNum = GetFreeIF()) == 2)    /* Judge if any interface is free */
    {
        return E_DRVCAN_NO_USEFUL_INTERFACE;
    }

    CAN->sMsgObj[u8MsgIfNum].u32CMASK = CAN_CMASK_CLRINTPND | CAN_CMASK_TXRQSTNEWDAT;
    CAN->sMsgObj[u8MsgIfNum].u32CREQ = 1 + u8MsgObj;

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_SetTxRqst                                                                           */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              - u8MsgObj: specifies the Message object number, from 0 to 31.                             */
/* Returns:                                                                                                */
/*              - E_SUCCESS: SUCCESS                                                                       */
/* Description:                                                                                            */
/*              The function is used to set transmit request bit in the target message object.             */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvCAN_SetTxRqst(uint8_t u8MsgObj)
{
    STR_CANMSG_T rMsg; 
    DrvCAN_ReadMsgObj(u8MsgObj,TRUE, &rMsg);
    CAN->sMsgObj[0].u32CMASK  = CAN_CMASK_WRRD |CAN_CMASK_TXRQSTNEWDAT ;
    CAN->sMsgObj[0].u32CREQ  = 1 + u8MsgObj;
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_ReadMsgObj                                                                          */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              - u8MsgObj: specifies the Message object number, from 0 to 31.                             */
/*              - u8Release: specifies the message release indicator.                                      */
/*                    This parameter can be one of the following values:								   */
/*                          - TRUE: the message object is released when getting the data.				   */
/*                          - FALSE:the message object is not released.								       */
/*              - pCanMsg: pointer to the message structure where received data is copied.				   */
/* Returns:                                                                                                */
/*               - E_SUCCESS: Success                                                                      */
/*               - E_DRVCAN_NO_PENDING_MSG: No any message received                                        */
/* Description:                                                                                            */
/*              Gets the message, if received.                                                             */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvCAN_ReadMsgObj(uint8_t u8MsgObj, uint8_t u8Release, STR_CANMSG_T* pCanMsg)
{
    if (!DrvCAN_IsNewDataReceived(u8MsgObj))
    {
        return E_DRVCAN_NO_PENDING_MSG;
    }

    CAN->STATUS.RXOK =0;

    /* read the message contents*/
    CAN->sMsgObj[1].u32CMASK = CAN_CMASK_MASK
                          | CAN_CMASK_ARB
                          | CAN_CMASK_CONTROL
                          | CAN_CMASK_CLRINTPND
                          | (u8Release ? CAN_CMASK_TXRQSTNEWDAT : 0)
                          | CAN_CMASK_DATAA
                          | CAN_CMASK_DATAB;

    CAN->sMsgObj[1].u32CREQ = 1 + u8MsgObj;

    if (CAN->sMsgObj[1].CREQ.BUSY ==1)
    {
        return E_DRVCAN_NO_USEFUL_INTERFACE;
    }

    if ((CAN->sMsgObj[1].ARB2.XTD) == 0)
    {
        /* standard ID*/
        pCanMsg->IdType = CAN_STD_ID;
        pCanMsg->Id     = (CAN->sMsgObj[1].ARB2.ID28_16 >> 2);
    }
    else
    {
        /* extended ID*/
        pCanMsg->IdType = CAN_EXT_ID;
        pCanMsg->Id  = (((CAN->sMsgObj[1].u32ARB2) & 0x1FFF)<<16) | CAN->sMsgObj[1].u32ARB1; 
    }
    
    pCanMsg->DLC     = CAN->sMsgObj[1].MCON.DLC;
    pCanMsg->Data[0] = CAN->sMsgObj[1].DAT_A1.DATA0;
    pCanMsg->Data[1] = CAN->sMsgObj[1].DAT_A1.DATA1;
    pCanMsg->Data[2] = CAN->sMsgObj[1].DAT_A2.DATA2;
    pCanMsg->Data[3] = CAN->sMsgObj[1].DAT_A2.DATA3;
    pCanMsg->Data[4] = CAN->sMsgObj[1].DAT_B1.DATA4;
    pCanMsg->Data[5] = CAN->sMsgObj[1].DAT_B1.DATA5;
    pCanMsg->Data[6] = CAN->sMsgObj[1].DAT_B2.DATA6;
    pCanMsg->Data[7] = CAN->sMsgObj[1].DAT_B2.DATA7;
 
    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_WaitEndOfTx                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              None                                                                                       */
/* Returns:                                                                                                */
/*               - E_SUCCESS: Transmission ended                                                           */
/* Description:                                                                                            */
/*              Waiting until current transmission is finished.                                            */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvCAN_WaitEndOfTx(void)
{
    while(CAN->STATUS.TXOK==0);
    return E_SUCCESS;
}
           

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_BasicSendMsg                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              pCanMsg: Pointer to the message structure containing data to transmit.                     */
/* Returns:                                                                                                */
/*                  - E_SUCCESS: Transmission OK                                                           */
/*                  - E_DRVCAN_ERR_TIMEOUT: Check busy flag of interface 0 is timeout                      */
/* Description:                                                                                            */
/*              The function is used to send CAN message in BASIC mode of test mode. Before call the API,  */
/*              the user should be call DrvCAN_EnterTestMode(CAN_TESTR_BASIC) and let CAN controller enter */
/*              basic mode of test mode. Please notice IF1 Registers used as Tx Buffer in basic mode       */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvCAN_BasicSendMsg(STR_CANMSG_T* pCanMsg)
{
    uint32_t i=0;
    while(CAN->sMsgObj[0].CREQ.BUSY==1);
   
    CAN->STATUS.TXOK = 0;

    CAN->sMsgObj[0].u32CMASK = CAN_CMASK_WRRD;  

    if (pCanMsg->IdType == CAN_STD_ID)
    {
        /* standard ID*/
        CAN->sMsgObj[0].u32ARB1 = 0;
		CAN->sMsgObj[0].u32ARB2 =  (((pCanMsg->Id)&0x7FF)<<2) ;
    }
    else
    {
        /* extended ID*/
        CAN->sMsgObj[0].u32ARB1 = (pCanMsg->Id)&0xFFFF;
        CAN->sMsgObj[0].u32ARB2 = ((pCanMsg->Id)&0x1FFF0000)>>16  | CAN_ARB2_XTD;

    }
    CAN->sMsgObj[0].ARB2.DIR = (pCanMsg->FrameType)?1:0;

    CAN->sMsgObj[0].MCON.DLC = pCanMsg->DLC ;

    CAN->sMsgObj[0].u32DAT_A1 = ((uint16_t)pCanMsg->Data[1]<<8) | pCanMsg->Data[0];
    CAN->sMsgObj[0].u32DAT_A2 = ((uint16_t)pCanMsg->Data[3]<<8) | pCanMsg->Data[2];
    CAN->sMsgObj[0].u32DAT_B1 = ((uint16_t)pCanMsg->Data[5]<<8) | pCanMsg->Data[4];
    CAN->sMsgObj[0].u32DAT_B2 = ((uint16_t)pCanMsg->Data[7]<<8) | pCanMsg->Data[6];

    /* request transmission*/
	CAN->sMsgObj[0].CREQ.BUSY = 0;
	if ( CAN->sMsgObj[0].CREQ.BUSY ==1 ) {
		DEBUG_PRINTF("Cannot clear busy for sending ...\n");
        return E_DRVCAN_ERR_TIMEOUT;
	}
	CAN->sMsgObj[0].CREQ.BUSY =1;						   // sending

    for ( i=0; i<0xFFFFF; i++) {
		if ( CAN->sMsgObj[0].CREQ.BUSY ==0) break;
	}

    if ( i >= 0xFFFFF ) {
		DEBUG_PRINTF("Cannot send out...\n");
        return E_DRVCAN_ERR_TIMEOUT;
	}
    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_BasicReceiveMsg                                                                     */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              pCanMsg: pointer to the message structure where message is copied.                         */
/* Returns:                                                                                                */
/*              - E_SUCCESS: Reception OK                                                                  */
/*              - E_DRVCAN_NO_PENDING_MSG: No any message received                                         */
/* Description:                                                                                            */
/*              Get a message information in BASIC mode. This mode does not use the message RAM            */
/*              Using IF2 to get receive message information                                               */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvCAN_BasicReceiveMsg(STR_CANMSG_T* pCanMsg)
{
    if (CAN->sMsgObj[1].MCON.NEWDAT == 0) /* In basic mode, receive data always save in IF2 */
    {
        return E_DRVCAN_NO_PENDING_MSG;
    }

    CAN->STATUS.RXOK = 0;
    
    CAN->sMsgObj[1].u32CMASK = CAN_CMASK_ARB
                             | CAN_CMASK_CONTROL
                             | CAN_CMASK_DATAA
                             | CAN_CMASK_DATAB;

    if (CAN->sMsgObj[1].ARB2.XTD == 0)
    {
        /* standard ID*/
        pCanMsg->IdType = CAN_STD_ID;
        pCanMsg->Id = (CAN->sMsgObj[1].u32ARB2 >> 2) & 0x07FF;

    }
    else
    {
        /* extended ID*/
        pCanMsg->IdType = CAN_EXT_ID;
        pCanMsg->Id  = (CAN->sMsgObj[1].u32ARB2 & 0x1FFF)<<16;
        pCanMsg->Id |= (uint32_t)CAN->sMsgObj[1].u32ARB1;
    }

    pCanMsg->FrameType = (CAN->sMsgObj[1].ARB2.DIR)?0:1;

    pCanMsg->DLC = CAN->sMsgObj[1].MCON.DLC;
    pCanMsg->Data[0] = CAN->sMsgObj[1].DAT_A1.DATA0;
    pCanMsg->Data[1] = CAN->sMsgObj[1].DAT_A1.DATA1;
    pCanMsg->Data[2] = CAN->sMsgObj[1].DAT_A2.DATA2;
    pCanMsg->Data[3] = CAN->sMsgObj[1].DAT_A2.DATA3;
    pCanMsg->Data[4] = CAN->sMsgObj[1].DAT_B1.DATA4;
    pCanMsg->Data[5] = CAN->sMsgObj[1].DAT_B1.DATA5;
    pCanMsg->Data[6] = CAN->sMsgObj[1].DAT_B2.DATA6;
    pCanMsg->Data[7] = CAN->sMsgObj[1].DAT_B2.DATA7;

    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_EnterInitMode                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*                  None                                                                                   */
/* Returns:                                                                                                */
/*              	None.                                                                                  */
/* Description:                                                                                            */
/*              This function is used to set CAN to enter initialization mode and enable access bit timing */
/*              register. After bit timing configuration ready, user must call DrvCAN_LeaveInitMode()      */
/*              to leave initialization mode and lock bit timing register to let new configuration         */
/*              take effect.                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
 
void DrvCAN_EnterInitMode(void)
{
    CAN->CON.INIT = 1;
    CAN->CON.CCE  = 1;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_LeaveInitMode                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*                  None                                                                                   */
/* Returns:                                                                                                */
/*              	None.                                                                                  */
/* Description:                                                                                            */
/*              This function is used to set CAN to leave initialization mode to let                       */
/*              bit timing configuration take effect after configuration ready.                            */
/*---------------------------------------------------------------------------------------------------------*/

void DrvCAN_LeaveInitMode(void)
{
    CAN->CON.INIT = 0;
    CAN->CON.CCE  = 0;
   	while ( CAN->CON.INIT==1);       /* Check INIT bit is released */
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_EnterTestMode                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              u8TestMask: specifies the configuration in test modes                                      */
/*                      CAN_TESTR_BASIC   : Enable basic mode of test mode                                 */
/*                      CAN_TESTR_SILENT  : Enable silent mode of test mode                                */
/*                      CAN_TESTR_LBACK   : Enable Loop Back Mode of test mode                             */
/*                      CAN_TESTR_TX0/CAN_TESTR_TX1: Control CAN_TX pin bit feild                          */
/*                                                                                                         */
/* Returns:                                                                                                */
/*              None.                                                                                      */
/* Description:                                                                                            */
/*              Switchs the CAN into test mode. There are four test mode (BASIC/SILENT/LOOPBACK/           */
/*              LOOPBACK combined SILENT/CONTROL_TX_PIN)could be selected. After setting test mode,user    */
/*              must call DrvCAN_LeaveInitMode() to let the setting take effect.                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvCAN_EnterTestMode(uint8_t u8TestMask)
{
    CAN->CON.TEST = 1;
	CAN->u32TEST = u8TestMask;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_LeaveTestMode                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*              	None.                                                                                  */
/* Returns:                                                                                                */
/*              	None.                                                                                  */
/* Description:                                                                                            */
/*              This function is used to Leave the current test mode (switch into normal mode).            */
/*---------------------------------------------------------------------------------------------------------*/

void DrvCAN_LeaveTestMode(void)
{
	CAN->CON.TEST = 1;
  	CAN->u32TEST &= ~(CAN_TESTR_LBACK | CAN_TESTR_SILENT | CAN_TESTR_BASIC);
  	CAN->CON.TEST = 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_IsNewDataReceived                                                                   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            	u8MsgObj: specifies the Message object number, from 0 to 31.                           */
/* Returns:                                                                                                */
/*              	A non-zero value if the corresponding message object has a new data bit is set, else 0.*/
/* Description:                                                                                            */
/*              This function is used to get the waiting status of a received message.                     */
/*---------------------------------------------------------------------------------------------------------*/

uint32_t DrvCAN_IsNewDataReceived(uint8_t u8MsgObj)
{
    return (u8MsgObj < 16 ? CAN->u32NDAT1 & (1 << u8MsgObj) : CAN->u32NDAT2 & (1 << (u8MsgObj-16)));
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_IsTxRqstPending                                                                     */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            	u8MsgObj: specifies the Message object number, from 0 to 31.                           */
/* Returns:                                                                                                */
/*              	A non-zero value if the corresponding message has an tx request pending, else 0.       */
/* Description:                                                                                            */
/*              This function is used to get the request pending status of a transmitted message.          */
/*---------------------------------------------------------------------------------------------------------*/

uint32_t DrvCAN_IsTxRqstPending(uint8_t u8MsgObj)
{
  	return (u8MsgObj < 16 ? CAN->u32TXREQ1 & (1 << u8MsgObj) : CAN->u32TXREQ2 & (1 << (u8MsgObj-16)));
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_IsIntPending                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            	u8MsgObj: specifies the Message object number, from 0 to 31.                           */
/* Returns:                                                                                                */
/*              	A non-zero value if the corresponding message has an interrupt pending, else 0.        */
/* Description:                                                                                            */
/*              This function is used to get the interrupt status of a message object                      */
/*---------------------------------------------------------------------------------------------------------*/

uint32_t DrvCAN_IsIntPending(uint8_t u8MsgObj)
{
  	return (u8MsgObj < 16 ? CAN->u32IPND1 & (1 << u8MsgObj) : CAN->u32IPND2 & (1 << (u8MsgObj-16)));
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_IsObjectValid                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            	u8MsgObj: specifies the Message object number, from 0 to 31.                           */
/* Returns:                                                                                                */
/*            		A non-zero value if the corresponding message object is	valid, else 0.                 */
/* Description:                                                                                            */
/*              This function is used to test the validity of a message object (ready to use).             */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvCAN_IsObjectValid(uint8_t u8MsgObj)
{
  	return (u8MsgObj < 16 ? CAN->u32MVLD1 & (1 << u8MsgObj) : CAN->u32MVLD2 & (1 << (u8MsgObj-16)));
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_ResetIF                                                                             */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            	u8IF_Num: specifies the message interface, 0 or 1.                                     */
/* Returns:                                                                                                */
/*            		None.                                                                                  */
/* Description:                                                                                            */
/*              This function is used to reset message interface parameters.                               */
/*---------------------------------------------------------------------------------------------------------*/
void DrvCAN_ResetIF(uint8_t u8IF_Num)
{
	if(u8IF_Num > 1)
        return;
    CAN->sMsgObj[u8IF_Num].u32CREQ     = 0x0;			// set bit15 for sending
	CAN->sMsgObj[u8IF_Num].u32CMASK    = 0x0;
	CAN->sMsgObj[u8IF_Num].u32MASK1    = 0x0;			// useless in basic mode
	CAN->sMsgObj[u8IF_Num].u32MASK2    = 0x0;		    // useless in basic mode
	CAN->sMsgObj[u8IF_Num].u32ARB1     = 0x0;		    // ID15~0
	CAN->sMsgObj[u8IF_Num].u32ARB2     = 0x0;		    // MsgVal, eXt, xmt, ID28~16
	CAN->sMsgObj[u8IF_Num].u32MCON     = 0x0;  	    	// DLC
	CAN->sMsgObj[u8IF_Num].u32DAT_A1   = 0x0;			// data0,1
	CAN->sMsgObj[u8IF_Num].u32DAT_A2   = 0x0;			// data2,3
	CAN->sMsgObj[u8IF_Num].u32DAT_B1   = 0x0;			// data4,5
	CAN->sMsgObj[u8IF_Num].u32DAT_B2   = 0x0;			// data6,7
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_WaitMsg                                                                             */
/*                                                                                                         */
/* Parameter:        																					   */	
/*                  None                                                                                   */
/* Returns:                                                                                                */
/*                  None                                                                                   */
/* Description:                                                                                            */
/*              This function is used to wait message into message buffer in basic mode. Please notice the */
/*              function is polling NEWDAT bit of MCON register by while loop and it is used in basic mode.*/
/*---------------------------------------------------------------------------------------------------------*/
void DrvCAN_WaitMsg(void)
{
	CAN->u32STATUS = 0x0;			/* clr status */

	while (1) 
	{
		if ( CAN->sMsgObj[1].MCON.NEWDAT == 1 )  /* check new data */
		{
			DEBUG_PRINTF("New Data IN\n");
			break;	
		}
        if ( CAN->STATUS.RXOK == 1) 
			DEBUG_PRINTF("Rx OK\n");

		if ( CAN->STATUS.LEC)
		{
			DEBUG_PRINTF("Error\n");
		}
	}
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvCAN_EnableInt                                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u16IntEnable - Interrupt Enable (CAN_CON_IE or CAN_CON_SIE or CAN_CON_EIE)                */
/*                              CAN_CON_IE  : Module Interrupt Enable                                      */
/*                              CAN_CON_SIE : Status Change Interrupt Enable                               */
/*                              CAN_CON_EIE : Error Interrupt Enable                                       */
/* Returns:                                                                                                */
/*               E_SUCCESS : Success                                                                       */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable CAN interrupt and corresponding NVIC of CAN                                        */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_EnableInt(uint16_t u16IntEnable)
{
    DrvCAN_EnterInitMode();

    CAN->u32CON = (CAN->u32CON & 0xF1) | ((u16IntEnable & CAN_CON_IE   )? CAN_CON_IE :0)
                                       | ((u16IntEnable & CAN_CON_SIE  )? CAN_CON_SIE:0)
                                       | ((u16IntEnable & CAN_CON_EIE  )? CAN_CON_EIE:0);

    NVIC_SetPriority(CAN0_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
    NVIC_EnableIRQ(CAN0_IRQn);

	DrvCAN_LeaveInitMode();
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: 	 DrvCAN_DisableInt                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u16IntEnable - Interrupt Enable (CAN_CON_IE or CAN_CON_SIE or CAN_CON_EIE)                */
/*                              CAN_CON_IE  : Module Interrupt Enable                                      */
/*                              CAN_CON_SIE : Status Change Interrupt Enable                               */
/*                              CAN_CON_EIE : Error Interrupt Enable                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS : Success                                                                       */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable CAN interrupt and corresponding NVIC of CAN                                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_DisableInt(uint16_t u16IntEnable)
{
    DrvCAN_EnterInitMode();

    CAN->u32CON = CAN->u32CON & ~(CAN_CON_IE | ((u16IntEnable & CAN_CON_SIE )?CAN_CON_SIE:0)
                                    | ((u16IntEnable & CAN_CON_EIE )?CAN_CON_EIE:0));
    NVIC_DisableIRQ(CAN0_IRQn);

    DrvCAN_LeaveInitMode();
   
    return E_SUCCESS;
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function: 	 DrvCAN_InstallCallback                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               Type          - [in]  It could be CALLBACK_RXOK / CALLBACK_TXOK / CALLBACK_EWARN          */
/*                                     CALLBACK_BOFF / CALLBACK_MSG / CALLBACK_WAKEUP.                     */
/*                                                                                                         */
/*                                     CALLBACK_RXOK: Received a Message Successfully callback fn.         */
/*                                     CALLBACK_TXOK: Transmitted a Message Successfully callback fn.      */
/*                                     CALLBACK_EWARN:Error Warning Status callback fn.                    */
/*                                     CALLBACK_BOFF: Bus-off Status callback fn.                          */
/*                                     CALLBACK_MSG:Target message object transmitted/received callback fn.*/
/*                                     CALLBACK_WAKEUP: Wakeup callback fn.                                */
/*                                                                                                         */
/*               callbackfn    - [in]  Call back function pointer                                          */
/* Returns:                                                                                                */
/*               E_SUCCESS             Success                                                             */
/*               E_DRVCAN_ERR_ARGUMENT Failed                                                              */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Install CAN call back function for CAN normal function                                    */
/*                                      MSG,RXOK,TXOK,EWARN,BOFF,WAKEUP                                    */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_InstallCallback(E_CAN_CALLBACK_TYPE Type, CAN_CALLBACK callbackfn)
{
    switch(Type)
    {
        
        case CALLBACK_MSG:      
                                CANHandler.MessageCallBackFn            = callbackfn;    
                                break;
        case CALLBACK_RXOK:     
                                CANHandler.RxOkCallBackFn               = callbackfn;    
                                break;
        case CALLBACK_TXOK:     
                                CANHandler.TxOkCallBackFn               = callbackfn;    
                                break;
        case CALLBACK_EWARN:    
                                CANHandler.ErrorWarningCallBackFn       = callbackfn;    
                                break;
        case CALLBACK_BOFF:     
                                CANHandler.BusOffCallBackFn             = callbackfn;    
                                break;
        case CALLBACK_WAKEUP:   
                                CANHandler.WakeupCallBackFn             = callbackfn;    
                                break;
        default:
            return E_DRVCAN_ERR_ARGUMENT;                               
    }
           
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_UninstallCallback                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               Type          - [in]  It could be CALLBACK_RXOK / CALLBACK_TXOK / CALLBACK_EWARN          */
/*                                     CALLBACK_BOFF / CALLBACK_MSG / CALLBACK_WAKEUP.                     */
/*                                                                                                         */
/*                                     CALLBACK_RXOK: Received a Message Successfully callback fn.         */
/*                                     CALLBACK_TXOK: Transmitted a Message Successfully callback fn.      */
/*                                     CALLBACK_EWARN:Error Warning Status callback fn.                    */
/*                                     CALLBACK_BOFF: Bus-off Status callback fn.                          */
/*                                     CALLBACK_MSG:Target message object transmitted/received callback fn.*/
/*                                     CALLBACK_WAKEUP: Wakeup callback fn.                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS             Success                                                             */
/*               E_DRVCAN_ERR_ARGUMENT Failed                                                              */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to uninstall exist callback function pointer                         */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvCAN_UninstallCallback(E_CAN_CALLBACK_TYPE Type)
{
    switch(Type)
    {
        
        case CALLBACK_MSG:      
                                CANHandler.MessageCallBackFn            = 0;    
                                break;
        case CALLBACK_RXOK:     
                                CANHandler.RxOkCallBackFn               = 0;    
                                break;
        case CALLBACK_TXOK:     
                                CANHandler.TxOkCallBackFn               = 0;    
                                break;
        case CALLBACK_EWARN:    
                                CANHandler.ErrorWarningCallBackFn       = 0;    
                                break;
        case CALLBACK_BOFF:     
                                CANHandler.BusOffCallBackFn             = 0;    
                                break;
        case CALLBACK_WAKEUP:   
                                CANHandler.WakeupCallBackFn             = 0;    
                                break;
        default:
            return E_DRVCAN_ERR_ARGUMENT;                               
    }
           
    return E_SUCCESS;

}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_EnableWakeUp                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to enable wakeup function                                            */
/*---------------------------------------------------------------------------------------------------------*/

void DrvCAN_EnableWakeUp(void)
{
    CAN->u32WU_EN = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAN_DisableWakeUp                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to disable wakeup function                                           */
/*---------------------------------------------------------------------------------------------------------*/

void DrvCAN_DisableWakeUp(void)
{
    CAN->u32WU_EN = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: 	 DrvCAN_GetRxErrCount                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               NONE                                                                                      */
/* Returns:                                                                                                */
/*               Current Receive Error Counter(REC)                                                        */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to get current receive error counter (REC)                           */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvCAN_GetRxErrCount(void)
{
    return CAN->ERR.REC;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: 	 DrvCAN_GetTxErrCount                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               NONE                                                                                      */
/* Returns:                                                                                                */
/*               Current Transmit Error Counter(TEC)                                                       */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to get current transmit error counter (TEC)                          */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvCAN_GetTxErrCount(void)
{
    return CAN->ERR.TEC;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:    DrvCAN_GetVersion                                                                          */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	            None                                                                                       */
/* Returns:                                                                                                */
/*              The version number of CAN driver                                                           */
/* Description:                                                                                            */
/*              This function is used to return the version number of CAN driver.                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvCAN_GetVersion(void)
{
   return DRVCAN_VERSION_NUM;
}



