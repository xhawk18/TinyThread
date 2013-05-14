/******************************************************************************
 * @file     Smpl_LCD.c
 * @brief    This file provides all the LCD display functions
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
#include "nano1xx.h"
#include "nano1xx_lcd.h"
#include "nano1xx_sys.h"


extern volatile uint32_t tkdata;

long long char_to_int(char c)
{
	if(c=='0') return 0;
	else if(c=='1') return 1;
	else if(c=='2') return 2;
	else if(c=='3') return 3;
	else if(c=='4') return 4;
	else if(c=='5') return 5;
	else if(c=='6') return 6;
	else if(c=='7') return 7;
	else if(c=='8') return 8;
	else if(c=='9') return 9;
	
	return -1;
}

long long local_atoi(char text[])
{
	int len = strlen(text);
	int len2, negflag=0;
	long long mul=len;
	long long i=0, j=0, mul2=1;
	long long result=0;

	if( text[0] == '-') 
	{
		negflag = 1;
		len2 = len - 1;
		for(i=0;i<len2;i++)
		{
			text[i] = text[i+1];
		}
		text[i] = '\0';
		len--;
		mul = len;
	}

	for(i=0; i < len; i++)
	{
		if(mul==1) mul2 = 1;
		else if(mul>1)
			for(j=0; j<(mul-1); j++)
				mul2 *= 10;
		result += mul2*char_to_int(text[i]);
		mul--;
		mul2=1;
	}

	if(negflag==1)
		result = 0 - result;
	
	return result;
}

void ControlSegment(bool onoff, int com, int seg)
{
	if(onoff==true)
		LCD_EnableSegment(com, seg);
	else
		LCD_DisableSegment(com, seg);
}

#if 1
void showSlider(uint16_t resolution)
{
	// Resolution (0 ~ 31) to LCD bar (0 ~ 11)

	double average, point=12, res;
	uint8_t lightpoint;

	average = 32 / point;
	//printf("average=%f\n", average);
	res = (double)resolution;
	point = res / average;

	//printf("point=%f\n", point);
	if(point > 12)
		printf("Touch key data over the resolution..\n");

	lightpoint = (uint8_t)point;

	LCD_DisableSegment(3, 37);	// PL
	LCD_DisableSegment(2, 37);	// P0
	LCD_DisableSegment(1, 37);	// P1
	LCD_DisableSegment(0, 37);	// P2
	LCD_DisableSegment(0, 36);	// P3
	LCD_DisableSegment(1, 36);	// P4
	LCD_DisableSegment(2, 36);	// P5
	LCD_DisableSegment(3, 36);	// P6
	LCD_DisableSegment(3, 35);	// P7
	LCD_DisableSegment(2, 35);	// P8
	LCD_DisableSegment(1, 35);	// P9
	LCD_DisableSegment(0, 35);	// PR
	

	if(lightpoint==0)
		ControlSegment(true, 3, 37);	// PL
	else if(lightpoint==1)
		ControlSegment(true, 2, 37);	// P0
	else if(lightpoint==2)
		ControlSegment(true, 1, 37);	// P1
	else if(lightpoint==3)
		ControlSegment(true, 0, 37);	// P2
	else if(lightpoint==4)
		ControlSegment(true, 0, 36);	// P3
	else if(lightpoint==5)
		ControlSegment(true, 1, 36);	// P4
	else if(lightpoint==6)
		ControlSegment(true, 2, 36);	// P5
	else if(lightpoint==7)
		ControlSegment(true, 3, 36);	// P6
	else if(lightpoint==8)
		ControlSegment(true, 3, 35);	// P7
	else if(lightpoint==9)
		ControlSegment(true, 2, 35);	// P8
	else if(lightpoint==10)
		ControlSegment(true, 1, 35);	// P9
	else if(lightpoint==11 || lightpoint==12)
		ControlSegment(true, 0, 35);	// PR
	
	
}
#else
void showSlider(uint8_t tkch)
{
	bool turnOn;
	if(((tkdata>>12)&0xF)==0xE)	// means that user touch on
	{
		turnOn = true;
	} else {
		turnOn = false;
	}

	if(tkch==9) 
		ControlSegment(turnOn, 0, 38);
	else if(tkch==10)
		ControlSegment(turnOn, 3, 38);
	else if(tkch==11)
	{
		ControlSegment(turnOn, 3, 37);
		ControlSegment(turnOn, 2, 37);
		ControlSegment(turnOn, 1, 37);
	}
	else if(tkch==12)
	{
		ControlSegment(turnOn, 0, 37);
		ControlSegment(turnOn, 0, 36);
	}
	else if(tkch==13)
	{
		ControlSegment(turnOn, 1, 36);
		ControlSegment(turnOn, 2, 36);
	}
	else if(tkch==14)
	{
		ControlSegment(turnOn, 3, 35);
		ControlSegment(turnOn, 3, 36);
	}
	else if(tkch==15)
	{
		ControlSegment(turnOn, 2, 35);
		ControlSegment(turnOn, 1, 35);
		ControlSegment(turnOn, 0, 35);
	}
	
}
#endif

void textticker(char *string, uint32_t delayus)
{
	char showstring[7], tmpstring[7];
	int showidx, textidx, storeshowidx, storetextidx;
	int textlen;

	strcpy(&showstring[0], "       ");
	
	textlen = strlen(string);

	showidx = 6;
	
	for(textidx=0; textidx<textlen; textidx++)
	{
		// clear showstring
		for(storetextidx=0; storetextidx<=6; storetextidx++) showstring[storetextidx] = ' ';
		
		storetextidx = textidx;
		if((6-showidx) > 0)
		{
			storetextidx -= (6-showidx);
		}
		for(storeshowidx=showidx; storeshowidx<=6; storeshowidx++)
		{			
			showstring[storeshowidx] = string[storetextidx++];
		}
		if(showidx!=0)
			showidx--;
		//printf("%s \r\n", showstring);
		LCD_Write(&showstring[0]);
		SYS_Delay(delayus);		
		
	}

	for(showidx=0; showidx<=6; showidx++)
	{
		strcpy(tmpstring, showstring);
		for(storeshowidx=0; storeshowidx<=6; storeshowidx++)
		{
			if((storeshowidx+1) <= 6)
				showstring[storeshowidx] = tmpstring[storeshowidx+1];
			else
				showstring[storeshowidx] = ' ';
		}
		//printf("%s \r\n", showstring);
		LCD_Write(&showstring[0]);
		SYS_Delay(delayus);
	}
	
}

// hour: 24-hours
void showTime(uint32_t hour, uint32_t minute)
{
	long long time;
	// show time
	time = hour * 100 + minute;

	LCD_Number(time);
	LCD_EnableSegment(3, 29);
}



/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


	 
