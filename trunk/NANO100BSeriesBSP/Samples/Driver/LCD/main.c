/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx LCD Driver Sample code
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
#include "nano1xx_gpio.h"
#include "nano1xx_uart.h"
#include "nano1xx_sys.h"

#define LCD_ALPHABET_NUM		13
#define LCD_DIGIT_NUM			13

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

uint32_t sysGetNum(void)
{
    uint8_t cInputTemp=0x00, InputString[16]={0};
    uint32_t nLoop = 0;
    while(cInputTemp != 0x0D)
    {
        cInputTemp = getchar();
        if(cInputTemp == 27)
        {
            return cInputTemp;
        }
        if(cInputTemp == 'x' || cInputTemp == 'X' || cInputTemp == 'f'||
        cInputTemp == 'F' || cInputTemp == 'r' || cInputTemp == 'R')
        {
            return cInputTemp;
        }
        if(cInputTemp == '-')
        {
            InputString[nLoop] = cInputTemp;
            printf("%c",cInputTemp);
            nLoop++;
        }
        else if(cInputTemp >= '0' && cInputTemp <= '9')
        {
            InputString[nLoop] = cInputTemp;
            printf("%c",cInputTemp);
            nLoop++;
        }
    }
	return atoi((const char *)InputString);
}

static void TestItem (void)
{
    printf("\n\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|                    LCD Sample Program                     |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| [0] Alphabet Display Test                                 |\n");
    printf("| [1] Numeric Display Test                                  |\n");
	printf("| [2] Blinking Display Test                                 |\n");
	printf("| [3] Any Dot Display Test                                  |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| [4] Quit                                                  |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("Select key : ");
}

int32_t main(void)							   
{
	int32_t bLoop = true;
	uint32_t com, seg, onoff;
	uint8_t u8Item;
	char input;
	char text[LCD_DIGIT_NUM]="";
	int32_t idx = 0, blinkfreq, ret;
	long long num;
	S_LCD_INIT lcdinit;
	STR_UART_T param;	
	
	SYS_SetChipClockSrc((CLK_PWRCTL_HXT_EN | CLK_PWRCTL_LXT_EN), 1);

	// Wait HXT and LXT stable
	while(SYS_CheckChipClockSrc(CLK_CLKSTATUS_LXT_STB | CLK_CLKSTATUS_HXT_STB) != 0) ;

	/* Select UART Clock Source From 12MHz */
	SYS_SelectIPClockSource_1(CLK_CLKSEL1_UART_MASK, CLK_CLKSEL1_UART_HXT);

	MFP_UART0_TO_PORTA();
	
    param.u32BaudRate         = 115200;
    param.u32cDataBits        = DRVUART_DATABITS_8;
    param.u32cStopBits        = DRVUART_STOPBITS_1;
    param.u32cParity          = DRVUART_PARITY_NONE;
    param.u32cRxTriggerLevel  = DRVUART_FIFO_1BYTES;
    param.u8TimeOut        	  = 0;
    UART_Init(UART0, &param);

	/* Select LCD Clock Source From 32KHz */
	SYS_SelectIPClockSource_1(CLK_CLKSEL1_LCD_MASK, CLK_CLKSEL1_LCD_LXT);

	/* Select LCD Clock Source From 10KHz */
	//SYS_SelectIPClockSource_1(CLK_CLKSEL1_LCD_MASK, CLK_CLKSEL1_LCD_LIRC);

	/* Select LCD COMs, SEGs, V1 ~ V3, DH1, DH2 */
	MFP_LCD_TYPEA();
	
	/* LCD Initialize */
	lcdinit.cpump_enable = true;
	lcdinit.internal_bias = false;
	lcdinit.cpump_freqdiv = LCD_CPUMP_DIV1;
	lcdinit.cpump_voltage = LCD_CPVOl_3V;
	lcdinit.bias = LCD_BIAS_THIRD;
	lcdinit.mux = LCD_MUX_ONE_FOURTH;
	lcdinit.freqdiv = LCD_FREQ_DIV64;
	LCD_Init(&lcdinit);

	while(bLoop)
	{
		idx = 0;
		strcpy(text, "");	// clear buffer
			
		TestItem();
		u8Item = getchar();
		printf("%c\n", u8Item);
		
		switch(u8Item)
		{
			case '0':
			{					
				printf("Input text: ");
				while(1)
				{
					input = getchar();
					printf("%c", input);
					if(input == 0xD) break;
					strcat( text, &input);
					idx++;
					if(idx >= LCD_ALPHABET_NUM) break;
				}
				printf("\n");
				printf("%s \n", text);

				LCD_Write(text);						

				break;
			}
			case '1':
			{
				printf("Input number: ");
				while(1)
				{
					input = getchar();
					printf("%c", input);
					if(input == 0xD) break;
					strcat( text, &input);
					idx++;
					if(idx >= LCD_DIGIT_NUM) break;
				}
				printf("\n");

				//num = atof(text);
				num = local_atoi(text);

				LCD_Number(num);						
								
				break;				
			}
			case '2':
			{
				//DrvLCD_Write("NUVOTON");
				LCD_Number(8888888888888);
input:					
				printf("Input the frequency of blinking (ms): ");
				blinkfreq = sysGetNum();
				printf("\n");
				ret = LCD_BlinkFrequency(blinkfreq);
				if(ret == ERR_LCD_CAL_BLINK_FAIL)
				{
					printf("Over the time range and input again...\n");
					goto input;
				}
				LCD_EnableInt(LCD_FRAMECOUNT_INT);
				LCD_EnableBlink();

				printf("Any key to end Blinking display...");
				getchar();
				LCD_DisableBlink();
				break;
			}
			case '3':
				printf("Pixel On/Off (1:On, 0:Off): ");
				onoff = sysGetNum();
				printf("\n");
				if(onoff>1) continue;

				printf("Input Com: ");
				com = sysGetNum();

				printf("\nInput Segment: ");
				seg = sysGetNum();

				if(onoff)
					LCD_EnableSegment(com, seg);
				else
					LCD_DisableSegment(com, seg);
				break;
			case '4':
				bLoop = false;
				break;
			default:
				printf("Wrong Item\n");
				break;			
		}
			
	}	

	LCD_Disable();
		
	return true;

}


#ifdef USE_ASSERT
/**
  * @brief  The function prints the source file name and line number where the assert_param() error
  *         occurs, and then stops in an infinite loop. User can add his own codes here if necessary.
  * @param[in] file Source file name
  * @param[in] line Line number
  * @return None
  */
void assert_error(uint8_t * file, uint32_t line)
{
 	GCR->PB_L_MFP = (GCR->PB_L_MFP & ~0x77) | (PB1_MFP_UART0_TX | PB0_MFP_UART0_RX);  /* Select multi-function pin for UART0 */
 	CLK->APBCLK |= CLK_APBCLK_UART0_EN;    /* Enable UART0 clock */
	CLK->CLKDIV0 &= ~CLK_CLKDIV0_UART_MASK;
	CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UART_MASK) | CLK_CLKSEL1_UART_HXT;  /* Select 12 Mhz XTAL */

 
    /* Set UART to 115200,n,8,1,none */
    UART0->BAUD = 0x67;             /* Baud Rate:115200 for 12MHz */
    UART0->TLCTL = 0x03;            /* Word len is 8 bits */          
	
	printf("[%s] line %d : wrong parameters.\r\n", file, line);

	/* Infinite loop */
	while(1) ;

}
#endif


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


	 
