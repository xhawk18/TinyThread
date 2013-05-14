/******************************************************************************
 * @file     EBI_LCM_OTM3225A.h
 * @brief    This file provides all the LCD firmware functions
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/    
#ifndef _EBI_LCM_OTM3225A_H_
#define _EBI_LCM_OTM3225A_H_
 
#include "Nano1xx.h"
#include "nano1xx_ebi.h"

#define ID_READ_ADDR								(0x0000)
#define DRIVER_OUT_CON_1_ADDR					(0x0001)
#define LCD_DRV_WAVEFORM_CON_ADDR			(0x0002)
#define ENTRY_MODE_ADDR							(0x0003)
#define SCALING_CON_ADDR							(0x0004)
#define DISPLAY_CON_1_ADDR						(0x0007)
#define DISPLAY_CON_2_ADDR						(0x0008)
#define DISPLAY_CON_3_ADDR						(0x0009)
#define FRAME_CYCLE_CON_ADDR					(0x000A)
#define EXT_DIS_INTERFACE_CON_1_ADDR		(0x000C)
#define FRAME_MARKER_POS_ADDR					(0x000D)
#define EXT_DIS_INTERFACE_CON_2_ADDR		(0x000F)
#define POWER_CON_1_ADDR							(0x0010)
#define POWER_CON_2_ADDR							(0x0011)
#define POWER_CON_3_ADDR							(0x0012)
#define POWER_CON_4_ADDR							(0x0013)
#define GRAM_ADDR_SET_HOR_ADDR				(0x0020)
#define GRAM_ADDR_SET_VER_ADDR				(0x0021)
#define GRAM_DATA_WR_ADDR						(0x0022)
#define POWER_CON_7_ADDR							(0x0029)
#define FRAMERATE_CON_ADDR						(0x002B)
#define GAMMA_CON_1_ADDR							(0x0030)
#define GAMMA_CON_2_ADDR							(0x0031)
#define GAMMA_CON_3_ADDR							(0x0032)
#define GAMMA_CON_4_ADDR							(0x0035)
#define GAMMA_CON_5_ADDR							(0x0036)
#define GAMMA_CON_6_ADDR							(0x0037)
#define GAMMA_CON_7_ADDR							(0x0038)
#define GAMMA_CON_8_ADDR							(0x0039)
#define GAMMA_CON_9_ADDR							(0x003C)
#define GAMMA_CON_10_ADDR						(0x003D)
#define WINDOW_HOR_RAM_START_ADDR			(0x0050)
#define WINDOW_HOR_RAM_END_ADDR				(0x0051)
#define WINDOW_VER_RAM_START_ADDR			(0x0052)
#define WINDOW_VER_RAM_END_ADDR				(0x0053)
#define GATE_DRIVER_SCAN_ADDR					(0x0060)
#define DRIVER_OUT_CON_2_ADDR					(0x0061)
#define VERTICAL_SCROLL_CON_ADDR				(0x006A)
#define DIS_POS_PARTIAL_DIS_1_ADDR			(0x0080)
#define RAM_ADDR_START_PARTIAL_1_ADDR		(0x0081)
#define RAM_ADDR_END_PARTIAL_1_ADDR			(0x0082)
#define DIS_POS_PARTIAL_DIS_2_ADDR			(0x0083)
#define RAM_ADDR_START_PARTIAL_2_ADDR		(0x0084)
#define RAM_ADDR_END_PARTIAL_2_ADDR			(0x0085)
#define PANEL_INTERFACE_CON_1_ADDR			(0x0090)
#define PANEL_INTERFACE_CON_2_ADDR			(0x0092)
#define PANEL_INTERFACE_CON_3_ADDR			(0x0095)
#define PANEL_INTERFACE_CON_4_ADDR			(0x0097)


#define RS_HIGH		GPIOB->DOUT |= (1<<6);			// RS = HIGH
#define RS_LOW		GPIOB->DOUT &= ~(1<<6);			// RS = LOW

#define CS_HIGH		GPIOB->DOUT |= (1<<7);			// CS = HIGH
#define CS_LOW		GPIOB->DOUT &= ~(1<<7);			// CS = LOW

#define RST_HIGH	GPIOD->DOUT |= (1<<8);			// RESET = HIGH
#define RST_LOW		GPIOD->DOUT &= ~(1<<8);			// RESET = LOW


/* Select resolution type */
typedef enum {
	RES_PORTRAIT_240X320 = 0,
	RES_LANDSCAPE_320X240 = 1
} E_RES_SIZE;


/* EBI Write/Read address */
#define EBIAccessWrite(value)			*((volatile unsigned char *)(EBI_BASE_ADDR))=value
#define EBIAccessRead()					(*((volatile unsigned char *)(EBI_BASE_ADDR)))


/* prototype */
void OTM3225A_IndexWrite(uint16_t addr);
void OTM3225A_DataWrite(uint16_t data);
void OTM3225A_DataRead(uint16_t *data);
void OTM3225A_DummyRead(void);
void OTM3225A_RegWrite(uint16_t addr, uint16_t data);
void OTM3225A_RegRead(uint8_t addr, uint16_t *data);
void OTM3225A_IDRead(uint16_t addr, uint16_t *data);
void OTM3225A_SleepIN(void);
void OTM3225A_ExitSleep(void);
void OTM3225A_EnterStandby(void);
void OTM3225A_ExitStandby(void);
void OTM3225_window_start(uint16_t startx, uint16_t starty);
void Show_Word32x32(uint16_t x, uint16_t y, uint8_t ascii, uint16_t color);
void Show_String32x32(uint16_t x, uint16_t y, uint8_t *string, uint16_t color);
void ClearScreen(uint16_t color);
void DrawPartialScreen(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void OTM3225_init(E_RES_SIZE res);

#endif //_EBI_LCM_OTM3225A_H_

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


