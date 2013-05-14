/******************************************************************************
 * @file     nano1xx_sys.h
 * @brief    NANO1xx SYS driver header file
 *           Defines constants, inline functions, function prototypes for GCR
 *           and clock control.
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __NANO1XX_SYS_H
#define __NANO1XX_SYS_H

#include "nano1xx.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_SYS_Driver SYS Driver
  @{
*/


/** @addtogroup NANO1xx_SYS_EXPORTED_CONSTANTS SYS Exported Constants
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/* Constant definitions                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
/* Chip clock source constants */
#define CHIP_CLOCKS_ALL_ENABLE	(CLK_PWRCTL_HXT_EN | CLK_PWRCTL_LXT_EN | CLK_PWRCTL_HIRC_EN | CLK_PWRCTL_LIRC_EN)

/// @cond
/*---------------------------------------------------------------------------------------------------------*/
/*  Define parameter checking                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define CHECK_HCLK_SOURCE(HclkSel)			((HclkSel == 0) || \
											 (HclkSel == 1)	|| \
											 (HclkSel == 2)	|| \
											 (HclkSel == 3)	|| \
											 (HclkSel == 7))						 
#define CHECK_PLLIN_CLK(PllInClk)	        ((PllInClk == E_SYS_PLLIN_4M)  || \
											 (PllInClk == E_SYS_PLLIN_8M)  || \
											 (PllInClk == E_SYS_PLLIN_12M) || \
											 (PllInClk == E_SYS_PLLIN_15M) || \
											 (PllInClk == E_SYS_PLLIN_24M))
#define CHECK_PLLOUT_CLK(PllOutClk)	        ((PllOutClk == E_SYS_PLLOUT_45M)  || \
											 (PllOutClk == E_SYS_PLLOUT_48M)  || \
											 (PllOutClk == E_SYS_PLLOUT_90M)  || \
											 (PllOutClk == E_SYS_PLLOUT_96M)  || \
											 (PllOutClk == E_SYS_PLLOUT_120M) || \
											 (PllOutClk == E_SYS_PLLOUT_128M))
#define CHECK_CLK_CONFIG(ClkCfg)			 (ClkCfg != NULL)

/// @endcond

/*---------------------------------------------------------------------------------------------------------*/
/*  PLL Clock In Freq.		     				                                          		           */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum 
{
	E_SYS_PLLIN_4M, 
	E_SYS_PLLIN_8M, 
	E_SYS_PLLIN_12M, 
	E_SYS_PLLIN_15M, 
	E_SYS_PLLIN_24M
} E_SYS_PLLIN_CLK;


/*---------------------------------------------------------------------------------------------------------*/
/*  PLL Clock Out Freq.		     				                                          		           */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum 
{
	E_SYS_PLLOUT_45M, 
	E_SYS_PLLOUT_48M, 
	E_SYS_PLLOUT_90M, 
	E_SYS_PLLOUT_96M, 
	E_SYS_PLLOUT_120M,
	E_SYS_PLLOUT_128M,
} E_SYS_PLLOUT_CLK;


/*@}*/ /* end of group NANO1xx_SYS_EXPORTED_CONSTANTS */



/** @addtogroup NANO1xx_SYS_EXPORTED_STRUCTS SYS Exported Structs
  @{
*/

/*
 * Struture to store HCLK (CPU) and PLL clock configurations
 */
typedef struct {
    uint32_t u32ChipClkEn;	/* CLK_PWRCTL_XXX_EN, where XXX could be HXT, LXT, HIRC or LIRC */
	uint32_t u32PLLClkSrc;	/* CLK_PLLCTL_PLLSRC_HIRC or CLK_PLLCTL_PLLSRC_HXT */
    uint8_t  u8PLLEnable;   /* 1 to enable PLL, 0 to disable PLL */
	uint8_t  ePLLInFreq;    /* E_SYS_PLLIN_CLK */
    uint8_t  ePLLOutFreq;   /* E_SYS_PLLOUT_CLK */
	uint8_t  RESERVED0[1];
    uint32_t u32HClkSrc; 	/* CLK_CLKSEL0_HCLK_XXX, where XXX could be HXT, LXT, PLL, HIRC or LIRC */
    uint32_t u32HClkDiv;	/* 0 ~ 15 */	
	uint32_t u32AHBClkEn;	/* CLK_AHBCLK_XXX_EN, where XXX could be GPIO, DMA, ISP, EBI, SRAM and TICK */
    uint32_t u32APBClkEn; 	/* CLK_APBCLK_XXX_EN, where XXX cound be WDT, RTC, TMR0~3, FDIV, SC0~2, I2C0~1, SPI0~2, UART0~1, PWM0_CH01 */
	                        /*                                       PWM0_CH23, PWM1_CH01, PWM1_CH23, TK, DAC, LCD, USBD, ADC, I2S */
} S_SYS_CHIP_CLKCFG;

/*@}*/ /* end of group NANO1xx_SYS_EXPORTED_STRUCTS */


/** @addtogroup NANO1xx_SYS_EXPORTED_FUNCTIONS SYS Exported Functions
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/* Define SYS inline functions                                                                             */
/*---------------------------------------------------------------------------------------------------------*/

/**
  * @brief  Read Product ID
  * @param  None
  * @return Product ID
  */
static __INLINE uint32_t SYS_ReadProductID(void)
{	
	return GCR->PDID;
}


/**
  * @brief  Unlock protected registers
  * @param  None
  * @return Lock status
  * @retval 1 unlock
  * @retval 0 still lock
  */
static __INLINE int32_t SYS_UnlockProtectedReg(void)
{
	uint32_t lockbit;

	GCR->RegLockAddr = 0x59;
	GCR->RegLockAddr = 0x16;
	GCR->RegLockAddr = 0x88;

	lockbit = GCR->RegLockAddr & 0x01;
	return (lockbit);
} 
 

/**
  * @brief  Lock protected registers
  * @param  None
  * @return Lock status
  * @retval 1 unlock
  * @retval 0 lock
  */
static __INLINE int32_t SYS_LockProtectedReg(void)
{
	uint32_t lockbit;

	GCR->RegLockAddr = 0x00;	

	lockbit = GCR->RegLockAddr & 0x01;
	return (lockbit);
}


/**
  * @brief  Check the status of protected registers
  * @param  None
  * @return Lock status
  * @retval 1 unlock
  * @retval 0 lock
  */
static __INLINE int32_t SYS_IsProtectedRegLocked(void)
{
	return GCR->RegLockAddr & 0x01;
}


/**
  * @brief  Read reset source from last operation
  * @param  None
  * @return Reset source value
  */
static __INLINE uint32_t SYS_GetResetSource(void)
{
	return GCR->RST_SRC;
}


/**
  * @brief  Clear reset source. 
  * @param  u32Src The reset source will be cleared if the corresponding bit is set to 1.
  * @return None.
  */
static __INLINE void SYS_ClearResetSource(uint32_t u32Src)
{
	GCR->RST_SRC = u32Src;
}


/**
  * @brief  Reset proteced peripheral functions. These functions include EBI, PDMA, CPU and Chip. 
  * @param  u32RstFlag GCR_IPRSTCTL1_XXX, where XXX could be CHIP, CPU,DMA or EBI.
  * @return None.
  */
static __INLINE void SYS_ResetProtectIP(uint32_t u32RstFlag)
{	
	
	GCR->IPRST_CTL1 |= u32RstFlag;
	GCR->IPRST_CTL1 &= ~u32RstFlag;
}


/**
  * @brief  Reset selected peripheral functions.
  * @param  u32RstFlag The combination of GCR_IPRSTCTL2_XXX, where XXX could be GPIO, TMR0~3, SC0~2, I2C0~1, SPI0~2, UART0~1
  *                                                                             PWM0~1, TK, DAC, LCD, USBD, ADC, I2S.
  * @return None.
  */
static __INLINE void SYS_ResetIP(uint32_t u32RstFlag)
{	
	GCR->IPRST_CTL2 |= u32RstFlag;
	GCR->IPRST_CTL2 &= ~u32RstFlag;
}



/**
  * @brief  Enable power on reset (POR) control. 
  * @param  None.
  * @return None.
  */
static __INLINE void SYS_EnablePOR(void)
{
	GCR->PORCTL = 0;
}


/**
  * @brief  Disable power on reset (POR) control. 
  * @param  None.
  * @return None.
  */
static __INLINE void SYS_DisablePOR(void)
{
	GCR->PORCTL = 0x5AA5;
}


/**
  * @brief  Enable or Disable AHB peripheral function clock. The functions include Tick, SRAM, EBI, ISP, DMA and GPIO
  * @param  u32IpFuncs The corresponding bits will be set to 1 for selected peripheral functions.
  * @param  i32Enable This value is 1 to enable clock, and 0 for disable.
  * @return None.
  */
static __INLINE void SYS_SetIPClock_1(uint32_t u32IpFuncs, uint32_t i32Enable)
{	
	/* Tick, SRAM, EBI, ISP, DMA and GPIO */  
	(i32Enable == 1) ? (CLK->AHBCLK |= u32IpFuncs) : (CLK->AHBCLK &= ~u32IpFuncs);	

}


/**
  * @brief  Enable or Disable APB peripheral function clock except for WDT. 
  * @param  u32IpFuncs The corresponding bits will be set to 1 for the selected peripheral functions.
  * @param  i32Enable This value is 1 to enable clock, and 0 for disable.
  * @return None.
  */
static __INLINE void SYS_SetIPClock_2(uint32_t u32IpFuncs, uint32_t i32Enable)
{	
    /* The APB peripheral functions except for WDT */
	(i32Enable == 1) ? (CLK->APBCLK |= u32IpFuncs) : (CLK->APBCLK &= ~u32IpFuncs);
}


/**
  * @brief  Enable or Disable Watchdog Timer clock. 
  * @param  i32Enable This value is 1 to enable clock, and 0 for disable.
  * @return None.
  */
static __INLINE void SYS_SetWDTClock(uint32_t i32Enable)
{	
	(i32Enable == 1) ? (CLK->APBCLK |= CLK_APBCLK_WDT_EN) : (CLK->APBCLK &= ~CLK_APBCLK_WDT_EN);		
}


/**
  * @brief  Select clock source for peripheral functions : UART, ADC, PWM0_CH01, PWM0_CH23, Timer 0, Timer 1, TK and LCD. 
  * @param  u32ClkMask The mask filed used to chnage clock source. Please refer nano1xx.h for possible values : CLK_CLKSEL1_XXX_MASK.
  * @param  u32ClkSrc The selected clock source. 
  * @return None.
  */
static __INLINE void SYS_SelectIPClockSource_1(uint32_t u32ClkMask, uint32_t u32ClkSrc)
{
 	CLK->CLKSEL1 = (CLK->CLKSEL1 & (~u32ClkMask)) | u32ClkSrc;
}


/**
  * @brief  Select clock source for peripheral functions : FRQDIV, PWM1_CH01, PWM2_CH23, Timer 2, Timer 3, I2S, SC, SPI 0, SPI 1, SPI 2. 
  * @param  u32ClkMask The mask filed used to chnage clock source. Please refer nano1xx.h for possible values : CLK_CLKSEL1_XXX_MASK.
  * @param  u32ClkSrc The selected clock source.
  * @return None.
  */
static __INLINE void SYS_SelectIPClockSource_2(uint32_t u32ClkMask, uint32_t u32ClkSrc)
{
	CLK->CLKSEL2 = (CLK->CLKSEL2 & (~u32ClkMask)) | u32ClkSrc;
}


/**
  * @brief  Select clock divider for HCLK 
  * @param  u32DivValue The clock divider, HCLK_CLK_DIVIDER(x) where x is 0 ~ 15. 
  * @return None.
  */
static __INLINE void SYS_SetHCLKDivider(int32_t u32DivValue)
{
    CLK->CLKDIV0 = (CLK->CLKDIV0 & (~CLK_CLKDIV0_HCLK_MASK)) | u32DivValue;

	SystemCoreClockUpdate();
}



/**
  * @brief  Select clock divider for peripheral functions : USB, I2S, ADC, TK and SC0. 
  * @param  u32ClkDivMask The mask filed used to chnage clock divider. Please refer nano1xx.h for possible values : CLK_CLKDIV0_XXX_MASK.
  * @param  u32DivValue The clock divider. XXX_CLK_DIVIDER(y) where XXX can be USB, I2S, ADC, TK or SC0. The y is 0 ~ 255 for ADC, and 0 ~ 15 for others.
  * @return None.
  */
static __INLINE void SYS_SetClockDivider_0(uint32_t u32ClkDivMask, uint32_t u32DivValue)
{
    CLK->CLKDIV0 = (CLK->CLKDIV0 & (~u32ClkDivMask)) | u32DivValue;
}


/**
  * @brief  Select clock divider for peripheral functions : SC1 and SC2. 
  * @param  u32ClkDivMask The mask filed used to chnage clock divider. Please refer nano1xx.h for possible values : CLK_CLKDIV0_XXX_MASK.
  * @param  u32DivValue The clock divider. XXX_CLK_DIVIDER(y) where XXX can be SC1 or SC2. The y is 0 ~ 15
  * @return None.
  */
static __INLINE void SYS_SetClockDivider_1(uint32_t u32ClkDivMask, uint32_t u32DivValue)
{
    CLK->CLKDIV1 = (CLK->CLKDIV1 & (~u32ClkDivMask)) | u32DivValue;
}



/**
  * @brief  Enable or disable internal oscillators and external crystals. 
  * @param  u32OscSrc The combination of CLK_PWRCTL_HXT_EN, CLK_PWRCTL_LXT_EN, CLK_PWRCTL_HIRC_EN and CLK_PWRCTL_LIRC_EN
  * @param  u32Enable This value is 1 to enable clock, and 0 for disable. 
  * @return None.
  */
static __INLINE void SYS_SetChipClockSrc(uint32_t u32OscSrc, uint32_t u32Enable)
{	
	(u32Enable == 1) ? (CLK->PWRCTL |= u32OscSrc) : (CLK->PWRCTL &= ~u32OscSrc);

}


/**
  * @brief  Check the status of selected chip clock sources 
  * @param  u32ChipClkSrc The combination of CLK_CLKSTATUS_HXT_STB, CLK_CLKSTATUS_LXT_STB, CLK_CLKSTATUS_PLL_STB, 
  *                                       CLK_CLKSTATUS_LIRC_STB, CLK_CLKSTATUS_HIRC_STB
  * @return clock status
  * @retval -1 one or more clock sources are unstable or disabled.
  * @retval 0 all selected clock sources are stable
  */
static __INLINE int32_t SYS_CheckChipClockSrc(uint32_t u32ChipClkSrc)
{
	if ((CLK->CLKSTATUS & u32ChipClkSrc) == u32ChipClkSrc)
		return 0;
	else
		return -1;
}


/**
  * @brief  Select PLL clock source. 
  * @param  u32PLLSrc The PLL clock source, either CLK_PLLCTL_PLLSRC_HIRC or CLK_PLLCTL_PLLSRC_HXT.
  * @return None.
  */
static __INLINE void SYS_SelectPLLSource(uint32_t u32PLLSrc)
{
	CLK->PLLCTL = (CLK->PLLCTL & ~CLK_PLLCTL_PLLSRC_MASK) | u32PLLSrc;
}


/**
  * @brief  Set PLL operate in power down or normal mode. 
  * @param  u32PowerDown 1 for power down mode while 0 for normal mode.
  * @return None.
  */
static __INLINE void SYS_SetPLLMode(uint32_t u32PowerDown)
{
	(u32PowerDown == 1) ? (CLK->PLLCTL |= CLK_PLLCTL_PD) : (CLK->PLLCTL &= ~CLK_PLLCTL_PD);
}


/**
  * @brief  Set PLL output frequency. 
  * @param  u32PllClkSrc The clock source, either CLK_PLLCTL_PLLSRC_HIRC or CLK_PLLCTL_PLLSRC_HXT.
  * @param  u32PllFreq The output frequency of PLL.	The supported value is defined in nano1xx_sys.h
  * @return None.
  */
static __INLINE void SYS_SetPLLFreq(uint32_t u32PllClkSrc, uint32_t u32PllFreq)
{
	CLK->PLLCTL = (u32PllClkSrc | u32PllFreq);
}


/**
  * @brief  Enable frequency clock output and set its divider value. The output frequency = Fin/2^(N+1), 
  *         Fin is the input clock frequency, N is 4-bit value 
  * @param  i32Flag 1 for Enable while 0 for Disable.
  * @param  u8Divider The divider of output frequency (0~15)
  * @return None.
  */
static __INLINE void SYS_SetFreqDividerOutput(int32_t i32Flag, uint8_t u8Divider)
{
	if (i32Flag)
		CLK->FRQDIV = CLK_FRQDIV_EN | u8Divider;
	else		
		CLK->FRQDIV &= ~CLK_FRQDIV_EN; 		
	   
	SYS_SetIPClock_2(CLK_APBCLK_FDIV_EN, i32Flag);
}


/**
  * @brief  Get the external clock frequency
  * @param None. 
  * @return __XTAL.
  */
static __INLINE int32_t SYS_GetExtClockFreq()
{
	return __XTAL;
}


/**
  * @brief  Check the status of PLL clock output
  * @param None. 
  * @return PLL clock status
  * @retval -1 PLL output clock is unstable or disabled.
  * @retval 0 PLL output clock is stable
  */
static __INLINE int32_t SYS_GetPLLStatus()
{
	int32_t i32Sts;

	(CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB) ? (i32Sts = 0) : (i32Sts = -1);
	return i32Sts;
}


/**
  * @brief  Check the result of switching HCLK source. 
  * @param None.
  * @ return clock switch status
  * @retval 0  success.
  * @retval -1  failed.
  */
static __INLINE uint32_t SYS_GetClockSwitchStatus(void)
{	
	int32_t i32Sts;

	(CLK->CLKSTATUS & CLK_CLKSTATUS_CLK_SW_FAIL) ? (i32Sts = -1) : (i32Sts = 0);
	return i32Sts;
}


/**
  * @brief  clear the clock switch failure status 
  * @param None.
  * @return None.
  */
static __INLINE void SYS_ClearClockSwitchStatus(void)
{
	 CLK->CLKSTATUS |= CLK_CLKSTATUS_CLK_SW_FAIL;
}	


/*---------------------------------------------------------------------------------------------------------*/
/* Define SYS functions prototype                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t  SYS_SelectHCLKSource(uint32_t u32ClkSrc);
void     SYS_SetUpPowerDown(uint32_t u32Enable);
uint32_t SYS_GetPLLClockFreq(void);
uint32_t SYS_GetHCLKFreq(void);
int32_t  SYS_InitChipClock(S_SYS_CHIP_CLKCFG *sChipCfg);
void     SYS_Delay(uint32_t us);

/*@}*/ /* end of group NANO1xx_SYS_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_SYS_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

#endif //__NANO1XX_SYS_H

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
