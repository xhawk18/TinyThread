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

#define BUF_LEN	16

uint8_t rx_buf0[BUF_LEN];
volatile uint8_t rx_head0 = 0, rx_tail0 = 0;
uint8_t tx_buf0[BUF_LEN];
volatile uint8_t tx_head0 = 0, tx_tail0 = 0;
uint8_t rx_buf1[BUF_LEN];
volatile uint8_t rx_head1 = 0, rx_tail1 = 0;
uint8_t tx_buf1[BUF_LEN];
volatile uint8_t tx_head1 = 0, tx_tail1 = 0;
uint8_t rx_buf2[BUF_LEN];
volatile uint8_t rx_head2 = 0, rx_tail2 = 0;
uint8_t tx_buf2[BUF_LEN];
volatile uint8_t tx_head2 = 0, tx_tail2 = 0;


//#define HAVE_SC0
//#define HAVE_SC1
#define HAVE_SC2


int32_t main(void)
{
	uint8_t buf[10];
	uint32_t len = 0;
	// Set to SC_UART to 1152008n1. Rx trigger level 3 bytes, timeout 20 clocks
	S_SC_UART config = {115200, 
			    SC_UACTL_DATA_LEN_8_BITS,
			    0, 
			    SC_UACTL_PBDIS,
			    SC_CTL_RX_FTRI_LEV_3BYTES,
			    20};

	/* Select SC Clock Source From 12MHz */
	CLK->CLKSEL2 = (CLK->CLKSEL2 & ~CLK_CLKSEL2_SC_MASK) | CLK_CLKSEL2_SC_HXT;

	printf("Polling mode: Input 10 characters to SC_UART port, and this code will echo them back\n");


#ifdef HAVE_SC0
	GCR->PA_H_MFP = (GCR->PA_H_MFP & ~0xF0) | 0x30;  // smartcard data pin GPA[9] as RX
	GCR->PA_H_MFP = (GCR->PA_H_MFP & ~0xF) | 3;  // smartcard clock pin GPA[8] as TX
	SMARTCARD_Init(SC0, 0);
	SMARTCARD_UartOpen(SC0, &config);
	while(len < 10) {
		len += SMARTCARD_UartRead(SC0, &buf[len], 10);
	}

	SMARTCARD_UartWrite(SC0, buf, len);


#endif
#ifdef HAVE_SC1
	GCR->PD_L_MFP = (GCR->PD_L_MFP & ~0xF0) | 0x40;  // smartcard data pin GPD[1] as RX
	GCR->PD_L_MFP = (GCR->PD_L_MFP & ~0xF) | 4;  // smartcard clock pin GPD[0] as TX
	SMARTCARD_Init(SC1, 0);
	SMARTCARD_UartOpen(SC1, &config);
	while(len < 10) {
		len += SMARTCARD_UartRead(SC1, &buf[len], 10);
	}

	SMARTCARD_UartWrite(SC1, buf, len);

#endif
#ifdef HAVE_SC2
	GCR->PB_H_MFP = (GCR->PB_H_MFP & ~0xF000) | 0x4000;  // smartcard data pin GPB[11] as RX
	GCR->PB_H_MFP = (GCR->PB_H_MFP & ~0xF00) | 0x400;  // smartcard clock pin GPB[10] as TX
	SMARTCARD_Init(SC2, 1);
	SMARTCARD_UartOpen(SC2, &config);
	while(len < 10) {
		len += SMARTCARD_UartRead(SC2, &buf[len], 10);
	}

	SMARTCARD_UartWrite(SC2, buf, len);
#endif

	printf("Interrupt mode: Input 10 characters to SC_UART port, and this code will echo them back\n");

#ifdef HAVE_SC0
	SMARTCARD_UartEnableInt(SC0);
	while(rx_tail0 < 10);
	memcpy(tx_buf0, rx_buf0, 10);
	tx_tail0 = 10;
	SC0->IER |= SC_IER_TBE_IE;
	while(tx_head0 < 10);
#endif
#ifdef HAVE_SC1
	SMARTCARD_UartEnableInt(SC1);
	while(rx_tail1 < 10);
	memcpy(tx_buf1, rx_buf1, 10);
	tx_tail1 = 10;
	SC1->IER |= SC_IER_TBE_IE;
	while(tx_head1 < 10);
#endif
#ifdef HAVE_SC2
	SMARTCARD_UartEnableInt(SC2);
	while(rx_tail2 < 10);
	memcpy(tx_buf2, rx_buf2, 10);
	tx_tail2 = 10;
	SC2->IER |= SC_IER_TBE_IE;
	while(tx_head2 < 10);
#endif	


	printf("test complete, bye~~\n");

	while(1);
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



