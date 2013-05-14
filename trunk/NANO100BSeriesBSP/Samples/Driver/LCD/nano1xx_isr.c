/******************************************************************************
 * @file     nano1xx_isr.c
 * @brief    This file contains interrupt service routines
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "nano1xx.h"


 /**
  * @brief  LCD ISR to handle interrupt event
  * @param  None
  * @retval None
  */
 void LCD_IRQHandler(void)
{

	if( LCD->FCSTS & LCD_FCSTS_FCSTS)
	{
		LCD->FCSTS = LCD_FCSTS_FCSTS;

		printf("IST: LCD Frame Count interrupt...\n");
	}
	else if( LCD->FCSTS & LCD_FCSTS_PDSTS)
	{
		LCD->FCSTS = LCD_FCSTS_PDSTS;

		printf("IST: LCD Power Down interrupt...\n");
	}

}



/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

