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
	    while(UART1->FSR.TX_FULL == 1);
		UART1->DATA = ch;
	    if(ch == '\n'){
	        while(UART1->FSR.TX_FULL == 1);
	        UART1->DATA = '\r';
	    }
	}
	else if(DEBUG_PORT == 0)
	{
		while(UART0->FSR.TX_FULL == 1);
		UART0->DATA = ch;
	    if(ch == '\n'){
	        while(UART0->FSR.TX_FULL == 1);
	        UART0->DATA = '\r';
	    }
	}
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
			if(UART1->FSR.RX_EMPTY == 0 )
			{
				return (UART1->DATA);
				
		    }
		}
	}
	else if(DEBUG_PORT == 0)
	{
		while (1){
			if(UART0->FSR.RX_EMPTY == 0 )
			{
				return (UART0->DATA);
				
		    }
		}
	}
	else 
	{
		while (1){
			if(UART2->FSR.RX_EMPTY == 0 )
			{
				return (UART2->DATA);
				
		    }
		}
	}

}

/*---------------------------------------------------------------------------------------------------------*/
/* Check if got any char                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
int kbhit(void)
{
#if(DEBUG_PORT == 0)
    return !UART0->FSR.RX_EMPTY;
#elif(DEBUG_PORT == 1)
    return !UART1->FSR.RX_EMPTY;
#else
    return !UART2->FSR.RX_EMPTY;
#endif    
}
/*---------------------------------------------------------------------------------------------------------*/
/* Check if message finished (FIFO empty of debug port)                                                    */
/*---------------------------------------------------------------------------------------------------------*/
int IsDebugFifoEmpty(void)
{
#if(DEBUG_PORT == 0)
    return UART0->FSR.TE_FLAG;
#elif(DEBUG_PORT == 1)
    return UART1->FSR.TE_FLAG;
#else
    return UART2->FSR.TE_FLAG;
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

#ifdef ferror
#	undef ferror
#endif
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


