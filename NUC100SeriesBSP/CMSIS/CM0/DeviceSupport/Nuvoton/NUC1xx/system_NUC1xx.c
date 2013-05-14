/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include "NUC1xx.h"


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock  = __HSI;   /*!< System Clock Frequency (Core Clock) */
uint32_t CyclesPerUs      = (__HSI / 1000000); /* Cycles per micro second */
uint32_t gau32ClkSrcTbl[] = {__XTAL, __RTC_XTAL, __HSI, __IRC10K, __IRC22M};

/*----------------------------------------------------------------------------
  Clock functions
  This function is used to update the variable SystemCoreClock 
  and must be called whenever the core clock is changed.
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
    uint32_t u32CoreFreq, u32ClkSrc, u32Shift;
 
    u32ClkSrc = SYSCLK->CLKSEL0.HCLK_S;
    if(u32ClkSrc > sizeof(gau32ClkSrcTbl)/sizeof(gau32ClkSrcTbl[0]))
        u32ClkSrc = sizeof(gau32ClkSrcTbl)/sizeof(gau32ClkSrcTbl[0]) - 1; 
    
    if(u32ClkSrc != 2)
    {
        /* Use the clock sources directly */
        u32CoreFreq = gau32ClkSrcTbl[u32ClkSrc];
    }
    else
    {
        /* Use PLL */
        u32Shift = SYSCLK->PLLCON.OUT_DV;   /* OUT_DV :DEF: {1, 2, 2, 4} */
        if(u32Shift > 1) u32Shift--;
        u32CoreFreq = ((SYSCLK->PLLCON.PLL_SRC)?__IRC22M:__XTAL) * (SYSCLK->PLLCON.FB_DV+2) / (SYSCLK->PLLCON.IN_DV+2) >> u32Shift;
    }
 
    SystemCoreClock = (u32CoreFreq/(SYSCLK->CLKDIV.HCLK_N + 1));
    CyclesPerUs = (SystemCoreClock + 500000) / 1000000;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: SystemInit                                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      The necessary initializaiton of systerm.                                                           */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void SystemInit (void)
{

}

