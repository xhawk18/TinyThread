/******************************************************************************
 * @file     Capture.c
 * @brief    Nano1xx PWM capture sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>																											
#include "nano1xx.h"
#include "nano1xx_sys.h"
#include "nano1xx_ebi.h"
#include "EBI_LCM_OTM3225A.h"
#include "assert.h"

uint32_t	CaptureValue[2];

extern void EBI_LCM_Initial(void);

void InitCapture(void)
{
 	// Configure pin function
	GCR->PA_H_MFP = (GCR->PA_H_MFP & ~0x000F0000) | 0x00010000;		
		
	//Enable PWM clock
	CLK->APBCLK |= CLK_APBCLK_PWM0_CH01_EN;
	//Select HXT for PWM clock source
	CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_PWM0_CH01_MASK) | CLK_CLKSEL1_PWM0_CH01_HXT;

	// Prescaler 0~255, Setting 0 to stop output clock
	PWM0->PRES = (PWM0->PRES & ~PWM_PRES_CP01_MASK) | 11;	
	// Clock divider->0:/2, 1:/4, 2:/8, 3:/16, 4:/1
	PWM0->CLKSEL = (PWM0->CLKSEL & ~PWM_CLKSEL_CLKSEL0_MASK) | PWM_CLKSEL_CLKSEL0_DIV1;	
									
	// Select PWM operation mode
	PWM0->CTL |= PWM_CTL_CH0MOD;
	// Set Reload register	
	PWM0->DUTY0 = 0xFFFF;		
	// Enable Capture function pin, capture function, counter reload on both rising and falling edge
	PWM0->CAPCTL = PWM_CAPCTL_CAPCH0EN |
			PWM_CAPCTL_CAPCH0PADEN |
			PWM_CAPCTL_CAPRELOADREN0 |
			PWM_CAPCTL_CAPRELOADFEN0;				


	// Enable Capture rising and falling edge interrupt
	PWM0->CAPINTEN = PWM_CAPINTEN_CRLIE0 | PWM_CAPINTEN_CFLIE0;		
	// Enable PWM interrupt for down-counter equal zero.
	PWM0->INTEN |= PWM_INTEN_TMIE0;	
	// Enable PWM inturrupt	
 	NVIC_EnableIRQ(PWM0_IRQn);

	// Enable PWM down counter
	PWM0->CTL |= PWM_CTL_CH0EN;
}

void InitPWM(void)
{
 	// Configure pin function
	GCR->PA_H_MFP = (GCR->PA_H_MFP & ~0x0F000000) | 0x01000000;	
				
	//Enable PWM clock
	CLK->APBCLK |= CLK_APBCLK_PWM0_CH23_EN;
	//Select HXT for PWM clock source
	CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_PWM0_CH23_MASK) | CLK_CLKSEL1_PWM0_CH23_HXT;

	// Prescaler 0~255, Setting 0 to stop output clock
	PWM0->PRES = (PWM0->PRES & ~PWM_PRES_CP23_MASK) | 0x100;	
	// Clock divider->0:/2, 1:/4, 2:/8, 3:/16, 4:/1
	PWM0->CLKSEL = (PWM0->CLKSEL & ~PWM_CLKSEL_CLKSEL2_MASK) | PWM_CLKSEL_CLKSEL2_DIV1;	

	// Select PWM operation mode
	PWM0->CTL |= PWM_CTL_CH2MOD;
	// Set Reload register, CN = 0x1FFF, CM = 0x0FFF	
	PWM0->DUTY2 = 0x0FFF1FFF;
				
	// Enable PWM down counter
	PWM0->CTL |= PWM_CTL_CH2EN;
	// Output enable
 	PWM0->OE |= PWM_OE_CH2;
}



/*----------------------------------------------------------------------------
  MAIN function
  ----------------------------------------------------------------------------*/
int32_t main (void)
{
	char string[18];

	/* initailize EBI and pins */
	EBI_LCM_Initial();

	/* panel initialization */
	OTM3225_init(RES_LANDSCAPE_320X240);
	ClearScreen(0x0000);
	Show_String32x32(1, 1, "Capture", 0xf800);
	Show_String32x32(1, 33, "A12<-A14", 0xf800);
	Show_String32x32(1, 65, "High:", 0xf800);
	Show_String32x32(1, 97, "Low:", 0xf800);

	InitPWM();
	InitCapture();

	
	while(1)
	{
		DrawPartialScreen(161, 65, 160, 32, 0x0000);
		if(CaptureValue[0]>=1000000)
		{
		   	sprintf(string,"%ds",CaptureValue[0]/1000000);
		}else if(CaptureValue[0]>=1000)
		{
		   	sprintf(string,"%dms",CaptureValue[0]/1000);
		}else			
			sprintf(string,"%dus",CaptureValue[0]);
		Show_String32x32(161, 65, (uint8_t *)string, 0xf800);

	
		DrawPartialScreen(129, 97, 160, 32, 0x0000);	
		if(CaptureValue[1]>=1000000)
		{
		   	sprintf(string,"%ds",CaptureValue[1]/1000000);
		}else if(CaptureValue[1]>=1000)
		{
		   	sprintf(string,"%dms",CaptureValue[1]/1000);
		}else			
			sprintf(string,"%dus",CaptureValue[1]);		
		Show_String32x32(129, 97, (uint8_t *)string, 0xf800);

	}				

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
