/******************************************************************************
 * @file     nano1xx_lcd_btl001_lb.h
 * @brief    NANO1xx LCD panel header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifdef __LCDDISPLAY_BTL001_LB_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define LCD_ALPHABET_NUM		4
#define LCD_DIGIT_NUM			2

#define LCD_ALPHABET_SEGNUM		14
#define LCD_DIGIT_SEGNUM				8

/**************************************************************************//**
 * 
 * Defines each text's segment (alphabet+numeric) in terms of COM and BIT numbers,
 * Using this way that text segment can be consisted of each bit in the
 * following bit pattern:
 * @illustration
 *               A
 *         ----------
 *         |\   |J  /|
 *        F| H  |  K |B
 *         |  \ | /  |
 *         --G-- --M--
 *         |   /| \  |
 *       E |  Q |  N |C
 *         | /  |P  \|   
 *         -----------     
 *             D         
 *
 *         -------0------
 *	    
 *        |   \7  |8  /9 |
 *        |5   \  |  /   |1
 *
 *         --6---  ---10--
 *
 *        |    /  |  \11 |
 *        |4  /13 |12 \  |2
 *							 
 *         -------3------	 
 *
 * 
 * 
 *****************************************************************************/
typedef struct
{
	uint32_t com[LCD_ALPHABET_SEGNUM]; /* LCD COM line (for multiplexing) */
	uint32_t bit[LCD_ALPHABET_SEGNUM]; /* LCD SEG bit number */
} CHAR_TypeDef;


/**************************************************************************//**
 * Defines segment COM and BIT fields for numeric display
 *****************************************************************************/
typedef struct
{
	uint32_t com[LCD_DIGIT_SEGNUM];
	uint32_t bit[LCD_DIGIT_SEGNUM];
} NUMBER_TypeDef;



/**************************************************************************//**
 * Defines all types of display for all segments
 *****************************************************************************/
typedef struct
{
	CHAR_TypeDef    Text[LCD_ALPHABET_NUM];
  	NUMBER_TypeDef  Number[LCD_DIGIT_NUM];

} NANO1XX_DISPLAY;

/**************************************************************************//**
 * Defines the COM/SEG for each alphabet and numeric 
 *****************************************************************************/
const NANO1XX_DISPLAY NANO1XXDISPLAY = {
  .Text        = {
    { /* 4 */
		/* A            B               C              D    */
      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 10, .bit[1] = 10, .bit[2] = 10, .bit[3] = 10,
		/* E            F               G              H    */
      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 3,
      .bit[4] = 8, .bit[5] = 8, .bit[6] = 9, .bit[7] = 9,
		/* J            K               M              N    */
      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 11, .bit[9] = 11, .bit[10] = 11, .bit[11] = 11,
		/* P            Q      */
      .com[12] = 0, .com[13] = 1,
      .bit[12] = 9, .bit[13] = 9
    },
    { /* 5 */

      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 16, .bit[1] = 16, .bit[2] = 16, .bit[3] = 16,

      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 3,
      .bit[4] = 14, .bit[5] = 14, .bit[6] = 15, .bit[7] = 15,

      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 17, .bit[9] = 17, .bit[10] = 17, .bit[11] = 17,

      .com[12] = 0, .com[13] = 1,
      .bit[12] = 15, .bit[13] = 15
    },
    { /* 6 */

      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 22, .bit[1] = 22, .bit[2] = 22, .bit[3] = 22,

      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 3,
      .bit[4] = 18, .bit[5] = 18, .bit[6] = 19, .bit[7] = 19,

      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 23, .bit[9] = 23, .bit[10] = 23, .bit[11] = 23,

      .com[12] = 0, .com[13] = 1,
      .bit[12] = 19, .bit[13] = 19
    },
    { /* 7 */

      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 32, .bit[1] = 32, .bit[2] = 32, .bit[3] = 32,

      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 3,
      .bit[4] = 24, .bit[5] = 24, .bit[6] = 25, .bit[7] = 25,

      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 33, .bit[9] = 33, .bit[10] = 33, .bit[11] = 33,

      .com[12] = 0, .com[13] = 1,
      .bit[12] = 25, .bit[13] = 25
    }
  },
  .Number      = {  	
    {	/* 10 */
		/* A            B               C              D    */		
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 3, .bit[1] = 2, .bit[2] = 2, .bit[3] = 2,
		/* E            F               G              M    */
      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 1,
      .bit[4] = 3, .bit[5] = 3, .bit[6] = 2, .bit[7] = 2
    },
    {	/* 11 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 1, .bit[1] = 0, .bit[2] = 0, .bit[3] = 0,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 1,
      .bit[4] = 1, .bit[5] = 1, .bit[6] = 0, .bit[7] = 0
    }
  }
};

/**************************************************************************//**
 * 
 * Defines segments for the alphabet
 * Bit pattern below defined for alphabet (text segments)
 * 
 *****************************************************************************/
const uint16_t Nuvo_alphabet[] = {
  0x0000, /* space */
  0x1100, /* ! */
  0x0280, /* " */
  0x0000, /* # */
  0x0000, /* $ */
  0x0000, /* % */
  0x0000, /* & */
  0x0000, /* £  */
  0x0039, /* ( */
  0x000f, /* ) */
  0x3fc0, /* * */
  0x1540, /* + */
  0x0000, /* , */
  0x0440, /* - */
  0x8000, /* . */
  0x2200, /* / */

  0x003f, /* 0 */
  0x0006, /* 1 */
  0x045b, /* 2 */
  0x044f, /* 3 */
  0x0466, /* 4 */
  0x046d, /* 5 */
  0x047d, /* 6 */
  0x0007, /* 7 */
  0x047f, /* 8 */
  0x046f, /* 9 */

  0x0000, /* : */
  0x0000, /* ; */
  0x0a00, /* < */
  0x0000, /* = */
  0x2080, /* > */
  0x0000, /* ? */
  0xffff, /* @ */

  0x0477, /* A */
  0x0a79, /* B */
  0x0039, /* C */
  0x20b0, /* D */
  0x0079, /* E */
  0x0071, /* F */
  0x047d, /* G */
  0x0476, /* H */
  0x0006, /* I */
  0x000e, /* J */
  0x0a70, /* K */
  0x0038, /* L */
  0x02b6, /* M */
  0x08b6, /* N */
  0x003f, /* O */
  0x0473, /* P */
  0x203f, /* Q */
  0x0c73, /* R */
  0x046d, /* S */
  0x1101, /* T */
  0x003e, /* U */
  0x2230, /* V */
  0x2836, /* W */
  0x2a80, /* X */
  0x046e, /* Y */
  0x2209, /* Z */

  0x0039, /* [ */
  0x0880, /* backslash */
  0x000f, /* ] */
  0x0001, /* ^ */
  0x0008, /* _ */
  0x0000, /* ` */

  0x1058, /* a */
  0x047c, /* b */
  0x0058, /* c */
  0x045e, /* d */
  0x2058, /* e */
  0x0471, /* f */
  0x0c0c, /* g */
  0x0474, /* h */
  0x0004, /* i */
  0x000e, /* j */
  0x0c70, /* k */
  0x0038, /* l */
  0x1454, /* m */
  0x0454, /* n */
  0x045c, /* o */
  0x0473, /* p */
  0x0467, /* q */
  0x0450, /* r */
  0x0c08, /* s */
  0x0078, /* t */
  0x001c, /* u */
  0x2010, /* v */
  0x2814, /* w */
  0x2a80, /* x */
  0x080c, /* y */
  0x2048, /* z */

  0x0000,
};

/**************************************************************************//**
 * Defines segments for the numeric display
 *****************************************************************************/
const uint16_t Nuvo_Numbers[] =
{
  0x3f, /* 0 */
  0x06, /* 1 */
  0xdb, /* 2 */
  0xcf, /* 3 */
  0xe6, /* 4 */
  0xed, /* 5 */
  0xfd, /* 6 */
  0x07, /* 7 */
  0xff, /* 8 */
  0xef, /* 9 */
  0xc0, /* - */
};


/** @defgroup S_LCDSYMBOL
  * @{
  */

typedef struct
{
	uint32_t com; /* Symbol COM line */
	uint32_t seg; /* Symbol SEG bit number */
} SYMBOL_TypeDef;

const SYMBOL_TypeDef S_LCDSYMBOL[] = 
{
	{0, 8},							/* 0 */
	{0, 14},						/* 1 */
	{0, 18},						/* 2 */
	{0, 24},						/* 3 */
	{3, 1},							/* 4 */
	{3, 8},							/* 5 */
	{3, 14},						/* 6 */
	{3, 18},						/* 7 */
	{3, 3},							/* 8 */
	{3, 36},						/* 9 */
	{2, 36},						/* 10 */
	{1, 36},						/* 11 */
	{0, 36},						/* 12 */
	{0, 37},						/* 13 */
	{1, 37},						/* 14 */
	{2, 37},						/* 15 */
	{3, 37},						/* 16 */
	{3, 38},						/* 17 */
	{2, 38},						/* 18 */
	{1, 38},						/* 19 */
	{0, 38},						/* 20 */
	{0, 27},						/* 21 */
	{3, 27},						/* 22 */
	{2, 27},						/* 23 */
	{1, 27},						/* 24 */	
	{3, 24},						/* 25 */
};
/**
  * @}
  */ 


#ifdef __cplusplus
}
#endif

#endif		/* __LCDDISPLAY_BTL001_LB_H */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/




