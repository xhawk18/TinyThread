/******************************************************************************
 * @file     nano1xx_isr.c
 * @brief    This file contains interrupt service routines
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


#include <stdint.h>
#include "nano1xx.h"

extern volatile uint8_t gu8AdcIntFlag;
extern volatile uint8_t gu8AdcCmp0IntFlag;
extern volatile uint8_t gu8AdcCmp1IntFlag;

/**
  * @brief  ADC IRQ handler
  * @param  None
  * @return None
  */
void ADC_IRQHandler(void)
{

        uint32_t reg = ADC->SR;

        if(reg & 1)
                gu8AdcIntFlag = 1;
        if(reg & 2)
                gu8AdcCmp0IntFlag = 1;
        if(reg & 4)
                gu8AdcCmp1IntFlag = 1;

        ADC->SR = reg & 0x7;
        return;

}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

