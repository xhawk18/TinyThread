/******************************************************************************
 * @file     TK_LCD_main.c
 * @brief    Nano1xx TK_LCD Driver Sample code
 * @version  1.0.2
 * @date     08, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "nano1xx.h"
#include "nano1xx_tk.h"
#include "nano1xx_lcd.h"
#include "nano1xx_gpio.h"
#include "nano1xx_uart.h"
#include "nano1xx_sys.h"
#include "nano1xx_pwm.h"
#include "nano1xx_i2c.h"
#include "nano1xx_rtc.h"
#include "libtk.h"

void showSlider(uint8_t tkch);
void textticker(char *string, uint32_t delayus);
void showTime(uint32_t hour, uint32_t minute);
void ControlSegment(bool onoff, int com, int seg);
void showText(uint32_t hex);
void planNextRTCInterrupt(S_DRVRTC_TIME_DATA_T *sCurTime);



// blue notes
const uint16_t note[] = {185, 196, 233, 261, 311, 349, 370, 392, 466};

uint8_t mute = 0;
uint16_t g_u16Frequency;
uint8_t volatile complete = 0;


uint32_t sysGetNum(void)
{
    uint8_t cInputTemp=0x00, InputString[16]={0};
    uint32_t nLoop = 0;
    while(cInputTemp != 0x0D)
    {
        cInputTemp = getchar();
        if(cInputTemp == 27)
        {
            return cInputTemp;
        }
        if(cInputTemp == 'x' || cInputTemp == 'X' || cInputTemp == 'f'||
        cInputTemp == 'F' || cInputTemp == 'r' || cInputTemp == 'R')
        {
            return cInputTemp;
        }
        if(cInputTemp == '-')
        {
            InputString[nLoop] = cInputTemp;
            printf("%c",cInputTemp);
            nLoop++;
        }
        else if(cInputTemp >= '0' && cInputTemp <= '9')
        {
            InputString[nLoop] = cInputTemp;
            printf("%c",cInputTemp);
            nLoop++;
        }
    }
	return atoi((const char *)InputString);
}

void key_callback(uint16_t status, uint16_t param)
{

	printf("Key %d status :%s\n", param, status == 0 ? "on" : "off");
	if(param == 0 && status == 0) {
		mute = 0;
		//PWM_Enable(4);
	}
	if(param == 1 && status == 0) {
		mute = 1;
		//PWM_Disable(4);
		PWM_SetCounter(4, 0, 0);
	}

	return;

}

void slider_callback(uint16_t status, uint16_t param)
{
	S_DRVPWM_TIME_DATA_T sPt;

	sPt.u8Mode = PWM_CTL_CHMOD_AUTO_RELOAD;
	sPt.u8Inv = 1;
	
	if(status == 0xFFFF) {
		printf("slider status :off\n");\
		PWM_SetCounter(4, 0, 0);
		
		LCD_DisableSegment(3, 36);	// PL
		LCD_DisableSegment(2, 36);	// P0
		LCD_DisableSegment(1, 36);	// P1
		LCD_DisableSegment(0, 36);	// P2
		LCD_DisableSegment(0, 37);	// P3
		LCD_DisableSegment(1, 37);	// P4
		LCD_DisableSegment(2, 37);	// P5
		LCD_DisableSegment(3, 37);	// P6
		LCD_DisableSegment(3, 38);	// P7
		LCD_DisableSegment(2, 38);	// P8
		LCD_DisableSegment(1, 38);	// P9
		LCD_DisableSegment(0, 38);	// PR

		return ;
	}
	else {
		printf("slider status :%d\n", status);
		
		g_u16Frequency =  note[status];
		showSlider(status);
	}
	if(!mute) {

		sPt.u8PreScale = 1;
		sPt.u8Div = PWM_CLKSEL_CLKSEL_DIV1;
		sPt.u32Cn = 6000000 / g_u16Frequency - 1;
		sPt.u16Cm = sPt.u32Cn * 9 / 10;  // 90% duty ratio

		PWM_SetClk(4, &sPt);
		PWM_Enable(4);
	}

	return;
}

volatile uint8_t pd = 0, pd1;
volatile uint8_t EndFlag0 = 0;


void touchkey_test(void)
{
	int id0, id1, id2;
	uint8_t slider_ch[] = {11, 12, 13, 14, 15};
	
	id0 = tk_add_key(7, key_callback, 0);
	id1 = tk_add_key(6, key_callback, 1);
	id2 = tk_add_slider(slider_ch, 5, LIBTK_RESOLUTION_8, slider_callback, 0);
	
	tk_enable_component(id0);
	tk_enable_component(id1);
	tk_enable_component(id2);

	/* Waiting for power stable */
    SYS_Delay(50000);

	while(1) 
	{
		//volatile uint32_t delay;
		complete = 0;
		tk_start_sense();
		//for(delay = 0; delay < 0x10000; delay++);
		while(complete == 0);
		
	}
	
}


void EINT1Callback(void)
{    
    pd ^= 1;

}


void PDWU_IRQHandler(void)
{
	if(CLK->WK_INTSTS & 0x1) 
	{
		CLK->WK_INTSTS = 0x1;
	}
}


int32_t main(void)							   
{
	int32_t bLoop = true;
	uint32_t com=0;
	uint8_t u8Item, item[4] = {'1', '2'};
	S_LCD_INIT lcdinit;
	STR_UART_T param;	
	S_DRVPWM_TIME_DATA_T sPt;
	S_DRVRTC_TIME_DATA_T /*sInitTime,*/ sCurTime;

	
	UNLOCKREG();

	CLK->PWRCTL |= CLK_PWRCTL_HXT_EN | CLK_PWRCTL_LXT_EN;
	CLK->APBCLK &= ~ CLK_APBCLK_WDT_EN;

	/* Waiting for Xtal stalble */
	while(SYS_CheckChipClockSrc(CLK_CLKSTATUS_LXT_STB | CLK_CLKSTATUS_HXT_STB) != 0) ;

	/* Select HCLK from external 12 Mhz */
	SYS_SelectHCLKSource(0);

	/* Select UART Clock Source From 12MHz */
	SYS_SelectIPClockSource_1(CLK_CLKSEL1_UART_MASK, CLK_CLKSEL1_UART_HXT);

	GCR->PA_H_MFP = (GCR->PA_H_MFP & ~(PA13_MFP_MASK)) | PA13_MFP_TK11;		// TK11
	GCR->PC_H_MFP = (GCR->PC_H_MFP & ~(PC8_MFP_MASK | PC9_MFP_MASK | PC10_MFP_MASK | PC11_MFP_MASK)) |
					PC8_MFP_TK12 | PC9_MFP_TK13 | PC10_MFP_TK14 | PC11_MFP_TK15;		// 12~15
	GCR->PD_L_MFP = (GCR->PD_L_MFP & ~(PD5_MFP_MASK)) | PD5_MFP_TK5; // 5
	GCR->PF_L_MFP = (GCR->PF_L_MFP & ~(PF4_MFP_MASK)) | PF4_MFP_TK6;    // 6

	MFP_UART0_TO_PORTA();

    param.u32BaudRate        = 115200;
    param.u32cDataBits       = DRVUART_DATABITS_8;
    param.u32cStopBits        = DRVUART_STOPBITS_1;
    param.u32cParity          = DRVUART_PARITY_NONE;
    param.u32cRxTriggerLevel  = DRVUART_FIFO_1BYTES;
	param.u8EnableDiv16		= DISABLE;
    UART_Init(UART0, &param);

	/* PWM */
	GCR->PC_H_MFP =	(GCR->PC_H_MFP & ~(PC12_MFP_MASK)) | PC12_MFP_PWM1_CH0;

	PWM_Init(4);
	PWM_ConfigOutput(4, 1);
	SYS_SelectIPClockSource_2(CLK_CLKSEL2_PWM1_CH01_MASK, CLK_CLKSEL2_PWM1_CH01_HXT);

	sPt.u8Mode = PWM_CTL_CHMOD_ONE_SHOT;
	sPt.u8Inv = 1;
	sPt.u8PreScale = 1;
	g_u16Frequency = note[0];
	sPt.u8Div = PWM_CLKSEL_CLKSEL_DIV1;
	sPt.u32Cn = 6000000 / g_u16Frequency - 1;
	sPt.u16Cm = sPt.u32Cn * 9 / 10;   	// 90% duty ratio
	PWM_SetClk(4, &sPt);
	PWM_Enable(4);

	/* RTC Initialize */
	RTC_Init();	
	
#if 0		
	/* Time Setting */
	sInitTime.u32Year 		= 2011;
	sInitTime.u32cMonth 	= 6;
	sInitTime.u32cDay 		= 9;
	sInitTime.u32cHour 		= 11;
	sInitTime.u32cMinute 	= 21;
	sInitTime.u32cSecond 	= 50;
	sInitTime.u32cDayOfWeek = DRVRTC_THURSDAY;
	sInitTime.u8cClockDisplay = DRVRTC_CLOCK_24;			

	/* Initialization the RTC timer */
	if(RTC_Open(&sInitTime) !=E_SUCCESS)
	{
		printf("RTC Open Fail!!\n");
		return false;
	}
#endif	

	/* Select LCD Clock Source From 32KHz */
	SYS_SelectIPClockSource_1(CLK_CLKSEL1_LCD_MASK, CLK_CLKSEL1_LCD_LXT);

	/* Select LCD Clock Source From 10KHz */
	//SYS_SelectIPClockSource_1(CLK_CLKSEL1_LCD_MASK, CLK_CLKSEL1_LCD_LIRC);

	SYS_SelectIPClockSource_1(CLK_CLKSEL1_TK_MASK, CLK_CLKSEL1_TK_HXT);
	SYS_SelectHCLKSource(0);
	
	/* Enable all segment registers */
	MFP_LCD_TYPEA();
	
	/* LCD Initialize */
	lcdinit.cpump_enable = true;
	lcdinit.internal_bias = false;
	lcdinit.cpump_freqdiv = LCD_CPUMP_DIV1;
	lcdinit.cpump_voltage = LCD_CPVOl_3V;
	lcdinit.bias = LCD_BIAS_THIRD;
	lcdinit.mux = LCD_MUX_ONE_FOURTH;
	lcdinit.freqdiv = LCD_FREQ_DIV64;
	LCD_Init(&lcdinit);

	/* keep display when power down? */
	//LCD_PowerDownDisplay(1);

	/* disable the segment pin */
	LCD_SegmentPinEnable(4, false);
	LCD_SegmentPinEnable(5, false);
	LCD_SegmentPinEnable(6, false);
	LCD_SegmentPinEnable(7, false);
	LCD_SegmentPinEnable(12, false);
	LCD_SegmentPinEnable(13, false);
	LCD_SegmentPinEnable(20, false);
	LCD_SegmentPinEnable(21, false);
	LCD_SegmentPinEnable(26, false);
	LCD_SegmentPinEnable(28, false);
	LCD_SegmentPinEnable(29, false);
	LCD_SegmentPinEnable(30, false);
	LCD_SegmentPinEnable(31, false);
	LCD_SegmentPinEnable(34, false);
	LCD_SegmentPinEnable(35, false);
	LCD_SegmentPinEnable(39, false);

	while(bLoop)
	{			
		printf("\r\nInput test case: ");
		//u8Item = getchar();
		u8Item = item[com++];
		printf("%c\n", u8Item);

		switch(u8Item)
		{
			case '1':
			{				
				RTC_Read(&sCurTime);
				printf("Current Time:%d/%02d/%02d %02d:%02d:%02d\n",sCurTime.u32Year,sCurTime.u32cMonth,sCurTime.u32cDay,sCurTime.u32cHour,sCurTime.u32cMinute,sCurTime.u32cSecond);
				
				showTime(sCurTime.u32cHour, sCurTime.u32cMinute);

				planNextRTCInterrupt(&sCurTime);

				// 1 second
				RTC_SetTickMode(DRVRTC_TICK_1_SEC);
				RTC_EnableInt(RTC_RIER_TIER);
				
				break;
			}
			case '2':
			{
				/* TK digital input path disable */
				GPIOC->OFFD |= 0x0F000000;	// ch12~15
				GPIOA->OFFD |= 0x30820000;	// ch9~11 & CAP
				
				touchkey_test();
				break;
			}

			case 'x':
				bLoop = false;
				break;
			default:
				printf("Wrong Item\n");
				break;			
		}
	}		

	LCD_Disable();

	return true;

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
/*               User can add his own codes here if necessary.                                                  */
/*---------------------------------------------------------------------------------------------------------*/
 
void assert_error(uint8_t * file, uint32_t line)
{
    GCR->PA_H_MFP_BITS.PA14 = 0x6;   /* UART0 multi-function pin*/
	GCR->PA_H_MFP_BITS.PA15 = 0x6;
    CLK->APBCLK_BITS.UART0_EN = 1;  /* Enable UART0 clock */
    CLK->CLKDIV0_BITS.UART_N = 0;   /* UART clock divider is 0 */
    CLK->CLKSEL1_BITS.UART_S = 0;   /* Select HXT (12MHz) as UART clock */
 
    /* Set UART to 115200,n,8,1,none */
    UART0->BAUD = 0x67;             /* Baud Rate:115200 for 12MHz */
    UART0->TLCTL = 0x03;            /* Word len is 8 bits */
                
        
    printf("[%s] line %d : wrong parameters.\r\n", file, line);
 
    /* Infinite loop */
    while(1) ;
 
}
#endif



/*** (C) COPYRIGHT 2011 Nuvoton Technology Corp. ***/


	 
