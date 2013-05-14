#include "../Inc/tt_thread.h"

#ifdef	TT_SUPPORT_MUTEX

/* Available in: irq, thread. */
void tt_mutex_init (TT_MUTEX_T *mutex)
{
	mutex->locked_thread	= NULL;
	mutex->owner_thread		= NULL;
}


static bool comp_priority(LIST_T *p1st, LIST_T *p2nd, void *arg)
{
	TT_THREAD_T *thread1 = GetParentAddr (p1st, TT_THREAD_T, list_schedule);
	TT_THREAD_T *thread2 = GetParentAddr (p2nd, TT_THREAD_T, list_schedule);

	return (thread1->priority < thread2->priority ? true : false);
}


static bool comp_priority2(LIST_T *p1st, LIST_T *p2nd, void *arg)
{
	TT_THREAD_T *thread1 = GetParentAddr (p1st, TT_THREAD_T, list_wait_node);
	TT_THREAD_T *thread2 = GetParentAddr (p2nd, TT_THREAD_T, list_wait_node);

	return (thread1->priority < thread2->priority ? true : false);
}

static void tt_reset_inherited_priority (TT_THREAD_T *thread)
{
	unsigned char priority = thread->fixed_priority;
	if (listIsEmpty (&thread->list_wait_head) == false)
	{
		TT_THREAD_T *wait_thread = GetParentAddr (listGetNext (&thread->list_wait_head), TT_THREAD_T, list_wait_node);
		if (priority > wait_thread->priority)
			priority = wait_thread->priority;
	}

	tt_set_priority_inherit (thread, priority);
}

#if 0
void dump_level(int level)
{
	while(level-- != 0)
		tt_printf(" ");
}

void dump_thread(int level, TT_THREAD_T *thread);

void dump_mutex(int level, TT_MUTEX_T *mutex)
{
	dump_level(level);
	tt_printf("m:%08x owner_thread=%08x locked_thread=", mutex, mutex->owner_thread);
	if(mutex->locked_thread != NULL)
	{
		LIST_T *list = &mutex->locked_thread->list_schedule; 
		do {
			TT_THREAD_T *thread = GetParentAddr(list, TT_THREAD_T, list_schedule);
			tt_printf(",%08x", thread);
			list = listGetNext(list);
		} while(list != &mutex->locked_thread->list_schedule);
	}
	tt_printf("\n");

	if(mutex->locked_thread != NULL)
	{
		LIST_T *list = &mutex->locked_thread->list_schedule; 
		do {
			TT_THREAD_T *thread = GetParentAddr(list, TT_THREAD_T, list_schedule);
			dump_thread(level+1, thread);
			list = listGetNext(list);
		} while(list != &mutex->locked_thread->list_schedule);
	}
}

void dump_thread(int level, TT_THREAD_T *thread)
{
	dump_level(level);
	tt_printf("t:%08x locked_by_mutex=%d wait_parent=%08x wait_node=",
		thread, (thread->locked_by_mutex?1:0), thread->wait_parent);

	{
		LIST_T *list;
		for(list = listGetNext(&thread->list_wait_head); list != &thread->list_wait_head; list = listGetNext(list))
		{
			TT_THREAD_T *thread = GetParentAddr(list, TT_THREAD_T, list_wait_node);
			tt_printf(" %08x,%08x", thread, thread->wait_parent);
		}
	}
	tt_printf("\n");
	
	{
		LIST_T *list;
		for(list = listGetNext(&thread->list_wait_head); list != &thread->list_wait_head; list = listGetNext(list))
		{
			TT_THREAD_T *thread = GetParentAddr(list, TT_THREAD_T, list_wait_node);
			dump_mutex(level + 1, thread->wait_parent);
		}
	}
	
}
#endif

void verify_mutex(TT_MUTEX_T *mutex)
{
#if 0
	if (mutex->locked_thread != NULL)
	{
		TT_ASSERT (mutex->owner_thread != NULL);
	}

	if (mutex->owner_thread == NULL)
	{
		TT_ASSERT (mutex->locked_thread == NULL);
	}
	else if (mutex->locked_thread != NULL)
	{
		LIST_T *list0;
		LIST_T *list1;
		int priority = mutex->locked_thread->priority;

		TT_ASSERT (mutex->owner_thread->priority <= priority);

		list0 = &mutex->locked_thread->list_schedule;
		list1 = &mutex->locked_thread->list_wait_node;
		do
		{
			TT_THREAD_T *thread0 = GetParentAddr(list0, TT_THREAD_T, list_schedule);
			TT_THREAD_T *thread1 = GetParentAddr(list1, TT_THREAD_T, list_wait_node);
			TT_ASSERT(thread0 == thread1);

			TT_ASSERT (priority <= thread0->priority);
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
		TT_THREAD_T *thread0 = GetParentAddr (listGetNext (&thread->list_wait_head), TT_THREAD_T, list_wait_node);
		int priority = thread0->priority;

		TT_ASSERT(thread->priority <= priority);

		for(list = listGetNext (&thread->list_wait_head); list != &thread->list_wait_head; list = listGetNext (list))
		{
			TT_THREAD_T *thread0 = GetParentAddr(list, TT_THREAD_T, list_wait_node);
			TT_ASSERT (priority <= thread0->priority);
			priority = thread0->priority;
		}
	}
#endif
}

/* Add/Del HEAD threads that's waiting on owner_thread.
   A head thread is the thread waiting on a mutex with highest priority */
static void tt_swap_wait_nodes (TT_THREAD_T *owner_thread, TT_THREAD_T *add_thread, TT_THREAD_T *del_thread)
{
	if (del_thread != NULL)
	{
		listDetach (&del_thread->list_wait_node);
		if (del_thread->wait_parent != NULL)
			((TT_MUTEX_T *)del_thread->wait_parent)->owner_thread = NULL;
	}
	
	if (add_thread != NULL)
	{
		listDetach (&add_thread->list_wait_node);
		if (owner_thread)
			listInsert (&owner_thread->list_wait_head, &add_thread->list_wait_node, comp_priority2, NULL);
		if (add_thread->wait_parent != NULL)
			((TT_MUTEX_T *)add_thread->wait_parent)->owner_thread = owner_thread;
	}
	
	/* Set new inherited priority */
	if (owner_thread)
		tt_reset_inherited_priority (owner_thread);
}

/* Add/Del threads that's waiting on mutex. */
static void tt_swap_locked_threads (TT_MUTEX_T *mutex, TT_THREAD_T *add_thread, TT_THREAD_T *del_thread)
{
	LIST_T list;

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
	}
	
	/* Add thread */
	if (add_thread != NULL)
	{
		listDetach (&add_thread->list_schedule);
		add_thread->wait_parent = mutex;
		add_thread->locked_by_mutex = true;
			
		/* Suspend current thread. */
		listInsert (&list, &add_thread->list_schedule, comp_priority, NULL);
	}
	
	/* Update the header of locked_thread */
	if (listIsEmpty(&list) != false)
		mutex->locked_thread = NULL;
	else
		mutex->locked_thread = GetParentAddr (listGetNext (&list), TT_THREAD_T, list_schedule);

	/* Remove the temp list */
	listDetach (&list);
}


/* Add/Del threads that's waiting on mutex. */
static void tt_fullswap_locked_threads (TT_MUTEX_T *mutex, TT_THREAD_T *add_thread, TT_THREAD_T *del_thread)
{
	while (1)
	{
		TT_THREAD_T *old_locked_thread;
		TT_THREAD_T *new_locked_thread;
		
		old_locked_thread = mutex->locked_thread;
		tt_swap_locked_threads (mutex, add_thread, del_thread);
		new_locked_thread = mutex->locked_thread;
			
		if (old_locked_thread == new_locked_thread)
			break;
		else if (mutex->owner_thread == NULL
			|| (old_locked_thread != NULL && new_locked_thread != NULL
				&& old_locked_thread->priority == new_locked_thread->priority))
		{
			tt_swap_wait_nodes (mutex->owner_thread, new_locked_thread, old_locked_thread);
			break;
		}
		else
		{
			int priority = mutex->owner_thread->priority;
			tt_swap_wait_nodes (mutex->owner_thread, new_locked_thread, old_locked_thread);
	
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

/* Add/Del HEAD threads that's waiting on owner_thread.
   A head thread is the thread waiting on a mutex with highest priority */
static void tt_fullswap_wait_nodes (TT_THREAD_T *owner_thread, TT_THREAD_T *add_thread, TT_THREAD_T *del_thread)
{
	int priority = owner_thread->priority;
	tt_swap_wait_nodes (owner_thread, add_thread, del_thread);
	
	/* Check if priority chanaged */
	if (priority != owner_thread->priority
		&& owner_thread->locked_by_mutex)
	{
		TT_MUTEX_T *mutex = (TT_MUTEX_T *)owner_thread->wait_parent;
		TT_THREAD_T *add_thread = mutex->owner_thread;
		TT_THREAD_T *del_thread = mutex->owner_thread;
		tt_fullswap_locked_threads (mutex, add_thread, del_thread);
	}			
}


void __tt_mutex_lock (TT_MUTEX_T *mutex, TT_THREAD_T *thread)
{
	verify_mutex(mutex);

	if (mutex->owner_thread != NULL)
	{
		tt_fullswap_locked_threads (mutex, thread, NULL);
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

/*
  wait_head:     list_wait_head, a double linked pointer
  wait_node:     list_wait_node, a double linked pointer
  schedule:      list_schedule, a double linked pointer
  locked_thread: a simple pointer

  Convertion when removing thread_b0

============================================================================================================================
The original threads and mutex.
----------------------------------------------------------------------------------------------------------------------------
                          mutex_a                 mutex_b                 mutex_c                ...
					  (locked_thread)         (locked_thread)         (locked_thread)
                             |                       |                       |
 old_owner(wait_head) -> thread_a0(wait_node) -> thread_b0(wait_node) -> thread_c0(wait_node) -> ...
                         (schedule)              (schedule)              (schedule)
                             |                       |                       |
                         thread_a1               thread_b1               thread_c1
                         (schedule)              (schedule)              (schedule)
                             |                       |                       |
                         thread_a2               thread_b2               thread_c2
                         (schedule)              (schedule)              (schedule)
                             |                       |                       |
                            ...                     ...                     ...

============================================================================================================================
The final threads and mutex
----------------------------------------------------------------------------------------------------------------------------
  Thread/mutex for old_owner
----------------------------------------------------------------------------------------------------------------------------
                          mutex_a                 mutex_b                 mutex_c                ...
					  (locked_thread)         (locked_thread)         (locked_thread)
                             |                       |                       |
 old_owner(wait_head) -> thread_a0(wait_node) -> thread_b1(wait_node) -> thread_c0(wait_node) -> ...
                         (schedule)              (schedule)              (schedule)
                             |                       |                       |
                         thread_a1               thread_b2               thread_c1
                         (schedule)              (schedule)              (schedule)
                             |                       |                       |
                         thread_a2                  ...                  thread_c2
                         (schedule)                                      (schedule)
                             |                                               |
                            ...                                             ...

NOTE: order of thread_a0/thread_b1/thread_c1 linked by wait_node may be changed according to priority.

 */
static void __tt_mutex_unlock_without_set_owner (TT_MUTEX_T *mutex, TT_THREAD_T *thread)
{
	verify_mutex(mutex);

	/* Wake up the thread. */
	tt_fullswap_locked_threads (mutex, NULL, thread);
	/* Append the thread to running thread */
	tt_set_thread_running (thread);
	
	verify_mutex(mutex);
}

/* Mutex with timeout. */
typedef struct
{
	TT_TIMER_T	timer;
	TT_THREAD_T	*thread;
	int			result;
	TT_MUTEX_T	*mutex;
} __TIMEOUT_LOCK_T;


extern LIST_T g_timers;
static void __tt_mutex_lock_timeout__helper (void *arg)
{
	__TIMEOUT_LOCK_T *mutex_args = (__TIMEOUT_LOCK_T *)arg;
	
	mutex_args->result = 0;	//mutex_args->result may be overwritten on timeout
	if (mutex_args->mutex->owner_thread != NULL)
	{
		if (tt_timer_is_active (&mutex_args->timer))
			__tt_mutex_lock (mutex_args->mutex, tt_thread_self ());
		else 
			mutex_args->result = -1;
	}
	else
		mutex_args->mutex->owner_thread = tt_thread_self ();
}

static void __tt_mutex_wakeup (void *arg)
{
	__TIMEOUT_LOCK_T *mutex_args = (__TIMEOUT_LOCK_T *)arg;
	if (mutex_args->thread->wait_parent == mutex_args->mutex)
	{
		TT_ASSERT (mutex_args->mutex->owner_thread != NULL);
		/* Wakeup thread */
		mutex_args->result = -1;
		
		/* Set thread locked by mutex_args->thread */
		__tt_mutex_unlock_without_set_owner (mutex_args->mutex, mutex_args->thread);
	}
}

/* Available in: thread. */
int tt_mutex_lock_timeout (TT_MUTEX_T *mutex, uint32_t msec)
{
	__TIMEOUT_LOCK_T mutex_args;

	mutex_args.mutex	= mutex;
	mutex_args.thread	= tt_thread_self ();
	tt_timer_start (&mutex_args.timer, __tt_mutex_wakeup, &mutex_args, msec);	
	tt_syscall ((void *)&mutex_args, __tt_mutex_lock_timeout__helper);
	tt_timer_kill (&mutex_args.timer);

	return mutex_args.result;	
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

	tt_syscall ((void *)&try_args, __tt_mutex_try_lock);
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


/*
  wait_head:     list_wait_head, a double linked pointer
  wait_node:     list_wait_node, a double linked pointer
  schedule:      list_schedule, a double linked pointer
  locked_thread: a simple pointer

  Convertion when unlocking mutex_b

============================================================================================================================
The original threads and mutex.
----------------------------------------------------------------------------------------------------------------------------
                          mutex_a                 mutex_b                 mutex_c                ...
					  (locked_thread)         (locked_thread)         (locked_thread)
                             |                       |                       |
 old_owner(wait_head) -> thread_a0(wait_node) -> thread_b0(wait_node) -> thread_c0(wait_node) -> ...
                         (schedule)              (schedule)              (schedule)
                             |                       |                       |
                         thread_a1               thread_b1               thread_c1
                         (schedule)              (schedule)              (schedule)
                             |                       |                       |
                         thread_a2               thread_b2               thread_c2
                         (schedule)              (schedule)              (schedule)
                             |                       |                       |
                            ...                     ...                     ...

============================================================================================================================
The final threads and mutex
----------------------------------------------------------------------------------------------------------------------------
  Thread/mutex for old_owner
----------------------------------------------------------------------------------------------------------------------------
                          mutex_a                                         mutex_c                ...
					  (locked_thread)                                 (locked_thread)
                             |                                               |
 old_owner(wait_head) -> thread_a0(wait_node) -------------------------> thread_c0(wait_node) -> ...
                         (schedule)                                      (schedule)
                             |                                               |
                         thread_a1                                       thread_c1
                         (schedule)                                      (schedule)
                             |                                               |
                         thread_a2                                       thread_c2
                         (schedule)                                      (schedule)
                             |                                               |
                            ...                                             ...
----------------------------------------------------------------------------------------------------------------------------
  Thread/mutex for thread_b
----------------------------------------------------------------------------------------------------------------------------
                                mutex_x                 mutex_b                 mutex_y                 ...
					         (locked_thread)         (locked_thread)         (locked_thread)
                                    |                       |                       |
 thread_b0(wait_head) -> ... -> thread_x0(wait_node) -> thread_b1(wait_node) -> thread_y0(wait_node) -> ...
                                (schedule)              (schedule)              (schedule)
                                    |                       |                       |
                                thread_x1               thread_b2               thread_y1
                                (schedule)              (schedule)              (schedule) 
                                    |                       |                       |
                                thread_x2                  ...                  thread_y2
                                (schedule)                                      (schedule) 
                                    |                                               |
                                   ...                                             ...


 */

void __tt_mutex_unlock (TT_MUTEX_T *mutex, TT_THREAD_T *new_owner)
{
	verify_mutex(mutex);
	
	if (new_owner != NULL)
	{
		tt_fullswap_wait_nodes (mutex->owner_thread, NULL, mutex->locked_thread);
		tt_fullswap_locked_threads (mutex, NULL, new_owner);
		tt_fullswap_wait_nodes (new_owner, mutex->locked_thread, NULL);
		mutex->owner_thread = new_owner;

		/* Append the thread to running thread */
		tt_set_thread_running (new_owner);
	}
	mutex->owner_thread = new_owner;
	
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


