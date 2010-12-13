#include "../Inc/tt_thread.h"




#ifdef	TT_SUPPORT_SLEEP
static LIST_T	g_thread_sleeping;
#endif	// TT_SUPPORT_SLEEP
static volatile uint64_t	g_current_ticks;
static volatile uint64_t	g_time_offset;	/* tt_set_time() only set this value */


void __tt_on_timer()
{
	g_current_ticks++;
	__tt_schedule_yield (NULL);
}


uint64_t tt_get_time (void)
{
	uint64_t u64_ticks_offset = g_time_offset * TT_TICKS_PER_SECOND;
	uint64_t u64_sec = (g_current_ticks + u64_ticks_offset) / TT_TICKS_PER_SECOND;
	return u64_sec;
}


uint64_t tt_set_time (uint64_t new_time)
{
	uint64_t rt = g_time_offset;
	g_time_offset = new_time;
	return rt;
}


void tt_timer_init (uint32_t systick_frequency)
{
#ifdef	TT_SUPPORT_SLEEP
	listInit (&g_thread_sleeping);
#endif	// TT_SUPPORT_SLEEP

	SysTick_Config(systick_frequency / TT_TICKS_PER_SECOND);
}



/* Available in: irq, thread. */
uint32_t tt_get_ticks (void)
{
	return g_current_ticks;
}


/* Available in: irq, thread. */
uint32_t tt_ticks_to_msec (uint32_t ticks)
{
	uint64_t u64_msec = 1000 * (uint64_t) ticks / TT_TICKS_PER_SECOND;
	uint32_t msec = (u64_msec >  (uint64_t) ~(uint32_t) 0
		? ~(uint32_t) 0 : (uint32_t) u64_msec);
	return msec;
}


/* Available in: irq, thread. */
uint32_t tt_msec_to_ticks (uint32_t msec)
{
	uint64_t u64_ticks = TT_TICKS_PER_SECOND * (uint64_t) msec / 1000;
	uint32_t ticks = (u64_ticks > (uint64_t) (uint32_t) 0xFFFFFFFF
		? (uint32_t) 0xFFFFFFFF : (uint32_t) u64_ticks);

	return ticks;
}


#ifdef	TT_SUPPORT_SLEEP
/* Available in: thread. */
static void __tt_sleep (void *arg)
{
	LIST_T *list;
	TT_THREAD_T *thread = tt_thread_self ();
	int32_t sleep_ticks = *(int32_t *) arg;
	uint32_t current_time = tt_get_ticks ();
	thread->wakeup_time = current_time + (uint32_t) sleep_ticks;
	
	for (list = g_thread_sleeping.pNext; list != &g_thread_sleeping; list = list->pNext)
	{
		TT_THREAD_T *thread = GetParentAddr (list, TT_THREAD_T, list_schedule);
		int32_t time_to_wakeup = (int32_t) (thread->wakeup_time - current_time);
		if (time_to_wakeup > sleep_ticks)
			break;
	}

	listMove (list, &thread->list_schedule);
	thread->wait_parent = &g_thread_sleeping;
	
	__tt_schedule ();
}


void __tt_wakeup (void)
{
	LIST_T *list;
	LIST_T *list_next;
	uint32_t current_time = tt_get_ticks ();
	for (list = g_thread_sleeping.pNext; list != &g_thread_sleeping; list = list_next)
	{
		TT_THREAD_T *thread = GetParentAddr (list, TT_THREAD_T, list_schedule);
		int32_t time_to_wakeup = (int32_t) (thread->wakeup_time - current_time);

		list_next = list->pNext;
		if (time_to_wakeup <= 0)
		{
			tt_set_thread_running (thread);
		}
		else
			break;
	}
}



void tt_sleep(uint32_t sec)
{
	uint64_t u64_ticks = TT_TICKS_PER_SECOND * (uint64_t) sec;
	uint32_t sleep_ticks = (u64_ticks > (uint64_t) (uint32_t) 0xFFFFFFFF
		? (uint32_t) 0xFFFFFFFF : (uint32_t) u64_ticks);

	tt_syscall ((void *) &sleep_ticks, __tt_sleep);
}


/* ticks >= 0 && < 2^31 */
void tt_msleep (uint32_t msec)
{
	uint32_t sleep_ticks = tt_msec_to_ticks (msec);
		
	tt_syscall ((void *) &sleep_ticks, __tt_sleep);
}
#endif	// TT_SUPPORT_SLEEP



#ifdef	TT_SUPPORT_USLEEP

static uint32_t g_loop_per_minisec = 0;

/* Available in: thread. */
void tt_enable_usleep (void)
{
	volatile uint32_t i;
	uint32_t loop = 1;
	uint32_t ticks;
	
	do
	{
		if (loop * 2 > loop)
			loop *= 2;
		else
		{
			sysSafePrintf ("Can not enable usleep!\n");
			while (1);
		}
		
		ticks = tt_get_ticks ();
		for (i = 0; i < loop; i++);
		ticks = tt_get_ticks () - ticks;
	} while (ticks * 1000 / TT_TICKS_PER_SECOND < 15);	//	>= 15 miniseconds.
	
	g_loop_per_minisec = loop / (ticks * 1000 / TT_TICKS_PER_SECOND);
	//sysSafePrintf ("Loops %d for %d sec (%d ticks)\n", loop, 1, ticks);
	//sysSafePrintf ("Loop per msec: %d\n", g_loop_per_minisec);
}


void tt_usleep (uint32_t usec)
{
	uint32_t loop = (uint32_t) (g_loop_per_minisec * (uint64_t) usec / 1000);
	volatile uint32_t i;
	for (i = 0; i < loop; i++);
}

#endif	// TT_SUPPORT_USLEEP

int g_ticks;	//Just for test
