/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "../../../Inc/tt_thread.h"

#include "nano1xx_timer.h"

TT_WQ_T	g_wait_queue;
int		g_is_irq_detected;

void TMR0_IRQHandler(void)
{
	TIMER0->ISR = TIMER0->ISR;
	
	/* Set event so that threads waiting on this event will be waked up */
	g_is_irq_detected = 1;
	tt_wq_set_event(&g_wait_queue);
}

void init_timer(void)
{
	/* Enable clock source for timer0 */
	UNLOCKREG();
	CLK->PWRCTL |= CLK_PWRCTL_HIRC_EN;
	while (!(CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB));
	CLK->CLKSEL1 = CLK->CLKSEL1
		& ~CLK_CLKSEL1_TMR0_MASK
		| CLK_CLKSEL1_TMR0_HIRC;
	LOCKREG();
	
	/* Set up timer0 and enable it's interrupt */
	TIMER_Init(TIMER0, 10, 1200000, TIMER_CTL_MODESEL_PERIODIC);
	TIMER_EnableInt(TIMER0, TIMER_IER_TMRIE);
	TIMER_Start(TIMER0);
}	


int main()
{
	/* The clock frequency of systick may be changed by user's application.
	   Please change the value of SYSTICK_CLOCK according to real situration */
	uint32_t SysGet_HCLKFreq(void);
#define TT_SYSTICK_CLOCK		SysGet_HCLKFreq()
	/* Initialize TinyThread */
	tt_init(TT_SYSTICK_CLOCK);

	/* Initialize flag and event queue for timer interrupt */
	g_is_irq_detected = 0;
	tt_wq_init(&g_wait_queue);
	
	/* Initialize timer */
	init_timer();

	
	while(1)
	{
		int rt;
		/* Check flag and then wait interrupt */ 
		tt_disable_irq();

		while(!g_is_irq_detected)
		{
			/* tt_wq_wait_event and tt_wq_wait_event_timeout will enable irq internally */
			rt = tt_wq_wait_event_timeout(&g_wait_queue, 300);
			if(rt != 0) break;
		}
		g_is_irq_detected = 0;

		tt_enable_irq();
		
		if(rt == 0)
			printf("Interrupt detected!\n");
		else
			printf("Waiting timeout detected!\n");
	}
}


