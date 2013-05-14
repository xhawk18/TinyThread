/******************************************************************************
 * @file     nano1xx_gpio.h
 * @brief    NANO1xx GPIO driver header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __NANO1XX_GPIO_H
#define __NANO1XX_GPIO_H

#include "nano1xx.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_GPIO_Driver GPIO Driver
  @{
*/


/******************************************************************************/
/*                GPIO Specific inline                                        */
/******************************************************************************/
/** @addtogroup NANO1xx_GPIO_EXPORTED_FUNCTIONS GPIO Exported Functions
  @{
*/

/**   
  * @brief  GPIO Initial, Enable clock.
  * @param  None.
  * @retval None.
  */
static __INLINE void GPIO_Init()
{
	CLK->AHBCLK |= CLK_AHBCLK_GPIO_EN;
}

/**   
  * @brief  GPIO DeInit, Disable clock.
  * @param  None.
  * @retval None.
  */
static __INLINE void GPIO_DeInit()
{
	CLK->AHBCLK &= ~CLK_AHBCLK_GPIO_EN;
}

/**   
  * @brief  GPIO Open, Specific GPIO bit operation mode.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  mode: specific operation mode
  * @param  mask: operation mode mask.
  * @retval None.
  */
static __INLINE void GPIO_Open(GPIO_TypeDef *port, uint32_t mode, uint32_t mask)
{
	port->PMD = (port->PMD & ~mask | mode);
}

/**   
  * @brief  GPIO Close, Close specific GPIO bit.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  bit : specific bit of GPIO port, 0 ~ 15.
  * @retval None.
  */
static __INLINE void GPIO_Close(GPIO_TypeDef *port, uint32_t bit)
{
	port->PMD &= ~(0x3<<(bit<<1));
}

/**   
  * @brief  GPIO_SetBit, Set the specified GPIO pin to 1.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  bit : specific bit of GPIO port, 0 ~ 15.
  * @retval None.
  */
static __INLINE void GPIO_SetBit(GPIO_TypeDef *port, uint32_t bit)
{
	port->DOUT |= (1 << bit);
}

/**   
  * @brief  GPIO_GetBit, Get the pin value from the specified input GPIO pin.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  bit : specific bit of GPIO port, 0 ~ 15.
  * @retval The specified input pin value.
  */
static __INLINE uint32_t GPIO_GetBit(GPIO_TypeDef *port, uint32_t bit)
{
	return ((port->PIN >> bit) & 0x1);
}

/**   
  * @brief  GPIO_ClrBit, Set the specific GPIO pin to 0.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  bit : specific bit of GPIO port, 0 ~ 15.
  * @retval None.
  */
static __INLINE void GPIO_ClrBit(GPIO_TypeDef *port, uint32_t bit)
{
	port->DOUT &= ~(1 << bit);
}

/**   
  * @brief  GPIO_SetPortBits, Set output value to the specified GPIO port.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  data: The data output value. It could be 0~0xFFFF.
  * @retval None.
  */
static __INLINE void GPIO_SetPortBits(GPIO_TypeDef *port, uint32_t data)
{
	port->DOUT = data;
}

/**   
  * @brief  GPIO_GetPortBits, Get the input port value from the specified GPIO port.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @retval The specified input port value.
  */
static __INLINE uint32_t GPIO_GetPortBits(GPIO_TypeDef *port)
{
	return (port->PIN);
}

/**   
  * @brief  GPIO_GetDoutBit, Get the bit value from the specified Data Output Value Register.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  bit : specific bit of GPIO port, 0 ~ 15.
  * @retval The bit value of the specified register.
  */
static __INLINE uint32_t GPIO_GetDoutBit(GPIO_TypeDef *port, uint32_t bit)
{
	return ((port->DOUT >> bit) & 0x1);
}

/**   
  * @brief  GPIO_GetPortDoutBits, Get the port value from the specified Data Output Value Register.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @retval The portt value of the specified register.
  */
static __INLINE uint32_t GPIO_GetPortDoutBits(GPIO_TypeDef *port)
{
	return (port->DOUT);
}

/**   
  * @brief  GPIO_SetBitMask, This function is used to protect the write data function of the 
  *         corresponding GPIO pin. When set the bit mask, the write signal is masked and write 
  *         data to the protect bit is ignored.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  bit : specific bit of GPIO port, 0 ~ 15.
  * @retval None.
  */
static __INLINE void GPIO_SetBitMask(GPIO_TypeDef *port, uint32_t bit)
{
	port->DMASK |= (1 << bit);
}

/**   
  * @brief  GPIO_GetBitMask, Get the bit value from the specified Data Output Write Mask Register 
  *         If the bit value is 1, it's meaning the corresponding bit is protected. 
  *         And write data to the bit is ignored.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  bit : specific bit of GPIO port, 0 ~ 15.
  * @retval The bit value of the specified register.
  */
static __INLINE uint32_t GPIO_GetBitMask(GPIO_TypeDef *port, uint32_t bit)
{
	return ((port->DMASK >> bit) & 0x1);
}

/**   
  * @brief  GPIO_ClrBitMask, This function is used to remove the write protect function of the the 
  *         corresponding GPIO pin. After remove the bit mask, write  data to the 
  *         corresponding bit is workable.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  bit : specific bit of GPIO port, 0 ~ 15.
  * @retval None.
  */
static __INLINE void GPIO_ClrBitMask(GPIO_TypeDef *port, uint32_t bit)
{
	port->DMASK &= ~(1 << bit);
}

/**   
  * @brief  GPIO_SetPortMask, This function is used to protect the write data function of the 
  *         corresponding GPIO pin. When set the bits are masked, write data to the protect 
  *          bits are ignored.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  data: Specify pins of the GPIO port. It could be 0~0xFFFF.
  * @retval None.
  */
static __INLINE void GPIO_SetPortMask(GPIO_TypeDef *port, uint32_t data)
{
	port->DMASK |= data;
}

/**   
  * @brief  GPIO_GetPortMask, Get the port value from the specified Data Output Write Mask Register. 
  *         If the bit value is 1, it's meaning the corresponding bit is protected. 
  *         And write data to the bit is ignored.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @retval The port value of the specified register.
  */
static __INLINE uint32_t GPIO_GetPortMask(GPIO_TypeDef *port)
{
	return (port->DMASK);
}

/**   
  * @brief  GPIO_ClrPortMask, This function is used to remove the write protect function of the the 
  *         corresponding GPIO pin. After remove the bit mask, write  data to the 
  *         corresponding bit is workable.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  data: Specify pins of the GPIO port. It could be 0~0xFFFF.
  * @retval None.
  */
static __INLINE void GPIO_ClrPortMask(GPIO_TypeDef *port, uint32_t data)
{
	port->DMASK &= ~data;
}

/**   
  * @brief  GPIO_EnablePullup, Enable the pullup resistor of the specified GPIO input pin. 
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  bit : specific bit of GPIO port, 0 ~ 15.
  * @retval None.
  */
static __INLINE void GPIO_EnablePullup(GPIO_TypeDef *port, uint32_t bit)
{
	port->PUEN |= (1 << bit);
}

/**   
  * @brief  GPIO_DisablePullup, Disable the pullup resistor of the specified GPIO input pin. 
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  bit : specific bit of GPIO port, 0 ~ 15.
  * @retval None.
  */
static __INLINE void GPIO_DisablePullup(GPIO_TypeDef *port, uint32_t bit)
{
	port->PUEN &= ~(1 << bit);
}

/**   
  * @brief  GPIO_EnableDebounce, Enable the debounce function of the specified GPIO input pin. 
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  bit : specific bit of GPIO port, 0 ~ 15.
  * @retval None.
  */
static __INLINE void GPIO_EnableDebounce(GPIO_TypeDef *port, uint32_t bit)
{
	GPIODBNCE->CON |= GPIO_DBNCECON_ICLK_ON;
	port->DBEN |= (1 << bit);
}

/**   
  * @brief  GPIO_DisableDebounce, Disable the debounce function of the specified GPIO input pin. 
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @param  bit : specific bit of GPIO port, 0 ~ 15.
  * @retval None.
  */
static __INLINE void GPIO_DisableDebounce(GPIO_TypeDef *port, uint32_t bit)
{
	port->DBEN &= ~(1 << bit);
}

/**   
  * @brief  GPIO_SetDebounceTime, Set the interrupt debounce sampling time
  *         based on the debounce counter clock source. 
  * @param  CycleSelection: The number of sampling cycle selection.
  * @param  ClockSource   : HCLK or 10K.
  * @retval None.
  */
static __INLINE void GPIO_SetDebounceTime(uint32_t CycleSelection, uint32_t ClockSource)
{
	GPIODBNCE->CON |= (CycleSelection|ClockSource) ;
}

/**   
  * @brief  GPIO_GetDebounceSampleCycle, This function is used to get the number
  *         of debounce sampling cycle selection. 
  * @param  None.
  * @retval Number of the sampling cycle selection.
  */
static __INLINE uint32_t GPIO_GetDebounceSampleCycle()
{
	return (GPIODBNCE->CON & GPIO_DBNCECON_DBCLKSEL_MASK);
}

/**   
  * @brief  GPIO_GetIntStatus, Get the port value from the specified Interrupt Trigger Source  
  *         Indicator Register. If the corresponding bit of the return port value is 1, it's meaning the 
  *         interrupt occurred at the corresponding bit. Otherwise, no interrupt occurred at that bit.
  * @param  port: specific GPIO port, GPIOA ~ GPIOF.
  * @retval The portt value of the specified register.
  */
static __INLINE uint32_t GPIO_GetIntStatus(GPIO_TypeDef *port)
{
	return (port->ISR);
}

/*------------------------------------------------------------*/
void GPIO_EnableInt(GPIO_TypeDef *port, uint32_t bit, uint32_t TriggerType, uint32_t Mode);
void GPIO_DisableInt(GPIO_TypeDef *port, uint32_t bit);
void GPIO_EnableEINT0(GPIO_TypeDef *port, uint32_t bit, uint32_t TriggerType, uint32_t Mode);
void GPIO_DisableEINT0(GPIO_TypeDef *port, uint32_t bit);
void GPIO_EnableEINT1(GPIO_TypeDef *port, uint32_t bit, uint32_t TriggerType, uint32_t Mode);
void GPIO_DisableEINT1(GPIO_TypeDef *port, uint32_t bit);

/*@}*/ /* end of group NANO1xx_GPIO_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_GPIO_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/







