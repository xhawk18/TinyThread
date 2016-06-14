#include "../Inc/tt_thread.h"


static void __tt_wq_add_event (TT_WQ_T *wait_queue)
{
	TT_THREAD_T *thread = tt_thread_self ();
	
	listMove (&wait_queue->list, &thread->list_schedule);
	thread->wait_parent = wait_queue;
}


static void __tt_wq_wait_event (void *arg)
{
	TT_WQ_T *wait_queue = (TT_WQ_T *)arg;
	__tt_wq_add_event (wait_queue);
	__tt_schedule ();
}


static void __tt_wq_set_event (void *arg)
{
	TT_WQ_T *wait_queue = (TT_WQ_T *)arg;
	
	while (!listIsEmpty (&wait_queue->list))
	{
		LIST_T *list = listGetNext (&wait_queue->list);
		TT_THREAD_T *thread = GetParentAddr (list, TT_THREAD_T, list_schedule);

		/* Append the thread to running thread */
		tt_set_thread_running (thread);
		
		__tt_schedule_yield (NULL);
	}	
}


/* Available in: thread (interruptable). */
void tt_wq_wait_event (TT_WQ_T *wait_queue)
{
	if (tt_is_irq_disabled ())
	{
		int i;
		__tt_wq_add_event (wait_queue);
		
		for (i = 0; tt_is_irq_disabled (); ++i)
			tt_enable_irq ();
		
		tt_syscall (NULL, (void (*)(void *))__tt_schedule);
		
		while (i-- != 0)
			tt_disable_irq ();
	}
	else
		tt_syscall ((void *)wait_queue, __tt_wq_wait_event);
}



/* Event with timeout. */
typedef struct
{
	TT_TIMER_T	timer;
	TT_THREAD_T	*thread;
	int			result;
	TT_WQ_T		*wait_queue;
} __TIMEOUT_WQ_T;


static void __tt_wq_wakeup (void *arg)
{
	__TIMEOUT_WQ_T *wq_args = (__TIMEOUT_WQ_T *)arg;
	
	if (wq_args->thread->wait_parent == wq_args->wait_queue)
	{
		/* Wakeup thread */
		wq_args->result = -1;
		__tt_wq_set_event (wq_args->wait_queue);
	}
}


static void __tt_wq_wait_event_timeout (void *arg)
{
	__TIMEOUT_WQ_T *wq_args = (__TIMEOUT_WQ_T *)arg;
	
	wq_args->result = 0;	//wq_args->result may be overwritten on timeout
	if (tt_timer_is_active (&wq_args->timer))
		__tt_wq_wait_event ((void *)wq_args->wait_queue);
	else 
		wq_args->result = -1;
}
static void __tt_wq_schedule_timeout (void *arg)
{
	__TIMEOUT_WQ_T *wq_args = (__TIMEOUT_WQ_T *)arg;
	
	wq_args->result = 0;	//wq_args->result may be overwritten on timeout
	if (tt_timer_is_active (&wq_args->timer))
		__tt_schedule ();
	else 
		wq_args->result = -1;
}


/* Available in: thread. */
int tt_wq_wait_event_timeout (TT_WQ_T *wait_queue, uint32_t msec)
{
	__TIMEOUT_WQ_T wq_args;

	wq_args.wait_queue	= wait_queue;
	wq_args.thread		= tt_thread_self ();
	
	if (tt_is_irq_disabled ())
	{
		int i;
		__tt_wq_add_event (wait_queue);
		
		for (i = 0; tt_is_irq_disabled (); ++i)
			tt_enable_irq ();
		
		tt_timer_start (&wq_args.timer, __tt_wq_wakeup, &wq_args, msec);
		tt_syscall ((void *)&wq_args, __tt_wq_schedule_timeout);
		tt_timer_kill (&wq_args.timer);
		
		while (i-- != 0)
			tt_disable_irq ();
	}
	else
	{
		tt_timer_start (&wq_args.timer, __tt_wq_wakeup, &wq_args, msec);	
		tt_syscall ((void *)&wq_args, __tt_wq_wait_event_timeout);
		tt_timer_kill (&wq_args.timer);
	}
	return wq_args.result;
}


/* Available in: irq, thread. */
void tt_wq_set_event (TT_WQ_T *wait_queue)
{
	if (tt_is_irq_disabled ())
		__tt_wq_set_event (wait_queue);
	else
		tt_syscall ((void *)wait_queue, __tt_wq_set_event);
}


