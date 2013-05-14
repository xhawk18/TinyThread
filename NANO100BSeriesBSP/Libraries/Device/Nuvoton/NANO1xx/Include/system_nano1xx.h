/******************************************************************************
 * @file     system_nano1xx.h
 * @brief    The system clock frequency is define in this file.
 * @version  V1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


#ifndef __SYSTEM_NANO1XX_H   
#define __SYSTEM_NANO1XX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __XTAL      (12000000UL)	/* HXT */
#define __RTC_XTAL  (32768UL)		/* LXT */
#define __IRC12M    (12000000UL)	/* HIRC */
#define __IRC10K    (10000UL)		/* LIRC */
#define __HSI       (__IRC12M)     


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock 
 *         retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_NANO1XX_H */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
