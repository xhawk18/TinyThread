/******************************************************************************
 * @file     Timer_WDT_RTC.c
 * @brief    Nano1xx Timer/WDT/RTC sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/  
#include <stdio.h>	
#include <string.h>																										 
#include "nano1xx.h"
#include "nano1xx_sys.h"
#include "nano1xx_ebi.h"
#include "EBI_LCM_OTM3225A.h"
#include "assert.h"

extern void EBI_LCM_Initial(void);


//---------------------------------------------------------------------------------WDT
void InitWDT(void)
{
	UNLOCKREG();		
	/* Enable WDT clock */         	
	CLK->APBCLK |= CLK_APBCLK_WDT_EN;//Enable WDT clock source	

	/* Select Timeout Interval */
	WDT->CTL = WDT_CTL_WTIS_2POW16; // Timeout about 6.6 sec, disable Watchdog Timer reset function

	/* Enable WDT interrupt */	
	WDT->ISR = WDT->ISR;//Write 1 to clear for safety		
	WDT->IER = WDT_IER_WDTIE;
	NVIC_EnableIRQ(WDT_IRQn);

	//Enable WDT and clear WDT counter
	WDT->CTL |= WDT_CTL_WTR | WDT_CTL_WTE;
	
	LOCKREG();	 
}

//---------------------------------------------------------------------------------RTC
void set_TLR (int32_t a,int32_t b,int32_t c,int32_t d,int32_t e,int32_t f)
{
	outpw(&RTC->TLR, a<<20|b<<16|c<<12|d<<8|e<<4|f)	 ;
}
void set_CLR (int32_t a,int32_t b,int32_t c,int32_t d,int32_t e,int32_t f)
{
	outpw(&RTC->CLR, a<<20|b<<16|c<<12|d<<8|e<<4|f)	 ;
}
void set_TAR(int32_t a,int32_t b,int32_t c,int32_t d,int32_t e,int32_t f)
{
	outpw(&RTC->TAR, a<<20|b<<16|c<<12|d<<8|e<<4|f)	;
}
void set_CAR (int32_t a,int32_t b,int32_t c,int32_t d,int32_t e,int32_t f)
{
	outpw(&RTC->CAR, a<<20|b<<16|c<<12|d<<8|e<<4|f)	;
}
void START_RTC(void)
{

  	while(1)
  	{
		RTC->INIR = 0xa5eb1357;
		if(inpw(&RTC->INIR)==1)
        	break; 
  	}   
  	while(1)
  	{
  		RTC->AER = (RTC->AER & ~RTC_AER_AER) | 0xA965;
    	if(RTC->AER & RTC_AER_ENF)
        	break;  
    }
}
void InitRTC(void)
{
	UNLOCKREG();		
	/* Step 1. Enable and RTC clock */     
	CLK->APBCLK = CLK_APBCLK_RTC_EN;//Enable RTC clock source	

	/* Step 2. Initiate and unlock RTC module */
	 
	START_RTC();

	/* Step 3. Initiate Time and Calendar  setting */
	RTC->TSSR |= RTC_TSSR_24hr;//Set 24hour mode
	//Set time and calendar for loading register, Calendar„³ 09/1/19, Time„³ 13:20:00
	set_CLR(0,9,0,1,1,9);
	set_TLR(1,3,2,0,0,0);

	/* Step 4. Set alarm interrupt */
	//Set time and calendar for alarm register , Calendar„³ 09/1/19, Time„³ 13:20:20
	set_CAR(0,9,0,1,1,9);
	set_TAR(1,3,2,0,2,0);		
	//Enable alarm interrupt
	RTC->RIER = RTC_RIER_AIER | RTC_RIER_TIER;
	RTC->RIIR = RTC->RIIR;
	NVIC_EnableIRQ(RTC_IRQn);
}


//---------------------------------------------------------------------------------TIMER
void InitTIMER0(void)
{
	/* Step 1. Enable and Select Timer clock source */          
	CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_TMR0_MASK) | CLK_CLKSEL1_TMR0_HXT;	//Select 12Mhz for Timer0 clock source 
	CLK->APBCLK |= CLK_APBCLK_TMR0_EN;	//Enable Timer0 clock source

	/* Step 2. Select Operation mode */	
	TIMER0->CTL = (TIMER0->CTL & ~TIMER_CTL_MODESEL_MASK) | TIMER_CTL_MODESEL_PERIODIC; //Select periodic mode for operation mode

	/* Step 3. Select Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)*/
	TIMER0->PRECNT=255;	// Set Prescale [0~255]
	TIMER0->CMPR = 46875;		// Set TCMPR [0~16777215]
								// (1/12000000)*(255+1)*(46875) = 1HZ

	/* Step 4. Enable interrupt */
	TIMER0->ISR = TIMER0->ISR;		//Write 1 to clear for safty
	TIMER0->IER = TIMER_IER_TMRIE;			
	NVIC_EnableIRQ(TMR0_IRQn);	//Enable Timer0 Interrupt

	/* Step 5. Enable Timer module */
	TIMER0->CTL |= TIMER_CTL_TMREN;		//Enable Timer0

}




/*----------------------------------------------------------------------------
  MAIN function
  ----------------------------------------------------------------------------*/
int32_t main (void)
{
	UNLOCKREG();
	CLK->PWRCTL |= CLK_PWRCTL_LXT_EN; //Enable 32Khz for RTC clock source

	while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_LXT_STB));

	LOCKREG();

	/* initailize EBI and pins */
	EBI_LCM_Initial();

	/* panel initialization */
	OTM3225_init(RES_LANDSCAPE_320X240);
	ClearScreen(0x0000);
	Show_String32x32(1, 1, "Nuvoton", 0xf800);
	Show_String32x32(1, 33, "Timer:-", 0xf800);
	Show_String32x32(1, 65, "--:--:--", 0xf800);                        
	InitTIMER0();
	InitRTC();
	InitWDT();
		 	
	while(1);
}

#ifdef USE_ASSERT
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     assert_error                                                                              */
/*                                                                                                         */
/* Parameter:                                                                                              */   
/*               file           -[in] source file name                                                     */
/*               line           -[in] line number                                                          */
/* Returns:                                                                                                */
/*               None.                                                                                     */
/* Description:                                                                                            */
/*               The function prints the source file name and line number where the assert_param() error   */
/*               occurs, and then stops in an infinite loop.                                               */
/*               User can add his own codes here if necessary.                                             */
/*---------------------------------------------------------------------------------------------------------*/

void assert_error(uint8_t * file, uint32_t line)
{
	printf("[%s] line %d : wrong parameters.\r\n", file, line);

	/* Infinite loop */
	while(1) ;

}
#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


