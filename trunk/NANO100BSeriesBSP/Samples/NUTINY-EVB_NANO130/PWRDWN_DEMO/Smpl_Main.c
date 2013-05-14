/******************************************************************************
 * @file     Smpl_Main.c
 * @brief    Nano1xx NUTINY-EVB_NANO130 Power Down Demo code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "nano1xx.h"
#include "nano1xx_lcd.h"
#include "nano1xx_sys.h"
#include "nano1xx_rtc.h"

#ifdef __DEBUG_MSG
#define DEBUG_MSG 	printf 
#else
#define DEBUG_MSG(...)
#endif


/* External functions */
void textticker(char *string, uint32_t delayus);
void showTime(uint32_t hour, uint32_t minute);
void planNextRTCInterrupt(S_DRVRTC_TIME_DATA_T *sCurTime);

/* Global variables */
__IO int32_t   _Wakeup_Flag = 0;	/* 1 indicates system wake up from power down mode */
__IO uint32_t  _Pin_Setting[11];	/* store Px_H_MFP and Px_L_MFP */
__IO uint32_t  _PullUp_Setting[6];	/* store GPIOx_PUEN */



/**
  * @brief  Store original setting of multi-function pin selection. 
  * @param  None.
  * @return None.
  */
void SavePinSetting()
{
  /* Save Pin selection setting */
  _Pin_Setting[0] = GCR->PA_L_MFP;
  _Pin_Setting[1] = GCR->PA_H_MFP;
  _Pin_Setting[2] = GCR->PB_L_MFP;
  _Pin_Setting[3] = GCR->PB_H_MFP;
  _Pin_Setting[4] = GCR->PC_L_MFP;
  _Pin_Setting[5] = GCR->PC_H_MFP;
  _Pin_Setting[6] = GCR->PD_L_MFP;
  _Pin_Setting[7] = GCR->PD_H_MFP;
  _Pin_Setting[8] = GCR->PE_L_MFP;
  _Pin_Setting[9] = GCR->PE_H_MFP;
  _Pin_Setting[10] = GCR->PF_L_MFP;
  
  /* Save Pull-up setting */
  _PullUp_Setting[0] =  GPIOA->PUEN;
  _PullUp_Setting[1] =  GPIOB->PUEN;
  _PullUp_Setting[2] =  GPIOC->PUEN;
  _PullUp_Setting[3] =  GPIOD->PUEN;
  _PullUp_Setting[4] =  GPIOE->PUEN;
  _PullUp_Setting[5] =  GPIOF->PUEN;
}


/**
  * @brief  Restore original setting of multi-function pin selection. 
  * @param  None.
  * @return None.
  */
void RestorePinSetting()
{
  /* Restore Pin selection setting */
  GCR->PA_L_MFP = _Pin_Setting[0];
  GCR->PA_H_MFP = _Pin_Setting[1];
  GCR->PB_L_MFP = _Pin_Setting[2];
  GCR->PB_H_MFP = _Pin_Setting[3];
  GCR->PC_L_MFP = _Pin_Setting[4];
  GCR->PC_H_MFP = _Pin_Setting[5];
  GCR->PD_L_MFP = _Pin_Setting[6];
  GCR->PD_H_MFP = _Pin_Setting[7];
  GCR->PE_L_MFP = _Pin_Setting[8];
  GCR->PE_H_MFP = _Pin_Setting[9];
  GCR->PF_L_MFP = _Pin_Setting[10];
  
  /* Rstore Pull-up setting */
  GPIOA->PUEN = _PullUp_Setting[0];
  GPIOB->PUEN = _PullUp_Setting[1];
  GPIOC->PUEN = _PullUp_Setting[2];
  GPIOD->PUEN = _PullUp_Setting[3];
  GPIOE->PUEN = _PullUp_Setting[4];
  GPIOF->PUEN = _PullUp_Setting[5];
}


/**
  * @brief  Save multi-functon pin setting and then go to power down. 
  * @param  None.
  * @return None.
  */
void Enter_PowerDown()
{
  /* Back up original setting */
  SavePinSetting();
  
  /* Set function pin to GPIO mode */
  GCR->PA_L_MFP = 0;
  GCR->PA_H_MFP = 0;
  GCR->PB_L_MFP = 0;
  GCR->PB_H_MFP = 0;
  GCR->PC_L_MFP = 0;
  GCR->PC_H_MFP = 0;
  GCR->PD_L_MFP = 0;
  GCR->PD_H_MFP = 0;
  GCR->PE_L_MFP = 0;
  GCR->PE_H_MFP = 0;
  GCR->PF_L_MFP = 0x00007700;

  /* Enable GPIO pull up */
  GPIOA->PUEN = 0xFFFF;
  GPIOB->PUEN = 0xFFFF;
  GPIOC->PUEN = 0xFFFF;
  GPIOD->PUEN = 0xFFFF;
  GPIOE->PUEN = 0xFFFF;
  GPIOF->PUEN = 0x0033;	  /* exclude GPF2 and GPF3 which are HXT OUT/IN */
  
  /* Disable LCD clock */
  SYS_SetIPClock_2(CLK_APBCLK_LCD_EN, 0);  /* Disable LCD clock */
  SYS_SetChipClockSrc(CLK_PWRCTL_LXT_EN, 0); /* disable LXT - 32Khz */

  SYS_SetUpPowerDown(0);	/* Disable PDWU interrupt */
  __WFI();	 /* system really enter power down here ! */
}


/**
  * @brief  This function will not be called.
  * @param  None.
  * @return None.
  */
void Leave_PowerDown()
{

  /* Restore pin setting */
  RestorePinSetting();

  /* Set PF.0 and PF.1 to ICE Data and Clock */
  GCR->PF_L_MFP |= 0x00000077;	 	

  /* Enable LCD clock */
  SYS_SetIPClock_2(CLK_APBCLK_LCD_EN, 1);
}


/**
  * @brief  Main routine. 
  * @param  None.
  * @return None.
  */
int32_t main(void)							   
{
	S_LCD_INIT lcdinit;
	S_DRVRTC_TIME_DATA_T sCurTime;

	DEBUG_MSG("\nNANO130 NuTiny EVB Demo Program V1.0.0\n");
	DEBUG_MSG("[Power Down Demo]\n"); 

	/* Unlock protected registers */
	UNLOCKREG();
	
	/* Init RTC */
	RTC_Init();	
	DEBUG_MSG("RTC Init. complete!\n");

	/* Init LCD */
	SYS_SelectIPClockSource_1(CLK_CLKSEL1_LCD_MASK, CLK_CLKSEL1_LCD_LXT); /* Select LCD Clock Source From 32KHz */
	
	/* Select LCD COMs, SEGs, V1 ~ V3, DH1, DH2 */
	MFP_LCD_TYPEA();		
	
	/* Do LCD Initializaton */
	lcdinit.cpump_enable = true;
	lcdinit.internal_bias = false;
	lcdinit.cpump_freqdiv = LCD_CPUMP_DIV1;
	lcdinit.cpump_voltage = LCD_CPVOl_3V;
	lcdinit.bias = LCD_BIAS_THIRD;
	lcdinit.mux = LCD_MUX_ONE_FOURTH;
	lcdinit.freqdiv = LCD_FREQ_DIV64;
	LCD_Init(&lcdinit);

	DEBUG_MSG("LCD Init. complete!\n");

	/* Start displaying on LCD */
	LCD_EnableSegment(3, 22);
	SYS_Delay(335000);

	textticker("*** NUVOTON NANO130 ***", 335000);
	SYS_Delay(335000);

	LCD_Write("NUVOTON");
	LCD_BlinkFrequency(250);
	LCD_EnableBlink();
	SYS_Delay(335000);
	SYS_Delay(335000);
	SYS_Delay(335000);
	SYS_Delay(335000);
	SYS_Delay(335000);
	LCD_DisableBlink();
	
	/* Read curent RTC time */		
	RTC_Read(&sCurTime);
	DEBUG_MSG("Current Time:%d/%02d/%02d %02d:%02d:%02d\n",sCurTime.u32Year,sCurTime.u32cMonth,sCurTime.u32cDay,sCurTime.u32cHour,sCurTime.u32cMinute,sCurTime.u32cSecond);
	
	/* Display RTC time */			
	showTime(sCurTime.u32cHour, sCurTime.u32cMinute);

	DEBUG_MSG("Start MAIN loop.\n");
	while(1) {
		textticker("*** GOING TO POWER DOWN ***", 335000);
		DEBUG_MSG("Going to Power Down...\n");
#ifdef __DEBUG_MSG
		while(!(UART0->FSR & UART_FSR_TE_F)) ;  /* waits for message send out */
#endif
		/* Enter power down mode */
		Enter_PowerDown();
				
		DEBUG_MSG("Program resume...\n");	
			
		if (_Wakeup_Flag == 1) 
		{
		   _Wakeup_Flag = 0;
		   
		   textticker("*** WAKE UP ***", 335000);
		   SYS_Delay(335000);
		   SYS_Delay(335000);
		   SYS_Delay(335000);
		}
	};  


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
 	MFP_UART0_TO_PORTA();				   /* UART0 TX/RX to PA14/PA15*/
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


	 
