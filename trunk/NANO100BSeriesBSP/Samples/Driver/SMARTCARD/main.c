/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx SMARTCARD Driver Sample code
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
#include "nano1xx_smartcard.h"

#define HAVE_SC0
//#define HAVE_SC1
//#define HAVE_SC2


int32_t main(void)
{
	bool bLoop = true;
	int32_t i, j;
	int32_t atrlength;
	uint8_t atrbuf[50];

#ifdef HAVE_SC0
	int32_t C0_state = 0, open0 = 0, C0_flag = 0;
#endif
#ifdef HAVE_SC1
	int32_t C1_state = 0, open1 = 0, C1_flag = 0;
#endif
#ifdef HAVE_SC2
	int32_t C2_state = 0, open2 = 0, C2_flag = 0;
#endif


	/* Select SC Clock Source From 12MHz */
	CLK->CLKSEL2 = (CLK->CLKSEL2 & ~CLK_CLKSEL2_SC_MASK) | CLK_CLKSEL2_SC_HXT;

	/* Set SC Clock divider which divide by 3 to 4Mhz be used */
#ifdef HAVE_SC0
	CLK->CLKDIV0 = (CLK->CLKDIV0 & ~CLK_CLKDIV0_SC0_MASK) | (2 << 28);
	MFP_SC0_TO_PORTA();
	SMARTCARD_Init(SC0, 1);
	SMARTCARD_PowerActiveLevel(SC0, 0);
	if(SCARD_PRESENT == SMARTCARD_CheckCardPresent(SC0)) {
		open0 = 1;
		C0_state = 1;
	}
#endif
#ifdef HAVE_SC1
	CLK->CLKDIV1 = (CLK->CLKDIV1 & ~CLK_CLKDIV0_SC1_MASK) | 2;
	MFP_SC1_TO_PORTD();
	SMARTCARD_Init(SC1, 1);
	SMARTCARD_PowerActiveLevel(SC1, 1);
	if(SCARD_PRESENT == SMARTCARD_CheckCardPresent(SC1)) {
		open1 = 1;
		C1_state = 1;
	}
#endif
#ifdef HAVE_SC2
	CLK->CLKDIV1 = (CLK->CLKDIV1 & ~CLK_CLKDIV0_SC2_MASK) | (2 << 2);
	MFP_SC2_TO_PORTB();
	SMARTCARD_Init(SC2, 1);
	SMARTCARD_PowerActiveLevel(SC2, 1);
	if(SCARD_PRESENT == SMARTCARD_CheckCardPresent(SC2)) {
		open2 = 1;
		C2_state = 1;
	}
#endif



	while ( bLoop )
	{
#ifdef HAVE_SC0
		if(open0 == 1)
		{
			i = SMARTCARD_Open(SC0);
			if(i < 0)
				printf("open card0 failed\n\n");
			else {
				atrlength = SMARTCARD_GetATRBuffer(SC0, atrbuf, sizeof(atrbuf));
				printf("[CARD 0] ATR:");
				for (j = 0;j < atrlength;j++)
				{
					printf(" %02X", atrbuf[j]);
				}
				printf("\n\n\n");
			}
		}
		open0 = 0;
#endif
#ifdef HAVE_SC1
		if(open1 == 1)
		{
			i = SMARTCARD_Open(SC1);
			if(i < 0)
				printf("open card1 failed\n\n");
			else {
				atrlength = SMARTCARD_GetATRBuffer(SC1, atrbuf, sizeof(atrbuf));
				printf("[CARD 1] ATR:");
				for (j = 0;j < atrlength;j++)
				{
					printf(" %02X", atrbuf[j]);
				}
				printf("\n\n\n");
			}
		}
		open1 = 0;
#endif
#ifdef HAVE_SC2
		if(open2 == 1)
		{
			i = SMARTCARD_Open(SC2);
			if(i < 0)
				printf("open card2 failed\n\n");
			else {
				atrlength = SMARTCARD_GetATRBuffer(SC2, atrbuf, sizeof(atrbuf));
				printf("[CARD 2] ATR:");
				for (j = 0;j < atrlength;j++)
				{
					printf(" %02X", atrbuf[j]);
				}
				printf("\n\n\n");
			}
		}
		open2 = 0;
#endif


		while(1)
		{
#ifdef HAVE_SC0
			if((SCARD_ABSENT==SMARTCARD_CheckCardPresent(SC0)) && (C0_state==1))
				C0_flag = 1;	// insert -> remove
			else if((SCARD_PRESENT==SMARTCARD_CheckCardPresent(SC0)) && (C0_state==1) && (C0_flag==1))
				C0_flag = 2;	// reomve -> insert
			else if((SCARD_ABSENT==SMARTCARD_CheckCardPresent(SC0)) && (C0_state==0))
				C0_state = 0;
			else if((SCARD_PRESENT==SMARTCARD_CheckCardPresent(SC0)) && (C0_state==0)) {
				C0_flag = 2;	// card present
				C0_state = 1;
			}
#endif
#ifdef HAVE_SC1
			if((SCARD_ABSENT==SMARTCARD_CheckCardPresent(SC1)) && (C1_state==1))
				C1_flag = 1;	// insert -> remove
			else if((SCARD_PRESENT==SMARTCARD_CheckCardPresent(SC1)) && (C1_state==1) && (C1_flag==1))
				C1_flag = 2;	// remove -> insert
			else if((SCARD_ABSENT==SMARTCARD_CheckCardPresent(SC1)) && (C1_state==0))
				C1_state = 0;
			else if((SCARD_PRESENT==SMARTCARD_CheckCardPresent(SC1)) && (C1_state==0)) {
				C1_flag = 2;	// card present
				C1_state = 1;
			}
#endif
#ifdef HAVE_SC2
			if((SCARD_ABSENT==SMARTCARD_CheckCardPresent(SC2)) && (C2_state==1))
				C2_flag = 1;	// insert -> remove
			else if((SCARD_PRESENT==SMARTCARD_CheckCardPresent(SC2)) && (C2_state==1) && (C2_flag==1))
				C2_flag = 2;	// remove -> insert
			else if((SCARD_ABSENT==SMARTCARD_CheckCardPresent(SC2)) && (C2_state==0))
				C2_state = 0;
			else if((SCARD_PRESENT==SMARTCARD_CheckCardPresent(SC2)) && (C2_state==0)) {
				C2_flag = 2;	// card present
				C2_state = 1;
			}
#endif
#ifdef HAVE_SC0
			if(C0_flag == 2) {
				C0_flag = 0;
				open0 = 1;
				break;
			}
#endif
#ifdef HAVE_SC1
			if(C1_flag == 2) {
				C1_flag = 0;
				open1 = 1;
				break;
			}
#endif
#ifdef HAVE_SC2
			if(C2_flag == 2) {
				C2_flag = 0;
				open2 = 1;
				break;
			}
#endif
		}

	}


	return true;

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



