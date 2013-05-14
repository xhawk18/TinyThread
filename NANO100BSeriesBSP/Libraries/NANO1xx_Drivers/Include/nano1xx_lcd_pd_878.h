/******************************************************************************
 * @file     nano1xx_lcd_pd_878.h
 * @brief    NANO1xx LCD panel header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifdef __LCDDISPLAY_PD_878_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define LCD_ALPHABET_NUM		10
#define LCD_DIGIT_NUM			10

#define LCD_ALPHABET_SEGNUM		16
#define LCD_DIGIT_SEGNUM				8

/**************************************************************************//**
 * 
 * Defines each text's segment (alphabet+numeric) in terms of COM and BIT numbers,
 * Using this way that text segment can be consisted of each bit in the
 * following bit pattern:
 * @illustration
 *               A
 *     --  ----------
 *   X \/  |\   |I  /|
 *        F| H  |  J |B
 *         |  \ | /  |
 *         --G-- --K--
 *         |   /| \  |
 *       E |  L |  N |C
 *         | /  |M  \|   _
 *         -----------  | |DP   
 *             D         -
 *
 *   14--  -------0------
 *	   \/
 *        |   \7  |8  /9 |
 *        |5   \  |  /   |1
 *
 *         --6---  ---10--
 *
 *        |    /  |  \13 |
 *        |4  /11 |12 \  |2
 *							  __
 *         -------3------	 |__| 15
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
    { /* 1 */						
		/* A            B               C              D    */
      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 2, .bit[1] = 3, .bit[2] = 3, .bit[3] = 3,
		/* E            F               G              H    */
      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 2,
      .bit[4] = 0, .bit[5] = 0, .bit[6] = 1, .bit[7] = 1,
		/* I            J               K              L    */
      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 1, .bit[9] = 2, .bit[10] = 2, .bit[11] = 0,
		/* M            N               X              DP    */
      .com[12] = 0, .com[13] = 0, .com[14] = 3, .com[15] = 3,
      .bit[12] = 1, .bit[13] = 2, .bit[14] = 0, .bit[15] = 3
    },
    { /* 2 */

      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 6, .bit[1] = 7, .bit[2] = 7, .bit[3] = 7,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 2,
      .bit[4] = 4, .bit[5] = 4, .bit[6] = 5, .bit[7] = 5,

      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 5, .bit[9] = 6, .bit[10] = 6, .bit[11] = 4,

      .com[12] = 0, .com[13] = 0, .com[14] = 3, .com[15] = 3,
      .bit[12] = 5, .bit[13] = 6, .bit[14] = 4, .bit[15] = 7
    },
    { /* 3 */

      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 10, .bit[1] = 11, .bit[2] = 11, .bit[3] = 11,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 2,
      .bit[4] = 8, .bit[5] = 8, .bit[6] = 9, .bit[7] = 9,

      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 9, .bit[9] = 10, .bit[10] = 10, .bit[11] = 8,

      .com[12] = 0, .com[13] = 0, .com[14] = 3, .com[15] = 3,
      .bit[12] = 9, .bit[13] = 10, .bit[14] = 8, .bit[15] = 11
    },
    { /* 4 */

      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 14, .bit[1] = 15, .bit[2] = 15, .bit[3] = 15,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 2,
      .bit[4] = 12, .bit[5] = 12, .bit[6] = 13, .bit[7] = 13,

      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 13, .bit[9] = 14, .bit[10] = 14, .bit[11] = 12,

      .com[12] = 0, .com[13] = 0, .com[14] = 3, .com[15] = 3,
      .bit[12] = 13, .bit[13] = 14, .bit[14] = 12, .bit[15] = 15
    },
    { /* 5 */

      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 18, .bit[1] = 19, .bit[2] = 19, .bit[3] = 19,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 2,
      .bit[4] = 16, .bit[5] = 16, .bit[6] = 17, .bit[7] = 17,

      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 17, .bit[9] = 18, .bit[10] = 18, .bit[11] = 16,

      .com[12] = 0, .com[13] = 0, .com[14] = 3, .com[15] = 3,
      .bit[12] = 17, .bit[13] = 18, .bit[14] = 16, .bit[15] = 19
    },
    { /* 6 */

      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 22, .bit[1] = 23, .bit[2] = 23, .bit[3] = 23,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 2,
      .bit[4] = 20, .bit[5] = 20, .bit[6] = 21, .bit[7] = 21,

      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 21, .bit[9] = 22, .bit[10] = 22, .bit[11] = 20,

      .com[12] = 0, .com[13] = 0, .com[14] = 3, .com[15] = 3,
      .bit[12] = 21, .bit[13] = 22, .bit[14] = 20, .bit[15] = 23
    },
    { /* 7 */

      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 26, .bit[1] = 27, .bit[2] = 27, .bit[3] = 27,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 2,
      .bit[4] = 24, .bit[5] = 24, .bit[6] = 25, .bit[7] = 25,

      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 25, .bit[9] = 26, .bit[10] = 26, .bit[11] = 24,

      .com[12] = 0, .com[13] = 0, .com[14] = 3, .com[15] = 3,
      .bit[12] = 25, .bit[13] = 26, .bit[14] = 24, .bit[15] = 27
    },
	{ /* 8 */

      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 30, .bit[1] = 31, .bit[2] = 31, .bit[3] = 31,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 2,
      .bit[4] = 28, .bit[5] = 28, .bit[6] = 29, .bit[7] = 29,

      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 29, .bit[9] = 30, .bit[10] = 30, .bit[11] = 28,

      .com[12] = 0, .com[13] = 0, .com[14] = 3, .com[15] = 3,
      .bit[12] = 29, .bit[13] = 30, .bit[14] = 28, .bit[15] = 31
    },
    { /* 9 */

      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 34, .bit[1] = 35, .bit[2] = 35, .bit[3] = 35,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 2,
      .bit[4] = 32, .bit[5] = 32, .bit[6] = 33, .bit[7] = 33,

      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 33, .bit[9] = 34, .bit[10] = 34, .bit[11] = 32,

      .com[12] = 0, .com[13] = 0, .com[14] = 3, .com[15] = 3,
      .bit[12] = 33, .bit[13] = 34, .bit[14] = 32, .bit[15] = 35
    },
    { /* 10 */

      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 38, .bit[1] = 39, .bit[2] = 39, .bit[3] = 39,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 2,
      .bit[4] = 36, .bit[5] = 36, .bit[6] = 37, .bit[7] = 37,

      .com[8] = 3, .com[9] = 2, .com[10] = 1, .com[11] = 0,
      .bit[8] = 37, .bit[9] = 38, .bit[10] = 38, .bit[11] = 36,

      .com[12] = 0, .com[13] = 0, .com[14] = 3, .com[15] = 3,
      .bit[12] = 37, .bit[13] = 38, .bit[14] = 36, .bit[15] = 39
    }
  },
  .Number      = {
    {	/* 1 */
		/* A            B               C              D    */
      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 2, .bit[1] = 3, .bit[2] = 3, .bit[3] = 3,
		/* E            F               G              K    */
      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 1,
      .bit[4] = 0, .bit[5] = 0, .bit[6] = 1, .bit[7] = 2
    },
    {	/* 2 */
      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 6, .bit[1] = 7, .bit[2] = 7, .bit[3] = 7,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 1,
      .bit[4] = 4, .bit[5] = 4, .bit[6] = 5, .bit[7] = 6
    },
    {	/* 3 */
      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 10, .bit[1] = 11, .bit[2] = 11, .bit[3] = 11,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 1,
      .bit[4] = 8, .bit[5] = 8, .bit[6] = 9, .bit[7] = 10
    },
    {	/* 4 */
      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 14, .bit[1] = 15, .bit[2] = 15, .bit[3] = 15,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 1,
      .bit[4] = 12, .bit[5] = 12, .bit[6] = 13, .bit[7] = 14
    },
	{	/* 5 */
      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 18, .bit[1] = 19, .bit[2] = 19, .bit[3] = 19,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 1,
      .bit[4] = 16, .bit[5] = 16, .bit[6] = 17, .bit[7] = 18
    },
	{	/* 6 */
      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 22, .bit[1] = 23, .bit[2] = 23, .bit[3] = 23,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 1,
      .bit[4] = 20, .bit[5] = 20, .bit[6] = 21, .bit[7] = 22
    },
	{	/* 7 */
      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 26, .bit[1] = 27, .bit[2] = 27, .bit[3] = 27,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 1,
      .bit[4] = 24, .bit[5] = 24, .bit[6] = 25, .bit[7] = 26
    },
	{	/* 8 */
      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 30, .bit[1] = 31, .bit[2] = 31, .bit[3] = 31,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 1,
      .bit[4] = 28, .bit[5] = 28, .bit[6] = 29, .bit[7] = 30
    },
    {	/* 9 */
      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 34, .bit[1] = 35, .bit[2] = 35, .bit[3] = 35,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 1,
      .bit[4] = 32, .bit[5] = 32, .bit[6] = 33, .bit[7] = 34
    },
    {	/* 10 */
      .com[0] = 3, .com[1] = 2, .com[2] = 1, .com[3] = 0,
      .bit[0] = 38, .bit[1] = 39, .bit[2] = 39, .bit[3] = 39,

      .com[4] = 1, .com[5] = 2, .com[6] = 1, .com[7] = 1,
      .bit[4] = 36, .bit[5] = 36, .bit[6] = 37, .bit[7] = 38
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
  0x0000, /* * */
  0x1540, /* + */
  0x0000, /* , */
  0x0440, /* - */
  0x8000, /* . */
  0x0A00, /* / */

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
  0x2200, /* < */
  0x0000, /* = */
  0x0880, /* > */
  0x0000, /* ? */
  0xffff, /* @ */

  0x0477, /* A */
  0x2279, /* B */
  0x0039, /* C */
  0x08b0, /* D */
  0x0079, /* E */
  0x0071, /* F */
  0x047d, /* G */
  0x0476, /* H */
  0x0006, /* I */
  0x000e, /* J */
  0x2270, /* K */
  0x0038, /* L */
  0x02b6, /* M */
  0x20b6, /* N */
  0x003f, /* O */
  0x0473, /* P */
  0x083f, /* Q */
  0x2473, /* R */
  0x046d, /* S */
  0x1101, /* T */
  0x003e, /* U */
  0x0A30, /* V */
  0x2836, /* W */
  0x2a80, /* X */
  0x046e, /* Y */
  0x0A09, /* Z */

  0x0039, /* [ */
  0x2080, /* backslash */
  0x000f, /* ] */
  0x0001, /* ^ */
  0x0008, /* _ */
  0x4000, /* ` */

  0x1058, /* a */
  0x047c, /* b */
  0x0058, /* c */
  0x045e, /* d */
  0x0858, /* e */
  0x0471, /* f */
  0x240c, /* g */
  0x0474, /* h */
  0x0004, /* i */
  0x000e, /* j */
  0x2470, /* k */
  0x0038, /* l */
  0x1454, /* m */
  0x0454, /* n */
  0x045c, /* o */
  0x0473, /* p */
  0x0467, /* q */
  0x0450, /* r */
  0x2408, /* s */
  0x0078, /* t */
  0x001c, /* u */
  0x0810, /* v */
  0x2814, /* w */
  0x2a80, /* x */
  0x200c, /* y */
  0x0848, /* z */

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
	{0, 0},							/* 0 */
};

#if 0
typedef enum
{
	LCD_SYMBOL_S1,						/* 0 */
} E_LCDSYMBOL;
#endif

/**
  * @}
  */ 


#ifdef __cplusplus
}
#endif

#endif		/* __LCDDISPLAY_PD_878_H */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/



