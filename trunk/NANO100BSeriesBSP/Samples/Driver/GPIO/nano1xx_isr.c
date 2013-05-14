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


/* ToDo: add interrupt service routines. */
/*---------------------------------------------------------------------------------------------------------*/
/* External INT0 ISR                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
/**   
  * @brief  EINT0_IRQHandler, External Interrupt 0 handler.
  * @param  None.
  * @retval None.
  */
void EINT0_IRQHandler(void)
{
	/* EINT0 (GPB14) Clear the interrupt */
	GPIOB->ISR = 1 << 14;
    printf("EINT0 Interrupt!\n");
}

/*---------------------------------------------------------------------------------------------------------*/
/* External INT1 ISR                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
/**   
  * @brief  EINT1_IRQHandler, External Interrupt 1 handler.
  * @param  None.
  * @retval None.
  */
void EINT1_IRQHandler(void)
{
	/* EINT1 (GPB15) Clear the interrupt */
	GPIOB->ISR = 1 << 15;
    printf("EINT1 Interrupt!\n");
}

/*---------------------------------------------------------------------------------------------------------*/
/* GPIO A,B,C ISR                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
/**   
  * @brief  GPABC_IRQHandler, GPIO PortA, PortB, PortC interrupt handler.
  * @param  None.
  * @retval None.
  */
void GPABC_IRQHandler(void)
{
	uint32_t u32GPAStatus, u32GPBStatus, u32GPCStatus;

	u32GPAStatus = GPIOA->ISR;
	u32GPBStatus = GPIOB->ISR;
	u32GPCStatus = GPIOC->ISR;

    printf("GPABC Interrupt! GPA:0x%04x  GPB:0x%04x  GPC:0x%04x\n", u32GPAStatus, u32GPBStatus, u32GPCStatus);

	/* Clear the interrupt */
	GPIOA->ISR = u32GPAStatus;
	GPIOB->ISR = u32GPBStatus;
	GPIOC->ISR = u32GPCStatus;
}

/*---------------------------------------------------------------------------------------------------------*/
/* GPIO D,E,F ISR                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
/**   
  * @brief  GPABC_IRQHandler, GPIO PortD, PortE, PortF interrupt handler.
  * @param  None.
  * @retval None.
  */
void GPDEF_IRQHandler(void)
{
	uint32_t u32GPDStatus, u32GPEStatus, u32GPFStatus;

	/* Keep the interrupt source */
	u32GPDStatus = GPIOD->ISR;
	u32GPEStatus = GPIOE->ISR;
	u32GPFStatus = GPIOF->ISR;

    printf("GPDEF Interrupt! GPD:0x%04x  GPE:0x%04x  GPF:0x%04x\n", u32GPDStatus, u32GPEStatus, u32GPFStatus);

	/* Clear the interrupt */
	GPIOD->ISR = u32GPDStatus;
	GPIOE->ISR = u32GPEStatus;
	GPIOF->ISR = u32GPFStatus;
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
