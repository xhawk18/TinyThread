/******************************************************************************
 * @file     nano1xx_pdma.h
 * @brief    NANO1xx PDMA driver header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __NANO1XX_PDMA_H
#define __NANO1XX_PDMA_H

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "nano1xx.h"


#ifdef  __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_PDMA_Driver PDMA Driver
  @{
*/


/******************************************************************************/
/*                DMA Specific Macros                                         */
/******************************************************************************/
/** @addtogroup NANO1xx_PDMA_EXPORTED_CONSTANTS PDMA Exported Constants
  @{
*/

/*!<Enable PDMA Channel 1 Service Selection */
#define PDMA_SPI0_TX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_SPI0_TX))
#define PDMA_SPI1_TX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_SPI1_TX))
#define PDMA_UART0_TX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_UART0_TX))
#define PDMA_UART1_TX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_UART1_TX))
#define PDMA_USB_TX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_USB_TX))
#define PDMA_I2S_TX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_I2S_TX))
#define PDMA_DAC0_TX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_DAC0_TX))
#define PDMA_DAC1_TX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_DAC1_TX))
#define PDMA_SPI2_TX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_SPI2_TX))
#define PDMA_TMR0_CH0_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_TMR0_CH0))
#define PDMA_TMR0_CH1_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_TMR0_CH1))
#define PDMA_TMR1_CH0_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_TMR1_CH0))
#define PDMA_TMR1_CH1_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_TMR1_CH1))

#define PDMA_SPI0_RX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_SPI0_RX))
#define PDMA_SPI1_RX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_SPI1_RX))
#define PDMA_UART0_RX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_UART0_RX))
#define PDMA_UART1_RX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_UART1_RX))
#define PDMA_USB_RX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_USB_RX))
#define PDMA_I2S_RX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_I2S_RX))
#define PDMA_ADC_TO_CH1()		((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_ADC))
#define PDMA_SPI2_RX_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_SPI2_RX))
#define PDMA_PWM0_CH0_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_PWM0_CH0))
#define PDMA_PWM0_CH2_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_PWM0_CH2))
#define PDMA_PWM1_CH0_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_PWM1_CH0))
#define PDMA_PWM1_CH2_TO_CH1()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH1_MASK | PDMA_CH1_PWM1_CH2))
#define PDMA_DISABLE_CH1()		((uint32_t)(PDMAGCR->PDSSR0 |= PDMA_CH1_DISABLE))

/*!<Enable PDMA Channel 2 Service Selection */
#define PDMA_SPI0_TX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_SPI0_TX))
#define PDMA_SPI1_TX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_SPI1_TX))
#define PDMA_UART0_TX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_UART0_TX))
#define PDMA_UART1_TX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_UART1_TX))
#define PDMA_USB_TX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_USB_TX))
#define PDMA_I2S_TX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_I2S_TX))
#define PDMA_DAC0_TX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_DAC0_TX))
#define PDMA_DAC1_TX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_DAC1_TX))
#define PDMA_SPI2_TX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_SPI2_TX))
#define PDMA_TMR0_CH0_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_TMR0_CH0))
#define PDMA_TMR0_CH1_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_TMR0_CH1))
#define PDMA_TMR1_CH0_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_TMR1_CH0))
#define PDMA_TMR1_CH1_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_TMR1_CH1))

#define PDMA_SPI0_RX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_SPI0_RX))
#define PDMA_SPI1_RX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_SPI1_RX))
#define PDMA_UART0_RX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_UART0_RX))
#define PDMA_UART1_RX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_UART1_RX))
#define PDMA_USB_RX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_USB_RX))
#define PDMA_I2S_RX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_I2S_RX))
#define PDMA_ADC_TO_CH2()		((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_ADC))
#define PDMA_SPI2_RX_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_SPI2_RX))
#define PDMA_PWM0_CH0_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_PWM0_CH0))
#define PDMA_PWM0_CH2_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_PWM0_CH2))
#define PDMA_PWM1_CH0_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_PWM1_CH0))
#define PDMA_PWM1_CH2_TO_CH2()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH2_MASK | PDMA_CH2_PWM1_CH2))
#define PDMA_DISABLE_CH2()		((uint32_t)(PDMAGCR->PDSSR0 |= PDMA_CH2_DISABLE))

/*!<Enable PDMA Channel 3 Service Selection */
#define PDMA_SPI0_TX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_SPI0_TX))
#define PDMA_SPI1_TX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_SPI1_TX))
#define PDMA_UART0_TX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_UART0_TX))
#define PDMA_UART1_TX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_UART1_TX))
#define PDMA_USB_TX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_USB_TX))
#define PDMA_I2S_TX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_I2S_TX))
#define PDMA_DAC0_TX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_DAC0_TX))
#define PDMA_DAC1_TX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_DAC1_TX))
#define PDMA_SPI2_TX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_SPI2_TX))
#define PDMA_TMR0_CH0_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_TMR0_CH0))
#define PDMA_TMR0_CH1_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_TMR0_CH1))
#define PDMA_TMR1_CH0_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_TMR1_CH0))
#define PDMA_TMR1_CH1_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_TMR1_CH1))

#define PDMA_SPI0_RX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_SPI0_RX))
#define PDMA_SPI1_RX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_SPI1_RX))
#define PDMA_UART0_RX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_UART0_RX))
#define PDMA_UART1_RX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_UART1_RX))
#define PDMA_USB_RX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_USB_RX))
#define PDMA_I2S_RX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_I2S_RX))
#define PDMA_ADC_TO_CH3()		((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_ADC))
#define PDMA_SPI2_RX_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_SPI2_RX))
#define PDMA_PWM0_CH0_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_PWM0_CH0))
#define PDMA_PWM0_CH2_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_PWM0_CH2))
#define PDMA_PWM1_CH0_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_PWM1_CH0))
#define PDMA_PWM1_CH2_TO_CH3()	((uint32_t)(PDMAGCR->PDSSR0 = PDMAGCR->PDSSR0 & ~PDMA_CH3_MASK | PDMA_CH3_PWM1_CH2))
#define PDMA_DISABLE_CH3()		((uint32_t)(PDMAGCR->PDSSR0 |= PDMA_CH3_DISABLE))

/*!<Enable PDMA Channel 4 Service Selection */
#define PDMA_SPI0_TX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_SPI0_TX))
#define PDMA_SPI1_TX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_SPI1_TX))
#define PDMA_UART0_TX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_UART0_TX))
#define PDMA_UART1_TX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_UART1_TX))
#define PDMA_USB_TX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_USB_TX))
#define PDMA_I2S_TX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_I2S_TX))
#define PDMA_DAC0_TX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_DAC0_TX))
#define PDMA_DAC1_TX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_DAC1_TX))
#define PDMA_SPI2_TX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_SPI2_TX))
#define PDMA_TMR0_CH0_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_TMR0_CH0))
#define PDMA_TMR0_CH1_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_TMR0_CH1))
#define PDMA_TMR1_CH0_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_TMR1_CH0))
#define PDMA_TMR1_CH1_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_TMR1_CH1))

#define PDMA_SPI0_RX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_SPI0_RX))
#define PDMA_SPI1_RX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_SPI1_RX))
#define PDMA_UART0_RX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_UART0_RX))
#define PDMA_UART1_RX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_UART1_RX))
#define PDMA_USB_RX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_USB_RX))
#define PDMA_I2S_RX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_I2S_RX))
#define PDMA_ADC_TO_CH4()		((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_ADC))
#define PDMA_SPI2_RX_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_SPI2_RX))
#define PDMA_PWM0_CH0_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_PWM0_CH0))
#define PDMA_PWM0_CH2_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_PWM0_CH2))
#define PDMA_PWM1_CH0_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_PWM1_CH0))
#define PDMA_PWM1_CH2_TO_CH4()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH4_MASK | PDMA_CH4_PWM1_CH2))
#define PDMA_DISABLE_CH4()		((uint32_t)(PDMAGCR->PDSSR1 |= PDMA_CH4_DISABLE))

/*!<Enable PDMA Channel 5 Service Selection */
#define PDMA_SPI0_TX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_SPI0_TX))
#define PDMA_SPI1_TX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_SPI1_TX))
#define PDMA_UART0_TX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_UART0_TX))
#define PDMA_UART1_TX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_UART1_TX))
#define PDMA_USB_TX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_USB_TX))
#define PDMA_I2S_TX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_I2S_TX))
#define PDMA_DAC0_TX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_DAC0_TX))
#define PDMA_DAC1_TX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_DAC1_TX))
#define PDMA_SPI2_TX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_SPI2_TX))
#define PDMA_TMR0_CH0_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_TMR0_CH0))
#define PDMA_TMR0_CH1_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_TMR0_CH1))
#define PDMA_TMR1_CH0_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_TMR1_CH0))
#define PDMA_TMR1_CH1_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_TMR1_CH1))

#define PDMA_SPI0_RX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_SPI0_RX))
#define PDMA_SPI1_RX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_SPI1_RX))
#define PDMA_UART0_RX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_UART0_RX))
#define PDMA_UART1_RX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_UART1_RX))
#define PDMA_USB_RX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_USB_RX))
#define PDMA_I2S_RX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_I2S_RX))
#define PDMA_ADC_TO_CH5()		((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_ADC))
#define PDMA_SPI2_RX_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_SPI2_RX))
#define PDMA_PWM0_CH0_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_PWM0_CH0))
#define PDMA_PWM0_CH2_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_PWM0_CH2))
#define PDMA_PWM1_CH0_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_PWM1_CH0))
#define PDMA_PWM1_CH2_TO_CH5()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH5_MASK | PDMA_CH5_PWM1_CH2))
#define PDMA_DISABLE_CH5()		((uint32_t)(PDMAGCR->PDSSR1 |= PDMA_CH5_DISABLE))

/*!<Enable PDMA Channel 6 Service Selection */
#define PDMA_SPI0_TX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_SPI0_TX))
#define PDMA_SPI1_TX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_SPI1_TX))
#define PDMA_UART0_TX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_UART0_TX))
#define PDMA_UART1_TX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_UART1_TX))
#define PDMA_USB_TX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_USB_TX))
#define PDMA_I2S_TX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_I2S_TX))
#define PDMA_DAC0_TX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_DAC0_TX))
#define PDMA_DAC1_TX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_DAC1_TX))
#define PDMA_SPI2_TX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_SPI2_TX))
#define PDMA_TMR0_CH0_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_TMR0_CH0))
#define PDMA_TMR0_CH1_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_TMR0_CH1))
#define PDMA_TMR1_CH0_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_TMR1_CH0))
#define PDMA_TMR1_CH1_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_TMR1_CH1))

#define PDMA_SPI0_RX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_SPI0_RX))
#define PDMA_SPI1_RX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_SPI1_RX))
#define PDMA_UART0_RX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_UART0_RX))
#define PDMA_UART1_RX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_UART1_RX))
#define PDMA_USB_RX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_USB_RX))
#define PDMA_I2S_RX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_I2S_RX))
#define PDMA_ADC_TO_CH6()		((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_ADC))
#define PDMA_SPI2_RX_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_SPI2_RX))
#define PDMA_PWM0_CH0_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_PWM0_CH0))
#define PDMA_PWM0_CH2_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_PWM0_CH2))
#define PDMA_PWM1_CH0_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_PWM1_CH0))
#define PDMA_PWM1_CH2_TO_CH6()	((uint32_t)(PDMAGCR->PDSSR1 = PDMAGCR->PDSSR1 & ~PDMA_CH6_MASK | PDMA_CH6_PWM1_CH2))
#define PDMA_DISABLE_CH6()		((uint32_t)(PDMAGCR->PDSSR1 |= PDMA_CH6_DISABLE))


/*@}*/ /* end of group NANO1xx_PDMA_EXPORTED_CONSTANTS */



/******************************************************************************/
/*                DMA Specific Structure                                      */
/******************************************************************************/
/** @addtogroup NANO1xx_PDMA_EXPORTED_STRUCTS PDMA Exported Structs
  @{
*/

/*!<PDMA Channel Structure */
typedef struct DRVPDMA_STRUCT		/* PDMA structure */
{
    uint32_t	SrcCtrl;    	    /* Source Transfer direction */
	uint32_t	SrcAddr;			/* Source Start Address */
	uint32_t	DestCtrl;		    /* Destination Transfer direction */
	uint32_t	DestAddr;			/* Destination Start Address */
    uint32_t	TransWidth;			/* Transfer Width */
    uint32_t	Mode;				/* Operation Mode */
	uint32_t	ByteCnt;			/* Byte Count */
    uint32_t	IsTimeOutEnable;	/* Enable Timeout function */
}STR_PDMA_T;

/*!<VDMA Channel Structure */
typedef struct DRVVDMA_STRUCT		/* VDMA structure */
{
	uint32_t	Dir;
    uint32_t	SrcAddr;			/* Start Address */
    uint32_t	DestAddr;			/* Start Address */
	uint32_t	ByteCnt;			/* Byte Count */
    uint32_t	IsStrideEnable;		/* Stride Mode Enable */
	uint16_t	u16Stbc;			/* Stride Transfer Byte Count */
	uint16_t	u16Sastobl;			/* Source Address Stride Offset Byte Length */
	uint16_t	u16Dastobl;			/* Destination Address Stride Offset Byte Length */
}STR_VDMA_T;

typedef struct DRVCRC_STRUCT			/* CRC structure */
{
	uint32_t	u32Mode;
    uint32_t	u32CPUWdlen;			/* CPU write data length */
    uint8_t		bIsChecksumComplement;	/* Checksum Complement */
    uint8_t		bIsWriteDataComplement;	/* Write Data Complement */
    uint8_t		bIsChecksumReverse;		/* Checksum Reverse */
    uint8_t		bIsWriteDataReverse;	/* Write Data Order Reverse */
    uint32_t	u32DMASrcAddr;			/* DMA Start Address */
	uint16_t	u16DMAByteCnt;			/* DMA Byte Count */
}STR_CRC_T;


/*@}*/ /* end of group NANO1xx_PDMA_EXPORTED_STRUCTS */


/******************************************************************************/
/*                DMA Specific inline                                         */
/******************************************************************************/
/** @addtogroup NANO1xx_PDMA_EXPORTED_FUNCTIONS PDMA Exported Functions
  @{
*/

/**   
  * @brief  DMA Initial, Enable clock.
  * @param  None.
  * @retval None.
  */
static __INLINE void PDMA_Init()
{
	CLK->AHBCLK |= CLK_AHBCLK_DMA_EN;
}

/**   
  * @brief  DMA DeInit, Disable clock.
  * @param  None.
  * @retval None.
  */
static __INLINE void PDMA_DeInit()
{
	CLK->AHBCLK &= ~CLK_AHBCLK_DMA_EN;
}

/**   
  * @brief  PDMA_EnableChannelClock, Enable Channel Clock.
  * @param  ch: initial PDMA channel.
  * @retval None.
  */
static __INLINE void PDMA_EnableChannelClock(uint32_t ch)
{
	PDMAGCR->CSR |= ch;
}

/**   
  * @brief  PDMA_SetTransferSetting, configures PDMA setting.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @param  u32SrcAddr: Source Address.
  * @param  u32DestAddr: Destination Address.
  * @param  u32TransferLength: Byte Count.
  * @retval None.
  */
static __INLINE void PDMA_SetTransferSetting(PDMA_TypeDef *ch, uint32_t u32SrcAddr, uint32_t u32DestAddr, uint32_t u32TransferLength)
{
	ch->SAR = u32SrcAddr;			/* Set Source Address */
	ch->DAR = u32DestAddr;			/* Set Destination Address */
	ch->BCR = u32TransferLength;	/* Set Byte Count Register */
}

/**   
  * @brief  PDMA_IsCHBusy, Get Channel Enable/Disable status.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @retval 1: The Channel is busy; 0: The Channel is usable.
  */
static __INLINE int32_t PDMA_IsCHBusy(PDMA_TypeDef *ch)
{
	return (ch->CSR & PDMA_CSR_TRIG_EN)? TRUE: FALSE;
}

/**   
  * @brief  PDMA_GetTransferLength, Get channel transfer length setting.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @retval transfer length.
  */
static __INLINE uint32_t PDMA_GetTransferLength(PDMA_TypeDef *ch)
{
	return (ch->BCR);
}

/**   
  * @brief  PDMA_SetAPBTransferWidth, set APB transfer width.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @param  TransferWidth: Transfer width.
  * @retval None.
  */
static __INLINE void PDMA_SetAPBTransferWidth(PDMA_TypeDef *ch, uint32_t TransferWidth)
{
	ch->CSR = ch->CSR & ~PDMA_CSR_APB_TWS_MASK | TransferWidth;
}

/**   
  * @brief  PDMA_CHSoftwareReset, do software reset Channel.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @retval None.
  */
static __INLINE void PDMA_CHSoftwareReset(PDMA_TypeDef *ch)
{
	ch->CSR |= PDMA_CSR_SWRST;
}

/**   
  * @brief  PDMA_CHEnableTransfer, enable PDMA channelx and trigger channel.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @retval None.
  */
static __INLINE void PDMA_CHEnableTransfer(PDMA_TypeDef *ch)
{
	ch->CSR |= (PDMA_CSR_CEN|PDMA_CSR_TRIG_EN);
}

/**   
  * @brief  PDMA_CHDisableTransfer, disable PDMA channelx and trigger channel.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @retval None.
  */
static __INLINE void PDMA_CHDisableTransfer(PDMA_TypeDef *ch)
{
	ch->CSR |= PDMA_CSR_SWRST;
	ch->CSR &= ~PDMA_CSR_CEN;
}

/**   
  * @brief  PDMA_DisableInt, Disable Interrupt for channel.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @param  IntSource: Intterrupt source.
  * @retval None.
  */

static __INLINE void PDMA_DisableInt(PDMA_TypeDef *ch, uint32_t IntSource)
{
	ch->IER &= ~IntSource;
}

/**   
  * @brief  PDMA_IsIntEnabled, Disable Interrupt for channel.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @param  IntSource: Intterrupt source.
  * @retval 1: The Specified Source Interrupt is enable.
  * @retval 0: The Specified Source Interrupt is disable.
  */

static __INLINE uint32_t PDMA_IsIntEnabled(PDMA_TypeDef *ch, uint32_t IntSource)
{
	return (ch->IER & IntSource)? TRUE: FALSE;
}

/**   
  * @brief  PDMA_ClearIntFlag, clear interrupt status for channel.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @param  IntFlag: Intterrupt source.
  * @retval None.
  */

static __INLINE void PDMA_ClearIntFlag(PDMA_TypeDef *ch, uint32_t IntFlag)
{
	ch->ISR |= IntFlag;
}

/**   
  * @brief  PDMA_PollInt, polling Interrupt for channel.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @param  IntFlag: Intterrupt source.
  * @retval 1: The Source Interrupt Flag is set .
  * @retval 0: The Source Interrupt Flag is clear.
  */

static __INLINE uint32_t PDMA_PollInt(PDMA_TypeDef *ch, uint32_t IntFlag)
{
	return (ch->ISR & IntFlag)? TRUE: FALSE;
}

/**   
  * @brief  PDMA_GetSharedBufData, Get shared buffer content for channel.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @retval Shard Buffer Content register.
  */

static __INLINE uint32_t PDMA_GetSharedBufData(PDMA_TypeDef *ch)
{
	return ch->BUF0;
}

/**   
  * @brief  PDMA_SetTimeOutCount, set timeout count to dma channel.
  *         The reference clock is internal 10K. The clock is (count * (1/10K)) second.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @param  Count: timeout Count: 0 ~ 0xffff.
  * @retval None.
  */
static __INLINE void PDMA_SetTimeOutCount(PDMA_TypeDef *ch, uint32_t Count)
{
	ch->TCR = Count;
}

/**   
  * @brief  PDMA_GetCurrentSourceAddr, Get current source address from channel.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @retval Current Source Address.
  */

static __INLINE uint32_t PDMA_GetCurrentSourceAddr(PDMA_TypeDef *ch)
{
	return ch->CSAR;
}

/**   
  * @brief  PDMA_GetCurrentDestAddr, Get current destination address from channel.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @retval Current destination Address.
  */

static __INLINE uint32_t PDMA_GetCurrentDestAddr(PDMA_TypeDef *ch)
{
	return ch->CDAR;
}

/**   
  * @brief  PDMA_GetRemainTransferCount, Get current remained byte count of channel.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @retval Current remained byte count.
  */

static __INLINE uint32_t PDMA_GetRemainTransferCount(PDMA_TypeDef *ch)
{
	return ch->CBCR;
}

/**   
  * @brief  PDMA_SetVDMATransferSetting, configures VDMA setting.
  * @param  u32Stbc: VDMA Stride Transfer Byte Count (0 ~ 0xffff).
  * @param  u32Sastobl: VDMA Source Address Stride Offset Byte Length (0 ~ 0xffff).
  * @param  u32Dastobl: VDMA Destination Address Stride Offset Byte Length (0 ~ 0xffff).
  * @retval None.
  */
static __INLINE void PDMA_SetVDMATransferSetting(uint32_t u32Stbc, uint32_t u32Sastobl, uint32_t u32Dastobl)
{
	VDMA->SASOCR = (u32Stbc << 16) | u32Sastobl;
	VDMA->DASOCR = u32Dastobl;			/* Set Destination Address Stride Offset Byte Length */
}

/**   
  * @brief  PDMA_SetCRCTransfer, configures CRC setting.
  * @param  u32SrcAddr: DMA source address.
  * @param  u16ByteCount: DMA transfer byte count.
  * @param  u32SeedValue: CRC seed.
  * @retval None.
  */
static __INLINE void PDMA_SetCRCTransfer(uint32_t u32SrcAddr, uint16_t u16ByteCount, uint32_t u32SeedValue)
{
	PDMACRC->DMASAR = u32SrcAddr;		/* Set Source Address */
	PDMACRC->DMABCR = u16ByteCount;		/* Set Byte Count Register */
	PDMACRC->SEED   = u32SeedValue;		/* Set Seed Register */
}

/**   
  * @brief  PDMA_IsCRCBusy, Get CRC Enable/Disable status.
  * @param  None.
  * @retval 1: The Channel is busy; 0: The Channel is usable.
  */
static __INLINE int32_t PDMA_IsCRCBusy()
{
	return (PDMACRC->CTL & PDMACRC_CTL_TRIG_EN)? TRUE: FALSE;
}

/**   
  * @brief  PDMA_SetCRCPolynomialMode, Set CRC Polynomial Mode.
  * @param  mode: CRC Polynomial Mode.
  * @retval None.
  */
static __INLINE void PDMA_SetCRCPolynomialMode(uint32_t mode)
{
	PDMACRC->CTL = PDMACRC->CTL & ~PDMACRC_CTL_MODE_MASK | mode;
}

/**   
  * @brief  PDMA_SetCRCCPUWriteDataLength, Set CRC CPU Write Data length.
  * @param  Wdlen: CPU Write Data length.
  * @retval None.
  */
static __INLINE void PDMA_SetCRCCPUWriteDataLength(uint32_t Wdlen)
{
	PDMACRC->CTL = PDMACRC->CTL & ~PDMACRC_CTL_CPU_WDLEN_MASK | Wdlen;
}

/**   
  * @brief  PDMA_CRCSoftwareReset, do software reset CRC.
  * @param  None.
  * @retval None.
  */
static __INLINE void PDMA_CRCSoftwareReset()
{
	PDMACRC->CTL |= PDMACRC_CTL_RST;
}

/**   
  * @brief  PDMA_CRCEnablelTransfer, enable and trigger CRC.
  * @param  None.
  * @retval None.
  */
static __INLINE void PDMA_CRCEnablelTransfer()
{
	PDMACRC->CTL |= (PDMA_CSR_CEN|PDMA_CSR_TRIG_EN);
}

/**   
  * @brief  PDMA_CRCDisableInt, Disable CRC Interrupt.
  * @param  IntSource: Intterrupt source.
  * @retval None.
  */

static __INLINE void PDMA_CRCDisableInt(uint32_t IntSource)
{
	PDMACRC->DMAIER &= ~IntSource;
}

/**   
  * @brief  PDMA_CRCClearIntFlag, clear CRC interrupt status.
  * @param  IntFlag: Intterrupt source.
  * @retval None.
  */

static __INLINE void PDMA_CRCClearIntFlag(uint32_t IntFlag)
{
	PDMACRC->DMAISR |= IntFlag;
}

/**   
  * @brief  PDMA_CRCPollInt, polling CRC Interrupt.
  * @param  IntFlag: Intterrupt source.
  * @retval 1: The Source Interrupt Flag is set .
  * @retval 0: The Source Interrupt Flag is clear.
  */

static __INLINE uint32_t PDMA_CRCPollInt(uint32_t IntFlag)
{
	return (PDMACRC->DMAISR & IntFlag)? TRUE: FALSE;
}

/**   
  * @brief  PDMA_CRCSetWriteData, write data into Write Data Register.
  * @param  u32Data Data to write into data register
  * @retval None.
  */
static __INLINE void PDMA_CRCSetWriteData(uint32_t u32Data)
{
	PDMACRC->WDATA = u32Data;
}

/**   
  * @brief  PDMA_CRCGetWriteData, read data from Write Data Register.
  * @param  None.
  * @retval read data from Write Data Register.
  */
static __INLINE uint32_t PDMA_CRCGetWriteData()
{
	return (PDMACRC->WDATA);
}

/**   
  * @brief  PDMA_CRCSetSeed, set CRC seed Register.
  * @param  u32Data CRC seed
  * @retval None.
  */
static __INLINE void PDMA_CRCSetSeed(uint32_t u32Data)
{
	PDMACRC->SEED = u32Data;
}

/**   
  * @brief  PDMA_CRCGetChecksum, read data from Write Data Register.
  * @param  None.
  * @retval read data from Write Data Register.
  */
static __INLINE uint32_t PDMA_CRCGetChecksum(void)
{
	return (PDMACRC->CHECKSUM & PDMACRC->SEED);
}


/***********************************************/
void PDMA_Close(void);
void PDMA_Open(PDMA_TypeDef *ch, STR_PDMA_T *sParam);
void PDMA_OpenVDMA(STR_VDMA_T *sParam);
void PDMA_EnableInt(PDMA_TypeDef *ch, uint32_t IntSource);
void PDMA_OpenCRC(STR_CRC_T *sParam);
void PDMA_CRCEnableInt(uint32_t IntSource);

/*@}*/ /* end of group NANO1xx_PDMA_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_PDMA_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

#ifdef  __cplusplus
}
#endif

#endif	// __NANO1XX_PDMA_H

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/













