/******************************************************************************
 * @file     nano1xx_timer.c
 * @brief    NANO1xx TIMER driver source file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "nano1xx_sys.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_TIMER_Driver TIMER Driver
  @{
*/


/** @addtogroup NANO1xx_TIMER_EXPORTED_FUNCTIONS TIMER Exported Functions
  @{
*/

/**
  * @brief  Initial timer channel
  * @param  timer Timer channel to configure
  * @param  u8Prescale Prescale value
  * @param  u32Compare Comparator value, valid valies are between 2~0xFFFFFF
  * @param  u32Mode Timer operating mode
  * @return None
  */
void TIMER_Init(TIMER_TypeDef *timer, uint8_t u8Prescale, uint32_t u32Compare, uint32_t u32Mode)
{
	assert_param((u32Compare > 1) && (u32Compare < 0x1000000));
	assert_param((u32Mode & ~TIMER_CTL_MODESEL_MASK) == 0);

	if(timer == TIMER0)
		CLK->APBCLK |= CLK_APBCLK_TMR0_EN;
	else if(timer == TIMER1)
		CLK->APBCLK |= CLK_APBCLK_TMR1_EN;
	else if(timer == TIMER2)
		CLK->APBCLK |= CLK_APBCLK_TMR2_EN;
	else
		CLK->APBCLK |= CLK_APBCLK_TMR3_EN;

	timer->CTL = 0;
	timer->ISR = (TIMER_ISR_TMRIS | TIMER_ISR_TCAPIS | TIMER_ISR_TMRWAKESTS | TIMER_ISR_NCAPDETSTS);
	timer->PRECNT = u8Prescale;
	timer->CMPR = u32Compare;
	timer->CTL = u32Mode;
        return;
}

/**
  * @brief  De-init timer channel
  * @param  timer Timer channel to disable
  * @return None
  */
void TIMER_DeInit(TIMER_TypeDef *timer)
{
	timer->CTL = TIMER_CTL_SWRST;
	while(timer->CTL & TIMER_CTL_SWRST);
	timer->IER = 0;

	if(timer == TIMER0) {
		CLK->APBCLK &= ~CLK_APBCLK_TMR0_EN;
		NVIC_DisableIRQ(TMR0_IRQn);
	} else if(timer == TIMER1) {
		CLK->APBCLK &= ~CLK_APBCLK_TMR1_EN;
		NVIC_DisableIRQ(TMR1_IRQn);
	} else if(timer == TIMER2) {
		CLK->APBCLK &= ~CLK_APBCLK_TMR2_EN;
		NVIC_DisableIRQ(TMR2_IRQn);
	} else {
		CLK->APBCLK &= ~CLK_APBCLK_TMR3_EN;
		NVIC_DisableIRQ(TMR3_IRQn);
	}

}

/**
  * @brief  Enable timer interrupt
  * @param  timer Timer channel to configure
  * @param  u32Mask Interrupt mask to enable, a combination of TIMER_IER_TMRIE and TIMER_IER_TMRIE
  * @return None
  */
void TIMER_EnableInt(TIMER_TypeDef *timer, uint32_t u32Mask)
{
        assert_param(u32Mask != 0);
        assert_param((u32Mask & ~(TIMER_IER_TMRIE | TIMER_IER_TCAPIE)) == 0);

        timer->ISR = timer->ISR;  // clear pending interrupt
        timer->IER |= u32Mask;

	if(timer == TIMER0) {
		NVIC_EnableIRQ(TMR0_IRQn);
	} else if(timer == TIMER1) {
		NVIC_EnableIRQ(TMR1_IRQn);
	} else if(timer == TIMER2) {
		NVIC_EnableIRQ(TMR2_IRQn);
	} else {
		NVIC_EnableIRQ(TMR3_IRQn);
	}
}

/**
  * @brief  Disable timer interrupt
  * @param  timer Timer channel to configure
  * @param  u32Mask Interrupt mask to Disable, a combination of TIMER_IER_TMRIE and TIMER_IER_TMRIE
  * @return None
  */
void TIMER_DisableInt(TIMER_TypeDef *timer, uint32_t u32Mask)
{
        assert_param(u32Mask != 0);
        assert_param((u32Mask & ~(TIMER_IER_TMRIE | TIMER_IER_TCAPIE)) == 0);

	timer->IER &= ~u32Mask;

        if(timer->IER == 0) {
		if(timer == TIMER0) {
			NVIC_DisableIRQ(TMR0_IRQn);
		} else if(timer == TIMER1) {
			NVIC_DisableIRQ(TMR1_IRQn);
		} else if(timer == TIMER2) {
			NVIC_DisableIRQ(TMR2_IRQn);
		} else {
			NVIC_DisableIRQ(TMR3_IRQn);
		}
        }

}


/**
  * @brief  Configure TIMERx trigger event
  * @param  timer Timer channel
  * @param  u32Event IP to be trigger, could a combination of TIMER_CTL_ADCTEEN | TIMER_CTL_DACTEEN | TIMER_CTL_PDMA_TEEN | TIMER_CTL_TKWAKEEN
  * @param  u32Src Source to trigger ADC/DAC/PDMA
  * @return None
  */
void TIMER_ConfigTriggerEvent(TIMER_TypeDef *timer, uint32_t u32Event, uint32_t u32Src)
{
	uint32_t reg;

        assert_param(u32Event != 0);
        assert_param((u32Event & ~(TIMER_CTL_ADCTEEN | TIMER_CTL_DACTEEN | TIMER_CTL_PDMA_TEEN | TIMER_CTL_TKWAKEEN)) == 0);
        assert_param(!((timer != TIMER0) && (u32Event & TIMER_CTL_TKWAKEEN)));
	assert_param((u32Src == TIMER_CTL_CAPTRGEN) || (u32Src == TIMER_CTL_TIMEOUTTRGEN));

	reg = timer->CTL & ~(TIMER_CTL_ADCTEEN | TIMER_CTL_DACTEEN | TIMER_CTL_PDMA_TEEN | TIMER_CTL_TKWAKEEN | TIMER_CTL_CAPTRGEN);
        timer->CTL = reg | u32Event | u32Src;
}





/**
  * @brief  Initialize WDT
  * @param  u32Interval Time out interval
  * @param  u32Delay Reset delay
  * @return None
  */
void WDT_Init(uint32_t u32Interval, uint32_t u32Delay)
{
	uint32_t reg;
	assert_param((u32Interval & ~WDT_CTL_WTIS_MASK) == 0);
	assert_param((u32Delay & ~WDT_CTL_WTRDSEL_MASK) == 0);

        CLK->APBCLK |= CLK_APBCLK_WDT_EN;
        reg = WDT->CTL;
        reg &= ~(WDT_CTL_WTIS_MASK | WDT_CTL_WTRDSEL_MASK);
        WDT->CTL = reg | u32Interval | u32Delay;
}

/**
  * @brief  Disable WDT
  * @param  None
  * @return None
  */
void WDT_DeInit(void)
{

	WDT->CTL &= ~WDT_CTL_WTE;
	WDT->IER &= ~WDT_IER_WDTIE;

	if(!(WWDT->IER & WWDT_IER_IE))
		NVIC_EnableIRQ(WDT_IRQn);

        CLK->APBCLK &= ~CLK_APBCLK_WDT_EN;

}

/**
  * @brief  Enable WDT interrupt
  * @param  None
  * @return None
  * @note   This API will clear pending interrupt status before enabled interrupt
  */
void WDT_EnableInt(void)
{
	WDT->ISR = WDT_ISR_WDTIS;
	WDT->IER |= WDT_IER_WDTIE;
        NVIC_EnableIRQ(WDT_IRQn);

}

/**
  * @brief  Disable WDT interrupt
  * @param  None
  * @return None
  */
void WDT_DisableInt(void)
{
	WDT->IER &= ~WDT_IER_WDTIE;
	if(!(WWDT->IER & WWDT_IER_IE))
		NVIC_EnableIRQ(WDT_IRQn);

}


/**
  * @brief  Enables WWDT clock, configure prescale and comparator
  * @param  u32Prescale WWDT clock prescale
  * @param  u8Cmp Comparator value
  * @return None
  * @note   This API can only call once after systme boot up
  */
void WWDT_Init(uint32_t u32Prescale, uint8_t u8Cmp)
{
        assert_param(u8Cmp < 64);
        assert_param((u32Prescale & ~WWDT_CR_PERIODSEL_MASK) == 0);

        CLK->APBCLK |= CLK_APBCLK_WDT_EN;
        WWDT->CR = u32Prescale | (u8Cmp << 16) | 1;

        return;
}

/**
  * @brief  Sorry, can't do so once WWDT init.
  * @param  none
  * @param  none
  */
void WWDT_DeInit(void)
{
        return;
}


/**
  * @brief  Check if WWDT is CPU reset source or not
  * @param  none
  * @return Yes or no.
  * @retval 0 WWDT is not CPU reset source
  * @retval 1 WWDT is CPU reset source
  * @note   This API will clear WWDT reset status
  */
uint8_t WWDT_IsResetSource(void)
{
        int8_t ret = (WWDT->STS & WWDT_STS_RF) ? 1 : 0;

        if(ret)
                WWDT->STS = WWDT_STS_RF;

        return(ret);

}


/**
  * @brief  Enable WWDT interrupt
  * @param  None
  * @return None
  * @note   This API will clear pending interrupt status before enabled interrupt
  */
void WWDT_EnableInterrupt(void)
{

        WWDT->STS = WWDT_STS_IF;	// clear pending interrupt, if any
        WWDT->IER = WWDT_IER_IE;

        NVIC_EnableIRQ(WDT_IRQn);

        return;
}


/*@}*/ /* end of group NANO1xx_TIMER_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_TIMER_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

