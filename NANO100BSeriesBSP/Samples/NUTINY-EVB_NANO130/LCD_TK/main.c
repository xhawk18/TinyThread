/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx NUTINY-EVB_NANO130 Touch Key Demo code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nano1xx.h"
#include "nano1xx_timer.h"
#include "nano1xx_tk.h"
#include "nano1xx_lcd.h"
#include "nano1xx_sys.h"
#include "libtk.h"


uint8_t volatile complete;
uint32_t offcount = 0;

void slider_callback(uint16_t status, uint16_t param)
{
	char str[8];
	if(status == 0xFFFF) {
		LCD_Write("OFF");
	} else {		
		sprintf(str, "%d", status);
		LCD_Write(str);
	}
	offcount = 0;
	return;
}


int32_t main(void)
{
	uint8_t slider_ch[] = {11, 12, 13, 14, 15};
	uint8_t id;
	int volatile i;
	S_LCD_INIT lcdinit;
	UNLOCKREG();

	CLK->PWRCTL |= CLK_PWRCTL_LXT_EN;
	while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_LXT_STB));  // wait 'til LXT stable


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

	/* Start displaying on LCD */
	LCD_EnableSegment(3, 22);
	LCD_Write("TK DEMO");

	/* Keep display when system power down */
	LCD_PowerDownDisplay(1);


	//printf("TK demo code begins\n");

	GCR->PA_H_MFP = (GCR->PA_H_MFP & ~0xF00000) | 0x600000;   // TK11
	GCR->PC_H_MFP = (GCR->PC_H_MFP & ~0xFFFF) | 0x6666;	  // TK12~15

	/* Enable GPIO pull up */
	GPIOA->PUEN = 0xFFFF;
	GPIOB->PUEN = 0xFFFF;
	GPIOC->PUEN = 0xFFFF;
	GPIOD->PUEN = 0xFFFF;
	GPIOE->PUEN = 0xFFFF;
	GPIOF->PUEN = 0x0033;	

	id = tk_add_slider(slider_ch, 5, LIBTK_RESOLUTION_32, slider_callback, 0);

	tk_enable_component(id);
	tk_start_calibration();

	for(i = 0; i < 20; i++) {    // for calibration

		complete = 0;
		tk_start_sense();
		while(complete == 0);
	}	

	tk_timer_trigger(1, 0);

	CLK->CLKSEL1 = ((CLK->CLKSEL1) & ~CLK_CLKSEL1_TMR0_MASK	) | CLK_CLKSEL1_TMR0_LXT;
	CLK->APBCLK |= CLK_APBCLK_TMR0_EN;
	TIMER0->CMPR = 0x8000/5;
	TIMER0->CTL = 0x8011;

	while(1) {
		tk_timer_trigger(0, 0);
		CLK->APBCLK &= ~CLK_APBCLK_TMR0_EN;
		if(offcount++ < 20) {
			complete = 0;
			tk_start_sense();
			while(complete == 0);

		} else {
			CLK->APBCLK |= CLK_APBCLK_TMR0_EN ;
			tk_timer_trigger(1, 0);
			LCD_Write("SLEEP");
			SYS_SetUpPowerDown(0);
			__WFI();
			offcount = 0;
		}
	}

}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
