#include "../Inc/tt_thread.h"

#ifdef	TT_SUPPORT_MUTEX

/* Available in: irq, thread. */
void tt_mutex_init (TT_MUTEX_T *mutex)
{
	mutex->locked_thread	= NULL;
	mutex->owner_thread		= NULL;
}


static bool comp_priority(LIST_T *p1st, LIST_T *p2nd, void *pArg)
{
	TT_THREAD_T *thread1 = GetParentAddr (p1st, TT_THREAD_T, list_schedule);
	TT_THREAD_T *thread2 = GetParentAddr (p2nd, TT_THREAD_T, list_schedule);

	return (thread1->priority < thread2->priority ? true : false);
}


static bool comp_priority2(LIST_T *p1st, LIST_T *p2nd, void *pArg)
{
	TT_THREAD_T *thread1 = GetParentAddr (p1st, TT_THREAD_T, list_wait_node);
	TT_THREAD_T *thread2 = GetParentAddr (p2nd, TT_THREAD_T, list_wait_node);

	return (thread1->priority < thread2->priority ? true : false);
}


static bool same_thread_and_priority (TT_THREAD_T *add_thread, TT_THREAD_T *del_thread)
{
	if (add_thread == del_thread)
        return true;
    else if(del_thread != NULL  // add_thread must != NULL
            && add_thread->priority == del_thread->priority)
		return true;
    else
	return false;
}


static void tt_reset_inherited_priority (TT_THREAD_T *thread)
{
	unsigned char priority = thread->fixed_priority;
	if (listIsEmpty (&thread->list_wait_head) == false)
	{
		TT_THREAD_T *wait_thread = GetParentAddr (thread->list_wait_head.pNext, TT_THREAD_T, list_wait_node);
		if (priority > wait_thread->priority)
		{
			priority = wait_thread->priority;
			
		}
	}

	tt_set_priority_inherit (thread, priority);
}



void verify_mutex(TT_MUTEX_T *mutex)
{
#if 0
	TT_THREAD_T	*locked_thread;
	/* "owner" points to the thread that locked this semaphore */
	TT_THREAD_T	*owner_thread;

	if (mutex->locked_thread != NULL)
	{
		ASSERT (mutex->owner_thread != NULL);
	}

	if (mutex->owner_thread == NULL)
	{
		ASSERT (mutex->locked_thread == NULL);
	}
	else if (mutex->locked_thread != NULL)
	{
		LIST_T *list0;
		LIST_T *list1;
		int priority = mutex->locked_thread->priority;

		ASSERT (mutex->owner_thread->priority <= priority);

		list0 = &mutex->locked_thread->list_schedule;
		list1 = &mutex->locked_thread->list_wait_node;
		do
		{
			TT_THREAD_T *thread0 = GetParentAddr(list0, TT_THREAD_T, list_schedule);
			TT_THREAD_T *thread1 = GetParentAddr(list1, TT_THREAD_T, list_wait_node);
			ASSERT(thread0 == thread1);

			ASSERT (priority <= thread0->priority);
			priority = thread0->priority;

		} while (list0 != &mutex->locked_thread->list_schedule);
	}
#endif
}


void verify_thread(TT_THREAD_T *thread)
{
#if 0
	LIST_T *list;
	if (listIsEmpty(&thread->list_wait_head) == false)
	{
		TT_THREAD_T *thread0 = GetParentAddr(thread->list_wait_head.pNext, TT_THREAD_T, list_wait_node);
		int priority = thread0->priority;

		ASSERT(thread->priority <= priority);

		for(list = thread->list_wait_head.pNext; list != &thread->list_wait_head; list = list->pNext)
		{
			TT_THREAD_T *thread0 = GetParentAddr(list, TT_THREAD_T, list_wait_node);
			ASSERT (priority <= thread0->priority);
			priority = thread0->priority;
		}
	}
#endif
}


static void tt_swap_wait_thread2 (TT_THREAD_T *owner_thread, TT_THREAD_T *add_thread, TT_THREAD_T *del_thread)
{

	if (del_thread != NULL)
	{
		listDetach (&del_thread->list_wait_node);	
	}
	
	if (add_thread != NULL)
	{
		listDetach (&add_thread->list_wait_node);
		listInsert (&owner_thread->list_wait_head, &add_thread->list_wait_node, comp_priority2, NULL);
	}
	
	/* Set new inherited priority */
	tt_reset_inherited_priority (owner_thread);

	verify_thread (owner_thread);
}

static void tt_swap_wait_thread (TT_MUTEX_T *mutex, TT_THREAD_T *add_thread, TT_THREAD_T *del_thread)
{
	while (1)
	{
		TT_THREAD_T *locked_thread = mutex->locked_thread;
		LIST_T list;
				
		ASSERT (mutex->owner_thread != NULL);

		/* Move all pending thread to a temp list */
		listInit (&list);
		if (mutex->locked_thread != NULL)
			listAttach (&list, &mutex->locked_thread->list_schedule);
	
		/* Remove thread */
		if (del_thread != NULL)
		{
			listDetach (&del_thread->list_schedule);
			del_thread->wait_parent = NULL;
			del_thread->locked_by_mutex = false;
	
			/* Update the header of locked_thread */
			//if (listIsEmpty(&list) != false)
			//	mutex->locked_thread = NULL;
		}
	
		/* Add thread */
        ASSERT(add_thread != NULL);
		//if (add_thread != NULL)
		{		
			listDetach (&add_thread->list_schedule);
			add_thread->wait_parent = mutex;
			add_thread->locked_by_mutex = true;
			
			/* Suspend current thread. */
			listInsert (&list, &add_thread->list_schedule, comp_priority, NULL);
	
			/* Update the header of locked_thread */
			mutex->locked_thread = GetParentAddr (list.pNext, TT_THREAD_T, list_schedule);
		}
	
		/* Remove the temp list */
		listDetach (&list);
		
		if (same_thread_and_priority (mutex->locked_thread, locked_thread) != false)
			break;
		else
		{
			int priority = mutex->owner_thread->priority;
			tt_swap_wait_thread2 (mutex->owner_thread, mutex->locked_thread, locked_thread);
	
			/* Check if priority chanaged */
			if (priority != mutex->owner_thread->priority
				&& mutex->owner_thread->locked_by_mutex)
			{
				mutex = (TT_MUTEX_T *)mutex->owner_thread->wait_parent;
				add_thread = mutex->owner_thread;
				del_thread = mutex->owner_thread;
				/* Continue to set priority of wait_parent thread ... */
			}
			else
				break;
		}
	}
}


void __tt_mutex_lock (TT_MUTEX_T *mutex, TT_THREAD_T *thread)
{
	verify_mutex(mutex);

	if (mutex->owner_thread != NULL)
	{
		tt_swap_wait_thread (mutex, thread, NULL);

		__tt_schedule ();
	}
	else
		mutex->owner_thread = thread;

	verify_mutex(mutex);
}


static void __tt_mutex_lock__helper (void *arg)
{
	TT_MUTEX_T *mutex	= (TT_MUTEX_T *)arg;
	TT_THREAD_T *thread	= tt_thread_self ();
			
	__tt_mutex_lock (mutex, thread);
}

/* Available in: thread. */
void tt_mutex_lock (TT_MUTEX_T *mutex)
{
	tt_syscall ((void *)mutex, __tt_mutex_lock__helper);
}



typedef struct
{
	TT_MUTEX_T	*mutex;
	int			result;
} __TRY_LOCK_T;

static void __tt_mutex_try_lock (void *arg)
{
	__TRY_LOCK_T *try_args = (__TRY_LOCK_T *) arg;
	TT_MUTEX_T *mutex = try_args->mutex;
	
	if (mutex->owner_thread != NULL)
		try_args->result = -1;
	else
	{
		mutex->owner_thread = tt_thread_self ();
		try_args->result = 0;
	}
}

/* Available in: irq, thread. */
int tt_mutex_try_lock (TT_MUTEX_T *mutex)
{
	__TRY_LOCK_T try_args;
	try_args.mutex = mutex;

	tt_syscall ((void *) &try_args, __tt_mutex_try_lock);
	return try_args.result;
}

/* Available in: irq, thread. */
int tt_mutex_can_lock (TT_MUTEX_T *mutex)
{
	if (mutex->owner_thread == NULL)
		return 0;
	else
		return -1;
}


void __tt_mutex_unlock (TT_MUTEX_T *mutex, TT_THREAD_T *new_owner)
{
	verify_mutex(mutex);

	if (new_owner == NULL)
	{
		ASSERT (mutex->owner_thread == tt_thread_self ());
		mutex->owner_thread = NULL;
	}
	else
	{
		/* Wake up the new_owner thread. */

		/* Remove all waiting thread from owner_thread */
		int priority = mutex->owner_thread->priority;
		//sysSafePrintf("Wakeup thread = %x\n", new_owner);

		listDetach (&new_owner->list_wait_node);
		tt_reset_inherited_priority (mutex->owner_thread);
		if (priority != mutex->owner_thread->priority
			&& mutex->owner_thread->locked_by_mutex)
		{
			tt_swap_wait_thread ((TT_MUTEX_T *)mutex->owner_thread->wait_parent,
				mutex->owner_thread,
				mutex->owner_thread);
		}

		/* Set new head thread */
		if (listIsEmpty (&new_owner->list_schedule) != false)
			mutex->locked_thread = NULL;
		else
		{
			LIST_T *locked_thread = new_owner->list_schedule.pNext;
			mutex->locked_thread = GetParentAddr (locked_thread, TT_THREAD_T, list_schedule);
			//sysSafePrintf("2 set %x wait on %x\n", mutex->locked_thread, mutex->owner);
		}

		/* The thread is not locked by a mutex now */
		new_owner->locked_by_mutex = false;

		/* Append the thread to running thread */
		tt_set_thread_running (new_owner);

		mutex->owner_thread = new_owner;
		
		priority = mutex->owner_thread->priority;

		if (mutex->locked_thread != NULL)
		{
			//sysSafePrintf("len1: %d\n", listLength(&mutex->owner_thread->list_wait_head));  
			//sysSafePrintf("len2: %d\n", listLength(&mutex->locked_thread->list_wait_node));
			listAttach (&mutex->owner_thread->list_wait_head, &mutex->locked_thread->list_wait_node);
		}
		tt_reset_inherited_priority (mutex->owner_thread);
		if (priority != mutex->owner_thread->priority
			&& mutex->owner_thread->locked_by_mutex)
		{
			tt_swap_wait_thread ((TT_MUTEX_T *)mutex->owner_thread->wait_parent,
				mutex->owner_thread,
				mutex->owner_thread);
		}
	}

	verify_mutex(mutex);
}

static void __tt_mutex_unlock__helper (void *arg)
{
	TT_MUTEX_T *mutex = (TT_MUTEX_T *)arg;
	__tt_mutex_unlock (mutex, mutex->locked_thread);
}

/* Available in: irq, thread. */
void tt_mutex_unlock (TT_MUTEX_T *mutex)
{
	tt_syscall ((void *)mutex, __tt_mutex_unlock__helper);
}

#endif	// TT_SUPPORT_MUTEX


