/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx DAC Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nano1xx_dac.h"
#include "nano1xx_timer.h"
#include "nano1xx_assert.h"

#define CURRENT_CLK  12000000

#define PCLK	12000000   // clock feed to dac
const uint8_t us = ((PCLK) / 1000000);
#define ECLK	6000000	   // clock feed to timer



void sine_wave(void)
{
        DAC_Init();
        DAC_SetRefVol(DAC_COMCTL_REFSEL_AVDD);
        DAC_SetDelayTime(0, us * 6, us * 2);
        DAC_SetLoadSel(0, DAC_CTL_LSEL_TIMER0_CH0);
        DAC_SetDelayTime(1, us * 6, us * 2);
        DAC_SetLoadSel(1, DAC_CTL_LSEL_TIMER0_CH1);

	DAC_CH0_POWER_ON;
	DAC_CH1_POWER_ON;

        //Wait 'til channel 0 & 1 ready
 		while(DAC_GetState(0) != DAC_IDLE);
 		while(DAC_GetState(1) != DAC_IDLE);

        printf("\n\nHit any key to start DAC output\n");
        getchar();

        DAC_EnableInterrupt(0);
        DAC_EnableInterrupt(1);

        TIMER_Start(TIMER0);
        TIMER_Start(TIMER1);

        printf("Hit any key to quit\n");

        getchar();
        TIMER_Stop(TIMER0);
        TIMER_Stop(TIMER1);
        DAC_DeInit();

        return;


}

int32_t main(void)
{

		GCR->PC_L_MFP = (GCR->PC_L_MFP & ~(PC7_MFP_MASK | PC6_MFP_MASK)) | PC6_MFP_DA_OUT0 | PC7_MFP_DA_OUT1; // Set DAC pin function

		TIMER_Init(TIMER0, 11, 1000, TIMER_CTL_MODESEL_PERIODIC);  // 12MHz / ((11 + 1) * 1000)	 = 1000Hz
		TIMER_Init(TIMER1, 11, 2000, TIMER_CTL_MODESEL_PERIODIC);  // 12MHz / ((11 + 1) * 2000)	 = 500Hz

        TIMER_ConfigTriggerEvent(TIMER0, TIMER_CTL_DACTEEN, TIMER_CTL_TIMEOUTTRGEN);
        TIMER_ConfigTriggerEvent(TIMER1, TIMER_CTL_DACTEEN, TIMER_CTL_TIMEOUTTRGEN);

        sine_wave();

        printf("Adios!!\n");
        return(0);
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

