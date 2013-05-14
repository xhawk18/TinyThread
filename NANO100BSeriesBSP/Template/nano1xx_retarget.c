/******************************************************************************
 * @file     nano1xx_retarget.c
 * @brief    This file defines I/O routines for debug console
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx.h"

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

//#define DEBUG_ENABLE_SEMIHOST   /* To enable semihosted. !!!The SEMIHOSTED of startup_nano1xx.s must be {TRUE} */

/* Using UART0 or UART1 */  
#define DEBUG_PORT   0 		    /*0:UART0  1:UART1  */

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#if !(defined(__ICCARM__) && (__VER__ >= 6010000))
struct __FILE { int handle; /* Add whatever you need here */ };
#endif
FILE __stdout;
FILE __stdin;

/*---------------------------------------------------------------------------------------------------------*/
/* Routine to write a char                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/

#if defined(DEBUG_ENABLE_SEMIHOST)
/* The static buffer is used to speed up the semihost */
static char g_buf[16];
static char g_buf_len = 0;

/* The function to process semihosted command */
extern int SH_DoCommand(int n32In_R0, int n32In_R1, int *pn32Out_R0);
#endif


void SendChar_ToUART(int ch)
{
	if(DEBUG_PORT == 1)
	{
	    while(UART1->FSR & UART_FSR_TX_FULL_F);
		UART1->THR = ch;
	    if(ch == '\n'){
	        while(UART1->FSR & UART_FSR_TX_FULL_F);
	        UART1->THR = '\r';
	    }
	}
	else if(DEBUG_PORT == 0)
	{
		while(UART0->FSR & UART_FSR_TX_FULL_F);
		UART0->THR = ch;
	    if(ch == '\n'){
	        while(UART0->FSR & UART_FSR_TX_FULL_F);
	        UART0->THR = '\r';
	    }
	}
	else 
		;
}


void SendChar(int ch)
{
#if defined(DEBUG_ENABLE_SEMIHOST)
	g_buf[g_buf_len++] = ch;
	g_buf[g_buf_len] = '\0';
	if(g_buf_len + 1 >= sizeof(g_buf) || ch == '\n' || ch == '\0')
	{

		/* Send the char */
		if(SH_DoCommand(0x04, (int)g_buf, NULL) != 0)
        {
            g_buf_len = 0;
			return;
        }
        else
        {
            int i;

            for(i=0;i<g_buf_len;i++)
                SendChar_ToUART(g_buf[i]);
		    g_buf_len = 0;
        }
	}
#else
    SendChar_ToUART(ch);
#endif
}


/*---------------------------------------------------------------------------------------------------------*/
/* Routine to get a char                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
char GetChar(void)
{
#if defined(DEBUG_ENABLE_SEMIHOST) && !defined(__ICCARM__) /* IAR don't support semihosted getchar */
	int nRet;
	while(SH_DoCommand(0x101, 0, &nRet) != 0)
	{
		if(nRet != 0)
		{
			SH_DoCommand(0x07, 0, &nRet);
			return (char)nRet;
		}
	}
#endif

	if(DEBUG_PORT == 1)
	{
		while (1){
			if((UART1->FSR & UART_FSR_RX_EMPTY_F) == 0 )
			{
				return (UART1->RBR);
				
		    }
		}
	}
	else if(DEBUG_PORT == 0)
	{
		while (1){
			if((UART0->FSR & UART_FSR_RX_EMPTY_F) == 0 )
			{
				return (UART0->RBR);
				
		    }
		}
	}
	else 
		return NULL;

}

/*---------------------------------------------------------------------------------------------------------*/
/* Check if got any char                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
int kbhit(void)
{
#if(DEBUG_PORT == 0)
    if (UART0->FSR & UART_FSR_RX_EMPTY_F)
	    return 0;
	else
		return 1;
#elif(DEBUG_PORT == 1)
    if (UART1->FSR & UART_FSR_RX_EMPTY_F)
	    return 0;
	else
		return 1;
#else
    if (UART2->FSR & UART_FSR_RX_EMPTY_F)
	    return 0;
	else
		return 1;
#endif    
}
/*---------------------------------------------------------------------------------------------------------*/
/* Check if message finished (FIFO empty of debug port)                                                    */
/*---------------------------------------------------------------------------------------------------------*/
int IsDebugFifoEmpty(void)
{
#if(DEBUG_PORT == 0)
    if (UART0->FSR & UART_FSR_TE_F)
	   return 1;
	else
		return 0;
#elif(DEBUG_PORT == 1)
    if (UART1->FSR & UART_FSR_TE_F)
	   return 1;
	else
	   return 0;
#else
    if (UART2->FSR & UART_FSR_TE_F)
	   return 1;
	else
	   return 0;
#endif    
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
  return ch;
}

int fgetc(FILE *f) {
  return (GetChar());
}


int ferror(FILE *f) {
  return EOF;
}

#ifdef DEBUG_ENABLE_SEMIHOST 
# ifdef __ICCARM__
void __exit(int return_code) {

    /* Check if link with ICE */
	if(SH_DoCommand(0x18, 0x20026, NULL) == 0)
    {
        /* Make sure all message is print out */
        while(IsDebugFifoEmpty() == 0);
    }
label:  goto label;  /* endless loop */
}
# else
void _sys_exit(int return_code) {

    /* Check if link with ICE */
	if(SH_DoCommand(0x18, 0x20026, NULL) == 0)
    {
        /* Make sure all message is print out */
        while(IsDebugFifoEmpty() == 0);
    }
label:  goto label;  /* endless loop */
}
# endif
#endif


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


