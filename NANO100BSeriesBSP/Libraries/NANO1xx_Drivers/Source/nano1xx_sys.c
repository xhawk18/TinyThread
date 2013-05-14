/******************************************************************************
 * @file     nano1xx_sys.c
 * @brief    NANO1xx SYS driver source file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx_sys.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_SYS_Driver SYS Driver
  @{
*/


/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

/// @cond 
static uint16_t _DrvSYS_PLL_Table[5][6] = {
     /* Output Freq. */
     /* 45MHz,                48MHz,               90MHz,               96MHz,               120MHz,               128MHz */
		{PLL_NOT_SUPPORTED,   PLL_IN_4M_OUT_48M,   PLL_NOT_SUPPORTED,   PLL_IN_4M_OUT_96M,   PLL_IN_4M_OUT_120M,   PLL_IN_4M_OUT_128M},		/* Input 4MHz */
		{PLL_NOT_SUPPORTED,   PLL_IN_8M_OUT_48M,   PLL_NOT_SUPPORTED,   PLL_IN_8M_OUT_96M,   PLL_NOT_SUPPORTED,    PLL_IN_8M_OUT_128M},		/* Input 8MHz */
		{PLL_NOT_SUPPORTED,   PLL_IN_12M_OUT_48M,  PLL_NOT_SUPPORTED,   PLL_IN_12M_OUT_96M,  PLL_IN_12M_OUT_120M,  PLL_NOT_SUPPORTED},		/* Input 12MHz */
		{PLL_IN_15M_OUT_45M,  PLL_NOT_SUPPORTED,   PLL_IN_15M_OUT_90M,  PLL_NOT_SUPPORTED,   PLL_IN_15M_OUT_120M,  PLL_NOT_SUPPORTED},		/* Input 15MHz */
		{PLL_NOT_SUPPORTED,   PLL_IN_24M_OUT_48M,  PLL_NOT_SUPPORTED,   PLL_IN_24M_OUT_96M,  PLL_IN_24M_OUT_120M,  PLL_NOT_SUPPORTED}		/* Input 24MHz */
};

/// @endcond

/** @addtogroup NANO1xx_SYS_EXPORTED_FUNCTIONS SYS Exported Functions
  @{
*/


/**
  * @brief  Select HCLK clock source. 
  * @param  u32ClkSrc The clock source. CLK_CLKSEL0_HCLK_XXX, where XXX can be HXT, LXT, PLL, HIRC or LIRC.
  * @return Status of clock selection result.
  * @retval 0 HCLK is sucessfully changed.
  * @retval -1 HCLK isn't changed.
  */
int32_t SYS_SelectHCLKSource(uint32_t u32ClkSrc)
{
	
	CLK->CLKSEL0 = (CLK->CLKSEL0 & ~CLK_CLKSEL0_HCLK_MASK) | u32ClkSrc;

	if (CLK->CLKSTATUS & CLK_CLKSTATUS_CLK_SW_FAIL)  {
		CLK->CLKSTATUS |= CLK_CLKSTATUS_CLK_SW_FAIL;   /* Clear Clock Switch Fail Flag */
        return -1;  /* HCLK isn't changed */
    }
		
	SystemCoreClockUpdate(); /* HCLK is changed successfully */
	
	return 0;
}


/**
  * @brief  System enter power down after this function is executed. 
  * @param  u32Enable This value should be 1 to enable PDWU interrupt. Otherwise, this value should be 0.
  * @return None.
  */
void SYS_SetUpPowerDown(uint32_t u32Enable)
{	

	CLK->PWRCTL |= (CLK_PWRCTL_PWRDOWN_EN | CLK_PWRCTL_DELY_EN);	   /* Set power down bit */
	SCB->SCR |= 0x04;						   /* Sleep Deep */

	if (u32Enable)  {
		CLK->PWRCTL |= CLK_PWRCTL_WAKEINT_EN;  /* Enable wake up interrupt source */
	    NVIC_EnableIRQ(PDWU_IRQn);			   /* Enable IRQ request for PDWU interupt */
	}

}


/**
  * @brief  Calculate current PLL clock frquency. 
  * @param  None.
  * @return PLL clock frquency. The clock UNIT is in Hz.
  */
uint32_t SYS_GetPLLClockFreq(void)
{
	uint32_t u32Freq =0, u32PLLSrc;
    uint32_t u32NO, u32NR, u32IN_DV, u32PllReg;

	u32PllReg = CLK->PLLCTL;

	if (u32PllReg & CLK_PLLCTL_PD)  
		return 0;	 /* PLL is in power down mode */
	
	if (u32PllReg & CLK_PLLCTL_PLLSRC_HIRC)
		u32PLLSrc = __IRC12M;
	else
		u32PLLSrc = __XTAL;

    u32NO = (u32PllReg & CLK_PLLCTL_OUT_DV) ? 2: 1;

	u32IN_DV = (u32PllReg & CLK_PLLCTL_IN_DIVIDER_MASK) >> 8;
	if (u32IN_DV == 0)
		u32NR = 2;
	else if (u32IN_DV == 1)
		u32NR = 4;
	else if (u32IN_DV == 2)
		u32NR = 8;
	else
		u32NR = 16;

	u32Freq = u32PLLSrc * ((u32PllReg & CLK_PLLCTL_FB_DIVIDER_MASK) +32) / u32NR / u32NO;

	return u32Freq;
}


/**
  * @brief  Get current HCLK clock frquency. 
  * @param  None.
  * @return HCLK clock frquency. The clock UNIT is in Hz.
  */
uint32_t SYS_GetHCLKFreq(void)
{
	
	uint32_t u32Freqout, u32AHBDivider, u32ClkSel;

	u32ClkSel = CLK->CLKSEL0 & CLK_CLKSEL0_HCLK_MASK;

	if (u32ClkSel == CLK_CLKSEL0_HCLK_HXT)	/* external HXT crystal clock */
	{
		u32Freqout = __XTAL;	
	}
	else if(u32ClkSel == CLK_CLKSEL0_HCLK_LXT)		/* external LXT crystal clock */ 
	{
		u32Freqout = __RTC_XTAL;
	}
	else if(u32ClkSel == CLK_CLKSEL0_HCLK_PLL)		/* PLL clock */
	{
		u32Freqout = SYS_GetPLLClockFreq();
	}
	else if(u32ClkSel == CLK_CLKSEL0_HCLK_LIRC)	/* internal LIRC oscillator clock */
	{
	 	u32Freqout = __IRC10K;
	}
	else									/* internal HIRC oscillator clock */
	{
	 	u32Freqout = __IRC12M;
	
	}
	u32AHBDivider = (CLK->CLKDIV0 & CLK_CLKDIV0_HCLK_MASK) + 1 ;
	
	return (u32Freqout/u32AHBDivider);	
}


/**
  * @brief  Enable chip clock sources including HXT, LXT, LIRX, HIRC and PLL. And then set HCLK clock source and divider.
  *         If the second parameter is NOT NULL, also enable the clock and set the source/divider of peripheral functions. 
  * @param  sChipCfg    - [in], The pointer to the structure which stores the configuration of chip clock source and HCLK.
  *                              u32ChipClkEn : CLK_PWRCTL_XXX_EN, where XXX could be HXT, LXT, HIRC or LIRC
  *                              u32PLLClkSrc : CLK_PLLCTL_PLLSRC_HIRC or CLK_PLLCTL_PLLSRC_HXT
  *                              u8PLLEnable : 1 to enable PLL, 0 to disable PLL        
  *                              ePLLInFreq  : E_SYS_PLLIN_4M, _8M, _12M, _15M or _24M                     
  *                              ePLLOutFreq : E_SYS_PLLOUT_45M, _48M, _90M, _96M, _120M or _128M  
  *                              u32HClkSrc : CLK_CLKSEL0_HCLK_XXX, where XXX could be HXT, LXT, PLL, HIRC or LIRC
  *                              u32HClkDiv : 0 ~ 15       
  *                                                                                                             
  * @return Execution result.
  * @retval  0 Success                                                              
  * @retval  -1 The chip clock sources are not stable
  * @retval  -2 The selected PLL setting is not supported
  * @retval  -3 The PLL clock output is not stable
  */
int32_t SYS_InitChipClock(S_SYS_CHIP_CLKCFG *sChipCfg)
{
	uint32_t u32ClkEn, u32ClkSts, u32PllCtl;	
	volatile uint32_t delayCnt;
	
	assert_param(CHECK_CLK_CONFIG(sChipCfg));               /* Check input parameter */
	assert_param(CHECK_HCLK_SOURCE(sChipCfg->u32HClkSrc));  /* Check HCLK clock select */

	/* enable chip clock sources */
	u32ClkEn = u32ClkSts = (sChipCfg->u32ChipClkEn) & 0x0F; 
	if (u32ClkSts & CLK_PWRCTL_HIRC_EN) 
		u32ClkSts = (u32ClkSts & ~CLK_PWRCTL_HIRC_EN) | CLK_CLKSTATUS_HIRC_STB;
	
	SYS_SetChipClockSrc(u32ClkEn, 1);  /* enable the selected chip clock sources */
	
	for (delayCnt=0; delayCnt<100000; delayCnt++)
		if ((CLK->CLKSTATUS & u32ClkSts) == u32ClkSts)
			break;

	if (delayCnt == 100000)
		return -1;

	/* enable PLL */
	if (sChipCfg->u8PLLEnable == 1)  {

		/* check PLL in clock freq. */
		assert_param(CHECK_PLLIN_CLK(sChipCfg->ePLLInFreq));
		assert_param(CHECK_PLLOUT_CLK(sChipCfg->ePLLOutFreq));
		
		u32PllCtl = _DrvSYS_PLL_Table[sChipCfg->ePLLInFreq][sChipCfg->ePLLOutFreq];

		if (u32PllCtl == PLL_NOT_SUPPORTED)
			return -2;

		/* select correct pll clock source */
		u32PllCtl |= sChipCfg->u32PLLClkSrc;

		/* set PLL control reg and waits for PLL clock stable */
		CLK->PLLCTL = u32PllCtl;

		for (delayCnt=0; delayCnt<100000; delayCnt++)
			if (CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB)
				break;

		if (delayCnt == 100000)
			return -3;		
	}

	/* set HCLK divider and source */
	CLK->CLKDIV0 = (CLK->CLKDIV0 & ~CLK_CLKDIV0_HCLK_MASK) | sChipCfg->u32HClkDiv;
	SYS_SelectHCLKSource(sChipCfg->u32HClkSrc); /* select required clock source for HCLk */

	return 0;
}


/**
  * @brief  Use the SysTick to generate the delay time. 
  * @param  us The required delay time in us.
  * @return None.
  */
void SYS_Delay(uint32_t us)
{
	SysTick->LOAD = (us * ((SystemCoreClock + 500000) / 1000000));
	SysTick->VAL  = (0x00);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & (1 << 16)) == 0);

}

/*@}*/ /* end of group NANO1xx_SYS_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_SYS_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
