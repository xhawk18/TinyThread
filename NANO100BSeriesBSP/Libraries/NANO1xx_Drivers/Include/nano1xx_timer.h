/******************************************************************************
 * @file     nano1xx_timer.h
 * @brief    NANO1xx TIMER driver header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef __NANO1XX_TIMER_H
#define __NANO1XX_TIMER_H

#include "nano1xx.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_TIMER_Driver TIMER Driver
  @{
*/


/** @addtogroup NANO1xx_TIMER_EXPORTED_CONSTANTS TIMER Exported Constants
  @{
*/
#define WWDT_RELOAD_KEY			   0x00005AA5

#define WDT_START		do{WDT->CTL |= WDT_CTL_WTE;}while(0)	///< Start WDT timer
#define WDT_STOP		do{WDT->CTL &= ~WDT_CTL_WTE;}while(0)	///< Stop WDT timer
#define WDT_RESET_TIMER		do{WDT->CTL |= WDT_CTL_WTR;}while(0)	///< Reset WDT timer
#define WDT_ENABLE_RESET	do{WDT->CTL |= WDT_CTL_WTRE;}while(0)	///< Enable WDT reset system
#define WDT_DISABLE_RESET	do{WDT->CTL &= ~WDT_CTL_WTRE}while(0)	///< Disable WDT reset system
#define WDT_ENABLE_WAKEUP	do{WDT->CTL |= WDT_CTL_WTWKE;}while(0)	///< Enable WDT wake up system
#define WDT_DISABLE_WAKEUP	do{WDT->CTL &= ~WDT_CTL_WTWKE;}while(0)	///< Disable WDT wkae up system

#define WWDT_RELOAD		do{WWDT->RLD = WWDT_RELOAD_KEY;}while(0) ///< Reload WWDT. Call this API at wrong time can reset the system

/*@}*/ /* end of group NANO1xx_TIMER_EXPORTED_CONSTANTS */



/** @addtogroup NANO1xx_TIMER_EXPORTED_FUNCTIONS TIMER Exported Functions
  @{
*/

/**
  * @brief  Check if a timer channel is active or not
  * @param  timer Timer channel
  * @return Yes or no.
  * @retval 0 TIMERx is not active
  * @retval 1 TIMERx is active
  */
static __INLINE uint32_t TIMER_IsActive(TIMER_TypeDef *timer)
{
        return(timer->CTL & TIMER_CTL_TMRACT ? 1 : 0);
}

/**
  * @brief  Get timer interrupt flag
  * @param  timer Timer channel to check
  * @return Interrupt mask
  */
static __INLINE uint32_t TIMER_GetIntFlag(TIMER_TypeDef *timer)
{
        return (timer->ISR & (TIMER_ISR_TMRIS | TIMER_ISR_TCAPIS));
}

/**
  * @brief  Clear timer interrupt flag
  * @param  timer Timer channel to clear
  * @return None
  */
static __INLINE void TIMER_ClearIntFlag(TIMER_TypeDef *timer)
{
	timer->ISR = (TIMER_ISR_TMRIS | TIMER_ISR_TCAPIS);
}

/**
  * @brief  Start timer
  * @param  timer Timer channel to start
  * @return None
  */
static __INLINE void TIMER_Start(TIMER_TypeDef *timer)
{
        timer->CTL |= TIMER_CTL_TMREN;
}

/**
  * @brief  Reset timer
  * @param  timer Timer channel to reset
  * @return None
  */
static __INLINE void TIMER_Reset(TIMER_TypeDef *timer)
{
        timer->CTL |= TIMER_CTL_SWRST;
        while(timer->CTL & TIMER_CTL_SWRST);
}


/**
  * @brief  Get timer current counter
  * @param  timer Timer channel to get counter
  * @return None
  */
static __INLINE uint32_t TIMER_GetCounter(TIMER_TypeDef *timer)
{
        return(timer->DR);
}


/**
  * @brief  Stop timer counter
  * @param  timer Timer channel to stop
  * @return None
  */
static __INLINE void TIMER_Stop(TIMER_TypeDef *timer)
{

        timer->CTL &= ~TIMER_CTL_TMREN;
}

/**
  * @brief  Set timer prescale. Can use this API to overwrite current value
  * @param  timer Timer channel to configure
  * @param  u8Prescale New prescale value
  * @return None
  */
static __INLINE void TIMER_SetPrescale(TIMER_TypeDef *timer, uint8_t u8Prescale)
{
        timer->PRECNT = u8Prescale;
}

/**
  * @brief  Set timer compare value. Can use this API to overwrite current value
  * @param  timer Timer channel to configure
  * @param  u32Compare New comparator value, between 2~0xFFFFFF
  * @return None
  */
static __INLINE void TIMER_SetComparator(TIMER_TypeDef *timer, uint8_t u32Compare)
{
	assert_param((u32Compare > 1) && (u32Compare < 0x1000000));
        timer->CMPR = u32Compare;
}

/**
  * @brief  Configure specified timer channels in inter timer trigger mode
  * @param  timer Timer channel to work on inter timer trigger mode, could be ether TIMER0 or TIMER2
  * @return None
  */
static __INLINE void TIMER_EnableInterTimerTriggerMode(TIMER_TypeDef *timer)
{
        assert_param((timer == TIMER0) || (timer == TIMER2));

        timer->CTL |= TIMER_CTL_INTRTRGEN;
}

/**
  * @brief  Disable specified timer channels to work on inter timer trigger mode
  * @param  timer Timer channel to stop working on inter timer trigger mode, could be ether TIMER0 or TIMER2
  * @return None
  */
static __INLINE void TIMER_DisableInterTimerTriggerMode(TIMER_TypeDef *timer)
{
        assert_param((timer == TIMER0) || (timer == TIMER2));

        timer->CTL &= ~TIMER_CTL_INTRTRGEN;
}

/**
  * @brief  Configure specified timer channel to work on counter reset function
  * @param  timer Timer channel to work on counter reset mode
  * @param  u32Edge Assign rising/falling edge to reset counter
  * @return None
  */
static __INLINE void TIMER_EnableCounterResetMode(TIMER_TypeDef *timer, uint32_t u32Edge)
{
	uint32_t reg;
	assert_param((u32Edge & ~TIMER_CTL_TCAPEDGE_MASK) == 0);

	reg = timer->CTL & ~TIMER_CTL_TCAPEDGE_MASK;
	timer->CTL = reg | u32Edge | TIMER_CTL_TCAPEN | TIMER_CTL_TCAPDEBEN | TIMER_CTL_TCAPMODE;
}

/**
  * @brief  Disablde specified timer channel to work on counter reset function
  * @param  timer Timer channel to stop working on counter reset function
  * @return None
  */
static __INLINE void TIMER_DisableCounterResetMode(TIMER_TypeDef *timer)
{
        timer->CTL &= ~(TIMER_CTL_TCAPEN | TIMER_CTL_TCAPDEBEN);
}

/**
  * @brief  Configure specified timer channel to work on capture mode
  * @param  timer Timer channel to work on capture mode
  * @param  u32Mode Assign capture mode to work on
  * @param  u32Edge Assign rising/falling edge to trigger capture function
  * @return None
  */
static __INLINE void TIMER_EnableCaptureMode(TIMER_TypeDef *timer, uint32_t u32Mode, uint32_t u32Edge)
{
	uint32_t reg;

	assert_param((u32Edge & ~TIMER_CTL_TCAPEDGE_MASK) == 0);
	assert_param((u32Mode == TIMER_CTL_CAPCNTMOD_FREE_CNT) || (u32Mode == TIMER_CTL_CAPCNTMOD_TRIGGER_CNT));

	reg = timer->CTL & ~(TIMER_CTL_TCAPMODE | TIMER_CTL_TCAPEDGE_MASK | TIMER_CTL_CAPCNTMOD);
	timer->CTL = reg | u32Mode | u32Edge | TIMER_CTL_TCAPEN | TIMER_CTL_TCAPDEBEN;
}

/**
  * @brief  Disablde specified timer channel to work on capture mode
  * @param  timer Timer channel to stop working on capture mode
  * @return None
  */
static __INLINE void TIMER_DisableCaptureMode(TIMER_TypeDef *timer)
{

        timer->CTL &= ~(TIMER_CTL_TCAPEN | TIMER_CTL_TCAPDEBEN);

}

/**
  * @brief  Configure specified timer channel to work on external event mode
  * @param  timer Timer channel to work on external event
  * @param  u32Edge Assign rising/falling edge to increase counter
  * @return None
  */
static __INLINE void TIMER_EnableExternalEventMode(TIMER_TypeDef *timer, uint32_t u32Edge)
{
	assert_param((u32Edge == TIMER_CTL_EVNTEDGE_R) || (u32Edge == TIMER_CTL_EVNTEDGE_F));

        timer->CTL = (timer->CTL & ~TIMER_CTL_EVNTEDGE_R) | TIMER_CTL_EVNTEN | TIMER_CTL_EVNTDEBEN | u32Edge;
}

/**
  * @brief  Disable specified timer channel to work on external event mode
  * @param  timer Timer channel to stop working on external event
  * @return None
  */
static __INLINE void TIMER_DisableExternalEventMode(TIMER_TypeDef *timer)
{
        timer->CTL &= ~(TIMER_CTL_EVNTEN | TIMER_CTL_EVNTDEBEN);

}

/**
  * @brief  Enable specified timer channel to wake up the system
  * @param  timer Timer channel enabled to wake up system
  * @return None
  */
static __INLINE void TIMER_EnableWakeUp(TIMER_TypeDef *timer)
{
        timer->CTL  |= TIMER_CTL_WAKEEN;
}

/**
  * @brief  Disable specified timer channel to wake up the system
  * @param  timer Timer channel Disabled to wake up system
  * @return None
  */
static __INLINE void TIMER_DisableWakeUp(TIMER_TypeDef *timer)
{
        timer->CTL  &= ~TIMER_CTL_WAKEEN;
}

/**
  * @brief  Get latest captured data of specified timer channel
  * @param  timer Timer channel to get latest captured data
  * @return Latest capture data
  */
static __INLINE uint32_t TIMER_GetCaptureData(TIMER_TypeDef *timer)
{
        return(timer->TCAP);
}

/**
  * @brief  Check if TIMERx is CPU wake up source or not
  * @param  timer Timer channel
  * @return Yes or no.
  * @retval 0 TIMERx is not CPU wake up source
  * @retval 1 TIMERx is CPU wake up source
  * @note   This API will clear TIMERx wake up status
  */
static __INLINE uint32_t TIMER_IsWakeUpSource(TIMER_TypeDef *timer)
{
        int8_t ret = (timer->ISR & TIMER_ISR_TMRWAKESTS) ? 1 : 0;

        if(ret)
                timer->ISR = TIMER_ISR_TMRWAKESTS;

        return(ret);
}

void TIMER_Init(TIMER_TypeDef *timer, uint8_t u8Prescale, uint32_t u32Compare, uint32_t u32Mode);
void TIMER_DeInit(TIMER_TypeDef *timer);
void TIMER_EnableInt(TIMER_TypeDef *timer, uint32_t u32Mask);
void TIMER_DisableInt(TIMER_TypeDef *timer, uint32_t u32Mask);
void TIMER_ConfigTriggerEvent(TIMER_TypeDef *timer, uint32_t u32Event, uint32_t u32Src);

/** WDT APIs */
void WDT_Init(uint32_t u32Interval, uint32_t u32Period);
void WDT_DeInit(void);
void WDT_EnableInt(void);
void WDT_DisableInt(void);

/** WWDT APIs */

/**
  * @brief  Get WWDT current counter
  * @param  none
  * @return Current counter

  */
static __INLINE uint8_t WWDT_GetCounter(void)
{
        return(WWDT->VAL);

}


void WWDT_Init(uint32_t u32Prescale, uint8_t u8Cmp);
void WWDT_DeInit(void);
void WWDT_EnableInterrupt(void);
uint8_t WWDT_IsResetSource(void);

/*@}*/ /* end of group NANO1xx_TIMER_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_TIMER_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

#endif  // __NANO1XX_TIMER_H


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

