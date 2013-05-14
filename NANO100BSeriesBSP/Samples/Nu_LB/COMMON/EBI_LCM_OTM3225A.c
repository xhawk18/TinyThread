/******************************************************************************
 * @file     EBI_LCM_OTM3225A.c
 * @brief    This file provides LCD drawing functions
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "nano1xx_sys.h"
#include "Ascii_32x32.h"
#include "EBI_LCM_OTM3225A.h"

#define ALIGN_CONTROL			10

/* current resolution */
E_RES_SIZE g_otm3225a_resolution;

static void delay(uint16_t time)
{
    while(time--);
}
static void Delay_ms(uint16_t Dtime)
{
   while(Dtime --)
   {
      delay(180);
   } 
}


void OTM3225A_IndexWrite(uint16_t addr)
{
	RS_LOW;								// RS = LOW
	CS_LOW;								// CS = LOW
	EBIAccessWrite(0x00);
	EBIAccessWrite(addr & 0xFF);	
	RS_HIGH;							// RS = HIGH
	CS_HIGH;							// CS = HIGH
}

void OTM3225A_DataWrite(uint16_t data)
{
	RS_HIGH;							// RS = HIGH
	CS_LOW;								// CS = LOW
	EBIAccessWrite((data & 0xFF00)>>8);
	EBIAccessWrite(data & 0xFF);
	CS_HIGH;							// CS = HIGH
}

void OTM3225A_DataRead(uint16_t *data)
{
	RS_HIGH;							// RS = HIGH
	CS_LOW;								// CS = LOW
	*data = 0;
	*data = (EBIAccessRead())<<8;
	*data |= EBIAccessRead();
	CS_HIGH;							// CS = HIGH
}

void OTM3225A_DummyRead(void)
{
	volatile uint8_t data;
	RS_HIGH;							// RS = HIGH
	CS_LOW;								// CS = LOW
	data = EBIAccessRead();
	CS_HIGH;							// CS = HIGH
}

void OTM3225A_RegWrite(uint16_t addr, uint16_t data)
{
	OTM3225A_IndexWrite(addr);
	OTM3225A_DataWrite(data);	
}

void OTM3225A_RegRead(uint8_t addr, uint16_t *data)
{
	OTM3225A_IndexWrite(addr);
	OTM3225A_DataRead(data);
}

void OTM3225A_IDRead(uint16_t addr, uint16_t *data)
{
	OTM3225A_IndexWrite(addr);
	
	RS_LOW;									// RS = LOW
	CS_LOW;									// CS = LOW
	*data = 0;
	*data = (EBIAccessRead())<<8;
	*data |= EBIAccessRead();
	RS_HIGH;								// RS = HIGH	
	CS_HIGH;								// CS = HIGH
}
 
void OTM3225A_SleepIN(void)
{
    OTM3225A_RegWrite(0x0007, 0x0000); //Display off
    SYS_Delay(50000);
    OTM3225A_RegWrite(0x0010, 0x0002); //Set to sleep mode
    SYS_Delay(500000);
}
 
void OTM3225A_ExitSleep(void)
{
    OTM3225A_RegWrite(0x0010, 0x1690);
    SYS_Delay(50000);
    OTM3225A_RegWrite(0x0007, 0x0133); //Display on
    SYS_Delay(500000);
}
 
void OTM3225A_EnterStandby(void)
{
    OTM3225A_RegWrite(0x0007, 0x0000); //Display off
    SYS_Delay(50000);
    OTM3225A_RegWrite(0x0010, 0x0001); //Set to deep stand by mode
    SYS_Delay(500000);
}
 
void OTM3225A_ExitStandby(void)
{
    OTM3225A_RegWrite(0x0010, 0x1690);
    SYS_Delay(50000);
    OTM3225A_RegWrite(0x0007, 0x0133); //Display on
    SYS_Delay(500000);
}
 
 
void OTM3225_window_start(uint16_t startx, uint16_t starty)
{
	uint16_t tmp;
	if(g_otm3225a_resolution==RES_LANDSCAPE_320X240)
	{
		tmp = starty;
		starty = startx;
		startx = tmp;
	}

	OTM3225A_RegWrite(0x0020, startx);  
	OTM3225A_RegWrite(0x0021, starty); 
	OTM3225A_IndexWrite(0x0022);
}

void OTM3225_init(E_RES_SIZE res)
{
	RST_HIGH;
	Delay_ms(1); // Delay 1ms
	RST_LOW;
	Delay_ms(30); // Delay 10ms. This delay time is necessary.
	RST_HIGH;
	Delay_ms(50); // Delay 50 ms 

	// LCD initialization
	OTM3225A_RegWrite(0x00E3, 0x3008); // Set internal timing
	OTM3225A_RegWrite(0x00E7, 0x0012); // Set internal timing
	OTM3225A_RegWrite(0x00EF, 0x1231); // Set internal timing
	
	OTM3225A_RegWrite(0x0001, 0x0000);	// SM:0, SS:0
	OTM3225A_RegWrite(0x0002, 0x0700);	// set 1 line inversion

	if(res == RES_LANDSCAPE_320X240)
		OTM3225A_RegWrite(0x0003, 0x1038); // set GRAM write direction and BGR=1. (resolution: 320x240)
	else if	(res == RES_PORTRAIT_240X320)
		OTM3225A_RegWrite(0x0003, 0x1030); // set GRAM write direction and BGR=1. (resolution: 240x320)

	OTM3225A_RegWrite(0x0004, 0x0000);	// RCV:0, RCH:0, RSZ:0
	
	OTM3225A_RegWrite(0x0008, 0x0808);	// FP[3:0]:0x8, BP[3:0]:0x8
	//OTM3225A_RegWrite(0x0008, 0x0207);   //OLD:0207  G:0202 

	OTM3225A_RegWrite(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]

	OTM3225A_RegWrite(0x000A, 0x0000);	// FMARK_OE:0, FMI[2:0]:0

	OTM3225A_RegWrite(0x000C, 0x0000);	// ENC[2:0]:0, RM:0(system interface), DM[1:0]:0, RIM[1:0]=0x0
	OTM3225A_RegWrite(0x000D, 0x0000);	// FMP[8:0]:0
	OTM3225A_RegWrite(0x000F, 0x0000);	// VSPL:0, HSPL:0, EPL:0, DPL:0

	//*************Power On sequence ****************//
	//OTM3225A_RegWrite(0x0010, 0x14B0);	// SAP:1, BT[2:0]:0x4, APE:1, AP[2:0]:0x3, SLP:0, STB:0
	//OTM3225A_RegWrite(0x0011, 0x0770);	// DC1[2:0]:0x7, DC0[2:0]:0x7, VC[2:0]:0
	OTM3225A_RegWrite(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	OTM3225A_RegWrite(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	OTM3225A_RegWrite(0x0012, 0x0000);	// VCIRE:0, VRH[3:0]:0
	OTM3225A_RegWrite(0x0013, 0x0000);	// VDV[4:0]:0
	Delay_ms(200);
	//OTM3225A_RegWrite(0x0010, 0x1090); // SAP, BT[3:0], AP, DSTB, SLP, STB
	OTM3225A_RegWrite(0x0010, 0x1690);   //OLD:0169 G:0149 Other:17B0
	//OTM3225A_RegWrite(0x0010, 0x0149);   //OLD:0169 G:0149 Other:17B0
	//OTM3225A_RegWrite(0x0010, 0x17B0);   //OLD:0169 G:0149 Other:17B0
	OTM3225A_RegWrite(0x0011, 0x0147); // Set DC1[2:0], DC0[2:0], VC[2:0]
	//OTM3225A_RegWrite(0x0011, 0x0227); // Set DC1[2:0], DC0[2:0], VC[2:0]
	Delay_ms(500);// Delay 50ms
	OTM3225A_RegWrite(0x0012, 0x0091); // External reference voltage= Vci;
	//OTM3225A_RegWrite(0x0012, 0x001B);   //OLD:001B G:0019 Other:0139
	//OTM3225A_RegWrite(0x0012, 0x0139);   //OLD:001B G:0019 Other:0139
	Delay_ms(500); // Delay 50ms
	OTM3225A_RegWrite(0x0013, 0x1300); // Set VDV[4:0] for VCOM amplitude
	//OTM3225A_RegWrite(0x0013, 0x1A00); // Set VDV[4:0] for VCOM amplitude
	//OTM3225A_RegWrite(0x0029, 0x0010);	// VCM[5:0]:0
	OTM3225A_RegWrite(0x0029, 0x0029);   //OLD:0029 G:0001
	//OTM3225A_RegWrite(0x0029, 0x0001);   //OLD:0029 G:0001
	OTM3225A_RegWrite(0x002B, 0x000D);	// FRS[3:0]:0xB (Frame-rate 70Hz)
	OTM3225A_RegWrite(0x002B, 0x000E);   //OLD:000E G:000D
	Delay_ms(500); // Delay 50ms
	
	OTM3225A_RegWrite(0x0020, 0x0000);	// AD[7:0]:0
	OTM3225A_RegWrite(0x0021, 0x0000);	// AD[16:8]:0

	// ----------- Adjust the Gamma Curve ----------//
	OTM3225A_RegWrite(0x0030, 0x0000);
	OTM3225A_RegWrite(0x0031, 0x0404);
	OTM3225A_RegWrite(0x0032, 0x0304);
	OTM3225A_RegWrite(0x0035, 0x0005);
	OTM3225A_RegWrite(0x0036, 0x1604);
	OTM3225A_RegWrite(0x0037, 0x0304);
	OTM3225A_RegWrite(0x0038, 0x0303);
	OTM3225A_RegWrite(0x0039, 0x0707);
	OTM3225A_RegWrite(0x003C, 0x0500);
	OTM3225A_RegWrite(0x003D, 0x000F);
	
	//------------------ Set GRAM area ---------------//
	OTM3225A_RegWrite(0x0050, 0x0000);	// HSA[7:0]:0
	OTM3225A_RegWrite(0x0051, 0x00EF);	// HEA[7:0]:0xEF
	OTM3225A_RegWrite(0x0052, 0x0000);	// VSA[7:0]:0
	OTM3225A_RegWrite(0x0053, 0x013F);	// VEA[7:0]:0x13F

	if(res == RES_LANDSCAPE_320X240)
		OTM3225A_RegWrite(0x0060, 0xA700); // Gate Scan Line
	else if	(res == RES_PORTRAIT_240X320)
		OTM3225A_RegWrite(0x0060, 0x2700);	// GS:0. NL[5:0]:0x27, SCN[5:0]:0

	OTM3225A_RegWrite(0x0061, 0x0001); // NDL,VLE, REV
	OTM3225A_RegWrite(0x006A, 0x0000);	// VL[8:0]:0
	
	//-------------- Partial Display Control ---------//
	OTM3225A_RegWrite(0x0080, 0x0000);	// PTD[8:0]:0
	OTM3225A_RegWrite(0x0081, 0x0000);	// PTS[8:0]:0
	OTM3225A_RegWrite(0x0082, 0x0000);	// PTE[8:0]:0
	OTM3225A_RegWrite(0x0083, 0x0000);	// PTD[8:0]:0
	OTM3225A_RegWrite(0x0084, 0x0000);	// PTS[8:0]:0
	OTM3225A_RegWrite(0x0085, 0x0000);	// PTE[8:0]:0

	//-------------- Panel Control -------------------//
	OTM3225A_RegWrite(0x0090, 0x0010);	// DIVI[1:0]:0, RTNI[4:0]:0x10
	OTM3225A_RegWrite(0x0092, 0x0600);	// NOWI[2:0]:0x6
	//OTM3225A_RegWrite(0x0093, 0x0003);
	//OTM3225A_RegWrite(0x0095, 0x021E);	// DIVE[1:0]:0x2, RTNE[4:0]:0x1E
	//OTM3225A_RegWrite(0x0095, 0x0110);
	//OTM3225A_RegWrite(0x0097, 0x0C00);	// NOWE[3:0]:0x6
	//OTM3225A_RegWrite(0x0097, 0x0000);
	//OTM3225A_RegWrite(0x0098, 0x0000);
	
	OTM3225A_RegWrite(0x0007, 0x0133); // 262K color and display ON

	g_otm3225a_resolution = res;
	
}


void Show_Word32x32(uint16_t x, uint16_t y, uint8_t ascii, uint16_t color)
{
	int row=0, column=0, bit=0;
	int dot=0;

	for(column=0; column<32; column++)
	{
		OTM3225_window_start(x, y);	
		
		for(row=0; row<4; row++)
		{
			for(bit=1; bit<=8; bit++)
			{
				if( (zk_table[ascii][dot] & 0x80) && (bit==1))
					OTM3225A_DataWrite(color);
				else if( (zk_table[ascii][dot] & 0x40) && (bit==2))
					OTM3225A_DataWrite(color);
				else if( (zk_table[ascii][dot] & 0x20) && (bit==3))
					OTM3225A_DataWrite(color);
				else if( (zk_table[ascii][dot] & 0x10) && (bit==4))
					OTM3225A_DataWrite(color);
				else if( (zk_table[ascii][dot] & 0x08) && (bit==5))
					OTM3225A_DataWrite(color);
				else if( (zk_table[ascii][dot] & 0x04) && (bit==6))
					OTM3225A_DataWrite(color);
				else if( (zk_table[ascii][dot] & 0x02) && (bit==7))
					OTM3225A_DataWrite(color);
				else if( (zk_table[ascii][dot] & 0x01) && (bit==8))
					OTM3225A_DataWrite(color);

				OTM3225_window_start(x+(row*8)+bit, y);	
			}

			dot++;
				
		}

		y++;
	}	
}

void Show_String32x32(uint16_t x, uint16_t y, uint8_t *string, uint16_t color)
{
	int row=0, startx=0;
	int length = strlen((const char *)string);

	for(row=0; row<length; row++)
	{
		if(row==0) {
			startx = x;
			Show_Word32x32(x, y, string[row], color);
		}
		else {
			startx = startx+32-ALIGN_CONTROL;
			Show_Word32x32(startx, y, string[row], color);			
		}
	}
}

void ClearScreen(uint16_t color)
{
	uint16_t xaxis, yaxis;
	
	OTM3225_window_start(0, 0);	
	for(xaxis=0; xaxis<320; xaxis++) 
	{
		for(yaxis=0; yaxis<240; yaxis++) 
		{
			OTM3225A_DataWrite(color);
		}
	}
}

void DrawPartialScreen(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
	uint16_t xaxis, yaxis;
	
	for(yaxis=0; yaxis<height; yaxis++) 
	{
		OTM3225_window_start(x, y++);	
		for(xaxis=0; xaxis<width; xaxis++)
		{
			OTM3225A_DataWrite(color);
		}
	}
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


	 
