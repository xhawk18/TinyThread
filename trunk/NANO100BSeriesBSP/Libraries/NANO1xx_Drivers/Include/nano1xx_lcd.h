/******************************************************************************
 * @file     nano1xx_lcd.h
 * @brief    NANO1xx LCD driver header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __NANO1XX_LCD_H
#define __NANO1XX_LCD_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "nano1xx.h"



/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_LCD_Driver LCD Driver
  @{
*/


/** @addtogroup NANO1xx_LCD_EXPORTED_CONSTANTS LCD Exported Constants
  @{
*/

/// @cond
/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVLCD_MAJOR_NUM	 1
#define DRVLCD_MINOR_NUM	 0
#define DRVLCD_BUILD_NUM	 1

/* Define module version number */
#define DRVLCD_VERSION_NUM (((DRVLCD_MAJOR_NUM) << 16) | ((DRVLCD_MINOR_NUM) << 8) | (DRVLCD_BUILD_NUM))

/// @endcond

#ifdef __LCDDISPLAY_GFN616A_H  
typedef enum
{
	LCD_SYMBOL_S1,						/* 0 */
	LCD_SYMBOL_S2,						/* 1 */
	LCD_SYMBOL_S3,						/* 2 */
	LCD_SYMBOL_S4,						/* 3 */
	LCD_SYMBOL_S5,						/* 4 */
	LCD_SYMBOL_S_EXCLAMATION,		/* 5 */
	LCD_SYMBOL_EXCLAMATION,		/* 6 */
	LCD_SYMBOL_PT,						/* 7 */
	LCD_SYMBOL_CT,						/* 8 */
	LCD_SYMBOL_LEFTARROW,			/* 9 */
	LCD_SYMBOL_RIGHTARROW,			/* 10 */
	LCD_SYMBOL_COM,					/* 11 */
	LCD_SYMBOL_BAL,					/* 12 */
	LCD_SYMBOL_PAR,					/* 13 */
	LCD_SYMBOL_T1,						/* 14 */
	LCD_SYMBOL_T2,						/* 15 */
	LCD_SYMBOL_T3,						/* 16 */
	LCD_SYMBOL_T4,						/* 17 */
	LCD_SYMBOL_L1,						/* 18 */
	LCD_SYMBOL_L2,						/* 19 */
	LCD_SYMBOL_L3,						/* 20 */
	LCD_SYMBOL_11,						/* 21 */
	LCD_SYMBOL_21,						/* 22 */
	LCD_SYMBOL_22,						/* 23 */
	LCD_SYMBOL_32,						/* 24 */
	LCD_SYMBOL_33,						/* 25 */
	LCD_SYMBOL_13,						/* 26 */
	LCD_SYMBOL_PULSEONE,			/* 27 */
	LCD_SYMBOL_PULSETWO,			/* 28 */
	LCD_SYMBOL_UPDOWN,				/* 29 */
	LCD_SYMBOL_DASH,					/* 30 */
	LCD_SYMBOL_MRIGHTSIDE,			/* 31 */
	LCD_SYMBOL_MLEFTSIDE,			/* 32 */
	LCD_SYMBOL_COL,					/* 33 */
	LCD_SYMBOL_M1,						/* 34 */
	LCD_SYMBOL_K1,						/* 35 */
	LCD_SYMBOL_V,						/* 36 */
	LCD_SYMBOL_A,						/* 37 */
	LCD_SYMBOL_H2,						/* 38 */
	LCD_SYMBOL_N,						/* 39 */
	LCD_SYMBOL_M2,						/* 40 */
	LCD_SYMBOL_K2,						/* 41 */
	LCD_SYMBOL_W,						/* 42 */
	LCD_SYMBOL_H3,						/* 43 */
	LCD_SYMBOL_M3,						/* 44 */
	LCD_SYMBOL_K3,						/* 45 */
	LCD_SYMBOL_VAR,					/* 46 */
	LCD_SYMBOL_H1,						/* 47 */
	LCD_SYMBOL_TH,						/* 48 */
	LCD_SYMBOL_D,						/* 49 */
	LCD_SYMBOL_PF,						/* 50 */
	LCD_SYMBOL_HZ,						/* 51 */
	LCD_SYMBOL_DMD,					/* 52 */
	LCD_SYMBOL_MAX,					/* 53 */
	LCD_SYMBOL_MIN,					/* 54 */
	LCD_SYMBOL_ERR,					/* 55 */
} E_LCDSYMBOL;
#endif
#ifdef __LCDDISPLAY_PD_878_H
typedef enum
{
	LCD_SYMBOL_XXX,						/* 0 */
} E_LCDSYMBOL;
#endif
#ifdef __LCDDISPLAY_BTL001_H
typedef enum
{
	LCD_SYMBOL_PLUS,					/* 0 */
	LCD_SYMBOL_MINUS,					/* 1 */
	LCD_SYMBOL_1DP,					/* 2 */
	LCD_SYMBOL_2DP,					/* 3 */
	LCD_SYMBOL_3DP,					/* 4 */
	LCD_SYMBOL_4DP,					/* 5 */
	LCD_SYMBOL_5DP,					/* 6 */
	LCD_SYMBOL_6DP,					/* 7 */
	LCD_SYMBOL_8DP,					/* 8 */
	LCD_SYMBOL_10DP,					/* 9 */
	LCD_SYMBOL_11DP,					/* 10 */
	LCD_SYMBOL_2COL,					/* 11 */
	LCD_SYMBOL_3COL,					/* 12 */
	LCD_SYMBOL_4COL,					/* 13 */
	LCD_SYMBOL_5COL,					/* 14 */
	LCD_SYMBOL_6COL,					/* 15 */
	LCD_SYMBOL_10COL,					/* 16 */
	LCD_SYMBOL_PL,						/* 17 */
	LCD_SYMBOL_P0,						/* 18 */
	LCD_SYMBOL_P1,						/* 19 */
	LCD_SYMBOL_P2,						/* 20 */
	LCD_SYMBOL_P3,						/* 21 */
	LCD_SYMBOL_P4,						/* 22 */
	LCD_SYMBOL_P5,						/* 23 */
	LCD_SYMBOL_P6,						/* 24 */
	LCD_SYMBOL_P7,						/* 25 */
	LCD_SYMBOL_P8,						/* 26 */
	LCD_SYMBOL_P9,						/* 27 */
	LCD_SYMBOL_PR,						/* 28 */
	LCD_SYMBOL_BRBL,					/* 29 */
	LCD_SYMBOL_B0,						/* 30 */
	LCD_SYMBOL_B1,						/* 31 */
	LCD_SYMBOL_B2,						/* 32 */	
	LCD_SYMBOL_SB,						/* 33 */	
} E_LCDSYMBOL;
#endif
#ifdef __LCDDISPLAY_BTL001_LB_H
typedef enum
{
	LCD_SYMBOL_3DP,					/* 0 */
	LCD_SYMBOL_4DP,					/* 1 */
	LCD_SYMBOL_5DP,					/* 2 */
	LCD_SYMBOL_6DP,					/* 3 */
	LCD_SYMBOL_11DP,					/* 4 */
	LCD_SYMBOL_4COL,					/* 5 */
	LCD_SYMBOL_5COL,					/* 6 */
	LCD_SYMBOL_6COL,					/* 7 */
	LCD_SYMBOL_10COL,					/* 8 */
	LCD_SYMBOL_PL,						/* 9 */
	LCD_SYMBOL_P0,						/* 10 */
	LCD_SYMBOL_P1,						/* 11 */
	LCD_SYMBOL_P2,						/* 12 */
	LCD_SYMBOL_P3,						/* 13 */
	LCD_SYMBOL_P4,						/* 14 */
	LCD_SYMBOL_P5,						/* 15 */
	LCD_SYMBOL_P6,						/* 16 */
	LCD_SYMBOL_P7,						/* 17 */
	LCD_SYMBOL_P8,						/* 18 */
	LCD_SYMBOL_P9,						/* 19 */
	LCD_SYMBOL_PR,						/* 20 */
	LCD_SYMBOL_BRBL,					/* 21 */
	LCD_SYMBOL_B0,						/* 22 */
	LCD_SYMBOL_B1,						/* 23 */
	LCD_SYMBOL_B2,						/* 24 */	
	LCD_SYMBOL_SB,						/* 25 */	
} E_LCDSYMBOL;
#endif

#define LCD_FREQ_DIV32		((uint32_t) 0x00000000)	/*!< Clock source (32 or 10KHz) divide by 32 */
#define LCD_FREQ_DIV64		((uint32_t) 0x00000010)	/*!< Clock source (32 or 10KHz) divide by 64 */
#define LCD_FREQ_DIV96		((uint32_t) 0x00000020)	/*!< Clock source (32 or 10KHz) divide by 96 */
#define LCD_FREQ_DIV128		((uint32_t) 0x00000030)	/*!< Clock source (32 or 10KHz) divide by 128 */
#define LCD_FREQ_DIV192		((uint32_t) 0x00000040)	/*!< Clock source (32 or 10KHz) divide by 192 */
#define LCD_FREQ_DIV256		((uint32_t) 0x00000050)	/*!< Clock source (32 or 10KHz) divide by 256 */
#define LCD_FREQ_DIV384		((uint32_t) 0x00000060)	/*!< Clock source (32 or 10KHz) divide by 384 */
#define LCD_FREQ_DIV512		((uint32_t) 0x00000070)	/*!< Clock source (32 or 10KHz) divide by 512 */

#define LCD_MUX_STATIC		((uint32_t) 0x00000000)	/*!< Static multiplexing */
#define LCD_MUX_ONE_SECOND	((uint32_t) 0x00000002)	/*!< Duplex multiplexing */
#define LCD_MUX_ONE_THIRD	((uint32_t) 0x00000004)	/*!< Triplex multiplexing */
#define LCD_MUX_ONE_FOURTH	((uint32_t) 0x00000006)	/*!< Quadruplex multiplexing */
#define LCD_MUX_ONE_FIFTH	((uint32_t) 0x00000008)	/*!< 1/5 duty */
#define LCD_MUX_ONE_SIXTH	((uint32_t) 0x0000000A)	/*!< 1/6 duty */

#define LCD_BIAS_STATIC		((uint32_t) 0x00000000)	/*!< Static bias */
#define LCD_BIAS_HALF		((uint32_t) 0x00000002)	/*!< 1/2 bias */
#define LCD_BIAS_THIRD		((uint32_t) 0x00000004)	/*!< 1/3 bias */

#define LCD_CPUMP_DIV1 		((uint32_t) 0x00000000)	/*!< Clock source (32 or 10KHz) divide by 1 and is used by analog component */
#define LCD_CPUMP_DIV2 		((uint32_t) 0x00000800)	/*!< Clock source (32 or 10KHz) divide by 2 */
#define LCD_CPUMP_DIV4 		((uint32_t) 0x00001000)	/*!< Clock source (32 or 10KHz) divide by 4 */
#define LCD_CPUMP_DIV8 		((uint32_t) 0x00001800)	/*!< Clock source (32 or 10KHz) divide by 8 */
#define LCD_CPUMP_DIV16 	((uint32_t) 0x00002000)	/*!< Clock source (32 or 10KHz) divide by 16 */
#define LCD_CPUMP_DIV32 	((uint32_t) 0x00002800)	/*!< Clock source (32 or 10KHz) divide by 32 */
#define LCD_CPUMP_DIV64 	((uint32_t) 0x00003000)	/*!< Clock source (32 or 10KHz) divide by 64 */
#define LCD_CPUMP_DIV128	((uint32_t) 0x00003100)	/*!< Clock source (32 or 10KHz) divide by 128 */

#define LCD_CPVOl_2_6V	((uint32_t) 0x00000000)	/*!< Set charge pump voltage to 2.6 V */
#define LCD_CPVOl_2_7V	((uint32_t) 0x00000100)	/*!< Set charge pump voltage to 2.7 V */
#define LCD_CPVOl_2_8V	((uint32_t) 0x00000200)	/*!< Set charge pump voltage to 2.8 V */
#define LCD_CPVOl_2_9V	((uint32_t) 0x00000300)	/*!< Set charge pump voltage to 2.9 V */
#define LCD_CPVOl_3V	((uint32_t) 0x00000400)	/*!< Set charge pump voltage to 3 V */
#define LCD_CPVOl_3_1V	((uint32_t) 0x00000500)	/*!< Set charge pump voltage to 3.1 V */
#define LCD_CPVOl_3_2V	((uint32_t) 0x00000600)	/*!< Set charge pump voltage to 3.2 V */
#define LCD_CPVOl_3_3V	((uint32_t) 0x00000700)	/*!< Set charge pump voltage to 3.3 V */

#define LCD_FCPRESC_DIV1 ((uint32_t) 0x00000000)	/*!< Set pre-scale divider value to 1 */
#define LCD_FCPRESC_DIV2 ((uint32_t) 0x00000004)	/*!< Set pre-scale divider value to 2 */
#define LCD_FCPRESC_DIV4 ((uint32_t) 0x00000008)	/*!< Set pre-scale divider value to 4 */
#define LCD_FCPRESC_DIV8 ((uint32_t) 0x0000000C)	/*!< Set pre-scale divider value to 8 */

#define LCD_FRAMECOUNT_INT	((uint32_t) 0x00000001)	/*!< Indicate frame count interrupt */
#define LCD_POWERDOWN_INT 	((uint32_t) 0x00000002)	/*!< Indicate power down interrupt */
#define LCD_ALL_INT 		((uint32_t) 0x00000003)	/*!< Indicate frame count & power down interrupt */

#define ERR_LCD_CAL_BLINK_FAIL		0xFFFF0000				/*!< Specifies that overflow to calculate the blinking frequency */

#define LCD_LQFP100_PIN				/*!< If ROMMAP[0] Bit[10] is clear and Bit[9] is set */
//#define LCD_LQFP64_PIN				/*!< If ROMMAP[0] Bit[9] is clear and Bit[10] is set */

/*@}*/ /* end of group NANO1xx_LCD_EXPORTED_CONSTANTS */



/** @addtogroup NANO1xx_LCD_EXPORTED_STRUCTS LCD Exported Structs
  @{
*/
typedef struct
{
	bool	cpump_enable;						/*!< Specifies if enable charge pump or not */

	bool	internal_bias;						/*!< Specifies if enable internal bias reference ladder or not */	
	
	uint32_t    mux;						/*!< Specifies Mux configuration */

	uint32_t    bias;						/*!< Specifies Bias configuration */

	uint32_t 	freqdiv;					/*!< Specifies LCD Frequency Divider Selection */	

	uint32_t	cpump_freqdiv;		/*!< Specifies frequency divider of charge pump */
	uint32_t	cpump_voltage;		/*!< Specifies voltage of charge pump */
} S_LCD_INIT;
/*@}*/ /* end of group NANO1xx_LCD_EXPORTED_STRUCTS */



/** @addtogroup NANO1xx_LCD_EXPORTED_FUNCTIONS LCD Exported Functions
  @{
*/
void LCD_Init(S_LCD_INIT *lcdinit);
void LCD_DeInit(void);
void LCD_ChargePumpInit(S_LCD_INIT *lcdinit);
void LCD_IRQHandler(void);


void LCD_AllOff(void);
void LCD_AllOn(void);

void LCD_FrameCountEnable( uint32_t prediv, uint32_t fcv );
void LCD_FrameCountDisable(void);

void LCD_EnableSegment(int com, int bitvalue);
void LCD_DisableSegment(int com, int bitvalue);


void LCD_Number(long long value);
void LCD_NumberOff(void);
void LCD_AlphaNumberOff(void);
void LCD_Symbol(E_LCDSYMBOL s, int on);
void LCD_Write(char *string);

int32_t LCD_BlinkFrequency(double mdelay);
__INLINE void LCD_EnableBlink(void);
void LCD_DisableBlink(void);

void LCD_CLKSrcSel(int src);

__INLINE void LCD_Disable(void);

__INLINE void LCD_PowerDownDisplay(int32_t keepdpy);

void LCD_EnableInt(uint32_t IntSrc);
void LCD_DisableInt(uint32_t IntSrc);

void LCD_SegmentPinEnable(uint32_t pinno, bool enable);
void LCD_UpdateLCDFrameFreq(void);
void LCD_UpdateChargePumpFreq(void);
__INLINE int32_t	LCD_GetVersion(void);

/**
 *  @brief Control the level of LCD contrast. This function is only workable when using internal charge pump
 *
 *  @param[in] level   \n                     
 *                    LCD_CPVOl_2_6V: Set charge pump voltage to 2.6 V \n
 *                    LCD_CPVOl_2_7V: Set charge pump voltage to 2.7 V \n
 *                    LCD_CPVOl_2_8V: Set charge pump voltage to 2.8 V \n
 *                    LCD_CPVOl_2_9V: Set charge pump voltage to 2.9 V \n
 *                    LCD_CPVOl_3V  : Set charge pump voltage to 3.0 V \n
 *                    LCD_CPVOl_3_1V: Set charge pump voltage to 3.1 V \n
 *                    LCD_CPVOl_3_2V: Set charge pump voltage to 3.2 V \n
 *                    LCD_CPVOl_3_3V: Set charge pump voltage to 3.3 V
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code 
 *  LCD_ContrastSet (LCD_CPVOl_3V);
 *  @endcode
 *
 */
__INLINE void LCD_ContrastSet(uint32_t level)
{
	LCD->DISPCTL = LCD->DISPCTL & ~LCD_DISPCTL_CPUMP_VOL_SET | level;
}


/**
 *  @brief Enable LCD controller
 *
 *  @param None
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  LCD_Enable();
 *  @endcode
 *
 */
__INLINE void LCD_Enable(void)
{
	/* Enable LCD */
	LCD->CTL |= LCD_CTL_EN;
}


/**
 *  @brief Enable Blink function in LCD controller
 *
 *  @param None
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  LCD_EnableBlink( );
 *  @endcode
 *
 */
__INLINE void LCD_EnableBlink(void)
{
	/* Enable Blink LCD */
	LCD->CTL |= LCD_CTL_BLINK;
}


/**
 *  @brief Set LCD voltage level of charge pump
 *
 *  @param[in] vol  \n
 *                  Set the voltage of charge pump \n
 *                  LCD_CPVOl_2_6V: Set charge pump voltage to 2.6 V
 *                  LCD_CPVOl_2_7V: Set charge pump voltage to 2.7 V
 *                  LCD_CPVOl_2_8V: Set charge pump voltage to 2.8 V
 *                  LCD_CPVOl_2_9V: Set charge pump voltage to 2.9 V
 *                  LCD_CPVOl_3V  : Set charge pump voltage to 3.0 V
 *                  LCD_CPVOl_3_1V: Set charge pump voltage to 3.1 V
 *                  LCD_CPVOl_3_4V: Set charge pump voltage to 3.2 V
 *                  LCD_CPVOl_3_3V: Set charge pump voltage to 3.3 V
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  // set LCD charge pump voltage to 3V
 *  LCD_ChargePump_VolSet(LCD_CPVOl_3V)
 *  @endcode
 *
 */
__INLINE void LCD_ChargePump_VolSet(uint32_t vol)
{
	LCD->DISPCTL = LCD->DISPCTL & ~LCD_DISPCTL_CPUMP_VOL_SET | vol;
}

/**
 *  @brief Disables LCD controller
 *
 *  @param None
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  LCD_Disable();
 *  @endcode
 *
 */
__INLINE void LCD_Disable(void)
{
	/* Disable LCD */
	LCD->CTL &= ~LCD_CTL_EN;
}



/**
 *  @brief Keep/Disable display when system get into power down mode
 *
 *  @param[in] keepdpy   1: keep display   \n
 *                       0: disable display
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  // Keep display when system get into power down mode
 *  LCD_PowerDownDisplay(1);
 *  @endcode
 *
 */
__INLINE void LCD_PowerDownDisplay(int32_t keepdpy)
{
	/* Power Down display function  */
	LCD->CTL &= ~LCD_CTL_PDDISP_EN;
	LCD->CTL |= (keepdpy) ? LCD_CTL_PDDISP_EN:0;
}


/**
 *  @brief Return the current version number of driver
 *
 *  @param None
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return DRVLCD_VERSION_NUM    LCD driver current version number: \n
 *                                [31:24]: 00000000   \n
 *                                [23:16]: MAJOR_NUM  \n
 *                                [15: 8]: MINOR_NUM  \n
 *                                [ 7: 0]: BUILD_NUM  
 *
 *  @par Example
 *  @code
 *  int32_t LCD_Version
 *  LCD_Version = LCD_GetVersion();
 *  @endcode
 *
 */
__INLINE int32_t LCD_GetVersion(void)
{
	return DRVLCD_VERSION_NUM;
}


/*@}*/ /* end of group NANO1xx_LCD_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO1xx_LCD_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */


#ifdef __cplusplus
}
#endif

#endif	/* __NANO1XX_LCD_H */



/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


