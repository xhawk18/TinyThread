/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __SYSTEM_NUC1xx_H
#define __SYSTEM_NUC1xx_H

#ifdef __cplusplus
 extern "C" {
#endif 
/*---------------------------------------------------------------------------------------------------------*/
/* Macro Definition                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

//#define DEBUG_ENABLE_SEMIHOST   /* To enable semihosted. !!!The SEMIHOSTED of startup_NUC1xx.s must be {TRUE} */

/* Using UART0 or UART1 */  
#define DEBUG_PORT   0 		    /*0:UART0  1:UART1 2:UART2 */

/*----------------------------------------------------------------------------
  Define SYSCLK
 *----------------------------------------------------------------------------*/
#define __XTAL      (12000000UL)
#define __RTC_XTAL  (32768UL)
#define __IRC22M    (22118400UL)
#define __IRC10K    (10000UL)
#define __HSI       (__IRC22M)      /* Factory Default is internal 22MHz */

extern uint32_t SystemCoreClock;                   /*!< System Clock Frequency (Core Clock) */
extern uint32_t CyclesPerUs;                       /* Cycles per micro second */

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system
 *         Initialise GPIO directions and values
 */
extern void SystemInit(void);


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

#endif
