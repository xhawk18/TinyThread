/******************************************************************************
 * @file     nano1xx_pwm.h
 * @brief    NANO1xx PWM driver header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef _NANO1XX_PWM_H
#define _NANO1XX_PWM_H



#include "nano1xx.h"

#ifdef  __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_PWM_Driver PWM Driver
  @{
*/

/** @addtogroup NANO1xx_PWM_EXPORTED_CONSTANTS PWM Exported Constants
  @{
*/

#define PWM_CH_NUM      8

/*@}*/ /* end of group NANO1xx_PWM_EXPORTED_CONSTANTS */


/** @addtogroup NANO1xx_PWM_EXPORTED_STRUCTS PWM Exported Structs
  @{
*/
/** Structure used to configure PWM output waveform */
typedef struct
{
	uint32_t  u32Cn;	 ///< CN (0~0xFFFF) or (0~0xFFFFFFFF) in cascade capture mode. 0 will stop PWM counter
	uint16_t  u16Cm;	 ///< CM (0~0xFFFF)
	uint8_t	  u8Mode;	 ///< PWM_CTL_CHMOD_AUTO_RELOAD or PWM_CTL_CHMOD_ONE_SHOT
	uint8_t	  u8PreScale;	 ///< Valid range is between 1~0xFF
	uint8_t   u8Div;	 ///< PWM_CLKSEL_CLKSEL_DIV1, PWM_CLKSEL_CLKSEL_DIV2, PWM_CLKSEL_CLKSEL_DIV4, PWM_CLKSEL_CLKSEL_DIV8, or PWM_CLKSEL_CLKSEL_DIV16
 	uint8_t	  u8Inv;	 ///< 0: output waveform not inversed, Other: output inversed waveform
}S_DRVPWM_TIME_DATA_T;

/*@}*/ /* end of group NANO1xx_PWM_EXPORTED_STRUCTS */


/** @addtogroup NANO1xx_PWM_EXPORTED_FUNCTIONS PWM Exported Functions
  @{
*/
// Generic APIs
void PWM_Init(uint8_t u8Ch);
void PWM_DeInit(uint8_t u8Ch);
void PWM_SetCounter(uint8_t u8Ch, uint32_t u32Cn, uint16_t u16Cm);
void PWM_SetClk(uint8_t u8Ch, S_DRVPWM_TIME_DATA_T *cfg);
// Counter APIs
void PWM_EnableInt(uint8_t u8Ch);
void PWM_DisableInt(uint8_t u8Ch);
void PWM_ClearInt(uint8_t u8Ch);
uint32_t PWM_GetIntFlag(uint8_t u8Ch);
void PWM_EnableDeadZone(uint8_t u8Ch, uint8_t u8Length);
void PWM_DisableDeadZone(uint8_t u8Ch);
void PWM_Enable(uint8_t u8Ch);
void PWM_Disable(uint8_t u8Ch);
void PWM_ConfigOutput(uint8_t u8Ch, uint32_t u32Enable);
// Capture APIs
void PWM_EnableCaptureInt(uint8_t u8Ch, uint8_t u8Int);
void PWM_DisableCaptureInt(uint8_t u8Ch);
void PWM_ClearCaptureInt(uint8_t u8Ch);
uint32_t PWM_GetCaptureIntFlag(uint8_t u8Ch);
uint32_t PWM_GetRisingCounter(uint8_t u8Ch);
uint32_t PWM_GetFallingCounter(uint8_t u8Ch);
void PWM_CaptureEnable(uint8_t u8Ch);
void PWM_CaptureDisable(uint8_t u8Ch);
void PWM_ConfigCaptureIntput(uint8_t u8Ch, uint32_t u32Enable);
void PWM_ConfigCascade(uint8_t u8Ch, uint32_t u32Enable);
void PWM_EnablePdma(uint8_t u8Ch, uint32_t u32Order, uint32_t u32Mode);
void PWM_DisablePdma(uint8_t u8Ch);
void PWM_SetCaptureAttribute(uint8_t u8Ch, uint8_t u8Inv, uint8_t u8Cas, uint32_t u32Reload);

/*@}*/ /* end of group NANO1xx_PWM_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO1xx_PWM_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

#ifdef  __cplusplus
}
#endif

#endif  //_NANO1XX_PWM_H

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


