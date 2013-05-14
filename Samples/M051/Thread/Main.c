/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "M051Series.h"
#include "../../../Inc/tt_thread.h"


#define LED0_GPIO_GRP	P3
#define LED0_GPIO_BIT	4
#define LED1_GPIO_GRP	P3
#define LED1_GPIO_BIT	5
#define LED2_GPIO_GRP	P3
#define LED2_GPIO_BIT	6
#define LED3_GPIO_GRP	P3
#define LED3_GPIO_BIT	7




#define THREAD_NUM	3
typedef struct
{
	GPIO_T		*gpio_group;
	int			gpio_bit;
	TT_SEM_T	*join_sem;
} THREAD_ARG_T;

static TT_SEM_T		join_sem[THREAD_NUM];
static TT_THREAD_T	*thread[THREAD_NUM];
static unsigned int	stack[THREAD_NUM][TT_THREAD_BUFFER_SIZE(256)/sizeof(unsigned int)];
static THREAD_ARG_T	arg[THREAD_NUM];


void thread_entry(void *arg)
{
	THREAD_ARG_T *the_arg = (THREAD_ARG_T *)arg;

	int start_ticks = tt_get_ticks ();
#define THREAD_RUN_MSEC 3000
	while (1)
	{
		int cur_ticks = tt_get_ticks ();
		if (tt_ticks_to_msec (cur_ticks - start_ticks) >= THREAD_RUN_MSEC)
			break;

		if(cur_ticks % 64 < 32)
			the_arg->gpio_group->DOUT &= ~(1 << the_arg->gpio_bit);	/* Turn off LED */
		else
			the_arg->gpio_group->DOUT |= (1 << the_arg->gpio_bit);	/* Turn on LED */
	}

	tt_sem_up (the_arg->join_sem);
}


int main()
{
	int i;

	/* The clock frequency of systick may be changed by user's application.
	   Please change the value of SYSTICK_CLOCK according to real situration */
#define TT_SYSTICK_CLOCK		22118400
	/* Initialize TinyThread */
	tt_init(TT_SYSTICK_CLOCK);
	
	/* Open LED GPIO for testing */
	_GPIO_SET_PIN_MODE(LED0_GPIO_GRP, LED0_GPIO_BIT, GPIO_PMD_OUTPUT);
	_GPIO_SET_PIN_MODE(LED1_GPIO_GRP, LED1_GPIO_BIT, GPIO_PMD_OUTPUT);
	_GPIO_SET_PIN_MODE(LED2_GPIO_GRP, LED2_GPIO_BIT, GPIO_PMD_OUTPUT);
	_GPIO_SET_PIN_MODE(LED3_GPIO_GRP, LED3_GPIO_BIT, GPIO_PMD_OUTPUT);

	/* Create and run thread */
	arg[0].gpio_group = LED0_GPIO_GRP;
	arg[1].gpio_group = LED1_GPIO_GRP;
	arg[2].gpio_group = LED2_GPIO_GRP;
	arg[0].gpio_bit = LED0_GPIO_BIT;
	arg[1].gpio_bit = LED1_GPIO_BIT;
	arg[2].gpio_bit = LED2_GPIO_BIT;
	for (i = 0; i < THREAD_NUM; ++i)
	{
		tt_sem_init (&join_sem[i], 0);
		arg[i].join_sem = &join_sem[i];

		thread[i] = tt_thread_create("thread",			/* thread Name */
									 0,					/* thread priority */
									 stack[i],			/* stack pointer */
									 sizeof(stack[i]),	/* stack size */
									 thread_entry,		/* thread entry function */
									 (void *)&arg[i]	/* argument for thread entry function */
									 );	
	}
	
	
	/* Join threads.
	 * TinyThread does not support build-in tt_thread_join() 
	 * since it is easy to emulate by a semaphore.
	 */
	for (i = 0; i < THREAD_NUM; ++i)
		tt_sem_down (&join_sem[i]);


	/* Set LED to indicate that run to here */
	LED0_GPIO_GRP->DOUT &= ~(1 << LED0_GPIO_BIT);		/* OFF */
	LED1_GPIO_GRP->DOUT &= ~(1 << LED1_GPIO_BIT);		/* OFF */
	LED2_GPIO_GRP->DOUT &= ~(1 << LED2_GPIO_BIT);		/* OFF */
	LED3_GPIO_GRP->DOUT |= (1 << LED3_GPIO_BIT);	/* ON */

	while(1);
}


