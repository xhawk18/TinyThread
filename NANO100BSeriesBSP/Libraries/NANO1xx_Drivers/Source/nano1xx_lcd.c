/******************************************************************************
 * @file     nano1xx_lcd.c
 * @brief    NANO1xx LCD driver source file.
 *			 The LCD driver can directly drives a LCD glass by creating the ac
 *           segment and common voltage signals automatically. It can support 
 *           static, 1/2 duty, 1/3 duty and 1/4 duty LCD glass with up to 38 
 *           segments with 6 COM or 40 segments with 4 COM.
 * @version  1.0.2
 * @date     26, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "nano1xx.h"
#include "nano1xx_lcd.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_LCD_Driver LCD Driver
  @{
*/

/// @cond
#ifdef __LCDDISPLAY_PD_878_H
#include "nano1xx_lcd_pd_878.h"
#endif
#ifdef __LCDDISPLAY_GFN616A_H
#include "nano1xx_lcd_gfn616a.h"
#endif
#ifdef __LCDDISPLAY_BTL001_H
#include "nano1xx_lcd_btl001.h"
#endif
#ifdef __LCDDISPLAY_BTL001_LB_H
#include "nano1xx_lcd_btl001_lb.h"
#endif


//#define LCD_DEBUG
#ifdef LCD_DEBUG
#define LCDDEBUG     printf
#else
#define LCDDEBUG(...)
#endif

/// @endcond


/** @addtogroup NANO1xx_LCD_EXPORTED_CONSTANTS LCD Exported Constants
  @{
*/
#define LCD_FCR_FCV_BIT_ADDR	4

#define LCD_CTL_FREQ_BIT_ADDR	4
#define LCD_CTL_MUX_BIT_ADDR	1

/*@}*/ /* end of group NANO1xx_LCD_EXPORTED_CONSTANTS */


/** @addtogroup NANO1xx_LCD_EXPORTED_VARIABLES LCD Exported Variables
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

uint32_t g_LCDClkSrc = 32768;	// in default, clock source from 32K
double g_LCDFreq;
double g_LCDFrameRate;	/* src:32768Hz, COM:4, FREQ Div:64, frame-rate 64Hz  */
								/* src:10240Hz, COM:4, FREQ Div:32, frame-rate 40Hz  */
double g_ChargePumpFreq;					/* (Hz) */								
double g_LCDFrameCountFreq;			/* (Hz) */
double g_LCDFrameCountINTFreq;		/* indicate the frequency of frame count interrupt (Hz) */

/*@}*/ /* end of group NANO1xx_LCD_EXPORTED_VARIABLES */


/** @addtogroup NANO1xx_LCD_EXPORTED_FUNCTIONS LCD Exported Functions
  @{
*/

/**
 *  @brief Enables a segment on the LCD display
 *
 *  @param[in] com      COM segment number
 *  @param[in] bitvalue Bit number for segment 
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  // turn on COM3-SEG10 dot
 *  LCD_EnableSegment ( 3, 10 );
 *  @endcode
 *
 */
void LCD_EnableSegment(int com, int bitvalue)
{
	int32_t memnum = bitvalue / 4;
	int32_t seg_shift = 8*(bitvalue-(4*memnum));

	if(memnum==0)
	{
		LCD->MEM_0 |= (1<<com)<<seg_shift;
	}
	else if(memnum==1)
	{
		LCD->MEM_1 |= (1<<com)<<seg_shift;
	}
	else if(memnum==2)
	{
		LCD->MEM_2 |= (1<<com)<<seg_shift;
	}
	else if(memnum==3)
	{
		LCD->MEM_3 |= (1<<com)<<seg_shift;
	}
	else if(memnum==4)
	{
		LCD->MEM_4 |= (1<<com)<<seg_shift;
	}
	else if(memnum==5)
	{
		LCD->MEM_5 |= (1<<com)<<seg_shift;
	}
	else if(memnum==6)
	{
		LCD->MEM_6 |= (1<<com)<<seg_shift;
	}
	else if(memnum==7)
	{
		LCD->MEM_7 |= (1<<com)<<seg_shift;
	}
	else if(memnum==8)
	{
		LCD->MEM_8 |= (1<<com)<<seg_shift;
	}
	else if(memnum==9)
	{
		LCD->MEM_9 |= (1<<com)<<seg_shift;
	}
}

/**
 *  @brief Disables a segment on the LCD Display
 *
 *  @param[in] com      COM segment number
 *  @param[in] bitvalue Bit number for segment 
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  // turn off COM3-SEG10 dot
 *  LCD_DisableSegment ( 3, 10 );
 *  @endcode
 *
 */
void LCD_DisableSegment(int com, int bitvalue)
{
	int32_t memnum = bitvalue / 4;
	int32_t seg_shift = 8*(bitvalue-(4*memnum));

	if(memnum==0)
	{
		LCD->MEM_0 &= ~((1<<com)<<seg_shift);
	}
	else if(memnum==1)
	{
		LCD->MEM_1 &= ~((1<<com)<<seg_shift);
	}
	else if(memnum==2)
	{
		LCD->MEM_2 &= ~((1<<com)<<seg_shift);
	}
	else if(memnum==3)
	{
		LCD->MEM_3 &= ~((1<<com)<<seg_shift);
	}
	else if(memnum==4)
	{
		LCD->MEM_4 &= ~((1<<com)<<seg_shift);
	}
	else if(memnum==5)
	{
		LCD->MEM_5 &= ~((1<<com)<<seg_shift);
	}
	else if(memnum==6)
	{
		LCD->MEM_6 &= ~((1<<com)<<seg_shift);
	}
	else if(memnum==7)
	{
		LCD->MEM_7 &= ~((1<<com)<<seg_shift);
	}
	else if(memnum==8)
	{
		LCD->MEM_8 &= ~((1<<com)<<seg_shift);
	}
	else if(memnum==9)
	{
		LCD->MEM_9 &= ~((1<<com)<<seg_shift);
	}
}


/**
 *  @brief Write number on numeric part on LCD display
 *
 *  @param[in] value \n	 
 *                   Numeric value to put on display, in range -9999999 to +99999999 (LQFP64 package) \n
 *	                And in range -999999999 to +9999999999 (LQFP100 package)
 *
 *  @par Include:
 * 	  nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  // show 99999999 number
 *  LCD_Number ( 99999999 );
 *  @endcode
 *
 */
void LCD_Number(long long value)
{
	long long num, i, com, bit, digit, div, neg, len, tmp;
  	uint16_t bitpattern;

  	/* Parameter consistancy check */
#ifdef __LCDDISPLAY_PD_878_H	
#ifdef LCD_LQFP100_PIN	
  	if (value >= 9999999999)
#elif defined(LCD_LQFP64_PIN)
  	if (value >= 99999999)
#endif
  	{
#ifdef LCD_LQFP100_PIN  	
    	value = 9999999999;
#elif defined(LCD_LQFP64_PIN)
		value = 99999999;
#endif
  	}
#ifdef LCD_LQFP100_PIN 
  	if (value <= -1000000000)
#elif defined(LCD_LQFP64_PIN)
  	if (value <= -10000000)
#endif
  	{
#ifdef LCD_LQFP100_PIN   	
    	value = -999999999;
#elif defined(LCD_LQFP64_PIN)
    	value = -9999999;
#endif
  	}
#endif

#ifdef __LCDDISPLAY_GFN616A_H
#ifdef LCD_LQFP100_PIN	
  	if (value >= 9999999999999)
  	{	
    	value = 9999999999999;
  	}
#endif	
#ifdef LCD_LQFP100_PIN 
  	if (value <= -1000000000000)
  	{
    	value = -999999999999;
  	}
#endif
#endif

  	if (value < 0)
  	{
    	value = abs(value);
    	neg   = 1;
  	}
  	else
  	{
    	neg = 0;
  	}

  	/* Turn off all number LCD segments */
  	LCD_NumberOff();

	/* Length of number */
	len = 0;
	tmp = value;
	while( 1 )
	{
		if( (tmp/10) || (tmp%10) )
		{
			tmp = tmp / 10;
			len++;
		}
		else break;
	}

  	/* Extract useful digits */
  	div = 1;
  	//for (digit = 0; digit < LCD_DIGIT_NUM; digit++)
  	for (digit = (LCD_DIGIT_NUM-1); digit >= 0; digit--)
  	{
    	num = (value / div) % 10;
    	if ((neg == 1) && (digit == (LCD_DIGIT_NUM-1-len))) num = 10;
    	bitpattern = Nuvo_Numbers[num];
    	for (i = 0; i < LCD_DIGIT_SEGNUM; i++)
    	{
      		bit = NANO1XXDISPLAY.Number[digit].bit[i];
      		com = NANO1XXDISPLAY.Number[digit].com[i];
      		if (bitpattern & (1 << i))
      		{
        		LCD_EnableSegment(com, bit);
      		}
    	}
    	div = div * 10;
  	}

}


/**
 *  @brief Turn all segments on numeric display off
 *
 *  @param None
 *
 *  @par Include:
 * 	  nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  LCD_NumberOff();
 *  @endcode
 *
 */
void LCD_NumberOff(void)
{
  	/* Turn off all number segments */
#ifdef __LCDDISPLAY_PD_878_H	
  	LCD->MEM_0 &= 0x00000000;
  	LCD->MEM_1 &= 0x00000000;
  	LCD->MEM_2 &= 0x00000000;
  	LCD->MEM_3 &= 0x00000000;
	LCD->MEM_4 &= 0x00000000;
	LCD->MEM_5 &= 0x00000000;
	LCD->MEM_6 &= 0x00000000;
	LCD->MEM_7 &= 0x00000000;
	LCD->MEM_8 &= 0x00000000;
	LCD->MEM_9 &= 0x00000000;
#endif	
#ifdef __LCDDISPLAY_GFN616A_H
	LCD->MEM_0 &= ~0x0f000000;
  	LCD->MEM_1 &= ~0x0f0f0f0f;
  	LCD->MEM_2 &= ~0x000f0f0f;
	LCD->MEM_5 &= ~0x0f0e0000;
	LCD->MEM_6 &= ~0x0f0f0f0f;
	LCD->MEM_7 &= ~0x0f0f0f0f;
	LCD->MEM_8 &= ~0x0f0f0f0f;
	LCD->MEM_9 &= ~0x070f0f0f;
#endif
#ifdef __LCDDISPLAY_BTL001_H
	#ifdef __NANO100A__
	LCD->MEM_0 &= ~0x0f0f0f0f;
  	LCD->MEM_1 &= ~0x0f0f0f0f;
  	LCD->MEM_2 &= ~0x0f0f0f0f;
  	LCD->MEM_3 &= ~0x0f0f0f0f;
	LCD->MEM_4 &= ~0x0f0f0f0f;
	LCD->MEM_5 &= ~0x0f070f0f;
	LCD->MEM_6 &= ~0x0f0f0f0f;
	LCD->MEM_7 &= ~0x0f0f0f0f;
	LCD->MEM_8 &= ~0x00000f0f;
	#else
	LCD->MEM_6 &= ~0x0f0f0000;
	LCD->MEM_7 &= ~0x0f0f0f0f;
	LCD->MEM_8 &= ~0x00000f0f;
	#endif
#endif
#ifdef __LCDDISPLAY_BTL001_LB_H
	LCD->MEM_0 &= ~0x0f0f0f0f;
#endif
  	return;
}

/**
 *  @brief Turn all segments on alphanumeric display off
 *
 *  @param None
 *
 *  @par Include:
 * 	  nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  LCD_AlphaNumberOff();
 *  @endcode
 *
 */
void LCD_AlphaNumberOff(void)
{
  	/* Turn off all number segments */
#ifdef __LCDDISPLAY_PD_878_H	
  	LCD->MEM_0 &= 0x00000000;
  	LCD->MEM_1 &= 0x00000000;
  	LCD->MEM_2 &= 0x00000000;
  	LCD->MEM_3 &= 0x00000000;
	LCD->MEM_4 &= 0x00000000;
	LCD->MEM_5 &= 0x00000000;
	LCD->MEM_6 &= 0x00000000;
	LCD->MEM_7 &= 0x00000000;
	LCD->MEM_8 &= 0x00000000;
	LCD->MEM_9 &= 0x00000000;
#endif
#ifdef __LCDDISPLAY_BTL001_H
	#ifdef __NANO100A__
	LCD->MEM_0 &= ~0x0f0f0f0f;
  	LCD->MEM_1 &= ~0x0f0f0f0f;
  	LCD->MEM_2 &= ~0x0f0f0f0f;
  	LCD->MEM_3 &= ~0x0f0f0f0f;
	LCD->MEM_4 &= ~0x0f0f0f0f;
	LCD->MEM_5 &= ~0x0f070f0f;
	LCD->MEM_6 &= ~0x00000f0f;
	#else
	LCD->MEM_0 &= ~0x0f0f0f0f;
  	LCD->MEM_1 &= ~0x0f0f0f0f; 
  	LCD->MEM_2 &= ~0x0f0f0f0f; 
  	LCD->MEM_3 &= ~0x0f0f0f0f;
	LCD->MEM_4 &= ~0x0f0f0f0f; 
	LCD->MEM_5 &= ~0x0f070f0f;
	LCD->MEM_6 &= ~0x00000f0f;
	LCD->MEM_9 &= ~0x0f060000;
	#endif
#endif
#ifdef __LCDDISPLAY_BTL001_LB_H
  	LCD->MEM_2 &= ~0x0f0f0f0f;
  	LCD->MEM_3 &= ~0x0f0f0000;
	LCD->MEM_4 &= ~0x0f0f0f0f;
	LCD->MEM_5 &= ~0x0f0f0000;
	LCD->MEM_6 &= ~0x00000f0f;
	LCD->MEM_8 &= ~0x00000f0f;
#endif
  	return;
}

/**
 *  @brief Write text on LCD display
 *
 *  @param[in] string  Text string to show on display  
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  // show the Nuvoton text on LCD
 *  LCD_Write ( "Nuvoton" );
 *  @endcode
 *
 */
void LCD_Write(char *string)
{
	int      data, length, index;
	uint16_t bitfield;
	//uint32_t value;
	uint32_t com, bit;
	int      i;

	length = strlen(string);
	index  = 0;


	/* Turn all segments off */
	LCD_AlphaNumberOff();

	/* fill out all characters on display */
	for (index = 0; index < LCD_ALPHABET_NUM; index++)
	{
    	if (index < length)
    	{
      		data = (int) *string;
    	}
    	else           /* padding with space */
    	{
      		data = 0x20; /* SPACE */
    	}
    	/* defined letters currently starts at "SPACE" - 0x20; */
    	data     = data - 0x20;
    	bitfield = Nuvo_alphabet[data];


    	for (i = 0; i < LCD_ALPHABET_SEGNUM; i++)
    	{
      		bit   = NANO1XXDISPLAY.Text[index].bit[i];
      		com   = NANO1XXDISPLAY.Text[index].com[i];
      		//value = (1 << bit);

      		if (bitfield & (1 << i))
      		{
        		/* Turn on segment */
        		LCD_EnableSegment(com, bit);
      		}
    	}
    	string++;
	}

}

/**
 *  @brief LCD Disable all segments
 *
 *  @param None
 *
 *  @par Include:
 * 	  nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  LCD_AllOff();
 *  @endcode
 *
 */
void LCD_AllOff(void)
{
	LCD->MEM_0 = 0x00000000;
  	LCD->MEM_1 = 0x00000000;
  	LCD->MEM_2 = 0x00000000;
  	LCD->MEM_3 = 0x00000000;
	LCD->MEM_4 = 0x00000000;
	LCD->MEM_5 = 0x00000000;
	LCD->MEM_6 = 0x00000000;
	LCD->MEM_7 = 0x00000000;
	LCD->MEM_8 = 0x00000000;
	LCD->MEM_9 = 0x00000000;
}

/**
 *  @brief LCD Enable all segments
 *
 *  @param None
 *
 *  @par Include:
 * 	  nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  LCD_AllOn();
 *  @endcode
 *
 */
void LCD_AllOn(void)
{
  	LCD->MEM_0 = 0xffffffff;
  	LCD->MEM_1 = 0xffffffff;
  	LCD->MEM_2 = 0xffffffff;
  	LCD->MEM_3 = 0xffffffff;	
	LCD->MEM_4 = 0xffffffff;
	LCD->MEM_5 = 0xffffffff;
	LCD->MEM_6 = 0xffffffff;
	LCD->MEM_7 = 0xffffffff;
#ifdef LCD_LQFP100_PIN	
	LCD->MEM_8 = 0xffffffff;
	LCD->MEM_9 = 0xffffffff;
#endif	
}

/**
 *  @brief LCD Light up or shut off various symbols on LCD Display
 *
 *  @param[in] s   Which symbol to turn on or off
 *  @param[in] on  Zero is off, non-zero is on
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  // show the symbol 3 on LCD
 *  LCD_Symbol(3, 1);
 *  @endcode
 *
 */
void LCD_Symbol(E_LCDSYMBOL s, int on)
{
	int com = 0;
	int bit = 0;

	com = S_LCDSYMBOL[s].com;
	bit = S_LCDSYMBOL[s].seg;
	
  	if (on)
  	{
    	LCD_EnableSegment(com, bit);
  	}
  	else
  	{		
	    LCD_DisableSegment(com, bit);
  	}
}

/**
 *  @brief Update Charge Pump frequency according to register settings
 *
 *  @param None
 *
 *  @par Include:
 * 	  nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  LCD_UpdateChargePumpFreq();
 *  @endcode
 *
 */
void LCD_UpdateChargePumpFreq(void)
{
	uint32_t clkdiv;
	uint32_t cpump_freq_div[] = {1, 2, 4, 8, 16, 32, 64, 128};

	if((CLK->CLKSEL1 & CLK_CLKSEL1_LCD_MASK) == 0)
		g_LCDClkSrc = 32 * 1024;
	else
		g_LCDClkSrc = 10 * 1024;
	
	//clkdiv = LCD->DISPCTL_BITS.CPUMP_FREQ;
	clkdiv = (LCD->DISPCTL & LCD_DISPCTL_CPUMP_FREQ) >> 11;

	g_ChargePumpFreq = g_LCDClkSrc / cpump_freq_div[clkdiv];
	
	LCDDEBUG("Charge Pump Frequency = %f Hz \n", g_ChargePumpFreq);
}

/**
 *  @brief LCD voltage use internal charge pump
 *
 *  @param[in] lcdinit   Pointer to initial structure
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  S_LCD_INIT lcdinit;
 *  //LCD Initialize
 *  lcdinit.cpump_enable = true;
 *  lcdinit.internal_bias = true;
 *  lcdinit.cpump_freqdiv = LCD_CPUMP_DIV64;
 *  lcdinit.cpump_voltage = LCD_CPVOl_3_3V;
 *  lcdinit.bias = LCD_BIAS_THIRD;
 *  lcdinit.mux = LCD_MUX_QUADRUPLEX;
 *  lcdinit.freqdiv = LCD_FREQ_DIV64;
 *  DrvLCD_Init(&lcdinit);
 *  DrvLCD_ChargePumpInit(&lcdinit);
 *  @endcode
 *
 */
void LCD_ChargePumpInit(S_LCD_INIT *lcdinit)
{

  	if (lcdinit->cpump_enable==false)
  	{
		/* disable charge pump */
		LCD->DISPCTL &= ~LCD_DISPCTL_CPUMP_EN;		// Disable charge pump	
		return ;
  	}

	LCD->DISPCTL &= ~LCD_DISPCTL_BV_SEL;		// internal source for charge pump
    LCD->DISPCTL = LCD->DISPCTL & ~LCD_DISPCTL_CPUMP_FREQ | lcdinit->cpump_freqdiv;
    LCD->DISPCTL = LCD->DISPCTL & ~LCD_DISPCTL_CPUMP_VOL_SET | lcdinit->cpump_voltage;
	LCD->DISPCTL &= ~LCD_DISPCTL_IBRL_EN;
	LCD->DISPCTL |= (lcdinit->internal_bias==true)?LCD_DISPCTL_IBRL_EN:0;
	LCD->DISPCTL |= LCD_DISPCTL_CPUMP_EN;		// enable charge pump

	/* record the frequency in charge pump */
	LCD_UpdateChargePumpFreq();
 
}

/**
 *  @brief Enable frame count and frame count interrupt
 *
 *  @param[in] prediv   \n
 *                      Set to pre-scale divider for frame count \n
 *                      LCD_FCPRESC_DIV1: The frame-rate is divided by one \n
 *                      LCD_FCPRESC_DIV2: The frame-rate is divided by two \n
 *                      LCD_FCPRESC_DIV4: The frame-rate is divided by four \n
 *                      LCD_FCPRESC_DIV8: The frame-rate is divided by eight 
 *
 *  @param[in] fcv      Set to frame count top value
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  LCD_FrameCountEnable( LCD_FCPRESC_DIV2, 20 );
 *  @endcode
 *
 */
void LCD_FrameCountEnable( uint32_t prediv, uint32_t fcv )
{
	uint32_t div = 1; 	// default prediv == LCD_FCPRESC_DIV1

	LCD_UpdateLCDFrameFreq();
	
	LCD->FCR = 0x00;
	LCD->FCSTS |= LCD_FCSTS_FCSTS;	// clear fcsts flag

	if(prediv) {
		if(prediv>LCD_FCPRESC_DIV8) {
			LCDDEBUG("LCD frame count prescalar value use dividing by 8 \n");
			prediv = LCD_FCPRESC_DIV8;
		}
		LCD->FCR = LCD->FCR & ~LCD_FCR_PRESCL | prediv;
		if(prediv==LCD_FCPRESC_DIV1) div = 1;
		if(prediv==LCD_FCPRESC_DIV2) div = 2;
		if(prediv==LCD_FCPRESC_DIV4) div = 4;
		if(prediv==LCD_FCPRESC_DIV8) div = 8;
	}

	if(fcv)
	{
		if(fcv > 0x3F) {	// top value max. 63 = 0x3F
			LCDDEBUG("LCD frame counter top value is too big, so use max. value 63 \n");
			fcv = 0x3F;
		}
		LCD->FCR = LCD->FCR & ~LCD_FCR_FCV | (fcv << LCD_FCR_FCV_BIT_ADDR);
	}

	/* calculate frame count & INT frequency */
	g_LCDFrameCountFreq = g_LCDFrameRate / div;
	
	g_LCDFrameCountINTFreq = g_LCDFrameCountFreq / (fcv+1);

	LCDDEBUG("Frame-rate is %f Hz \n", g_LCDFrameRate);
	LCDDEBUG("FCV:%d, Interrupt frequency %f Hz, period time %f sec	\n", fcv, g_LCDFrameCountINTFreq, 1/g_LCDFrameCountINTFreq);
	
	LCD->FCR |= LCD_FCR_FCEN;		// enable LCD frame count
	
}

/**
 *  @brief Disable frame count function
 *
 *  @param None
 *
 *  @par Include:
 * 	  nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  LCD_FrameCountDisable( );
 *  @endcode
 *
 */
void LCD_FrameCountDisable(void)
{
	LCD->FCR = 0x00;		// disable LCD frame count

	if( LCD->FCSTS & LCD_FCSTS_FCSTS) 	// clear status flag
		LCD->FCSTS |= LCD_FCSTS_FCSTS;
}

/**
 *  @brief Update LCD clock frequency according to register settings
 *
 *  @param None
 *
 *  @par Include:
 * 	  nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  LCD_UpdateLCDFrameFreq( );
 *  @endcode
 *
 */
void LCD_UpdateLCDFrameFreq(void)
{
	uint32_t clkdiv, muldiv;
	uint32_t lcd_freq_div[] = {32, 64, 96, 128, 192, 256, 384, 512};
	uint32_t multiplex_freq_div[] = {2, 4, 6, 8, 10, 12};

	if((CLK->CLKSEL1 & CLK_CLKSEL1_LCD_MASK) == 0)
		g_LCDClkSrc = 32 * 1024;
	else
		g_LCDClkSrc = 10 * 1024;

	clkdiv = (LCD->CTL & LCD_CTL_FREQ) >> LCD_CTL_FREQ_BIT_ADDR;
	muldiv = (LCD->CTL & LCD_CTL_MUX) >> LCD_CTL_MUX_BIT_ADDR;

	g_LCDFreq = (double)g_LCDClkSrc / lcd_freq_div[clkdiv];
	LCDDEBUG("\n\ng_LCDFreq = %f Hz	\n", g_LCDFreq);
    g_LCDFrameRate = (double)g_LCDFreq / multiplex_freq_div[muldiv];
    LCDDEBUG("g_LCDFrameRate = %f Hz \n", g_LCDFrameRate);
    
}

/**
 *  @brief Switch LCD Segment Pin for enable/disable
 *
 *  @param[in] pinno   \n
 *                      Segment pin number \n
 *                      LQFP100_PIN: Segment pin from 0 ~ 39 \n
 *                      LQFP64_PIN : Segment pin from 0 ~ 31 
 *
 *  @param[in] enable   ENABLE/DISABLE LCD Segment Pin
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  // Disable LCD Segment 5
 *  LCD_SegmentPinEnable(5, DISABLE);
 *  @endcode
 *
 */
void LCD_SegmentPinEnable(uint32_t pinno, bool enable)
{

#ifdef LCD_LQFP100_PIN
	//uint32_t bitset;
	//if(enable == true) bitset = 7;
	//else bitset = 0;

	if (pinno == 0) {
		// Seg 0
		GCR->PD_H_MFP &= ~PD15_MFP_MASK;
		GCR->PD_H_MFP |= (enable == true) ? PD15_MFP_LCD_S0:0;
		GPIOD->OFFD = (enable == true)? (GPIOD->OFFD | GPIO_OFFD_OFFD_15) : (GPIOD->OFFD &~GPIO_OFFD_OFFD_15);
	} else if (pinno == 1) {
		// Seg 1
  		GCR->PD_H_MFP &= ~PD14_MFP_MASK;
		GCR->PD_H_MFP |= (enable == true) ? PD14_MFP_LCD_S1:0;
		GPIOD->OFFD = (enable == true)? (GPIOD->OFFD | GPIO_OFFD_OFFD_14) : (GPIOD->OFFD &~GPIO_OFFD_OFFD_14);
	} else if (pinno == 2) {
		// Seg 2
  		GCR->PD_L_MFP &= ~PD7_MFP_MASK;
		GCR->PD_L_MFP |= (enable == true) ? PD7_MFP_LCD_S2:0;
		GPIOD->OFFD = (enable == true)? (GPIOD->OFFD | GPIO_OFFD_OFFD_7) : (GPIOD->OFFD &~GPIO_OFFD_OFFD_7);
	} else if (pinno == 3) {
		// Seg 3
  		GCR->PD_L_MFP &= ~PD6_MFP_MASK;
		GCR->PD_L_MFP |= (enable == true) ? PD6_MFP_LCD_S3:0;
		GPIOD->OFFD = (enable == true)? (GPIOD->OFFD | GPIO_OFFD_OFFD_6) : (GPIOD->OFFD &~GPIO_OFFD_OFFD_6);
	} else if (pinno == 4) {
		// Seg 4
  		GCR->PB_L_MFP &= ~PB3_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB3_MFP_LCD_S4:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_3) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_3);
	} else if (pinno == 5) {
		// Seg 5
		GCR->PB_L_MFP &= ~PB2_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB2_MFP_LCD_S5:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_2) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_2);
	} else if (pinno == 6) {
		// Seg 6
		GCR->PB_L_MFP &= ~PB1_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB1_MFP_LCD_S6:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_1) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_1);
	} else if (pinno == 7) {
		// Seg 7
		GCR->PB_L_MFP &= ~PB0_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB0_MFP_LCD_S7:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_0) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_0);
	} else if (pinno == 8) {
		// Seg 8
		GCR->PE_L_MFP &= ~PE7_MFP_MASK;
		GCR->PE_L_MFP |= (enable == true) ? PE7_MFP_LCD_S8:0;
		GPIOE->OFFD = (enable == true)? (GPIOE->OFFD | GPIO_OFFD_OFFD_7) : (GPIOE->OFFD &~GPIO_OFFD_OFFD_7);
	} else if (pinno == 9) {
		// Seg 9
		GCR->PE_H_MFP &= ~PE8_MFP_MASK;
		GCR->PE_H_MFP |= (enable == true) ? PE8_MFP_LCD_S9:0;
		GPIOE->OFFD = (enable == true)? (GPIOE->OFFD | GPIO_OFFD_OFFD_8) : (GPIOE->OFFD &~GPIO_OFFD_OFFD_8);
	} else if (pinno == 10) {
		// Seg 10
		GCR->PB_L_MFP &= ~PB7_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB7_MFP_LCD_S10:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_7) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_7);
	} else if (pinno == 11) {
		// Seg 11
		GCR->PB_L_MFP &= ~PB6_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB6_MFP_LCD_S11:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_6) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_6);
	} else if (pinno == 12) {
		// Seg 12
		GCR->PB_L_MFP &= ~PB5_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB5_MFP_LCD_S12:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_5) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_5);
	} else if (pinno == 13) {
		// Seg 13
		GCR->PB_L_MFP &= ~PB4_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB4_MFP_LCD_S13:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_4) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_4);
	} else if (pinno == 14) {
		// Seg 14
		GCR->PD_H_MFP &= ~PD13_MFP_MASK;
		GCR->PD_H_MFP |= (enable == true) ? PD13_MFP_LCD_S14:0;
		GPIOD->OFFD = (enable == true)? (GPIOD->OFFD | GPIO_OFFD_OFFD_13) : (GPIOD->OFFD &~GPIO_OFFD_OFFD_13);
	} else if (pinno == 15) {
		// Seg 15
		GCR->PD_H_MFP &= ~PD12_MFP_MASK;
		GCR->PD_H_MFP |= (enable == true) ? PD12_MFP_LCD_S15:0;
		GPIOD->OFFD = (enable == true)? (GPIOD->OFFD | GPIO_OFFD_OFFD_12) : (GPIOD->OFFD &~GPIO_OFFD_OFFD_12);
	} else if (pinno == 16) {
		// Seg 16
		GCR->PD_H_MFP &= ~PD11_MFP_MASK;
		GCR->PD_H_MFP |= (enable == true) ? PD11_MFP_LCD_S16:0;
		GPIOD->OFFD = (enable == true)? (GPIOD->OFFD | GPIO_OFFD_OFFD_11) : (GPIOD->OFFD &~GPIO_OFFD_OFFD_11);
	} else if (pinno == 17) {
		// Seg 17
		GCR->PD_H_MFP &= ~PD10_MFP_MASK;
		GCR->PD_H_MFP |= (enable == true) ? PD10_MFP_LCD_S17:0;
		GPIOD->OFFD = (enable == true)? (GPIOD->OFFD | GPIO_OFFD_OFFD_10) : (GPIOD->OFFD &~GPIO_OFFD_OFFD_10);
	} else if (pinno == 18) {
		// Seg 18
		GCR->PD_H_MFP &= ~PD9_MFP_MASK;
		GCR->PD_H_MFP |= (enable == true) ? PD9_MFP_LCD_S18:0;
		GPIOD->OFFD = (enable == true)? (GPIOD->OFFD | GPIO_OFFD_OFFD_9) : (GPIOD->OFFD &~GPIO_OFFD_OFFD_9);
	} else if (pinno == 19) {
		// Seg 19
		GCR->PD_H_MFP &= ~PD8_MFP_MASK;
		GCR->PD_H_MFP |= (enable == true) ? PD8_MFP_LCD_S19:0;
		GPIOD->OFFD = (enable == true)? (GPIOD->OFFD | GPIO_OFFD_OFFD_8) : (GPIOD->OFFD &~GPIO_OFFD_OFFD_8);
	} else if (pinno == 20) {
		// Seg 20
		GCR->PA_H_MFP &= ~PA8_MFP_MASK;
		GCR->PA_H_MFP |= (enable == true) ? PA8_MFP_LCD_S20:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_8) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_8);
	} else if (pinno == 21) {
		// Seg 21
		GCR->PA_H_MFP &= ~PA9_MFP_MASK;
		GCR->PA_H_MFP |= (enable == true) ? PA9_MFP_LCD_S21:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_9) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_9);
	} else if (pinno == 22) {
		// Seg 22
		GCR->PA_H_MFP &= ~PA10_MFP_MASK;
		GCR->PA_H_MFP |= (enable == true) ? PA10_MFP_LCD_S22:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_10) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_10);
	} else if (pinno == 23) {
		// Seg 23
		GCR->PA_H_MFP &= ~PA11_MFP_MASK;
		GCR->PA_H_MFP |= (enable == true) ? PA11_MFP_LCD_S23:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_11) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_11);
	} else if (pinno == 24) {
		// Seg 24
		GCR->PB_H_MFP &= ~PB12_MFP_MASK;
		GCR->PB_H_MFP |= (enable == true) ? PB12_MFP_LCD_S24:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_12) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_12);
	} else if (pinno == 25) {
		// Seg 25
		GCR->PB_H_MFP &= ~PB13_MFP_MASK;
		GCR->PB_H_MFP |= (enable == true) ? PB13_MFP_LCD_S25:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_13) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_13);
	} else if (pinno == 26) {
		// Seg 26
		GCR->PB_H_MFP &= ~PB14_MFP_MASK;
		GCR->PB_H_MFP |= (enable == true) ? PB14_MFP_LCD_S26:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_14) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_14);
	} else if (pinno == 27) {
		// Seg 27
		GCR->PE_H_MFP &= ~PE13_MFP_MASK;
		GCR->PE_H_MFP |= (enable == true) ? PE13_MFP_LCD_S27:0;
		GPIOE->OFFD = (enable == true)? (GPIOE->OFFD | GPIO_OFFD_OFFD_13) : (GPIOE->OFFD &~GPIO_OFFD_OFFD_13);
	} else if (pinno == 28) {
		// Seg 28
		GCR->PE_H_MFP &= ~PE14_MFP_MASK;
		GCR->PE_H_MFP |= (enable == true) ? PE14_MFP_LCD_S28:0;
		GPIOE->OFFD = (enable == true)? (GPIOE->OFFD | GPIO_OFFD_OFFD_14) : (GPIOE->OFFD &~GPIO_OFFD_OFFD_14);
	} else if (pinno == 29) {
		// Seg 29
		GCR->PE_H_MFP &= ~PE15_MFP_MASK;
		GCR->PE_H_MFP |= (enable == true) ? PE15_MFP_LCD_S29:0;
		GPIOE->OFFD = (enable == true)? (GPIOE->OFFD | GPIO_OFFD_OFFD_15) : (GPIOE->OFFD &~GPIO_OFFD_OFFD_15);
	} else if (pinno == 30) {
		// Seg 30
		GCR->PB_H_MFP &= ~PB8_MFP_MASK;
		GCR->PB_H_MFP |= (enable == true) ? PB8_MFP_LCD_S30:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_8) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_8);
	} else if (pinno == 31) {
		// Seg 31
		GCR->PB_H_MFP &= ~PB15_MFP_MASK;
		GCR->PB_H_MFP |= (enable == true) ? PB15_MFP_LCD_S31:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_15) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_15);
	} else if (pinno == 32) {
		// Seg 32
		GCR->PC_H_MFP &= ~PC14_MFP_MASK;
		GCR->PC_H_MFP |= (enable == true) ? PC14_MFP_LCD_S32:0;
		GPIOC->OFFD = (enable == true)? (GPIOC->OFFD | GPIO_OFFD_OFFD_14) : (GPIOC->OFFD &~GPIO_OFFD_OFFD_14);
	} else if (pinno == 33) {
		// Seg 33
		GCR->PC_H_MFP &= ~PC15_MFP_MASK;
		GCR->PC_H_MFP |= (enable == true) ? PC15_MFP_LCD_S33:0;
		GPIOC->OFFD = (enable == true)? (GPIOC->OFFD | GPIO_OFFD_OFFD_15) : (GPIOC->OFFD &~GPIO_OFFD_OFFD_15);
	} else if (pinno == 34) {
		// Seg 34
		GCR->PD_L_MFP &= ~PD5_MFP_MASK;
		GCR->PD_L_MFP |= (enable == true) ? PD5_MFP_LCD_S34:0;
		GPIOD->OFFD = (enable == true)? (GPIOD->OFFD | GPIO_OFFD_OFFD_5) : (GPIOD->OFFD &~GPIO_OFFD_OFFD_5);
	} else if (pinno == 35) {
		// Seg 35
		GCR->PD_L_MFP &= ~PD4_MFP_MASK;
		GCR->PD_L_MFP |= (enable == true) ? PD4_MFP_LCD_S35:0;
		GPIOD->OFFD = (enable == true)? (GPIOD->OFFD | GPIO_OFFD_OFFD_4) : (GPIOD->OFFD &~GPIO_OFFD_OFFD_4);
	} else if (pinno == 36) {
		// Seg 36
		GCR->PA_L_MFP &= ~PA7_MFP_MASK;
		GCR->PA_L_MFP |= (enable == true) ? PA7_MFP_LCD_S36:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_7) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_7);
	} else if (pinno == 37) {
		// Seg 37
		GCR->PA_L_MFP &= ~PA6_MFP_MASK;
		GCR->PA_L_MFP |= (enable == true) ? PA6_MFP_LCD_S37:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_6) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_6);
	} else if (pinno == 38) {
		// Seg 38
		GCR->PA_L_MFP &= ~PA5_MFP_MASK;
		GCR->PA_L_MFP |= (enable == true) ? PA5_MFP_LCD_S38:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_5) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_5);
	} else if (pinno == 39) {
		// Seg 39
		GCR->PA_L_MFP &= ~PA4_MFP_MASK;
		GCR->PA_L_MFP |= (enable == true) ? PA4_MFP_LCD_S39:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_4) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_4);
	}
#endif

#ifdef LCD_LQFP64_PIN
	uint32_t bitset;
	if(enable == true) bitset = 7;
	else bitset = 0;

	if (pinno == 0) {
		// Seg 0
		GCR->PB_L_MFP &= ~PB1_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB1_MFP_LCD_S0:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_1) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_1);
	} else if (pinno == 1) {
		// Seg 1
		GCR->PB_L_MFP &= ~PB0_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB0_MFP_LCD_S1:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_0) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_0);
	} else if (pinno == 2) {
		// Seg 2
		GCR->PB_L_MFP &= ~PB7_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB7_MFP_LCD_S2:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_7) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_7);
	} else if (pinno == 3) {
		// Seg 3
		GCR->PB_L_MFP &= ~PB6_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB6_MFP_LCD_S3:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_6) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_6);
	} else if (pinno == 4) {
		// Seg 4
		GCR->PB_L_MFP &= ~PB5_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB5_MFP_LCD_S4:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_5) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_5);
	} else if (pinno == 5) {
		// Seg 5
		GCR->PB_L_MFP &= ~PB4_MFP_MASK;
		GCR->PB_L_MFP |= (enable == true) ? PB4_MFP_LCD_S5:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_4) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_4);
	} else if (pinno == 6) {
		// Seg 6
		GCR->PA_H_MFP &= ~PA8_MFP_MASK;
		GCR->PA_H_MFP |= (enable == true) ? PA8_MFP_LCD_S6:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_8) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_8);
	} else if (pinno == 7) {
		// Seg 7
		GCR->PA_H_MFP &= ~PA9_MFP_MASK;
		GCR->PA_H_MFP |= (enable == true) ? PA9_MFP_LCD_S7:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_9) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_9);
	} else if (pinno == 8) {
		// Seg 8
		GCR->PA_H_MFP &= ~PA10_MFP_MASK;
		GCR->PA_H_MFP |= (enable == true) ? PA10_MFP_LCD_S8:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_10) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_10);
	} else if (pinno == 9) {
		// Seg 9
		GCR->PA_H_MFP &= ~PA11_MFP_MASK;
		GCR->PA_H_MFP |= (enable == true) ? PA11_MFP_LCD_S9:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_11) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_11);
	} else if (pinno == 10) {
		// Seg 10
		GCR->PB_H_MFP &= ~PB12_MFP_MASK;
		GCR->PB_H_MFP |= (enable == true) ? PB12_MFP_LCD_S10:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_12) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_12);
	} else if (pinno == 11) {
		// Seg 11
		GCR->PB_H_MFP &= ~PB13_MFP_MASK;
		GCR->PB_H_MFP |= (enable == true) ? PB13_MFP_LCD_S11:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_13) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_13);
	} else if (pinno == 12) {
		// Seg 12
		GCR->PB_H_MFP &= ~PB14_MFP_MASK;
		GCR->PB_H_MFP |= (enable == true) ? PB14_MFP_LCD_S12:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_14) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_14);
	} else if (pinno == 13) {
		// Seg 13
		GCR->PB_H_MFP &= ~PB8_MFP_MASK;
		GCR->PB_H_MFP |= (enable == true) ? PB8_MFP_LCD_S13:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_8) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_8);
	} else if (pinno == 14) {
		// Seg 14
		GCR->PB_H_MFP &= ~PB15_MFP_MASK;
		GCR->PB_H_MFP |= (enable == true) ? PB15_MFP_LCD_S14:0;
		GPIOB->OFFD = (enable == true)? (GPIOB->OFFD | GPIO_OFFD_OFFD_15) : (GPIOB->OFFD &~GPIO_OFFD_OFFD_15);
	} else if (pinno == 15) {
		// Seg 15
		GCR->PC_H_MFP &= ~PC14_MFP_MASK;
		GCR->PC_H_MFP |= (enable == true) ? PC14_MFP_LCD_S15:0;
		GPIOC->OFFD = (enable == true)? (GPIOC->OFFD | GPIO_OFFD_OFFD_14) : (GPIOC->OFFD &~GPIO_OFFD_OFFD_14);
	} else if (pinno == 16) {
		// Seg 16
		GCR->PC_H_MFP &= ~PC15_MFP_MASK;
		GCR->PC_H_MFP |= (enable == true) ? PC15_MFP_LCD_S16:0;
		GPIOC->OFFD = (enable == true)? (GPIOC->OFFD | GPIO_OFFD_OFFD_15) : (GPIOC->OFFD &~GPIO_OFFD_OFFD_15);
	} else if (pinno == 17) {
		// Seg 17
		GCR->PC_L_MFP &= ~PC7_MFP_MASK;
		GCR->PC_L_MFP |= (enable == true) ? PC7_MFP_LCD_S17:0;
		GPIOC->OFFD = (enable == true)? (GPIOC->OFFD | GPIO_OFFD_OFFD_7) : (GPIOC->OFFD &~GPIO_OFFD_OFFD_7);
	} else if (pinno == 18) {
		// Seg 18
		GCR->PA_L_MFP &= ~PA7_MFP_MASK;
		GCR->PA_L_MFP |= (enable == true) ? PA7_MFP_LCD_S18:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_7) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_7);
	} else if (pinno == 19) {
		// Seg 19
		GCR->PA_L_MFP &= ~PA6_MFP_MASK;
		GCR->PA_L_MFP |= (enable == true) ? PA6_MFP_LCD_S19:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_6) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_6);
	} else if (pinno == 20) {
		// Seg 20
		GCR->PA_L_MFP &= ~PA5_MFP_MASK;
		GCR->PA_L_MFP |= (enable == true) ? PA5_MFP_LCD_S20:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_5) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_5);
	} else if (pinno == 21) {
		// Seg 21
		GCR->PA_L_MFP &= ~PA4_MFP_MASK;
		GCR->PA_L_MFP |= (enable == true) ? PA4_MFP_LCD_S21:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_4) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_4);
	} else if (pinno == 22) {
		// Seg 22
		GCR->PA_L_MFP &= ~PA3_MFP_MASK;
		GCR->PA_L_MFP |= (enable == true) ? PA3_MFP_LCD_S22:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_3) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_3);
	} else if (pinno == 23) {
		// Seg 23
		GCR->PA_L_MFP &= ~PA2_MFP_MASK;
		GCR->PA_L_MFP |= (enable == true) ? PA2_MFP_LCD_S23:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_2) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_2);
	} else if (pinno == 24) {
		// Seg 24
		GCR->PA_H_MFP &= ~PA12_MFP_MASK;
		GCR->PA_H_MFP |= (enable == true) ? PA12_MFP_LCD_S24:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_12) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_12);
	} else if (pinno == 25) {
		// Seg 25
		GCR->PA_H_MFP &= ~PA13_MFP_MASK;
		GCR->PA_H_MFP |= (enable == true) ? PA13_MFP_LCD_S25:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_13) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_13);
	} else if (pinno == 26) {
		// Seg 26
		GCR->PA_H_MFP &= ~PA14_MFP_MASK;
		GCR->PA_H_MFP |= (enable == true) ? PA14_MFP_LCD_S26:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_14) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_14);
	} else if (pinno == 27) {
		// Seg 27
		GCR->PA_H_MFP &= ~PA15_MFP_MASK;
		GCR->PA_H_MFP |= (enable == true) ? PA15_MFP_LCD_S27:0;
		GPIOA->OFFD = (enable == true)? (GPIOA->OFFD | GPIO_OFFD_OFFD_15) : (GPIOA->OFFD &~GPIO_OFFD_OFFD_15);
	} else if (pinno == 28) {
		// Seg 28
		GCR->PC_H_MFP &= ~PC8_MFP_MASK;
		GCR->PC_H_MFP |= (enable == true) ? PC8_MFP_LCD_S28:0;
		GPIOC->OFFD = (enable == true)? (GPIOC->OFFD | GPIO_OFFD_OFFD_8) : (GPIOC->OFFD &~GPIO_OFFD_OFFD_8);
	} else if (pinno == 29) {
		// Seg 29
		GCR->PC_H_MFP &= ~PC9_MFP_MASK;
		GCR->PC_H_MFP |= (enable == true) ? PC9_MFP_LCD_S29:0;
		GPIOC->OFFD = (enable == true)? (GPIOC->OFFD | GPIO_OFFD_OFFD_9) : (GPIOC->OFFD &~GPIO_OFFD_OFFD_9);
	} else if (pinno == 30) {
		// Seg 30
		GCR->PC_H_MFP &= ~PC10_MFP_MASK;
		GCR->PC_H_MFP |= (enable == true) ? PC10_MFP_LCD_S30:0;
		GPIOC->OFFD = (enable == true)? (GPIOC->OFFD | GPIO_OFFD_OFFD_10) : (GPIOC->OFFD &~GPIO_OFFD_OFFD_10);
	} else if (pinno == 31) {
		// Seg 31
		GCR->PC_H_MFP &= ~PC11_MFP_MASK;
		GCR->PC_H_MFP |= (enable == true) ? PC11_MFP_LCD_S31:0;
		GPIOC->OFFD = (enable == true)? (GPIOC->OFFD | GPIO_OFFD_OFFD_11) : (GPIOC->OFFD &~GPIO_OFFD_OFFD_11);
	}

#endif

}

/**
 *  @brief Switch pin function for LCD
 *
 *  @param None
 *
 *  @par Include:
 * 	  nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  LCD_PinSwitch_Set();
 *  @endcode
 *
 */
void LCD_PinSwitch_Set(void)
{

#ifdef LCD_LQFP100_PIN
	/* switch multi-function pin to LCD for LQFP-100 */
	// SEG39
	GCR->PA_L_MFP |= PA4_MFP_LCD_S39;
	// SEG38
	GCR->PA_L_MFP |= PA5_MFP_LCD_S38;
	// SEG37
	GCR->PA_L_MFP |= PA6_MFP_LCD_S37;
	// SEG36
	GCR->PA_L_MFP |= PA7_MFP_LCD_S36;
	// SEG20
	GCR->PA_H_MFP |= PA8_MFP_LCD_S20;
	// SEG21
	GCR->PA_H_MFP |= PA9_MFP_LCD_S21;
	// SEG22
	GCR->PA_H_MFP |= PA10_MFP_LCD_S22;
	// SEG23
	GCR->PA_H_MFP |= PA11_MFP_LCD_S23;
	
	// SEG7
	GCR->PB_L_MFP |= PB0_MFP_LCD_S7;
	// SEG6
	GCR->PB_L_MFP |= PB1_MFP_LCD_S6;
	// SEG5
	GCR->PB_L_MFP |= PB2_MFP_LCD_S5;
	// SEG4
	GCR->PB_L_MFP |= PB3_MFP_LCD_S4;
	// SEG13
	GCR->PB_L_MFP |= PB4_MFP_LCD_S13;
	// SEG12
	GCR->PB_L_MFP |= PB5_MFP_LCD_S12;
	// SEG11
	GCR->PB_L_MFP |= PB6_MFP_LCD_S11;
	// SEG10
	GCR->PB_L_MFP |= PB7_MFP_LCD_S10;
	// SEG30
	GCR->PB_H_MFP |= PB8_MFP_LCD_S30;
	// V1
	GCR->PB_H_MFP |= PB9_MFP_LCD_V1;
	// V2
	GCR->PB_H_MFP |= PB10_MFP_LCD_V2;
	// V3
	GCR->PB_H_MFP |= PB11_MFP_LCD_V3;
	// SEG24
	GCR->PB_H_MFP |= PB12_MFP_LCD_S24;
	// SEG25
	GCR->PB_H_MFP |= PB13_MFP_LCD_S25;
	// SEG26
	GCR->PB_H_MFP |= PB14_MFP_LCD_S26;
	// SEG31
	GCR->PB_H_MFP |= PB15_MFP_LCD_S31;
	
	// DH1
	GCR->PC_L_MFP |= PC0_MFP_LCD_DH1;
	// DH2
	GCR->PC_L_MFP |= PC1_MFP_LCD_DH2;
	// COM0
	GCR->PC_L_MFP |= PC2_MFP_LCD_COM0;
	// COM1
	GCR->PC_L_MFP |= PC3_MFP_LCD_COM1;
	// COM2
	GCR->PC_L_MFP |= PC4_MFP_LCD_COM2;
	// COM3
	GCR->PC_L_MFP |= PC5_MFP_LCD_COM3;
	// SEG32
	GCR->PC_H_MFP |= PC14_MFP_LCD_S32;
	// SEG33
	GCR->PC_H_MFP |= PC15_MFP_LCD_S33;

	// SEG35
	GCR->PD_L_MFP |= PD4_MFP_LCD_S35;
	// SEG34
	GCR->PD_L_MFP |= PD5_MFP_LCD_S34;
	// SEG3
	GCR->PD_L_MFP |= PD6_MFP_LCD_S3;
	// SEG2
	GCR->PD_L_MFP |= PD7_MFP_LCD_S2;
	// SEG19
	GCR->PD_H_MFP |= PD8_MFP_LCD_S19;
	// SEG18
	GCR->PD_H_MFP |= PD9_MFP_LCD_S18;
	// SEG17
	GCR->PD_H_MFP |= PD10_MFP_LCD_S17;
	// SEG16
	GCR->PD_H_MFP |= PD11_MFP_LCD_S16;
	// SEG15
	GCR->PD_H_MFP |= PD12_MFP_LCD_S15;
	// SEG14
	GCR->PD_H_MFP |= PD13_MFP_LCD_S14;
	// SEG1
	GCR->PD_H_MFP |= PD14_MFP_LCD_S1;
	// SEG0
	GCR->PD_H_MFP |= PD15_MFP_LCD_S0;

	// SEG8
	GCR->PE_L_MFP |= PE7_MFP_LCD_S8;
	// SEG9
	GCR->PE_H_MFP |= PE8_MFP_LCD_S9;
	// SEG27
	GCR->PE_H_MFP |= PE13_MFP_LCD_S27;
	// SEG28
	GCR->PE_H_MFP |= PE14_MFP_LCD_S28;
	// SEG29
	GCR->PE_H_MFP |= PE15_MFP_LCD_S29;
#endif

#ifdef LCD_LQFP64_PIN
	/* switch multi-function pin to LCD for LQFP-64 */
	// SEG23
	GCR->PA_L_MFP |= PA2_MFP_LCD_S23;
	// SEG22
	GCR->PA_L_MFP |= PA3_MFP_LCD_S22;
	// SEG21
	GCR->PA_L_MFP |= PA4_MFP_LCD_S21;
	// SEG20
	GCR->PA_L_MFP |= PA5_MFP_LCD_S20;
	// SEG19
	GCR->PA_L_MFP |= PA6_MFP_LCD_S19;
	// SEG18
	GCR->PA_L_MFP |= PA7_MFP_LCD_S18;
	// SEG6
	GCR->PA_H_MFP |= PA8_MFP_LCD_S6;
	// SEG7
	GCR->PA_H_MFP |= PA9_MFP_LCD_S7;
	// SEG8
	GCR->PA_H_MFP |= PA10_MFP_LCD_S8;
	// SEG9
	GCR->PA_H_MFP |= PA11_MFP_LCD_S9;
	// SEG24
	GCR->PA_H_MFP |= PA12_MFP_LCD_S24;
	// SEG25
	GCR->PA_H_MFP |= PA13_MFP_LCD_S25;
	// SEG26
	GCR->PA_H_MFP |= PA14_MFP_LCD_S26;
	// SEG27
	GCR->PA_H_MFP |= PA15_MFP_LCD_S27;
	// SEG1
	GCR->PB_L_MFP |= PB0_MFP_LCD_S1;
	// SEG0
	GCR->PB_L_MFP |= PB1_MFP_LCD_S0;
	// COM3
	GCR->PB_L_MFP |= PB2_MFP_LCD_COM3;
	// COM2
	GCR->PB_L_MFP |= PB3_MFP_LCD_COM2;
	// SEG5
	GCR->PB_L_MFP |= PB4_MFP_LCD_S5;
	// SEG4
	GCR->PB_L_MFP |= PB5_MFP_LCD_S4;
	// SEG3
	GCR->PB_L_MFP |= PB6_MFP_LCD_S3;
	// SEG2
	GCR->PB_L_MFP |= PB7_MFP_LCD_S2;
	// SEG13
	GCR->PB_H_MFP |= PB8_MFP_LCD_S13;

	// V1
	GCR->PB_H_MFP |= PB9_MFP_LCD_V1;
	// V2
	GCR->PB_H_MFP |= PB10_MFP_LCD_V2;
	// V3
	GCR->PB_H_MFP |= PB11_MFP_LCD_V3;

	// SEG10
	GCR->PB_H_MFP |= PB12_MFP_LCD_S10;
	// SEG11
	GCR->PB_H_MFP |= PB13_MFP_LCD_S11;
	// SEG12
	GCR->PB_H_MFP |= PB14_MFP_LCD_S12;
	// SEG14
	GCR->PB_H_MFP |= PB15_MFP_LCD_S14;

	// DH1
	GCR->PC_L_MFP |= PC0_MFP_LCD_DH1;
	// DH2
	GCR->PC_L_MFP |= PC1_MFP_LCD_DH2;
	// COM0
	GCR->PC_L_MFP |= PC2_MFP_LCD_COM0;
	// COM1
	GCR->PC_L_MFP |= PC3_MFP_LCD_COM1;
	
	// SEG17
	GCR->PC_L_MFP |= PC7_MFP_LCD_S17;

	// SEG28
	GCR->PC_H_MFP |= PC8_MFP_LCD_S28;
	// SEG29
	GCR->PC_H_MFP |= PC9_MFP_LCD_S29;
	// SEG30
	GCR->PC_H_MFP |= PC10_MFP_LCD_S30;
	// SEG31
	GCR->PC_H_MFP |= PC11_MFP_LCD_S31;

	// SEG15
	GCR->PC_H_MFP |= PC14_MFP_LCD_S15;
	// SEG16
	GCR->PC_H_MFP |= PC15_MFP_LCD_S16;
#endif


}

/**
 *  @brief LCD Initialization routine for NANO100 display
 *
 *  @param[in] lcdinit  \n
 *                      S_LCD_INIT is a data structure to store initial settings. \n
 *                      typedef struct                                            \n
 *                      {                                                         \n
 *                          bool cpump_enable;     // Specifies if enable charge pump or not \n
 *                          bool internal_bias;    // Specifies if enable internal bias reference ladder or not \n
 *                          E_LCD_MUX mux; 	    // Specifies Mux configuration \n 
 *                          E_LCD_BIAS bias;	    // Specifies Bias configuration \n 
 *                          E_LCD_FREQDIV freqdiv; // Specifies LCD Frequency Divider Selection \n   
 *                          E_LCD_CPUMP_FREQDIV cpump_freqdiv; // Specifies frequency divider of charge pump \n
 *                          E_LCD_CPUMP_VOL cpump_voltage; // Specifies voltage of charge pump \n
 *                      } S_LCD_INIT;
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code
 *  S_LCD_INIT lcdinit;
 *  // LCD Initialize 
 *  lcdinit.cpump_enable = false;
 *  lcdinit.internal_bias = false;
 *  lcdinit.bias = LCD_BIAS_THIRD;
 *  lcdinit.mux = LCD_MUX_QUADRUPLEX;
 *  lcdinit.freqdiv = LCD_FREQ_DIV64;
 *  DrvLCD_Init(&lcdinit);
 *  @endcode
 *
 */
void LCD_Init(S_LCD_INIT *lcdinit)
{
  	/* Enable LCD Clock  */
	CLK->APBCLK |= CLK_APBCLK_LCD_EN; 

  	/* IP reset */
	GCR->IPRST_CTL2 |= GCR_IPRSTCTL2_LCD;
	GCR->IPRST_CTL2 &= ~GCR_IPRSTCTL2_LCD;

	LCD_Disable();

  	/* Turn all segments off */
  	LCD_AllOff();

	// V1
	GCR->PB_H_MFP |= PB9_MFP_LCD_V1;
	// V2
	GCR->PB_H_MFP |= PB10_MFP_LCD_V2;
	// V3
	GCR->PB_H_MFP |= PB11_MFP_LCD_V3;
	// DH1
	GCR->PC_L_MFP |= PC0_MFP_LCD_DH1;
	// DH2
	GCR->PC_L_MFP |= PC1_MFP_LCD_DH2;

	/* Digital Input Path Disable */
	/* V1, V2 and V3 */
	GPIOB->OFFD |= (0xE000000);
	/* DH1 and DH2 */
	GPIOC->OFFD |= (0x30000);

	/* voltage & bias generator settings */
	if(lcdinit->cpump_enable == true)
	{
		LCD_ChargePumpInit(lcdinit);
	} 
	else
	{
		LCD->DISPCTL &= ~LCD_DISPCTL_CPUMP_EN;
		LCD->DISPCTL |= LCD_DISPCTL_BV_SEL;
		LCD->DISPCTL &= ~LCD_DISPCTL_IBRL_EN;
		LCD->DISPCTL |= (lcdinit->internal_bias==true)?LCD_DISPCTL_IBRL_EN:0;	
	}
	
	/* Normal Settings */
	LCD->CTL = LCD->CTL & ~LCD_CTL_FREQ | lcdinit->freqdiv;	// LCD frequency div 512 default // 32768/512 = 64Hz
  	LCD->CTL = LCD->CTL & ~LCD_CTL_MUX | lcdinit->mux;
	LCD->DISPCTL = LCD->DISPCTL & ~LCD_DISPCTL_BIAS_SEL | lcdinit->bias; 

	LCD_Enable();	// Enable LCD

	LCD_UpdateLCDFrameFreq();
}

/**
 *  @brief The function is used to disable LCD controller, disable LCD clock and disable ISR 
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
 *  LCD_DeInit( );
 *  @endcode
 *
 */
void LCD_DeInit(void)
{
	LCD_Disable();

	CLK->APBCLK &= ~CLK_APBCLK_LCD_EN;
	NVIC_DisableIRQ (LCD_IRQn);
}

/**
 *  @brief Set Blinking frequency in LCD controller
 *
 *  @param[in] mdelay   Blinking delay time (Uint: ms)
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return  0: Successful \n
 *           ERR_LCD_CAL_BLINK_FAIL: Can't calculate the appropriate hardware setting to apply the blinking frequency.
 *
 *  @par Example
 *  @code
 *  // the frequency of blinking is 2 times per second
 *  DrvLCD_BlinkFrequency(500);
 *  @endcode
 *
 */
int32_t LCD_BlinkFrequency(double mdelay)
{
	uint32_t prescale=LCD_FCPRESC_DIV1, div=1;
	uint32_t framecount;

	LCDDEBUG("In present, frame rate is %f Hz \n", g_LCDFrameRate);

	framecount = (uint32_t) (mdelay / ((1/g_LCDFrameRate)*1000)) ;

	if(framecount > 0x3F) 
	{
		for(div=2; div<=8; div*=2)
		{
			framecount = (uint32_t) (mdelay / ((1/(g_LCDFrameRate/div)*1000) ));
			if( framecount <= 0x40 )
				break;
		}
		if(div==2) prescale = LCD_FCPRESC_DIV2;
		else if(div==4) prescale = LCD_FCPRESC_DIV4;
		else if(div==8) prescale = LCD_FCPRESC_DIV8;
		else return ERR_LCD_CAL_BLINK_FAIL;
	} 
	else if(framecount == 0)
	{
		framecount = 1;
	}
	LCDDEBUG("Actual blinking frequency %f second(s) \n", (1/(g_LCDFrameRate/div))*(framecount) );
	LCDDEBUG("FCV=%d, PreScale divider=%d \n", framecount, div);

	LCD->FCR = LCD->FCR & ~LCD_FCR_PRESCL | prescale;
	LCD->FCR = LCD->FCR & ~LCD_FCR_FCV | ((framecount - 1) << LCD_FCR_FCV_BIT_ADDR);
	LCD->FCR |= LCD_FCR_FCEN;
	
	return 0;
}
 

/**
 *  @brief Disable Blink function in LCD controller
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
 *  LCD_DisableBlink( );
 *  @endcode
 *
 */
void LCD_DisableBlink(void)
{
	/* Disable Blink LCD */
	LCD->CTL &= ~LCD_CTL_BLINK;
	/* Disable frame count */
	LCD_FrameCountDisable();
}

/**
 *  @brief This function is used to enable LCD interrupt
 *
 *  @param[in] IntSrc         Interrupt Source: LCD_FRAMECOUNT_INT / LCD_POWERDOWN_INT / LCD_ALL_INT
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code 
 *  LCD_EnableInt (LCD_FRAMECOUNT_INT);
 *  @endcode
 *
 */
void LCD_EnableInt(uint32_t IntSrc)
{
	switch (IntSrc)
    {
    	case LCD_FRAMECOUNT_INT:
        {
			LCD->FCR |= LCD_FCR_FCINTEN; 
   			break;
        }
        case LCD_POWERDOWN_INT:
        {
			LCD->CTL |= LCD_CTL_PDINT_EN; 
            break;
        }
    }
	NVIC_EnableIRQ(LCD_IRQn); 
}

/**
 *  @brief This function is used to disable LCD specified interrupt
 *
 *  @param[in] IntSrc   Interrupt Source: LCD_FRAMECOUNT_INT / LCD_POWERDOWN_INT / LCD_ALL_INT
 *
 *  @par Include:
 *    nano1xx_drivers/nano1xx_lcd.h
 *
 *  @return None
 *
 *  @par Example
 *  @code 
 *  LCD_DisableInt( LCD_POWERDOWN_INT );
 *  @endcode
 *
 */
void LCD_DisableInt(uint32_t IntSrc)
{
	
	if((IntSrc & LCD_FRAMECOUNT_INT) == LCD_FRAMECOUNT_INT )
	{
		LCD->FCR &= ~LCD_FCR_FCINTEN; 
		LCD->FCSTS |= LCD_FCSTS_FCSTS;
	}
	else if((IntSrc & LCD_POWERDOWN_INT) == LCD_POWERDOWN_INT )
	{
		LCD->CTL &= ~LCD_CTL_PDINT_EN; 
		LCD->FCSTS |= LCD_FCSTS_PDSTS; 
	}

}

/*@}*/ /* end of group NANO1xx_LCD_EXPORTED_FUNCTIONS */
 
/*@}*/ /* end of group NANO1xx_LCD_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

