/******************************************************************************
 * @file     nano1xx_gpio.c
 * @brief    NANO1xx GPIO driver source file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include "nano1xx_gpio.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_GPIO_Driver GPIO Driver
  @{
*/


/** @addtogroup NANO1xx_GPIO_EXPORTED_FUNCTIONS GPIO Exported Functions
  @{
*/


/**   
  * @brief  GPIO_EnableInt, Enable the interrupt function of the specified GPIO pin.
  * @param  port: Specify GPIO port, GPIOA ~ GPIOF.
  * @param  bit : Specify pin of the GPIO port. 0~15.
  * @param  TriggerType : Specify the interrupt trigger type.
  * @param  Mode : Specify the interrupt mode.
  * @retval None.
  */
void GPIO_EnableInt(GPIO_TypeDef *port, uint32_t bit, uint32_t TriggerType, uint32_t Mode)
{
	port->IER |= TriggerType;
	port->IMD = (port->IMD & ~(1 << bit) | Mode);

	/* Enable the relative interrupt of M0 */
	if ((port == GPIOA) || (port == GPIOB) || (port == GPIOC))
	{
		//NVIC_SetPriority(GPABC_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
		NVIC_EnableIRQ(GPABC_IRQn);
	}

	if ((port == GPIOD) || (port == GPIOE) || (port == GPIOF))
	{
		//NVIC_SetPriority(GPDEF_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
		NVIC_EnableIRQ(GPDEF_IRQn);
	}
}

/**   
  * @brief  GPIO_DisableInt, Disable the interrupt function of the specified GPIO pin.
  * @param  port: Specify GPIO port, GPIOA ~ GPIOF.
  * @param  bit : Specify pin of the GPIO port. 0~15.
  * @retval None.
  */
void GPIO_DisableInt(GPIO_TypeDef *port, uint32_t bit)
{
	port->IER &= ~((1 << bit) | (1 << (bit+16)));

	/* Disable the relative interrupt of M0 */
	if ((port == GPIOA) || (port == GPIOB) || (port == GPIOC))
	{
		NVIC_DisableIRQ(GPABC_IRQn);
	}

	if ((port == GPIOD) || (port == GPIOE) || (port == GPIOF))
	{
		NVIC_DisableIRQ(GPDEF_IRQn);
	}
}

/**   
  * @brief  GPIO_EnableEINT0, Enable the interrupt function for external GPIO interrupt.
  * @param  port: Specify GPIO port, GPIOA ~ GPIOF.
  * @param  bit : Specify pin of the GPIO port. 0~15.
  * @param  TriggerType : Specify the interrupt trigger type.
  * @param  Mode : Specify the interrupt mode.
  * @retval None.
  */
void GPIO_EnableEINT0(GPIO_TypeDef *port, uint32_t bit, uint32_t TriggerType, uint32_t Mode)
{
	port->IER |= TriggerType;
	port->IMD = (port->IMD & ~(1 << bit) | Mode);
	NVIC_EnableIRQ(EINT0_IRQn);
}

/**   
  * @brief  GPIO_DisableEINT0, Disable the interrupt function for external GPIO interrupt.
  * @param  port: Specify GPIO port, GPIOA ~ GPIOF.
  * @param  bit : Specify pin of the GPIO port. 0~15.
  * @retval None.
  */
void GPIO_DisableEINT0(GPIO_TypeDef *port, uint32_t bit)
{
	port->IER &= (~((1 << bit) | (1 << (bit + 16))));
	NVIC_DisableIRQ(EINT0_IRQn);
}

/**   
  * @brief  GPIO_EnableEINT1, Enable the interrupt function for external GPIO interrupt.
  * @param  port: Specify GPIO port, GPIOA ~ GPIOF.
  * @param  bit : Specify pin of the GPIO port. 0~15.
  * @param  TriggerType : Specify the interrupt trigger type.
  * @param  Mode : Specify the interrupt mode.
  * @retval None.
  */
void GPIO_EnableEINT1(GPIO_TypeDef *port, uint32_t bit, uint32_t TriggerType, uint32_t Mode)
{
	port->IER |= TriggerType;
	port->IMD = (port->IMD & ~(1 << bit) | Mode);
	NVIC_EnableIRQ(EINT1_IRQn);
}

/**   
  * @brief  GPIO_DisableEINT1, Disable the interrupt function for external GPIO interrupt.
  * @param  port: Specify GPIO port, GPIOA ~ GPIOF.
  * @param  bit : Specify pin of the GPIO port. 0~15.
  * @retval None.
  */
void GPIO_DisableEINT1(GPIO_TypeDef *port, uint32_t bit)
{
	port->IER &= (~((1 << bit) | (1 << (bit + 16))));
	NVIC_DisableIRQ(EINT1_IRQn);
}

/*@}*/ /* end of group NANO1xx_GPIO_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO1xx_GPIO_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
