/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "../../../Inc/tt_thread.h"



#define THREAD_NUM	3
typedef struct
{
	volatile int	arg;
} THREAD_ARG_T;

static unsigned int	stack[THREAD_NUM][TT_THREAD_BUFFER_SIZE(64)/sizeof(unsigned int)];
static THREAD_ARG_T	arg[THREAD_NUM];

void thread_entry(void *arg)
{
	THREAD_ARG_T *the_arg = (THREAD_ARG_T *)arg;

	while(1)
	{
		++the_arg->arg;
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
	
	/* Create and run thread */
	for (i = 0; i < THREAD_NUM; ++i)
	{
		arg[i].arg = i;

		tt_thread_create("thread",			/* thread Name */
						 0,					/* thread priority */
						 stack[i],			/* stack pointer */
						 sizeof(stack[i]),	/* stack size */
						 thread_entry,		/* thread entry function */
						 (void *)&arg[i]	/* argument for thread entry function */
						 );	
	}
		
    tt_thread_exit();
}


