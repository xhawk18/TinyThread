/******************************************************************************
 * @file     nano1xx_uart.c
 * @brief    NANO1xx UART driver source file
 *           
 * @version  1.0.2
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "nano1xx_uart.h"
#include "nano1xx_sys.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_UART_Driver UART Driver
  @{
*/


/** @addtogroup NANO1xx_UART_EXPORTED_FUNCTIONS UART Exported Functions
  @{
*/

/**
 *  @brief The function is used to get UART clock.
 *
 *  @param[in] None
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h
 *
 *  @return clk: UART clock
 *
 *  @par Example
 *  @code
 *  //Get UART clock
 *  uint32_t clk;
 *  clk = UART_GetUartClk ();
 *  @endcode
 *
 */
uint32_t UART_GetUartClk(void)
{
	uint32_t clk =0 , div;

	div = ( (CLK->CLKDIV0 & CLK_CLKDIV0_UART_MASK) >> 8) + 1;

	switch (CLK->CLKSEL1 & CLK_CLKSEL1_UART_MASK)
	{
		case 0:  
			clk = __XTAL; /* HXT */
			break;
		case 1: 
			clk = __RTC_XTAL;  /* LXT */
			break;
		case 2:  
			clk = SYS_GetPLLClockFreq(); /* PLL */
			break;
		case 3: 
			clk = __IRC12M; /* HIRC */
			break;								
	}

	clk /= div;
	
	return clk;
}


/**
 *  @brief The function is used to set RTS setting.
 *
 *  @param[in] tUART    Specify UART0/UART1
 *  @param[in] u8Value \n
 *                     Set 0: Drive RTS pin to logic 1 (If the LEV_RTS set to low level triggered). \n
 *                            Drive RTS pin to logic 0 (If the LEV_RTS set to high level triggered). \n
 *                     Set 1: Drive RTS pin to logic 0 (If the LEV_RTS set to low level triggered). \n	
 *                            Drive RTS pin to logic 1 (If the LEV_RTS set to high level triggered). \n
 *                     Note: LEV_RTS is RTS Trigger Level. 0 is low level and 1 is high level. 
 *
 *  @param[in] u16TriggerLevel RTS Trigger Level :DRVUART_FIFO_1BYTES to DRVUART_FIFO_14BYTES
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  //Condition: Drive RTS to logic 1 in UART channel 1 and Set RTS trigger level is 1 bytes
 *  UART_SetRTS (UART_PORT1,1, DRVUART_FIFO_1BYTES);
 *  @endcode
 *
 */
void UART_SetRTS(UART_TypeDef  *tUART, uint8_t u8Value, uint16_t u16TriggerLevel)
{
	assert_param(CHECK_UART_RXTRIGGERLEVEL(u16TriggerLevel));

	tUART->MCSR &= ~UART_MCSR_LEV_RTS;
	tUART->MCSR |= u8Value & UART_MCSR_LEV_RTS;
	tUART->TLCTL &= ~UART_TLCTL_RTS_TRI_LEV;
	tUART->TLCTL |= (u16TriggerLevel << 12) & UART_TLCTL_RTS_TRI_LEV;
}	



/**
 *  @brief       The function is used to initialize UART. 
 *               It consists of baud-rate, parity, data-bits, stop-bits, rx-trigger-level and timeout interval settings.
 *
 *  @param[in]   tUART     Specify UART0/UART1
 *  @param[in]   sParam      Specify the property of UART. It includes
 *                           - \b u32BaudRate: Baud rate (Hz)
 *                           - \b u8cParity: NONE/EVEN/ODD parity \n
 *                           It could be: \n
 *                              DRVUART_PARITY_NONE (None parity) \n
 *                              DRVUART_PARITY_EVEN (Even parity) \n 
 *                              DRVUART_PARITY_ODD(Odd parity) \n  
 *                           - \b u8cDataBits: data bit setting \n
 *                             It could be: \n
 *                                DRVUART_DATA_BITS_5 (5 data bits) \n
 *                                DRVUART_DATA_BITS_6 (6 data bits) \n 
 *                                DRVUART_DATA_BITS_7 (7 data bits) \n
 *                                DRVUART_DATA_BITS_8 (8 data bits) \n
 *                           - \b u8cStopBits: stop bits setting \n
 *                             It could be: \n
 *                                DRVUART_STOPBITS_1 (1 stop bit) \n
 *                                DRVUART_STOPBITS_1_5 (1.5 stop bit) \n
 *                                DRVUART_STOPBITS_2 (2 stop bits) \n
 *                           - \b u8cRxTriggerLevel: Rx FIFO interrupt trigger Level \n
 *                             LEVEL_X_BYTE means the trigger level of UART channel is X bytes \n
 *                             It could be: \n
 *                                DRVUART_FIFO_1BYTE \n
 *                                DRVUART_FIFO_4BYTES \n
 *                                DRVUART_FIFO_8BYTES \n
 *                                DRVUART_FIFO_14BYTES \n
 *                           - \b u8TimeOut: Time out value "N". It represents N-clock cycle and the counting clock is baud rate.
 *                           - \b u8EnableDiv16: Divider 16 ENABLE/DISABLE.
 * @par Include:
 *      nano1xx_drivers/nano1xx_uart.h
 *
 * @return E_SUCCESS: Success
 *
 * @par Example: 
 * @code
 *  //Set UART0 under 115200bps, 8 data bits ,1 stop bit and none parity and 1 byte Rx trigger level settings, disable divider 16.
 *  STR_UART_T sParam; 
 *  sParam.u32BaudRate 		= 115200; 
 *  sParam.u8cDataBits 		= DRVUART_DATABITS_8; 
 *  sParam.u8cStopBits 		= DRVUART_STOPBITS_1; 
 *  sParam.u8cParity 		= DRVUART_PARITY_NONE; 
 *  sParam.u8cRxTriggerLevel	= DRVUART_FIFO_1BYTES; 
 *  sParam.u8EnableDiv16		= DISABLE; 
 *  UART_Init (UART_PORT0, &sParam);
 * @endcode
 *
*/
int32_t UART_Init(UART_TypeDef  *tUART, STR_UART_T *sParam)
{
	uint32_t clk, factor, divider;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Check UART port                                                                                     */
    /*-----------------------------------------------------------------------------------------------------*/
    assert_param(CHECK_UART_PORT(tUART));


    /*-----------------------------------------------------------------------------------------------------*/
    /* Check the supplied parity                                                                           */
    /*-----------------------------------------------------------------------------------------------------*/
    assert_param(CHECK_UART_PARITY(sParam->u32cParity));

	
    /*-----------------------------------------------------------------------------------------------------*/
    /* Check the supplied number of data bits                                                              */
    /*-----------------------------------------------------------------------------------------------------*/
    assert_param(CHECK_UART_DATABIT(sParam->u32cDataBits));

	
    /*-----------------------------------------------------------------------------------------------------*/
    /* Check the supplied number of stop bits                                                              */
    /*-----------------------------------------------------------------------------------------------------*/
    assert_param(CHECK_UART_STOPBIT(sParam->u32cStopBits));


    /*-----------------------------------------------------------------------------------------------------*/
    /* Check the supplied nember of trigger level bytes                                                    */
    /*-----------------------------------------------------------------------------------------------------*/
    assert_param(CHECK_UART_RXTRIGGERLEVEL(sParam->u32cRxTriggerLevel));				
	
	if(tUART == UART0)
	{
		/* Reset IP */
		GCR->IPRST_CTL2 |= GCR_IPRSTCTL2_UART0;
		GCR->IPRST_CTL2 &= ~GCR_IPRSTCTL2_UART0;
		
		/* Enable UART clock */
		CLK->APBCLK |= CLK_APBCLK_UART0_EN;
	}
	else
	{
		/* Reset IP */
		GCR->IPRST_CTL2 |= GCR_IPRSTCTL2_UART1;
		GCR->IPRST_CTL2 &= ~GCR_IPRSTCTL2_UART1;
		
		/* Enable UART clock */
		CLK->APBCLK |= CLK_APBCLK_UART1_EN;
	}
	
	/* Tx FIFO Reset & Rx FIFO Reset & FIFO Mode Enable */
	tUART->CTL |= UART_CTL_RX_RST;
  	tUART->CTL |= UART_CTL_TX_RST;

	/* Set Rx Trigger Level, Parity & Data bits & Stop bits */
	tUART->TLCTL = ((sParam->u32cRxTriggerLevel << 8) |
					(sParam->u32cParity << 3) |
					(sParam->u32cStopBits << 2) |
					(sParam->u32cDataBits) );
	
	/* Set Time-Out */
	tUART->TMCTL &= ~UART_TMCTL_TOIC;
	tUART->TMCTL |= sParam->u8TimeOut & UART_TMCTL_TOIC;

	/* Set BaudRate */
	clk = UART_GetUartClk();

	if (sParam->u8EnableDiv16 == 1)
	{
		factor = 16 * sParam->u32BaudRate;
		tUART->BAUD |= UART_BAUD_DIV_16_EN; 
	}
	else
	{
		factor = sParam->u32BaudRate;
		tUART->BAUD &= ~UART_BAUD_DIV_16_EN;
	}
	
	divider = (clk / factor) - 1;
	if ((clk % factor) >= (factor / 2))
		divider++;

	if (divider > 0xFFF)
	{
		tUART->BAUD |= UART_BAUD_DIV_16_EN;
		divider = divider / 16;
	}

	tUART->BAUD &= ~(UART_BAUD_BRD);
	tUART->BAUD |= divider & UART_BAUD_BRD;

	return E_SUCCESS;
}



/**
 *  @brief The function is used to disable UART clock, disable ISR and clear callback function pointer after checking the TX empty.
 *
 *  @param[in] tUART     Specify UART0/UART1
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h
 *
 *  @return None
 *
 *  @par Example:
 *  @code
 *   //Close UART channel 0
 *   UART_DeInit (UART_PORT0);
 *  @endcode
 *
 */
void UART_DeInit(UART_TypeDef  *tUART)
{
	while(!(tUART->FSR & UART_FSR_TE_F));

	if(tUART == UART1)
	{
		CLK->APBCLK &= ~CLK_APBCLK_UART1_EN;
        NVIC_DisableIRQ (UART1_IRQn); 
	}
	else
	{
		CLK->APBCLK &= ~CLK_APBCLK_UART0_EN;
        NVIC_DisableIRQ(UART0_IRQn); 
	}

}



/**
 *  @brief The function is used to enable specified UART interrupt and enable NVIC UART IRQ.
 *
 *  @param[in] tUART             Specify UART0/UART1
 *  @param[in] u32InterruptFlag  \n
 *                               \b DRVUART_LININT    : LIN Interrupt Enable \n
 *                               \b DRVUART_ABAUDINT  : Auto-Baud Rate Interrupt Enable \n
 *                               \b DRVUART_WAKEINT   : Wakeup Interrupt \n
 *                               \b DRVUART_BUFERRINT : Buffer Error Interrupt Enable \n
 *                               \b DRVUART_TOUTINT   : Time-out Interrupt Enable \n
 *                               \b DRVUART_MOSINT    : MODEM Status Interrupt Enable \n
 *                               \b DRVUART_RLSNT     : Receive Line Status Interrupt Enable \n
 *                               \b DRVUART_THREINT   : Transmit Holding Register Empty Interrupt Enable \n
 *                               \b DRVUART_RDAINT    : Receive Data Available Interrupt Enable 
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h
 *
 *  @return None
 *
 *  @note Use "|" to connect the interrupt flags to enable multiple interrupts simultaneously. \n
 *        If you call the function twice in a project, the settings is dependent on the second setting.
 *
 *  @par Example:
 *  @code
 *   //Enable UART channel 0 RDA and THRE interrupt.
 *   UART_EnableInt(UART_PORT0, (DRVUART_RDAINT | DRVUART_THREINT ) );
 *
 *  @endcode
 */
void UART_EnableInt(UART_TypeDef  *tUART, uint32_t  u32InterruptFlag)
{
	tUART->IER = u32InterruptFlag;

	if (tUART == UART0) 
	{
		NVIC_EnableIRQ(UART0_IRQn); 
	}
	else
	{
		NVIC_EnableIRQ(UART1_IRQn); 
	}
}


/**
 *  @brief The function is used to disable UART specified interrupt and disable NVIC UART IRQ.
 *
 *  @param[in] tUART            Specify UART0/UART1
 *  @param[in] u32InterruptFlag \n
 *                              \b DRVUART_LININT    : LIN Interrupt Enable \n
 *                              \b DRVUART_ABAUDINT  : Auto-Baud Rate Interrupt Enable \n
 *                              \b DRVUART_WAKEINT   : Wakeup Interrupt \n
 *                              \b DRVUART_BUFERRINT : Buffer Error Interrupt Enable \n
 *                              \b DRVUART_TOUTINT   : Time-out Interrupt Enable \n
 *                              \b DRVUART_MOSINT    : MODEM Status Interrupt Enable \n
 *                              \b DRVUART_RLSNT     : Receive Line Status Interrupt Enable \n
 *                              \b DRVUART_THREINT   : Transmit Holding Register Empty Interrupt Enable \n
 *                              \b DRVUART_RDAINT    : Receive Data Available Interrupt Enable 
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h
 *
 *  @return None
 *
 *  @note Use "|" to connect the interrupt flags to disable multiple interrupts simultaneously.
 *
 *  @par Example:
 *  @code
 *   //To disable the THRE interrupt enable flag
 *   UART_DisableInt (UART_PORT0, DRVUART_THREINT);
 *  @endcode
 *
 */
void UART_DisableInt(UART_TypeDef  *tUART, uint32_t u32InterruptFlag)
{
	tUART->IER &= ~u32InterruptFlag;

	if (tUART == UART0)
	{
        NVIC_DisableIRQ(UART0_IRQn);   
	}
	else
	{
       	NVIC_DisableIRQ(UART1_IRQn);    
	}
}


/**
 *  @brief The function is used to clear UART specified interrupt flag
 *
 *  @param[in] tUART            Specify UART0/UART1
 *  @param[in] u32InterruptFlag \n
 *                              \b DRVUART_LININT    : LIN Interrupt Enable \n
 *                              \b DRVUART_ ABAUDINT : Auto-Baud Rate Interrupt Enable \n
 *                              \b DRVUART_WAKEINT   : Wakeup Interrupt \n
 *                              \b DRVUART_BUFERRINT : Buffer Error Interrupt Enable \n
 *                              \b DRVUART_MOSINT    : MODEM Status Interrupt Enable \n
 *                              \b DRVUART_RLSNT     : Receive Line Status Interrupt Enable 
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h
 *
 *  @return E_SUCESS  Success
 *
 *  @par Example:
 *  @code
 *   //To clear UART0 LIN break interrupt flag.
 *   UART_ ClearIntFlag (UART_PORT0, DRVUART_LININT);
 *  @endcode
 *
 */
uint32_t UART_ClearIntFlag(UART_TypeDef  *tUART, uint32_t u32InterruptFlag)
{

	if((u32InterruptFlag & DRVUART_RDAINT) ||
	   (u32InterruptFlag & DRVUART_TOUTINT)||
	   (u32InterruptFlag & DRVUART_THREINT))
	{
	   	return E_DRVUART_ARGUMENT;
	}	   

	if(u32InterruptFlag & DRVUART_RLSINT) /* clear Receive Line Status Interrupt */	   
	{
		tUART->FSR |= UART_FSR_BI_F | UART_FSR_FE_F | UART_FSR_PE_F;
		tUART->TRSR |= UART_TRSR_RS485_ADDET_F;
	}

	if(u32InterruptFlag & DRVUART_MOSINT)  /* clear Modem Interrupt */	   
		tUART->MCSR |= UART_MCSR_DCT_F;

    if(u32InterruptFlag & DRVUART_LININT)  /* clear LIN break Interrupt */	   
    {
		tUART->TRSR |= UART_TRSR_LIN_TX_F | UART_TRSR_LIN_RX_F | UART_TRSR_BIT_ERR_F;
    }

	if(u32InterruptFlag & DRVUART_WAKEUPINT)  /* clear wake up Interrupt */	   
		tUART->ISR |= UART_ISR_WAKE_IS;

	if(u32InterruptFlag & DRVUART_ABAUDINT)  /* clear auto-baud rate Interrupt */
	{
		tUART->TRSR |= UART_TRSR_ABAUD_TOUT_F | UART_TRSR_ABAUD_F;
	}

	if(u32InterruptFlag & DRVUART_BUFERRINT)  /* clear Buffer ErrorInterrupt */	   
	{
		tUART->FSR |= UART_FSR_RX_OVER_F | UART_FSR_TX_OVER_F;
	}
	return E_SUCCESS;
}



/**
 *  @brief The function is used to get the specified UART interrupt status
 *
 *  @param[in] tUART             Specify UART0/UART1
 *  @param[in] u32InterruptFlag  \n
 *                               \b DRVUART_LININT    : LIN Interrupt Enable \n
 *                               \b DRVUART_ABAUDINT  : Auto-Baud Rate Interrupt Enable \n
 *                               \b DRVUART_WAKEINT   : Wakeup Interrupt \n
 *                               \b DRVUART_BUFERRINT : Buffer Error Interrupt Enable \n
 *                               \b DRVUART_TOUTINT   : Time-out Interrupt Enable \n
 *                               \b DRVUART_MOSINT    : MODEM Status Interrupt Enable \n
 *                               \b DRVUART_RLSNT     : Receive Line Status Interrupt Enable \n
 *                               \b DRVUART_THREINT   : Transmit Holding Register Empty Interrupt Enable \n
 *                               \b DRVUART_RDAINT    : Receive Data Available Interrupt Enable 
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h
 *
 *  @return 
 *       0: The specified interrupt did not happen \n
 *       1: The specified interrupt happened \n
 *       E_DRVUART_ARGUMENT: Error Parameter
 *
 *  @note It is recommended to poll one interrupt at a time.
 *
 *  @par Example:
 *  @code
 *   //To get the THRE interrupt enable flag.
 *   If(UART_GetIntStatus (UART_PORT0, DRVUART_THREINT))
 *      printf("THRE INT is happened!\n");
 *   else 
 *      printf("THRE INT is not happened or error parameter\n");
 *  @endcode
 *
 */
int32_t UART_GetIntStatus(UART_TypeDef  *tUART,uint32_t u32InterruptFlag)
{
	switch(u32InterruptFlag)
	{
		case DRVUART_MOSINT:				  		/* MODEM Status Interrupt */
				return ((tUART->ISR & DRVUART_MOSINT)?1:0) ;
		
		case DRVUART_RLSINT:						/* Receive Line Status Interrupt */
				return ((tUART->ISR & DRVUART_RLSINT)?1:0);

		case DRVUART_THREINT:						/* Transmit Holding Register Empty Interrupt */
				return ((tUART->ISR & DRVUART_THREINT)?1:0);

		case DRVUART_RDAINT:						/* Receive Data Available Interrupt */
				return ((tUART->ISR & DRVUART_RDAINT)?1:0);

		case DRVUART_TOUTINT:						/* Time-out Interrupt */
				return ((tUART->ISR & DRVUART_TOUTINT)?1:0);

		case DRVUART_LININT:						/* LIN Break Interrupt */
				return ((tUART->ISR & DRVUART_LININT)?1:0);

		case DRVUART_BUFERRINT:						/* Buffer Error Interrupt */
				return ((tUART->ISR & DRVUART_BUFERRINT)?1:0);

		case DRVUART_ABAUDINT:						/* Auto-baud rate Interrupt */
				return ((tUART->ISR & DRVUART_ABAUDINT)?1:0);

		case DRVUART_WAKEUPINT:					/* Wake upInterrupt */
				return ((tUART->ISR & DRVUART_WAKEUPINT)?1:0);
		
		default:
			return E_DRVUART_ARGUMENT;
	}
	
}


/**
 *  @brief The function is used to get CTS pin value and detect CTS change state
 *
 *  @param[in]  tUART             Specify UART0/UART1
 *  @param[out] pu8CTSValue       Specify the buffer to receive the CTS value.Retrun current CTS pin state
 *  @param[out] pu8CTSChangeState Specify the buffer to receive the CTS change state. 
 *                                Return CTS pin state is changed or not.
 *                                1 means changed and 0 means not yet.
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h
 *
 *  @return None
 *
 *  @par Example:
 *  @code
 *  //To get CTS pin status and save to u8CTS_value. To get detect CTS change flag and save to u8CTS_state.
 *  uint8_t u8CTS_value, u8CTS_state;
 *  UART_GetCTSInfo(UART_PORT1,& u8CTS_value,& u8CTS_state);
 *  @endcode
 *
 */
void DrvUART_GetCTSInfo(UART_TypeDef  *tUART, uint8_t *pu8CTSValue, uint8_t	*pu8CTSChangeState)
{
	*pu8CTSValue 		= (tUART->MCSR & UART_MCSR_CTS_ST)?1:0 ;
	*pu8CTSChangeState 	= (tUART->MCSR & UART_MCSR_DCT_F)?1:0;
}	

 
 /**
  *  @brief The function is used to read Rx data from RX FIFO and the data will be stored in pu8RxBuf.
  *
  *  @param[in]  tUART        Specify UART0/UART1
  *
  *  @param[out] pu8RxBuf	  Specify the buffer to receive the data of receive FIFO.
  *
  *  @param[in]  u32ReadBytes Specify the read bytes number of data.
  *
  *  @par Include:
  *       nano1xx_drivers/nano1xx_uart.h
  *
  *  @return 
  * 		 E_SUCCESS: Success.\n
  * 		 E_DRVUART_TIMEOUT: FIFO polling timeout.
  *
  *  @par Example:
  *  @code
  *  //Condition: Read RX FIFO 1 byte and store in bInChar buffer. 
  *  uint8_t bInChar[1];
  *  UART_Read(UART_PORT0, bInChar,1);
  *  @endcode
  *
  */
int32_t UART_Read(UART_TypeDef  *tUART, uint8_t *pu8RxBuf, uint32_t u32ReadBytes)
{
    uint32_t  u32Count, u32delayno;

    for (u32Count=0; u32Count < u32ReadBytes; u32Count++)
    {
	     u32delayno = 0;

		 while (tUART->FSR & UART_FSR_RX_EMPTY_F) /* Check RX empty => failed */	
	     {
	     	 u32delayno++;        
	        if ( u32delayno >= 0x40000000 )        
	        	return E_DRVUART_ERR_TIMEOUT;               
            }
            pu8RxBuf[u32Count] = tUART->RBR;					    /* Get Data from UART RX  */
    }

    return E_SUCCESS;
	
}



/**
 *  @brief The function is to write data into TX buffer to transmit data by UART
 *
 *  @param[in] tUART         Specify UART0/UART1
 *  @param[in] pu8TxBuf      Specify the buffer to send the data to UART transmission FIFO.
 *  @param[in] u32WriteBytes Specify the byte number of data.
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h
 *
 *  @return 
 *          E_SUCCESS: Success
 *          E_DRVUART_TIMEOUT: FIFO polling timeout
 *
 *  @par Example:
 *  @code
 *  //Condition: Send 1 byte from bInChar buffer to TX FIFO.
 *  uint8_t bInChar[1] = 0x55;
 *  UART_Write(UART_PORT0, bInChar,1);
 *  @endcode
 *
 */
int32_t UART_Write(UART_TypeDef  *tUART, uint8_t *pu8TxBuf, uint32_t u32WriteBytes)
{
    uint32_t  u32Count, u32delayno;

    for (u32Count=0; u32Count<u32WriteBytes; u32Count++)
    {
        u32delayno = 0;

		while (!(tUART->FSR & UART_FSR_TX_EMPTY_F))  /* Wait Tx empty and Time-out manner */
        {
       	    u32delayno++;
       	    if ( u32delayno >= 0x40000000 )     	    
       	       return E_DRVUART_ERR_TIMEOUT;     	       
				   
        }
	   	tUART->THR = pu8TxBuf[u32Count]; /* Send UART Data from buffer */
    }

    return E_SUCCESS;
	
  
}


/**
 *  @brief The function is used to configure IRDA relative settings. It consists of TX or RX mode and Inverse TX or RX signals.
 *
 *  @param[in] tUART    Specify UART0/UART1
 *  @param[in] TxXSelect	Set IrDA TX mode or Rx mode\n
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h 
 *
 *  @return None
 *           
 *  @note Before using the API, you should configure UART setting firstly.
 *        And make sure the baud-rate setting is used mode 0 (UART divider is 16)in baud-rate configure.
 *
 *  @par Example:
 *  @code
 *  //Set UART1 to IRDA function and select TX mode. 
 *  UART_SetFnIRDA(UART_PORT1,IrDA_TXSelect);
 *  @endcode
 *
 */
void UART_SetFnIRDA(UART_TypeDef  *tUART, uint8_t TxXSelect)
{
	/* Enable IrDA function and configure */
	tUART->IRCR	&=  ~UART_IRCR_INV_TX;
	tUART->IRCR |= 	UART_IRCR_INV_RX;
	tUART->IRCR	= TxXSelect ? tUART->IRCR | UART_IRCR_TX_SELECT:tUART->IRCR &~ UART_IRCR_TX_SELECT;
	tUART->FUN_SEL = FUN_IRDA;	
}


/**
 *  @brief The function is used to set LIN relative setting
 *
 *  @param[in] tUART          Specify UART0/UART1
 *  @param[in] u16Mode        Specify LIN direction : MODE_TX and/or MODE_RX
 *  @param[in] u16BreakLength Specify break count value. It should be larger than 13 bit time according LIN protocol.
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h  
 *
 *  @return None
 *
 *  @par EXample:
 *  @code
 *  //Change UART1 to LIN function and set to transmit the header information.
 *  UART_SetFnLIN(uart_ch,MODE_TX | MODE_RX,13);
 *  @endcode
 *
 */
void UART_SetFnLIN(UART_TypeDef  *tUART, uint16_t u16Mode, uint16_t u16BreakLength)
{
	tUART->FUN_SEL = FUN_LIN;
	tUART->ALT_CTL &= ~(UART_ALT_CTL_LIN_TX_BCNT | UART_ALT_CTL_LIN_RX_EN | UART_ALT_CTL_LIN_TX_EN);
	tUART->ALT_CTL |= u16BreakLength & UART_ALT_CTL_LIN_TX_BCNT;
	tUART->ALT_CTL |= (u16Mode & MODE_TX) ? UART_ALT_CTL_LIN_TX_EN:0;
	tUART->ALT_CTL |= (u16Mode & MODE_RX) ? UART_ALT_CTL_LIN_RX_EN:0;

}


/**
 *  @brief The function is used to set RS485 relative setting
 *
 *  @param[in] tUART     Specify UART0/UART1
 *  @param[in] str_RS485
 *                       The structure of RS485 \n
 *                       It includes of \n
 *                       - \b u8cModeSelect: Select operation mode \n
 *                       -- MODE_RS485_NMM: RS-485 Normal Multi-drop Mode \n
 *                       -- MODE_RS485_AAD: RS-485 Auto Address Detection Mode \n
 *                       -- MODE_RS485_AUD: RS-485 Auto Direction Mode \n
 *                       - \b u8cAddrEnable: Enable or Disable RS-485 Address Detection 
 *                       - \b u8cAddrValue: Set Address match value
 *                       - \b u8cDelayTime: Set transmit delay time value
 *                       - \b u8cRxDisable: Enable or Disable receiver function.
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h
 *
 *  @return None
 *
 *  @par Example:
 *  @code
 *  //Condition: Change UART1 to RS485 function. Set relative setting as below.
 *  STR_RS485_T sParam_RS485;
 *  sParam_RS485.u8cAddrEnable 	= ENABLE; 
 *  sParam_RS485.u8cAddrValue  	= 0xC0;	 //Address 
 *  sParam_RS485.u8cModeSelect 	= MODE_RS485_AAD|MODE_RS485_AUD;
 *  sParam_RS485.u8cDelayTime  	= 0;
 *  sParam_RS485.u8cRxDisable  	= TRUE;
 *  UART_SetFnRS485(UART_PORT1,&sParam_RS485);
 *  @endcode
 *
 */
void UART_SetFnRS485(UART_TypeDef  *tUART, STR_RS485_T *str_RS485)
{
	tUART->FUN_SEL = FUN_RS485;

	tUART->ALT_CTL |= ((str_RS485->u8cAddrValue << 24) |
					   (str_RS485->u8cAddrEnable << 19) |
					   (str_RS485->u8cModeSelect  << 16));
					     
	tUART->TMCTL &= ~UART_TMCTL_DLY;
	tUART->TMCTL |= (str_RS485->u8cDelayTime << 16) & UART_TMCTL_DLY;
	tUART->CTL &= ~UART_CTL_RX_DIS;
	tUART->CTL |= (str_RS485->u8cRxDisable) ? UART_CTL_RX_DIS:0;
} 

/*@}*/ /* end of group NANO1xx_UART_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_UART_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/



