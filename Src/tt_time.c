#include "../Inc/tt_thread.h"



LIST_T				g_timers;
static volatile uint64_t	g_current_ticks;
static volatile uint64_t	g_time_offset;	/* tt_set_time() only set this value */


void SysTick_Handler ()
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
	listInit (&g_timers);
	SysTick_Config (systick_frequency / TT_TICKS_PER_SECOND);
}



/* Available in: irq, thread. */
uint32_t tt_get_ticks (void)
{
	return g_current_ticks;
}


/* Available in: irq, thread. */
uint32_t tt_ticks_to_msec (uint32_t ticks)
{
	uint64_t u64_msec = 1000 * (uint64_t)ticks / TT_TICKS_PER_SECOND;
	uint32_t msec = (u64_msec >  (uint64_t)~(uint32_t)0
		? ~(uint32_t)0 : (uint32_t)u64_msec);
	return msec;
}


/* Available in: irq, thread. */
uint32_t tt_msec_to_ticks (uint32_t msec)
{
	uint64_t u64_ticks = TT_TICKS_PER_SECOND * (uint64_t)msec / 1000;
	uint32_t ticks = (u64_ticks > (uint64_t)(uint32_t)0xFFFFFFFF
		? (uint32_t)0xFFFFFFFF : (uint32_t)u64_ticks);

	return ticks;
}


static void __tt_add_timer (void *arg)
{
	LIST_T *list;
	TT_TIMER_T *me = (TT_TIMER_T *)arg;
	uint32_t current_ticks = tt_get_ticks ();
	uint32_t sleep_ticks = me->wakeup_ticks;
	me->wakeup_ticks = current_ticks + (uint32_t)sleep_ticks;
	
	for (list = listGetNext (&g_timers); list != &g_timers; list = listGetNext (list))
	{
		TT_TIMER_T *timer = GetParentAddr (list, TT_TIMER_T, list);
		int32_t ticks_to_wakeup = (int32_t)(timer->wakeup_ticks - current_ticks);
		if (ticks_to_wakeup > sleep_ticks)
			break;
	}

	listMove (list, &me->list);
}

static void __tt_del_timer (void *arg)
{
	TT_TIMER_T *me = (TT_TIMER_T *)arg;
	listDetach (&me->list);
}

static void __tt_wait_timer (void *arg)
{
	TT_TIMER_T *me = (TT_TIMER_T *)arg;
	if (tt_timer_is_active (me))
	{
		//The timer must have not be waked up
		TT_THREAD_T *thread = tt_thread_self ();
		listDetach (&thread->list_schedule);
		thread->wait_parent = me;

		__tt_schedule ();
	}
}

void __tt_wakeup (void)
{
	LIST_T *list;
	LIST_T *list_next;
	uint32_t current_ticks = tt_get_ticks ();
	for (list = listGetNext (&g_timers); list != &g_timers; list = list_next)
	{
		TT_TIMER_T *timer = GetParentAddr (list, TT_TIMER_T, list);
		int32_t ticks_to_wakeup = (int32_t)(timer->wakeup_ticks - current_ticks);
		list_next = listGetNext (list);

		if (ticks_to_wakeup <= 0)
		{
			listDetach (&timer->list);
			(*timer->on_timer) (timer->arg);
		}
		else
			break;
	}
}


static void __tt_timer_start (TT_TIMER_T *timer,
	void (*on_timer) (void *arg),
	void *arg,
	uint32_t ticks)
{
	listInit (&timer->list);
	timer->on_timer 	= on_timer;
	timer->arg			= arg;
	timer->wakeup_ticks	= ticks;
	
	tt_syscall ((void *)timer, __tt_add_timer);
}

/* Available in: thread. */
void tt_timer_start (TT_TIMER_T *timer,
	void (*on_timer) (void *arg),
	void *arg,
	uint32_t msec)
{
	__tt_timer_start (timer, on_timer, arg, tt_msec_to_ticks (msec));
}

/* Available in: irq, thread */
void tt_timer_kill (TT_TIMER_T *timer)
{
	tt_syscall ((void *)timer, __tt_del_timer);
}

/* Available in: thread */
void tt_timer_wait (TT_TIMER_T *timer)
{
	tt_syscall ((void *)timer, __tt_wait_timer);
}

static void __tt_wakeup_thread (void *arg)
{
	TT_THREAD_T *thread = (TT_THREAD_T *)arg;
	tt_set_thread_running (thread);
}

void tt_sleep (uint32_t sec)
{
	TT_TIMER_T timer;
	uint64_t u64_ticks = TT_TICKS_PER_SECOND * (uint64_t)sec;
	uint32_t sleep_ticks = (u64_ticks > (uint64_t)(uint32_t)0xFFFFFFFF
		? (uint32_t)0xFFFFFFFF : (uint32_t)u64_ticks);

	__tt_timer_start (&timer, __tt_wakeup_thread, tt_thread_self (), sleep_ticks);	
	tt_timer_wait (&timer);
}


/* ticks >= 0 && < 2^31 */
void tt_msleep (uint32_t msec)
{
	TT_TIMER_T timer;
	tt_timer_start (&timer, __tt_wakeup_thread, tt_thread_self (), msec);
	tt_timer_wait (&timer);
}



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
			tt_printf ("Can not enable usleep!\n");
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
	uint32_t loop = (uint32_t)(g_loop_per_minisec * (uint64_t)usec / 1000);
	volatile uint32_t i;
	for (i = 0; i < loop; i++);
}

#endif	// TT_SUPPORT_USLEEP

