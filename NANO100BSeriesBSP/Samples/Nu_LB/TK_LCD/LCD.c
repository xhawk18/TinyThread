/******************************************************************************
 * @file     LCD.c
 * @brief    Nano1xx TK_LCD Driver Sample code
 * @version  1.0.1
 * @date     08, October, 2012
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

int8_t int_to_char(int32_t i)
{
	if(i==0) return '0';
	else if(i==1) return '1';
	else if(i==2) return '2';
	else if(i==3) return '3';
	else if(i==4) return '4';
	else if(i==5) return '5';
	else if(i==6) return '6';
	else if(i==7) return '7';
	else if(i==8) return '8';
	else if(i==9) return '9';
	else if(i==10) return 'A';
	else if(i==11) return 'B';
	else if(i==12) return 'C';
	else if(i==13) return 'D';
	else if(i==14) return 'E';
	else if(i==15) return 'F';
	
	return -1;
}

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

void showSlider(uint16_t resolution)
{

#if 1
	// Resolution (0 ~ 8) to LCD bar (0 ~ 11)
	
	if(resolution > 8)
		printf("Touch key data over the resolution..\n");

	LCD_DisableSegment(3, 36);	// PL
	LCD_DisableSegment(2, 36);	// P0
	LCD_DisableSegment(1, 36);	// P1
	LCD_DisableSegment(0, 36);	// P2
	LCD_DisableSegment(0, 37);	// P3
	LCD_DisableSegment(1, 37);	// P4
	LCD_DisableSegment(2, 37);	// P5
	LCD_DisableSegment(3, 37);	// P6
	LCD_DisableSegment(3, 38);	// P7
	LCD_DisableSegment(2, 38);	// P8
	LCD_DisableSegment(1, 38);	// P9
	LCD_DisableSegment(0, 38);	// PR
	

	if(resolution==0) {
		ControlSegment(true, 3, 36);	// PL
		ControlSegment(true, 2, 36);	// P0
	}
	else if(resolution==1)
		ControlSegment(true, 1, 36);	// P1
	else if(resolution==2)
		ControlSegment(true, 0, 36);	// P2
	else if(resolution==3)
		ControlSegment(true, 0, 37);	// P3
	else if(resolution==4) {
		ControlSegment(true, 1, 37);	// P4
		ControlSegment(true, 2, 37);	// P5
	}
	else if(resolution==5)
		ControlSegment(true, 3, 37);	// P6
	else if(resolution==6)
		ControlSegment(true, 3, 38);	// P7
	else if(resolution==7)
		ControlSegment(true, 2, 38);	// P8
	else if(resolution==8) {
		ControlSegment(true, 1, 38);	// P9
		ControlSegment(true, 0, 38);	// PR
	}
#else
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

	LCD_DisableSegment(3, 36);	// PL
	LCD_DisableSegment(2, 36);	// P0
	LCD_DisableSegment(1, 36);	// P1
	LCD_DisableSegment(0, 36);	// P2
	LCD_DisableSegment(0, 37);	// P3
	LCD_DisableSegment(1, 37);	// P4
	LCD_DisableSegment(2, 37);	// P5
	LCD_DisableSegment(3, 37);	// P6
	LCD_DisableSegment(3, 38);	// P7
	LCD_DisableSegment(2, 38);	// P8
	LCD_DisableSegment(1, 38);	// P9
	LCD_DisableSegment(0, 38);	// PR
	

	if(lightpoint==0)
		ControlSegment(true, 3, 36);	// PL
	else if(lightpoint==1)
		ControlSegment(true, 2, 36);	// P0
	else if(lightpoint==2)
		ControlSegment(true, 1, 36);	// P1
	else if(lightpoint==3)
		ControlSegment(true, 0, 36);	// P2
	else if(lightpoint==4)
		ControlSegment(true, 0, 37);	// P3
	else if(lightpoint==5)
		ControlSegment(true, 1, 37);	// P4
	else if(lightpoint==6)
		ControlSegment(true, 2, 37);	// P5
	else if(lightpoint==7)
		ControlSegment(true, 3, 37);	// P6
	else if(lightpoint==8)
		ControlSegment(true, 3, 38);	// P7
	else if(lightpoint==9)
		ControlSegment(true, 2, 38);	// P8
	else if(lightpoint==10)
		ControlSegment(true, 1, 38);	// P9
	else if(lightpoint==11 || lightpoint==12)
		ControlSegment(true, 0, 38);	// PR
#endif	
	
}

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
	char timestr[5];
	
	// show time
	timestr[0] = int_to_char(hour/10);
	timestr[1] = int_to_char(hour%10);
	timestr[2] = int_to_char(minute/10);
	timestr[3] = int_to_char(minute%10);

	LCD_Write(&timestr[0]);
	LCD_EnableSegment(3, 18);
}


// show current
void showCurrent(double c)
{
	char current[5] = "";
	char A[3] = "";
	char mA[3] = "mA";
	char uA[3] = "uA";
	int8_t dot_flag = 0;

	A[2] = '\0';
	if((c<100000) && (c>=1000))				// mA
	{
		if((c/10000) >= 1)
		{
			A[0] = int_to_char((int32_t)(c/10000));
			A[1] = int_to_char((int32_t)((int32_t)(c/1000)%10));
			dot_flag = 0;
		}
		else
		{
			A[0] = int_to_char((int32_t)(c/1000));
			A[1] = int_to_char((int32_t)((int32_t)(c/100)%10));
			dot_flag = 1;
		}
		strcat(current, A);
		strcat(current, mA);
	}
	else if(c < 100)			// uA
	{
		if((c/10) >= 1)
		{
			A[0] = int_to_char((int32_t)(c/10));
			A[1] = int_to_char((int32_t)((int32_t)c%10));
			dot_flag = 0;
		}
		else
		{
			A[0] = int_to_char((int32_t)(c));
			A[1] = '0';
			dot_flag = 1;
		}
		strcat(current, A);
		strcat(current, uA);
	}
	else
	{
		A[0] = '*';
		A[1] = '*';
		dot_flag = 0;
		strcat(current, A);
		strcat(current, uA);
		printf("Can not show it correctly \n");
	}

	if(dot_flag==0)
		printf("current %s \n", current);
	else
		printf("current %c.%c%c%c \n", current[0], current[1], current[2], current[3]);
	
	LCD_Write(&current[0]);
	if(dot_flag)
		LCD_EnableSegment(0, 14);
	else
		LCD_DisableSegment(0, 14);
}


// show voltage
void showVoltage(double temp)
{
	long long num;

	//printf("temp = %f \n", temp);

	num = (long long)(temp * 10);

	//printf("num = %lld \n", num);

	LCD_Number(num);
	LCD_Symbol(LCD_SYMBOL_11DP, 1);
}

void showText(uint32_t hex)
{
	char show[5] = "";

#if 0
	show[3] = int_to_char ( hex&0xF );
	show[2] = int_to_char ( (hex&0xF0)>>4 );
	show[1] = int_to_char ( (hex&0xF00)>>8 );
	show[0] = int_to_char ( (hex&0xF000)>>12 );
#else
	show[3] = int_to_char ( (hex%10) );
	show[2] = int_to_char ( ((hex%100)/10) );
	show[1] = int_to_char ( ((hex%1000)/100) );
	show[0] = int_to_char ( (hex/1000) );
#endif
	LCD_Write( &show[0] );
	
}

#if 0
void FUNCpinsToGPIOpins(void)
{

	// SEG 0~39
	GCR->PD_H_MFP_BITS.PD15 = 0;
	GCR->PD_H_MFP_BITS.PD14 = 0;
	GCR->PD_L_MFP_BITS.PD7 = 0;
 	GCR->PD_L_MFP_BITS.PD6 = 0;
 	GCR->PB_L_MFP_BITS.PB3 = 0;
  	GCR->PB_L_MFP_BITS.PB2 = 0;
 	GCR->PB_L_MFP_BITS.PB1 = 0;
 	GCR->PB_L_MFP_BITS.PB0 = 0;
  	GCR->PE_L_MFP_BITS.PE7 = 0;
 	GCR->PE_H_MFP_BITS.PE8 = 0;
 	GCR->PB_L_MFP_BITS.PB7 = 0;
  	GCR->PB_L_MFP_BITS.PB6 = 0;
 	GCR->PB_L_MFP_BITS.PB5 = 0;
 	GCR->PB_L_MFP_BITS.PB4 = 0;
 	GCR->PD_H_MFP_BITS.PD13 = 0;
 	GCR->PD_H_MFP_BITS.PD12 = 0;
 	GCR->PD_H_MFP_BITS.PD11 = 0;
  	GCR->PD_H_MFP_BITS.PD10 = 0;
 	GCR->PD_H_MFP_BITS.PD9 = 0;
 	GCR->PD_H_MFP_BITS.PD8 = 0;
 	//GCR->PA_H_MFP_BITS.PA8 = 0;
 	//GCR->PA_H_MFP_BITS.PA9 = 0;
 	GCR->PA_H_MFP_BITS.PA10 = 0;
 	GCR->PA_H_MFP_BITS.PA11 = 0;
 	GCR->PB_H_MFP_BITS.PB12 = 0;
 	GCR->PB_H_MFP_BITS.PB13 = 0;
 	GCR->PB_H_MFP_BITS.PB14 = 0;
 	GCR->PE_H_MFP_BITS.PE13 = 0;
 	GCR->PE_H_MFP_BITS.PE14 = 0;
 	GCR->PE_H_MFP_BITS.PE15 = 0;
  	GCR->PB_H_MFP_BITS.PB8 = 0;
  	//GCR->PB_H_MFP_BITS.PB15 = 0;
  	GCR->PC_H_MFP_BITS.PC14 = 0;
  	GCR->PC_H_MFP_BITS.PC15 = 0;
  	GCR->PD_L_MFP_BITS.PD5 = 0;
  	GCR->PD_L_MFP_BITS.PD4 = 0;
  	GCR->PA_L_MFP_BITS.PA7 = 0;
  	GCR->PA_L_MFP_BITS.PA6 = 0;
  	GCR->PA_L_MFP_BITS.PA5 = 0;
  	GCR->PA_L_MFP_BITS.PA4 = 0;

	// disable charge pump
	//LCD->DISPCTL_BITS.CPUMP_EN = 0;
	// V1
	//GCR->PB_H_MFP_BITS.PB9 = 0;
	// V2
	//GCR->PB_H_MFP_BITS.PB10 = 0;
	// V3
	//GCR->PB_H_MFP_BITS.PB11 = 0;
	// DH1
	//GCR->PC_L_MFP_BITS.PC0 = 0;
	// DH2
	//GCR->PC_L_MFP_BITS.PC1 = 0;
	// COM0
	GCR->PC_L_MFP_BITS.PC2 = 0;
	// COM1
	GCR->PC_L_MFP_BITS.PC3 = 0;
	// COM2
	GCR->PC_L_MFP_BITS.PC4 = 0;
	// COM3
	GCR->PC_L_MFP_BITS.PC5 = 0;

	/* for ICE pins */
	GCR->PF_L_MFP &= ~0xFF;	  // ICE pin
	GPIOF->OFFD |= 0x00030000;

#if 0	
	printf("PA_L_MFP = 0x%x \n", GCR->PA_L_MFP);
	printf("PA_H_MFP = 0x%x \n", GCR->PA_H_MFP);
	printf("PB_L_MFP = 0x%x \n", GCR->PB_L_MFP);
	printf("PB_H_MFP = 0x%x \n", GCR->PB_H_MFP);
	printf("PC_L_MFP = 0x%x \n", GCR->PC_L_MFP);
	printf("PC_H_MFP = 0x%x \n", GCR->PC_H_MFP);
	printf("PD_L_MFP = 0x%x \n", GCR->PD_L_MFP);
	printf("PD_H_MFP = 0x%x \n", GCR->PD_H_MFP);
	printf("PE_L_MFP = 0x%x \n", GCR->PE_L_MFP);
	printf("PE_H_MFP = 0x%x \n", GCR->PE_H_MFP);
	printf("PF_L_MFP = 0x%x \n", GCR->PF_L_MFP);
	printf("*****************************************\n");
#endif

	/* UART pins to GPIO */
	GCR->PA_H_MFP_BITS.PA14 = 0;
	GCR->PA_H_MFP_BITS.PA15 = 0;
	
	/* pwm pins to gpio */
	GCR->PC_H_MFP_BITS.PC12 = 0;

	/* pc[13] set output high */
	GPIOC->PMD_BITS.PMD13 = 1;		// set output mode
	GPIOC->DOUT |= (1<<13);

	/* pd[4] set output low */
	GPIOD->PMD_BITS.PMD4 = 1;		// set output mode
	GPIOD->DOUT &= ~(1<<4);
	
}

void GPIOpinsToFUNCpins(void)
{
	// SEG 0~39
	GCR->PD_H_MFP_BITS.PD15 = 7;
	GCR->PD_H_MFP_BITS.PD14 = 7;
	GCR->PD_L_MFP_BITS.PD7 = 7;
 	GCR->PD_L_MFP_BITS.PD6 = 7;
 	//GCR->PB_L_MFP_BITS.PB3 = 7;
  	//GCR->PB_L_MFP_BITS.PB2 = 7;
 	//GCR->PB_L_MFP_BITS.PB1 = 7;
 	//GCR->PB_L_MFP_BITS.PB0 = 7;
  	GCR->PE_L_MFP_BITS.PE7 = 7;
 	GCR->PE_H_MFP_BITS.PE8 = 7;
 	GCR->PB_L_MFP_BITS.PB7 = 7;
  	GCR->PB_L_MFP_BITS.PB6 = 7;
 	//GCR->PB_L_MFP_BITS.PB5 = 7;
 	//GCR->PB_L_MFP_BITS.PB4 = 7;
 	GCR->PD_H_MFP_BITS.PD13 = 7;
 	GCR->PD_H_MFP_BITS.PD12 = 7;
 	GCR->PD_H_MFP_BITS.PD11 = 7;
  	GCR->PD_H_MFP_BITS.PD10 = 7;
 	GCR->PD_H_MFP_BITS.PD9 = 7;
 	GCR->PD_H_MFP_BITS.PD8 = 7;
 	//GCR->PA_H_MFP_BITS.PA8 = 7;
 	//GCR->PA_H_MFP_BITS.PA9 = 7;
 	GCR->PA_H_MFP_BITS.PA10 = 7;
 	GCR->PA_H_MFP_BITS.PA11 = 7;
 	GCR->PB_H_MFP_BITS.PB12 = 7;
 	GCR->PB_H_MFP_BITS.PB13 = 7;
 	//GCR->PB_H_MFP_BITS.PB14 = 7;
 	GCR->PE_H_MFP_BITS.PE13 = 7;
 	//GCR->PE_H_MFP_BITS.PE14 = 7;
 	//GCR->PE_H_MFP_BITS.PE15 = 7;
  	//GCR->PB_H_MFP_BITS.PB8 = 7;
  	//GCR->PB_H_MFP_BITS.PB15 = 7;
  	GCR->PC_H_MFP_BITS.PC14 = 7;
  	GCR->PC_H_MFP_BITS.PC15 = 7;
  	//GCR->PD_L_MFP_BITS.PD5 = 7;
  	//GCR->PD_L_MFP_BITS.PD4 = 7;
  	GCR->PA_L_MFP_BITS.PA7 = 7;
  	GCR->PA_L_MFP_BITS.PA6 = 7;
  	GCR->PA_L_MFP_BITS.PA5 = 7;
  	//GCR->PA_L_MFP_BITS.PA4 = 7;

	// disable charge pump
	//LCD->DISPCTL_BITS.CPUMP_EN = 7;
	// V1
	//GCR->PB_H_MFP_BITS.PB9 = 7;
	// V2
	//GCR->PB_H_MFP_BITS.PB10 = 7;
	// V3
	//GCR->PB_H_MFP_BITS.PB11 = 7;
	// DH1
	//GCR->PC_L_MFP_BITS.PC0 = 7;
	// DH2
	//GCR->PC_L_MFP_BITS.PC1 = 7;
	// COM0
	GCR->PC_L_MFP_BITS.PC2 = 7;
	// COM1
	GCR->PC_L_MFP_BITS.PC3 = 7;
	// COM2
	GCR->PC_L_MFP_BITS.PC4 = 7;
	// COM3
	GCR->PC_L_MFP_BITS.PC5 = 7;

	/* GPIO to UART pins */			
	GCR->PA_H_MFP_BITS.PA14	= 0x6;
	GCR->PA_H_MFP_BITS.PA15	= 0x6;

	/* pc[13] set output LOW */
	GPIOC->PMD_BITS.PMD13 = 1;		// set output mode
	GPIOC->DOUT &= ~(1<<13);

	/* for pwm pins */
	GCR->PC_H_MFP_BITS.PC12 = 2;
}
#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


	
