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
		
		tt_syscall (NULL, __tt_schedule);
		
		while (i-- != 0)
			tt_disable_irq ();
	}
	else
		tt_syscall ((void *)wait_queue, __tt_wq_wait_event);
}


/* Available in: irq, thread. */
void tt_wq_set_event (TT_WQ_T *wait_queue)
{
	if (tt_is_irq_disabled ())
		__tt_wq_set_event (wait_queue);
	else
		tt_syscall ((void *)wait_queue, __tt_wq_set_event);
}


