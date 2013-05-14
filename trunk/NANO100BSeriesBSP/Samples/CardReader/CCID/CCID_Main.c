/******************************************************************************
 * @file     CCID_Main.c
 * @brief    Nano1xx USB CCID Sample code
 * @version  1.0.2
 * @date     02, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx_sys.h"
#include "nano1xx_usb.h"
#include "nano1xx_smartcard.h"
#include "nano1xx_gpio.h"

extern int32_t CCID_MainProcess(void);

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function									                                           			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{
	SYS_SetClockDivider_0(CLK_CLKDIV0_USB_MASK, USB_CLK_DIVIDER(1));

	/* PB[5] indicates the output voltage of level-shifter */
	GPIOB->PMD = (GPIOB->PMD & ~0xC00) | 0x400;
	//DrvGPIO_ClrBit(E_GPB, 5);			// 3V
	GPIO_SetBit(GPIOB, 5);				// 5V

	printf("+-----------------------------------------------+\n");
	printf("|          NANO100 USB CCID Sample Code         |\n");
	printf("+-----------------------------------------------+\n");					  

	printf("NANO100 USB CCID Start!\n");

	/* Select SC Clock Source From 12MHz */
	CLK->CLKSEL2 = (CLK->CLKSEL2 & ~CLK_CLKSEL2_SC_MASK) | CLK_CLKSEL2_SC_HXT;

	/* Set SC Clock divider which divide by 3 to 4Mhz be used */
	CLK->CLKDIV0 = (CLK->CLKDIV0 & ~CLK_CLKDIV0_SC0_MASK) | (2 << 28);
	CLK->CLKDIV1 = (CLK->CLKDIV1 & ~CLK_CLKDIV1_SC1_MASK) | 2;

	MFP_SC0_TO_PORTA();
	//MFP_SC1_TO_PORTC();
	//MFP_SC1_TO_PORTD();  // NuTiny
	
	/* SmartCard 0 Initialization */	
	SMARTCARD_Init(SC0, 1);
	SMARTCARD_PowerActiveLevel(SC0, 0);	// Power pin is active LOW
	SMARTCARD_Open(SC0);

	/* SmartCard 1 Initialization */
	//SMARTCARD_Init(SC1, 1);
	//SMARTCARD_PowerActiveLevel(SC1, 1);	// Power pin is active LOW
	//SMARTCARD_IgnoreCardAbsent(SC1);		// card 1 is always present
	//SMARTCARD_Open(SC1);

	/* Execute CCID process */
	CCID_MainProcess();
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
	printf("[%s] line %d : wrong parameters.\r\n", file, line);

	/* Infinite loop */
	while(1) ;

}
#endif


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

