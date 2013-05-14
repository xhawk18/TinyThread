/******************************************************************************
 * @file     Main.c
 * @brief    Nano1xx Sample code
 * @version  1.0.1
 * @date     08, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx_sys.h"
#include "nano1xx_lcd.h"
#include "nano1xx_gpio.h"

void delay_loop(void)
{
	__IO uint32_t i,j;

	for(i=0;i<3;i++)
	{
		for(j=0;j<60000;j++);
	}
}

char int_to_char(int32_t i)
{
	if(i==0) return '0';
	else if(i==1) return '1';
	else if(i==2) return '2';
	else if(i==3) return '3';
	else if(i==4) return '4';
	else if(i==5) return '5';
	else if(i==6) return '6';
	else if(i==7) return '7';
	else if(i==8) return '8';
	else if(i==9) return '9';
	else if(i==10) return 'A';
	else if(i==11) return 'B';
	else if(i==12) return 'C';
	else if(i==13) return 'D';
	else if(i==14) return 'E';
	else if(i==15) return 'F';
	
	return 0xff;
}

void Timer_initial(void)
{
	/* Step 1. Enable and Select Timer clock source */
	CLK->CLKSEL1 = CLK->CLKSEL1 & ~CLK_CLKSEL1_TMR0_MASK | CLK_CLKSEL1_TMR0_HXT;	//Select 12Mhz for Timer0 clock source
	CLK->APBCLK |= CLK_APBCLK_TMR0_EN;	//Enable Timer0 clock source

	/* Step 2. Select Operation mode */
	TIMER0->CTL = TIMER_CTL_MODESEL_PERIODIC;	//Select periodic mode for operation mode

	/* Step 3. Select Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)*/
	TIMER0->PRECNT = 0;			// Set Prescale [0~255]
	TIMER0->CMPR = 1000000;		// Set TICR(TCMP) [0~16777215]
								// (1/22118400)*(0+1)*(2765)= 125.01usec or 7999.42Hz

	/* Step 4. Enable interrupt */
	TIMER0->IER = TIMER_IER_TMRIE;
	TIMER0->ISR = TIMER_ISR_TMRIS;		//Write 1 to clear for safty
	NVIC_EnableIRQ(TMR0_IRQn);			//Enable Timer0 Interrupt

	/* Step 5. Enable Timer module */
	TIMER0->CTL |= TIMER_CTL_SWRST;		//Reset up counter
	TIMER0->CTL |= TIMER_CTL_TMREN;		//Enable Timer0

	TIMER0->CTL |= TIMER_CTL_INTRTRGEN;	// Enable TDR function
}

/**
  * @brief  main function.
  * @param  None
  * @return None
  */
int32_t main(void)
{
	int i, j;
	S_LCD_INIT lcdinit;
	char start_str[5];

	UNLOCKREG();	

	SYS_SetChipClockSrc(CLK_PWRCTL_LXT_EN, 1);
	while(SYS_CheckChipClockSrc(CLK_CLKSTATUS_LXT_STB));

	/* Select LCD Clock Source From 32KHz */
	SYS_SelectIPClockSource_1(CLK_CLKSEL1_LCD_MASK, CLK_CLKSEL1_LCD_LXT);
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

	start_str[0] = 'N';
	start_str[1] = 'A';
	start_str[2] = 'N';
	start_str[3] = 'O';
	LCD_Write(&start_str[0]);

	for(j=0;j<10;j++)
		delay_loop();

	/*************************/
	GPIO_Init();
	GPIO_Open(GPIOB, GPIO_PMD_PMD8_OUTPUT, GPIO_PMD_PMD8_MASK);
	GPIO_SetBit(GPIOB, 8);
	Timer_initial();

	while(1)
	{
		for(i=0;i<9;i++)
		{
			GPIO_ClrBit(GPIOB, 8);
			for(j=0;j<4;j++)
			{
				LCD_AlphaNumberOff();
				start_str[0] = ' ';
				start_str[1] = int_to_char(i);
				start_str[2] = ' ';
				start_str[3] = int_to_char(j);
				LCD_Write(&start_str[0]);
				delay_loop();
			}
			GPIO_SetBit(GPIOB, 8);
			delay_loop();
			delay_loop();
		}
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
	printf("[%s] line %d : wrong parameters.\r\n", file, line);

	/* Infinite loop */
	while(1) ;

}
#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

