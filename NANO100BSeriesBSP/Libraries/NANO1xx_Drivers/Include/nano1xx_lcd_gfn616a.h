/******************************************************************************
 * @file     nano1xx_qfn616a.h
 * @brief    NANO1xx LCD panel header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifdef __LCDDISPLAY_GFN616A_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define LCD_ALPHABET_NUM		13
#define LCD_DIGIT_NUM			13

#define LCD_ALPHABET_SEGNUM		8
#define LCD_DIGIT_SEGNUM				8

/**************************************************************************//**
 * 
 * Defines each text's segment (alphabet+numeric) in terms of COM and BIT numbers,
 * Using this way that text segment can be consisted of each bit in the
 * following bit pattern:
 * @illustration
 *               A
 *         ----------
 *         |         |
 *        F|         |B
 *         |         |
 *         -----G----- 
 *         |         |
 *       E |         |C
 *         |         |   _
 *         -----------  | |DP   
 *             D         -
 *
 *         -------0------
 *	     
 *        |              |
 *        |5             |1
 *
 *         -------6------- 
 *
 *        |              |
 *        |4             |2
 *							  __
 *         -------3------	 |__| 7
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
	{	/* 1 */
		/* A            B               C              D    */
      .com[0] = 3, .com[1] = 3, .com[2] = 1, .com[3] = 0,
      .bit[0] = 31, .bit[1] = 30, .bit[2] = 30, .bit[3] = 31,
		/* E            F               G              DP   */
      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 0,
      .bit[4] = 31, .bit[5] = 31, .bit[6] = 30, .bit[7] = 30
    },
    {	/* 2 */
      .com[0] = 3, .com[1] = 3, .com[2] = 1, .com[3] = 0,
      .bit[0] = 29, .bit[1] = 28, .bit[2] = 28, .bit[3] = 29,

      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 0,
      .bit[4] = 29, .bit[5] = 29, .bit[6] = 28, .bit[7] = 28
    },
    {	/* 3 */
      .com[0] = 3, .com[1] = 3, .com[2] = 1, .com[3] = 0,
      .bit[0] = 27, .bit[1] = 26, .bit[2] = 26, .bit[3] = 27,

      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 0,
      .bit[4] = 27, .bit[5] = 27, .bit[6] = 26, .bit[7] = 26
    },
    {	/* 4 */
      .com[0] = 3, .com[1] = 3, .com[2] = 1, .com[3] = 0,
      .bit[0] = 25, .bit[1] = 24, .bit[2] = 24, .bit[3] = 25,

      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 0,
      .bit[4] = 25, .bit[5] = 25, .bit[6] = 24, .bit[7] = 24
    },
	{	/* 5 */
      .com[0] = 3, .com[1] = 3, .com[2] = 1, .com[3] = 0,
      .bit[0] = 23, .bit[1] = 22, .bit[2] = 22, .bit[3] = 23,

      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 0,
      .bit[4] = 23, .bit[5] = 23, .bit[6] = 22, .bit[7] = 0
    },
	{	/* 6 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 3, .bit[1] = 4, .bit[2] = 4, .bit[3] = 3,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 3, .bit[5] = 3, .bit[6] = 4, .bit[7] = 4
    },
	{	/* 7 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 5, .bit[1] = 6, .bit[2] = 6, .bit[3] = 5,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 5, .bit[5] = 5, .bit[6] = 6, .bit[7] = 6
    },
	{	/* 8 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 7, .bit[1] = 8, .bit[2] = 8, .bit[3] = 7,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 7, .bit[5] = 7, .bit[6] = 8, .bit[7] = 8
    },
    {	/* 9 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 9, .bit[1] = 10, .bit[2] = 10, .bit[3] = 9,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 9, .bit[5] = 9, .bit[6] = 10, .bit[7] = 10
    },
    {	/* 10 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 32, .bit[1] = 33, .bit[2] = 33, .bit[3] = 32,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 32, .bit[5] = 32, .bit[6] = 33, .bit[7] = 33
    },
    {	/* 11 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 34, .bit[1] = 35, .bit[2] = 35, .bit[3] = 34,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 34, .bit[5] = 34, .bit[6] = 35, .bit[7] = 35
    },
    {	/* 12 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 36, .bit[1] = 37, .bit[2] = 37, .bit[3] = 36,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 36, .bit[5] = 36, .bit[6] = 37, .bit[7] = 37
    },
    {	/* 13 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 38, .bit[1] = 39, .bit[2] = 39, .bit[3] = 38,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 0,
      .bit[4] = 38, .bit[5] = 38, .bit[6] = 39, .bit[7] = 0
    }
  },
  .Number      = {
    {	/* 1 */
		/* A            B               C              D    */
      .com[0] = 3, .com[1] = 3, .com[2] = 1, .com[3] = 0,
      .bit[0] = 31, .bit[1] = 30, .bit[2] = 30, .bit[3] = 31,
		/* E            F               G              DP   */
      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 0,
      .bit[4] = 31, .bit[5] = 31, .bit[6] = 30, .bit[7] = 30
    },
    {	/* 2 */
      .com[0] = 3, .com[1] = 3, .com[2] = 1, .com[3] = 0,
      .bit[0] = 29, .bit[1] = 28, .bit[2] = 28, .bit[3] = 29,

      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 0,
      .bit[4] = 29, .bit[5] = 29, .bit[6] = 28, .bit[7] = 28
    },
    {	/* 3 */
      .com[0] = 3, .com[1] = 3, .com[2] = 1, .com[3] = 0,
      .bit[0] = 27, .bit[1] = 26, .bit[2] = 26, .bit[3] = 27,

      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 0,
      .bit[4] = 27, .bit[5] = 27, .bit[6] = 26, .bit[7] = 26
    },
    {	/* 4 */
      .com[0] = 3, .com[1] = 3, .com[2] = 1, .com[3] = 0,
      .bit[0] = 25, .bit[1] = 24, .bit[2] = 24, .bit[3] = 25,

      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 0,
      .bit[4] = 25, .bit[5] = 25, .bit[6] = 24, .bit[7] = 24
    },
	{	/* 5 */
      .com[0] = 3, .com[1] = 3, .com[2] = 1, .com[3] = 0,
      .bit[0] = 23, .bit[1] = 22, .bit[2] = 22, .bit[3] = 23,

      .com[4] = 1, .com[5] = 2, .com[6] = 2, .com[7] = 0,
      .bit[4] = 23, .bit[5] = 23, .bit[6] = 22, .bit[7] = 0
    },
	{	/* 6 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 3, .bit[1] = 4, .bit[2] = 4, .bit[3] = 3,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 3, .bit[5] = 3, .bit[6] = 4, .bit[7] = 4
    },
	{	/* 7 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 5, .bit[1] = 6, .bit[2] = 6, .bit[3] = 5,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 5, .bit[5] = 5, .bit[6] = 6, .bit[7] = 6
    },
	{	/* 8 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 7, .bit[1] = 8, .bit[2] = 8, .bit[3] = 7,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 7, .bit[5] = 7, .bit[6] = 8, .bit[7] = 8
    },
    {	/* 9 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 9, .bit[1] = 10, .bit[2] = 10, .bit[3] = 9,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 9, .bit[5] = 9, .bit[6] = 10, .bit[7] = 10
    },
    {	/* 10 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 32, .bit[1] = 33, .bit[2] = 33, .bit[3] = 32,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 32, .bit[5] = 32, .bit[6] = 33, .bit[7] = 33
    },
    {	/* 11 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 34, .bit[1] = 35, .bit[2] = 35, .bit[3] = 34,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 34, .bit[5] = 34, .bit[6] = 35, .bit[7] = 35
    },
    {	/* 12 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 36, .bit[1] = 37, .bit[2] = 37, .bit[3] = 36,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 3,
      .bit[4] = 36, .bit[5] = 36, .bit[6] = 37, .bit[7] = 37
    },
    {	/* 13 */
      .com[0] = 0, .com[1] = 0, .com[2] = 2, .com[3] = 3,
      .bit[0] = 38, .bit[1] = 39, .bit[2] = 39, .bit[3] = 38,

      .com[4] = 2, .com[5] = 1, .com[6] = 1, .com[7] = 0,
      .bit[4] = 38, .bit[5] = 38, .bit[6] = 39, .bit[7] = 0
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
  0x0000, /* ! */
  0x0000, /* " */
  0x0000, /* # */
  0x0000, /* $ */
  0x0000, /* % */
  0x0000, /* & */
  0x0000, /* £  */
  0x0000, /* ( */
  0x0000, /* ) */
  0x0000, /* * */
  0x0000, /* + */
  0x0000, /* , */
  0x0040, /* - */
  0x0080, /* . */
  0x0000, /* / */

  0x003f, /* 0 */
  0x0006, /* 1 */
  0x005b, /* 2 */
  0x004f, /* 3 */
  0x0066, /* 4 */
  0x006d, /* 5 */
  0x007d, /* 6 */
  0x0007, /* 7 */
  0x007f, /* 8 */
  0x006f, /* 9 */

  0x0000,
};

/**************************************************************************//**
 * Defines segments for the numeric display
 *****************************************************************************/
const uint16_t Nuvo_Numbers[] =
{
  0x3f, /* 0 */
  0x06, /* 1 */
  0x5b, /* 2 */
  0x4f, /* 3 */
  0x66, /* 4 */
  0x6d, /* 5 */
  0x7d, /* 6 */
  0x07, /* 7 */
  0x7f, /* 8 */
  0x6f, /* 9 */
  0x40, /* - */
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
	{2, 0},							/* 0 */
	{3, 14},						/* 1 */
	{3, 1},							/* 2 */
	{2, 21},						/* 3 */
	{1, 21},						/* 4 */
	{3, 0},							/* 5 */
	{2, 20},						/* 6 */
	{0, 1},							/* 7 */
	{0, 2},							/* 8 */
	{1, 1},							/* 9 */
	{2, 1},							/* 10 */
	{0, 21},						/* 11 */
	{0, 14},						/* 12 */
	{1, 14},						/* 13 */
	{0, 15},						/* 14 */
	{1, 15},						/* 15 */
	{2, 15},						/* 16 */
	{3, 15},						/* 17 */
	{1, 18},						/* 18 */
	{3, 19},						/* 19 */
	{0, 19},						/* 20 */
	{2, 18},						/* 21 */
	{3, 18},						/* 22 */
	{2, 19},						/* 23 */
	{1, 19},						/* 24 */
	{0, 20},						/* 25 */
	{1, 20},						/* 26 */
	{3, 20},						/* 27 */
	{3, 21},						/* 28 */
	{1, 2},							/* 29 */
	{2, 2},							/* 30 */
	{3, 2},							/* 31 */
	{0, 18},						/* 32 */
	{3, 39},						/* 33 */
	{2, 14},						/* 34 */
	{3, 13},						/* 35 */
	{2, 13},						/* 36 */
	{1, 13},						/* 37 */
	{0, 13},						/* 38 */
	{0, 12},						/* 39 */
	{1, 12},						/* 40 */
	{2, 12},						/* 41 */
	{3, 12},						/* 42 */
	{3, 11},						/* 43 */
	{2, 11},						/* 44 */
	{1, 11},						/* 45 */
	{0, 11},						/* 46 */
	{0, 17},						/* 47 */
	{0, 16},						/* 48 */
	{1, 16},						/* 49 */
	{2, 16},						/* 50 */
	{3, 16},						/* 51 */
	{3, 17},						/* 52 */
	{1, 17},						/* 53 */
	{2, 17},						/* 54 */
	{0, 22},						/* 55 */
};

#if 0
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

/**
  * @}
  */ 
  

#ifdef __cplusplus
}
#endif

#endif		/* __LCDDISPLAY_GFN616A_H */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/



