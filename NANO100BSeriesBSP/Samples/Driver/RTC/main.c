/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx RTC Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>

#include "nano1xx.h"
#include "nano1xx_rtc.h"
#include "nano1xx_gpio.h"
#include "nano1xx_uart.h"
#include "nano1xx_sys.h"


/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

volatile uint32_t  g_u32TICK = false;
volatile int32_t   g_bAlarm  = false;
volatile int32_t   g_bSnooper  = false;

#if 1
/*---------------------------------------------------------------------------------------------------------*/
/* RTC Tick Handle                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void RTC_TickHandle(void)
{
	S_DRVRTC_TIME_DATA_T sCurTime;
	
	/* Get the currnet time */
	RTC_Read(&sCurTime);
	
	printf("Current Time:%d/%02d/%02d %02d:%02d:%02d\n",sCurTime.u32Year,sCurTime.u32cMonth,sCurTime.u32cDay,sCurTime.u32cHour,sCurTime.u32cMinute,sCurTime.u32cSecond);				
	
	g_u32TICK++;
}
#endif


#if 1
/*---------------------------------------------------------------------------------------------------------*/
/* RTC Alarm Handle                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void RTC_AlarmHandle(void)
{
	
	printf("Alarm!!\n");
	
	g_bAlarm = true; 
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* RTC Snooper Handle                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void RTC_SnooperHandle(void)
{
	
	printf("Had Detected Snooper Event!!\n");
	
	g_bSnooper = true; 

}


static void TestItem (void)
{
    printf("\n\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|                    RTC Sample Program                     |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| [0] Time Display Test                                     |\n");
    printf("| [1] Alarm Test                                            |\n");
	printf("| [2] Snooper Detection Test                                |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| [3] Quit                                                  |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("Select key : \n");
}

void Delay(uint32_t ucnt)
{
    volatile uint32_t i = ucnt;

    while (i--);
}




/*---------------------------------------------------------------------------------------------------------*/
/* RTC Test Sample 				                                                                           */
/* Test Item					                                                                           */
/*  1. Time Display Test																				   */	
/*	   Use RTC Tick interrupt to display time every one second.                      					   */
/*  2. Alarm Test																						   */
/*     Get the current and alarm after 10 seconds														   */	
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)							   
{
	//STR_UART_T param;		
	S_DRVRTC_TIME_DATA_T sInitTime;
	int32_t bLoop = true;
	uint32_t edge;
	uint8_t u8Item, num;
	
	/* Enable HXT and LXT */
	SYS_SetChipClockSrc((CLK_PWRCTL_HXT_EN | CLK_PWRCTL_LXT_EN), 1);

	// Wait HXT and LXT stable
	while(SYS_CheckChipClockSrc(CLK_CLKSTATUS_LXT_STB | CLK_CLKSTATUS_HXT_STB) != 0) ;

	/* Select Snooper Pin to detect */
	((uint32_t)(GCR->PB_H_MFP = GCR->PB_H_MFP & ~PB15_MFP_SNOOPER | PB15_MFP_SNOOPER));
	//((uint32_t)(GCR->PC_H_MFP = GCR->PC_H_MFP & ~PC13_MFP_SNOOPER | PC13_MFP_SNOOPER))
	
	{		
		/* RTC Initialize */
		RTC_Init();	
		
		/* Time Setting */
		sInitTime.u32Year 		= 2012;
		sInitTime.u32cMonth 	= 7;
		sInitTime.u32cDay 		= 1;
		sInitTime.u32cHour 		= 12;
		sInitTime.u32cMinute 	= 30;
		sInitTime.u32cSecond 	= 0;
		sInitTime.u32cDayOfWeek = DRVRTC_TUESDAY;
		sInitTime.u8cClockDisplay = DRVRTC_CLOCK_24;

		/* Initialization the RTC timer */
		if(RTC_Open(&sInitTime) !=E_SUCCESS)
		{
			printf("RTC Open Fail!!\n");
			return false;
		}		
		
		while(bLoop)
		{
			TestItem();
			u8Item = getchar();
			
			switch(u8Item)
			{
				case '0':
				{

					printf("\n0. RTC Time Display Test (Exit after 5 seconds)\n");
					/* Set Tick setting */
					RTC_SetTickMode(DRVRTC_TICK_1_SEC);

					/* Enable RTC Tick Interrupt */
					RTC_EnableInt(RTC_RIER_TIER);	
					
					g_u32TICK = 0;
					while(g_u32TICK < 5);

					/* Disable RTC Tick Interrupt */
					RTC_DisableInt(RTC_RIER_TIER);	

					break;
				}
				case '1':
				{
					S_DRVRTC_TIME_DATA_T sCurTime;	
					
					printf("\n1. RTC Alarm Test (Alarm after 10 seconds)\n");
						
					g_bAlarm = false;	
				
					/* Get the currnet time */
					//RTC_Read(DRVRTC_CURRENT_TIME, &sCurTime);
					RTC_Read(&sCurTime);
				
					printf("Current Time:%d/%02d/%02d %02d:%02d:%02d\n",sCurTime.u32Year,sCurTime.u32cMonth,sCurTime.u32cDay,sCurTime.u32cHour,sCurTime.u32cMinute,sCurTime.u32cSecond);
					
					/* The alarm time setting */	
					sCurTime.u32cSecond = sCurTime.u32cSecond + 10; 	
					
					/* Set the alarm time (Install the call back function and enable the alarm interrupt)*/
					//RTC_Write(DRVRTC_ALARM_TIME, &sCurTime);
					RTC_WriteAlarm(&sCurTime);
							
					/* Enable RTC Alarm Interrupt */
					RTC_EnableInt(RTC_RIER_AIER);
					
					while(!g_bAlarm);
					
					printf("Current Time:%d/%02d/%02d %02d:%02d:%02d\n",sCurTime.u32Year,sCurTime.u32cMonth,sCurTime.u32cDay,sCurTime.u32cHour,sCurTime.u32cMinute,sCurTime.u32cSecond);

					/* Disable RTC Alarm Interrupt */
					RTC_DisableInt(RTC_RIER_AIER);	
								
					break;				
				}
				case '2':
				{					
					g_bSnooper = false;	

					printf("Select detection edge:\n");
					printf("[0] Rising Edge \n");
					printf("[1] Falling Edge \n");
					printf("Select: ");
					edge = getchar();
					printf("%c\n", edge);
					if(edge == '0')
						edge = DRVRTC_SNOOPER_RISING;
					else if(edge == '1')
						edge = DRVRTC_SNOOPER_FALLING;
					else
						break;					

					printf("Please connect GPB[15] snooper pin to test signal...\n");
					printf("Any key to continue the test...");
					getchar();
					printf("\n");

					/* write */
					for(num=0; num<20; num++)
						RTC_SpareRegsAccess(num, rand(), 1);
					
					/* Set the configuration of spare function */
					RTC_EnableSpareFunc(edge);

					/* Enable RTC Snooper Interrupt */
					RTC_EnableInt(RTC_RIER_SNOOPIER);

					// TODO: toggle the test pin to generate a interrupt
					// ...
					
					while(!g_bSnooper);

					/* check */
					for(num=0; num<20; num++) {
						if( 0 != RTC_SpareRegsAccess(num, 0, 0) )
							printf("Error! \n");
					}
					
					/* Disable RTC Snooper Interrupt */
					RTC_DisableInt(RTC_RIER_SNOOPIER);	

					/* Disable RTC Snooper Detection */
					RTC_DisableSpareFunc();
								
					break;				
				}
				case '3':
					bLoop = false;
					break;
				default:
					printf("Wrong Item\n");
					break;			
			}
			
		}
		
		/* Disable RTC Clock */		
		RTC_Close();
		
		
		return true;
	}

}


#ifdef USE_ASSERT
/**
  * @brief  The function prints the source file name and line number where the assert_param() error
  *         occurs, and then stops in an infinite loop. User can add his own codes here if necessary.
  * @param[in] file Source file name
  * @param[in] line Line number
  * @return None
  */
void assert_error(uint8_t * file, uint32_t line)
{
 	GCR->PB_L_MFP = (GCR->PB_L_MFP & ~0x77) | (PB1_MFP_UART0_TX | PB0_MFP_UART0_RX);  /* Select multi-function pin for UART0 */
 	CLK->APBCLK |= CLK_APBCLK_UART0_EN;    /* Enable UART0 clock */
	CLK->CLKDIV0 &= ~CLK_CLKDIV0_UART_MASK;
	CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UART_MASK) | CLK_CLKSEL1_UART_HXT;  /* Select 12 Mhz XTAL */

 
    /* Set UART to 115200,n,8,1,none */
    UART0->BAUD = 0x67;             /* Baud Rate:115200 for 12MHz */
    UART0->TLCTL = 0x03;            /* Word len is 8 bits */          
	
	printf("[%s] line %d : wrong parameters.\r\n", file, line);

	/* Infinite loop */
	while(1) ;

}
#endif



/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


	 
