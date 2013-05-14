/******************************************************************************
 * @file     nano1xx_isr.c
 * @brief    This file contains interrupt service routines
 * @version  1.0.1
 * @date     08, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


#include <stdint.h>
#include "nano1xx.h"
#include "nano1xx_gpio.h"


/**   
  * @brief  TMR0_IRQHandler, USB interrupt handler.
  * @param  None.
  * @retval None.
  */
static uint8_t count = 0;
static uint8_t loop = 12;

void TMR0_IRQHandler(void)
{
	uint8_t i=0;

	TIMER0->ISR = TIMER_ISR_TMRIS;
	count++;
	if(count==5)
	{
		GPIO_ClrBit(GPIOC, loop);
		loop++;
		count=0;
		if(loop == 17)
		{
			for(i=12;i<16;i++)
			{
				GPIO_SetBit(GPIOC, i);
			}
			loop=12;
	   }
	}
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
