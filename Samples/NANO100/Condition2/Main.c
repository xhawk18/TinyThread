/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "../../../Inc/tt_thread.h"

#include "nano1xx_gpio.h"

#define LED_GPIO_ICE_GRP	GPIOB
#define LED_GPIO_ICE_BIT	4
#define LED_GPIO_ISP_GRP	GPIOB
#define LED_GPIO_ISP_BIT	5
#define LED_GPIO_RED_GRP	GPIOB
#define LED_GPIO_RED_BIT	6
#define LED_GPIO_GREEN_GRP	GPIOB
#define LED_GPIO_GREEN_BIT	7



/* MY_SEM_T is implemented by a condition and a mutex */
typedef struct
{
	TT_RMUTEX_T		mutex;
	TT_COND_T		cond;
	unsigned int	value;
} MY_SEM_T;


void my_sem_init (MY_SEM_T *sem, unsigned int value)
{
	tt_rmutex_init (&sem->mutex);
	tt_cond_init (&sem->cond);

	sem->value = value;
}


void my_sem_down (MY_SEM_T *sem)
{
	tt_rmutex_lock (&sem->mutex);
	while (sem->value == 0)
	{
		tt_cond_wait2 (&sem->cond, &sem->mutex);
	}
	--sem->value;
	tt_rmutex_unlock (&sem->mutex);
}

void my_sem_up (MY_SEM_T *sem)
{
	/* The 4 piece of code should all work! */
#if 1
	tt_rmutex_lock (&sem->mutex);
	++sem->value;
	tt_cond_signal (&sem->cond);
	tt_rmutex_unlock (&sem->mutex);

#elif 1
	tt_rmutex_lock (&sem->mutex);
	++sem->value;
	tt_cond_broadcast (&sem->cond);
	tt_rmutex_unlock (&sem->mutex);

#elif 1
	tt_rmutex_lock (&sem->mutex);
	++sem->value;
	tt_rmutex_unlock (&sem->mutex);

	tt_cond_signal (&sem->cond);
#else

	tt_rmutex_lock (&sem->mutex);
	++sem->value;
	tt_rmutex_unlock (&sem->mutex);

	tt_cond_broadcast (&sem->cond);
#endif
	
}



#define THREAD_NUM	4
typedef struct
{
	GPIO_TypeDef    *gpio_group;
	int				gpio_bit;
	MY_SEM_T		*sem;
} THREAD_ARG_T;

static TT_THREAD_T	*thread[THREAD_NUM];
static unsigned int	stack[THREAD_NUM][TT_THREAD_BUFFER_SIZE(96)/sizeof(unsigned int)];


void thread_entry(void *arg)
{
	THREAD_ARG_T *the_arg = (THREAD_ARG_T *)arg;

	my_sem_down (the_arg->sem);
	while (1)
	{
		int cur_ticks = tt_get_ticks ();

		if (cur_ticks % 1024 < 4)
		{
			my_sem_up (the_arg->sem);
			//tt_thread_yield ();
			//tt_sleep(1);
			tt_msleep(rand()%64);
			my_sem_down (the_arg->sem);
		}

		
		if(cur_ticks % 64 < 32)
			GPIO_SetBit(the_arg->gpio_group, the_arg->gpio_bit);	/* Turn off LED */
		else
			GPIO_ClrBit(the_arg->gpio_group, the_arg->gpio_bit);	/* Turn on LED */
	}
}


int main()
{
	int i;
	THREAD_ARG_T	arg[THREAD_NUM];
	MY_SEM_T		sem;

	/* The clock frequency of systick may be changed by user's application.
	   Please change the value of SYSTICK_CLOCK according to real situration */
	uint32_t SysGet_HCLKFreq(void);
#define TT_SYSTICK_CLOCK		SysGet_HCLKFreq()
	/* Initialize TinyThread */
	tt_init(TT_SYSTICK_CLOCK);
	
	/* Open LED GPIO for testing */
    GPIO_Open(GPIOB,
          GPIO_PMD_PMD4_OUTPUT
        | GPIO_PMD_PMD5_OUTPUT
        | GPIO_PMD_PMD6_OUTPUT
        | GPIO_PMD_PMD7_OUTPUT,
          GPIO_PMD_PMD4_MASK
        | GPIO_PMD_PMD5_MASK
        | GPIO_PMD_PMD6_MASK
        | GPIO_PMD_PMD7_MASK);

	/* Set semaphore value to 3, so that always 3 threads' LED is blinking */
	my_sem_init (&sem, 3);

	/* Create and run thread */
	arg[0].gpio_group = LED_GPIO_RED_GRP;
	arg[1].gpio_group = LED_GPIO_ISP_GRP;
	arg[2].gpio_group = LED_GPIO_ICE_GRP;
	arg[3].gpio_group = LED_GPIO_GREEN_GRP;
	arg[0].gpio_bit = LED_GPIO_RED_BIT;
	arg[1].gpio_bit = LED_GPIO_ISP_BIT;
	arg[2].gpio_bit = LED_GPIO_ICE_BIT;
	arg[3].gpio_bit = LED_GPIO_GREEN_BIT;

	for (i = 0; i < THREAD_NUM; ++i)
	{
		arg[i].sem = &sem;
		thread[i] = tt_thread_create("thread",			/* thread Name */
									 0,					/* thread priority */
									 stack[i],			/* stack pointer */
									 sizeof(stack[i]),	/* stack size */
									 thread_entry,		/* thread entry function */
									 (void *)&arg[i]	/* argument for thread entry function */
									 );	
	}
	

	tt_thread_exit ();
	return 0;
}


