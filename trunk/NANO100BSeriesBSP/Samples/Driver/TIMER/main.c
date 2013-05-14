/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx TIMER Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_timer.h"
#include "nano1xx_assert.h"


static void TestItem (void)
{
        printf("\n\n");
        printf("+-----------------------------------------------------------+\n");
        printf("|               Timer Sample Program                        |\n");
        printf("+-----------------------------------------------------------+\n");
        printf("| Timer 0/1/2/3 Frequency test                        - [1] |\n");
        printf("| WDT Callback                                        - [2] |\n");
        printf("| Timer Counter Mode                                  - [3] |\n");
        printf("| WWDT Test                                           - [4] |\n");
        printf("+-----------------------------------------------------------+\n");
        printf("| Quit                                        	    - [ESC] |\n");
        printf("+-----------------------------------------------------------+\n");
        printf("Please select key (1 ~ 4): ");
}


uint32_t __IO u32Timer0Cnt=0, u32Timer1Cnt=0, u32Timer2Cnt=0, u32Timer3Cnt=0;
uint8_t __IO b8WDTINT = FALSE;

int volatile wwdt_cnt = 0;
uint32_t u32CounterINT = 0;


void CounterGenerator(uint32_t pin, uint32_t u32Counters)
{
        while (u32Counters--) {
				GPIOC->DOUT |= (1 << pin);
				GPIOC->DOUT &= ~(1 << pin);
        }
}

void Func_ExternalTimerCounter(void)
{
        uint8_t i, j;
        uint32_t u32Boundary;
        uint32_t u32CounterValue[] = {5, 10, 20, 40};
        TIMER_TypeDef *timer[] = {TIMER0, TIMER1, TIMER2, TIMER3};

        printf("Please connect GPC.00 --> Timer0.\n");
        printf("       connect GPC.01 --> Timer1.\n");
        printf("       connect GPC.02 --> Timer2.\n");
        printf("       connect GPC.03 --> Timer3.\n");
        printf("Press any key to continue ...\n");
        getchar();


        GPIOC->PMD &= 0x1111;
		GPIOC->DOUT &= ~(0xF);

		GCR->PB_H_MFP = (GCR->PB_H_MFP & ~(PB8_MFP_MASK | PB9_MFP_MASK | PB10_MFP_MASK | PB11_MFP_MASK)) |
						PB8_MFP_TMR0_EXT | PB9_MFP_TMR1_EXT | PB10_MFP_TMR2_EXT | PB11_MFP_TMR3_EXT;


        printf("\n>>> Periodic mode <<<\n");
        for (i=0; i<4; i++) {
                for (j=0; j<4; j++) {
                        u32Boundary = u32CounterValue[j];

                        TIMER_Init(timer[i], 0, u32Boundary, TIMER_CTL_MODESEL_PERIODIC);
                        TIMER_EnableExternalEventMode(timer[i], TIMER_CTL_EVNTEDGE_F);
                        TIMER_EnableInt(timer[i], TIMER_IER_TMRIE);
                        TIMER_Start(timer[i]);

                        while (TIMER_GetCounter(timer[i]) != 0);

                        u32CounterINT = 0;
                        CounterGenerator(i, u32Boundary);

                        CounterGenerator(i, u32Boundary);

                        CounterGenerator(i, u32Boundary);

                        while(1) {
                                if ((u32CounterINT == 3) && (TIMER_GetCounter(timer[i]) == 0)) {
                                        printf("Timer[%d] -- Counter = %d\n", i, u32Boundary);
                                        break;
                                }
                        }
                        TIMER_DeInit(timer[i]);
                }
        }

        printf("\n");
        printf("Case 3 Finished \n");
}


int main (void)
{
        int8_t item;

        printf("\n\n TIMER Sample Code \n");

		CLK->PWRCTL |= CLK_PWRCTL_LIRC_EN;	// for WDT
		while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_LIRC_STB));

		UNLOCKREG();

        do {
                TestItem();
                item = getchar();
                printf("%c\n",item);

                switch (item) {
                case '1': {
                        uint32_t i;
                        uint32_t begin0 = 0;

                        printf("[TIMER Frequency Test]\n");
                        printf(" This sample code will use four timer and set four different settings as below\n");
                        printf(" TIMER0: 1 ticks/sec\n");
                        printf(" TIMER1: 2 ticks/sec\n");
                        printf(" TIMER2: 3 ticks/sec\n");
                        printf(" TIMER3: 4 ticks/sec\n");
                        printf(" Check their multiple relation \n\n");

                        /* Using TIMER0 in PERIODIC_MODE */
                        // Timer clk = (12MHz / (prescale + 1 = 12)) = 1MHz
                        TIMER_Init(TIMER0, 11, 1000000, TIMER_CTL_MODESEL_PERIODIC);
                        TIMER_Init(TIMER1, 11, 500000, TIMER_CTL_MODESEL_PERIODIC);
                        TIMER_Init(TIMER2, 11, 250000, TIMER_CTL_MODESEL_PERIODIC);
                        TIMER_Init(TIMER3, 11, 125000, TIMER_CTL_MODESEL_PERIODIC);

                        /* Enable TIMER0 Intettupt */
                        TIMER_EnableInt(TIMER0, TIMER_IER_TMRIE);
                        TIMER_EnableInt(TIMER1, TIMER_IER_TMRIE);
                        TIMER_EnableInt(TIMER2, TIMER_IER_TMRIE);
                        TIMER_EnableInt(TIMER3, TIMER_IER_TMRIE);

                        /* Start counting */
                        u32Timer0Cnt = u32Timer1Cnt = u32Timer2Cnt = u32Timer3Cnt = 0;
                        TIMER_Start(TIMER0);
                        TIMER_Start(TIMER1);
                        TIMER_Start(TIMER2);
                        TIMER_Start(TIMER3);

                        for (i=0; i<10; i++) {
                                begin0 = u32Timer0Cnt;
                                //printf("T0:%02d,T1:%02d,T2:%02d,T3:%02d\n", u32Timer0Cnt, u32Timer1Cnt, u32Timer2Cnt, u32Timer3Cnt);
                                printf("T0_Tick: %02d, \tT1_Tick: %02d, \tT2_Tick: %02d, \tT3_Tick: %02d\n",\
                                       u32Timer0Cnt, u32Timer1Cnt, u32Timer2Cnt, u32Timer3Cnt);
                                while (begin0 == u32Timer0Cnt);
                        }

                        if ((u32Timer0Cnt*2 == u32Timer1Cnt) && (u32Timer0Cnt*4 == u32Timer2Cnt) && (u32Timer0Cnt*8 == u32Timer3Cnt))
                                printf("  >> Frequency OK.\n");
                        else
                                printf("  >> Frequency Error.\n");


                        /* De-init TIMER */
                        TIMER_DeInit(TIMER0);
                        TIMER_DeInit(TIMER1);
                        TIMER_DeInit(TIMER2);
                        TIMER_DeInit(TIMER3);

                        printf("Case 1 Finished \n");
                        break;
                }

                case '2': {
                        printf("\n[WDT Callback Function Test]\n");
                        printf(" The sample code will set WDT interval as 2^14.\n");
                        printf(" Wait about 2 seconds to exit WDT test function.\n\n");

                        b8WDTINT = FALSE;
                        WDT_Init(WDT_CTL_WTIS_2POW14, WDT_CTL_WTRDSEL_1026CLK);
                        WDT_EnableInt();
                        WDT_START;

                        printf("Wait WDT interrupt happened ...\n");
                        while (1) {
                                if ( b8WDTINT )
                                        break;
                        }
                        WDT_DeInit();
                        printf("Case 2 Finished.\n");
                        break;
                }

                case '3': {
                        printf("\n[TIMER Counter mode Test]\n");
                        printf(" This sample code will test the TIMER0/1/2/3/4 form external counter in\n");
                        printf(" One Shot Mode, and Periodic Mode.\n\n");

                        Func_ExternalTimerCounter();
                        break;
                }

                case '4': {
                        printf("\n[WWDT Test ]\n");
                        printf(" Callback function reload 4 times, then force reset by writing RLD at wrong time\n");

                        WWDT_Init(WWDT_CR_PERIODSEL_512, 0x20);
                        WWDT_EnableInterrupt();
                        while(1) {
                                if(wwdt_cnt >= 4) {
                                        WWDT->RLD = WWDT_RELOAD_KEY;
                                }
                        }
                }


                default: {
                        break;
                }
                }
        } while (item != 27);

        printf("\nExit TIMER Sample Code ...\n");

        return 0;
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

