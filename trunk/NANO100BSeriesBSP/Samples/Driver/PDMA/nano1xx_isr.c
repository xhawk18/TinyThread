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

extern int32_t IsTestOver;


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     PDMA_IRQHandler     		                                                               */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 None 												                                       */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to  PDMA ISR                                                         */
/*---------------------------------------------------------------------------------------------------------*/
/**   
  * @brief  PDMA_IRQHandler.
  * @param  None.
  * @retval None.
  */
void PDMA_IRQHandler(void)
{
	uint32_t u32ISR;

	u32ISR = PDMA1->ISR;
	if(u32ISR & 0x01)	 		/* Check Target Abort INT and clear */
	{
		PDMA1->ISR = 0x01;
		printf("\tTransfer Target Abort !\r\n");
	}
	else if(u32ISR & 0x02)	  
	{							/* Transfer Done INT and clear */
		PDMA1->ISR = 0x2;	
		printf("\tTransfer Done!\r\n");
		IsTestOver = TRUE;
	}
	else if(u32ISR & 0x04)
	{							/* Wrap around Transfer complete and clear */
		PDMA1->ISR = 0x04;
		printf("\tWrap around Transfer complete!\r\n");
	}
	else if(u32ISR & 0x10)
	{							/* Wrap around Transfer Half and clear */
		PDMA1->ISR = 0x10;
		printf("\tWrap around Transfer Half!\r\n");
	}
	else if(u32ISR & 0x40)
	{							/* Transfer Timeout INT and clear */
		PDMA1->ISR = 0x40;
		printf("\tTransfer Timeout!\r\n");
	}
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
