/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "NUC1xx.h"

#if defined ( __CC_ARM   )
#if (__ARMCC_VERSION < 400000)
#else
/* Insist on keeping widthprec, to avoid X propagation by benign code in C-lib */
#pragma import _printf_widthprec
#endif
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Macro Definition                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
/* Using UART0 or UART1 */  
												   
#define DEBUG_PORT  0     	/* 0:UART0   1:UART1 */

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;


/*---------------------------------------------------------------------------------------------------------*/
/* Routine to write a char                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void SendChar(int ch)
{
	if(DEBUG_PORT == 1)
	{
	    while(UART1->FSR.TX_FULL == 1);
		UART1->DATA = ch;
	    if(ch == '\n')
	    {
	        while(UART1->FSR.TX_FULL == 1);
	        UART1->DATA = '\r';
	    }
	}
	else
	{
		while(UART0->FSR.TX_FULL == 1);
		UART0->DATA = ch;
	    if(ch == '\n')
	    {
	        while(UART0->FSR.TX_FULL == 1);
	        UART0->DATA = '\r';
	    }
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Routine to get a char                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
char GetChar()
{
	if(DEBUG_PORT == 1)
	{
		while (1){
			if(UART1->FSR.RX_EMPTY == 0 )
			{
				return (UART1->DATA);
				
		    }
		}
	}
	else
	{
		while (1){
			if(UART0->FSR.RX_EMPTY == 0 )
			{
				return (UART0->DATA);
				
		    }
		}
	}
}


/*---------------------------------------------------------------------------------------------------------*/
/* C library retargetting                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/

void _ttywrch(int ch)
{
  SendChar(ch);
  return;
}

int fputc(int ch, FILE *f)
{
  SendChar(ch);
  return 0;
}

int fgetc(FILE *f) {
  return (GetChar());
}


int ferror(FILE *f) {
  return EOF;
}
