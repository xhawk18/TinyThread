/******************************************************************************
 * @file     main.c
 * @brief    NuTouch Sample code.
 * @version  1.0.1
 * @date     02, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nano1xx.h"
#include "nano1xx_tk.h"
#include "nano1xx_gpio.h"
#include "libtk.h"

extern void UART_Setting(void);

typedef struct {
	uint16_t level_off;  
	uint16_t data_off;
	uint16_t level_on;
	uint16_t data_on;
}tk_sense_result;

tk_sense_result result[12][15]; // clock div from 1~12, charge current from 1/15~15/15
uint8_t led;
uint8_t volatile complete;

void key_callback(uint16_t status, uint16_t param)
{

	printf("Key %d status :%s\n", param, status == 0 ? "on" : "off");

	if(status == 0)	{		
		led |= (1 << param);
	} else {	
		led &= ~(1 << param);				
	}

	return;
}

void slider_callback(uint16_t status, uint16_t param)
{

	if(status == 0xFFFF)
		printf("slider status :off\n");
	else		
		printf("slider status :%d\n", status);

	return;
}

int32_t main(void)
{

	uint8_t slider_ch[] = {0, 1, 2, 3, 4, 5, 6, 7};
	int id0, id1, id2, id3, id4, id5, id6, id7, id8;
	int volatile i;

	printf("TK demo code begins\n");

	GCR->PA_L_MFP = (GCR->PA_L_MFP & ~(PA0_MFP_MASK | PA1_MFP_MASK)) | PA0_MFP_TK8 | PA1_MFP_TK9;		// TK8, 9
	GCR->PA_H_MFP = (GCR->PA_H_MFP & ~(PA12_MFP_MASK | PA13_MFP_MASK)) | PA12_MFP_TK10 | PA13_MFP_TK11;		// TK10, 11
	GCR->PC_H_MFP = (GCR->PC_H_MFP & ~(PC8_MFP_MASK | PC9_MFP_MASK | PC10_MFP_MASK | PC11_MFP_MASK)) |
					PC8_MFP_TK12 | PC9_MFP_TK13 | PC10_MFP_TK14 | PC11_MFP_TK15;		// 12~15
	GCR->PD_L_MFP = (GCR->PD_L_MFP & ~(PD0_MFP_MASK | PD1_MFP_MASK | PD2_MFP_MASK | PD3_MFP_MASK | PD4_MFP_MASK | PD5_MFP_MASK)) |  
					PD0_MFP_TK0 | PD1_MFP_TK1 | PD2_MFP_TK2 | PD3_MFP_TK3 | PD4_MFP_TK4 | PD5_MFP_TK5;    // 0~5
	GCR->PF_L_MFP = (GCR->PF_L_MFP & ~(PF4_MFP_MASK | PF5_MFP_MASK)) | PF4_MFP_TK6 | PF5_MFP_TK7;    // 6, 7


	id0 = tk_add_key(8, key_callback, 0);
	id1 = tk_add_key(9, key_callback, 1);
	id2 = tk_add_key(10, key_callback, 2);  // that's right. channel 11, and then 10...
	id3 = tk_add_key(11, key_callback, 3);
	id4 = tk_add_key(12, key_callback, 4);
	id5 = tk_add_key(13, key_callback, 5);
	id6 = tk_add_key(14, key_callback, 6);
	id7 = tk_add_key(15, key_callback, 7);

	// LED control
	GPIOC->DOUT &= ~0x20C0;
	GPIOC->PMD &= ~0x0C00F000;
	GPIOC->PUEN |= 0x20C0;
	GPIOE->DOUT &= ~0x1F;
	GPIOE->PMD &= ~0x3FF;
	GPIOE->PUEN |= 0x1F;




	id8 = tk_add_slider(slider_ch, sizeof(slider_ch)/sizeof(uint8_t), LIBTK_RESOLUTION_32, slider_callback, 0);

	tk_enable_component(id0);
	tk_enable_component(id1);
	tk_enable_component(id2);
	tk_enable_component(id3);
	tk_enable_component(id4);
	tk_enable_component(id5);
	tk_enable_component(id6);
	tk_enable_component(id7);

	tk_enable_component(id8);

	tk_start_calibration();

	while(1) {
		uint32_t volatile delay;

		complete = 0;
		tk_start_sense();

		while(complete == 0);

		// output mode
		GPIOC->PMD |= 0x04005000;
		GPIOE->PMD |= 0x155;

		// flash LEDs...
		for(delay = 0; delay < 0x800; delay++) {
			for(i = 0; i < 5; i++) {   
				if(led & (1 << i)) {				
					GPIOE->DOUT &= ~(1 << i);
				}
			}
	
			if(led & (1 << 5)) {			
				GPIOC->DOUT &= ~(1 << 6);
			}
			
			if(led & (1 << 6)) {
				GPIOC->DOUT &= ~(1 << 7);
			}
	
			if(led & (1 << 7)) {		
				GPIOC->DOUT &= ~(1 << 13);
			}
			for(i = 0; i < 5; i++) {   				
				GPIOE->DOUT |= (1 << i);
			}
			GPIOC->DOUT |= (1 << 6);
			GPIOC->DOUT |= (1 << 7);
			GPIOC->DOUT |= (1 << 13);
		}

		// input mode
		GPIOC->PMD &= ~0x0C00F000;
		GPIOE->PMD &= ~0x3FF;
	
	}	
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
