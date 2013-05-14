/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "../../../Inc/tt_thread.h"

#include "Driver/DrvGPIO.h"

#define LED_GPIO_ICE_GRP	E_GPB
#define LED_GPIO_ICE_BIT	4
#define LED_GPIO_ISP_GRP	E_GPB
#define LED_GPIO_ISP_BIT	5
#define LED_GPIO_RED_GRP	E_GPB
#define LED_GPIO_RED_BIT	6
#define LED_GPIO_GREEN_GRP	E_GPB
#define LED_GPIO_GREEN_BIT	7




#define THREAD_NUM	3
typedef struct
{
	E_DRVGPIO_PORT	gpio_group;
	int				gpio_bit;
	TT_SEM_T		*join_sem;
} THREAD_ARG_T;

static TT_SEM_T		join_sem[THREAD_NUM];
static TT_THREAD_T	*thread[THREAD_NUM];
static unsigned int	stack[THREAD_NUM][TT_THREAD_BUFFER_SIZE(64)/sizeof(unsigned int)];
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
			DrvGPIO_SetBit(the_arg->gpio_group, the_arg->gpio_bit);	/* Turn off LED */
		else
			DrvGPIO_ClrBit(the_arg->gpio_group, the_arg->gpio_bit);	/* Turn on LED */
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
	DrvGPIO_Open(LED_GPIO_GREEN_GRP, LED_GPIO_GREEN_BIT, E_IO_OUTPUT);
	DrvGPIO_Open(LED_GPIO_RED_GRP, LED_GPIO_RED_BIT, E_IO_OUTPUT);
	DrvGPIO_Open(LED_GPIO_ISP_GRP, LED_GPIO_ISP_BIT, E_IO_OUTPUT);
	DrvGPIO_Open(LED_GPIO_ICE_GRP, LED_GPIO_ICE_BIT, E_IO_OUTPUT);

	/* Create and run thread */
	arg[0].gpio_group = LED_GPIO_RED_GRP;
	arg[1].gpio_group = LED_GPIO_ISP_GRP;
	arg[2].gpio_group = LED_GPIO_ICE_GRP;
	arg[0].gpio_bit = LED_GPIO_RED_BIT;
	arg[1].gpio_bit = LED_GPIO_ISP_BIT;
	arg[2].gpio_bit = LED_GPIO_ICE_BIT;
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
	DrvGPIO_SetBit(LED_GPIO_RED_GRP, LED_GPIO_RED_BIT);		/* OFF */
	DrvGPIO_SetBit(LED_GPIO_ISP_GRP, LED_GPIO_ISP_BIT);		/* OFF */
	DrvGPIO_SetBit(LED_GPIO_ISP_GRP, LED_GPIO_ISP_BIT);		/* OFF */

	DrvGPIO_ClrBit(LED_GPIO_GREEN_GRP, LED_GPIO_GREEN_BIT);	/* ON */
		
	while(1);
}


