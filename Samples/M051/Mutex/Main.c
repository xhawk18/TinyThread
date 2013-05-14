/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
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
	int			*value;
	TT_RMUTEX_T	*mutex;
	GPIO_T		*gpio_group;
	int			gpio_bit;
} THREAD_ARG_T;

static TT_THREAD_T	*thread[THREAD_NUM];
static unsigned int	stack[THREAD_NUM][TT_THREAD_BUFFER_SIZE(256)/sizeof(unsigned int)];
static int			value[THREAD_NUM];
static TT_RMUTEX_T	mutex;
static THREAD_ARG_T	arg[THREAD_NUM];

void thread_entry(void *arg)
{
	THREAD_ARG_T *the_arg = (THREAD_ARG_T *)arg;

	while(1)
	{
		int i;
		int value;

		/* Show LED so that users know that the thread is running. */
		int cur_ticks = tt_get_ticks ();
		if(cur_ticks % 64 < 32)
			the_arg->gpio_group->DOUT &= ~(1 << the_arg->gpio_bit);	/* Turn off LED */
		else
			the_arg->gpio_group->DOUT |= (1 << the_arg->gpio_bit);	/* Turn on LED */


			
		/* Lock the mutex */
		tt_rmutex_lock(the_arg->mutex);

		/* Update value */
		for(i = 0; i < THREAD_NUM; ++i)
			++the_arg->value[i];
	
		/* (Recursive) Lock the mutex again */
		tt_rmutex_lock(the_arg->mutex);
		
		/* Check the lock operation by checking value */
		value = the_arg->value[0];
		for(i = 0; i < THREAD_NUM; ++i)
		{
			if(the_arg->value[i] != value)
			{
				/* To run to here indicates that the test for mutex lock has been failed. */
				tt_printf("Failed\n");
				while(1);
			}
		}
		
		/* (Recursive) Unlock the mutex */
		tt_rmutex_unlock(the_arg->mutex);
	
		/* Unlock the mutex */
		tt_rmutex_unlock(the_arg->mutex);
	}
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

	/* Test threads lock by value[]
	   the element in value should be the same,
	   if not, there must be an error */
	tt_rmutex_init(&mutex);
	for(i = 0; i < THREAD_NUM; ++i)
		value[i] = 0;

	arg[0].gpio_group = LED0_GPIO_GRP;
	arg[1].gpio_group = LED1_GPIO_GRP;
	arg[2].gpio_group = LED2_GPIO_GRP;
	arg[0].gpio_bit = LED0_GPIO_BIT;
	arg[1].gpio_bit = LED1_GPIO_BIT;
	arg[2].gpio_bit = LED2_GPIO_BIT;

	for(i = THREAD_NUM; i-- != 0; )
	{
		arg[i].value = value;
		arg[i].mutex = &mutex;
		thread[i] = tt_thread_create("th", 
			0,
			stack[i], sizeof(stack[i]),
			thread_entry, (void *)&arg[i]);
	}
	
	tt_sleep(1024);
	
	while(1);
	return 0;
}


