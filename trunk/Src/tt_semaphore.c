#include "../Inc/tt_thread.h"

#ifdef	TT_SUPPORT_SEMAPHORE

/* Available in: irq, thread. */
void tt_sem_init (TT_SEM_T *sem, unsigned int count)
{
	if (sem == NULL)
		return;

	/* +1 and then -1, so that the semaphore may be owned by thread */
	sem->count			= count + 1;
	sem->locked_thread	= NULL;

	tt_sem_down (sem);
}


static void __tt_sem_down (void *arg)
{
	TT_SEM_T *sem = (TT_SEM_T *) arg;

	if (sem->count == 0)
	{
		/* Suspend current thread. */
		TT_THREAD_T *thread = tt_thread_self ();
		listDetach (&thread->list_schedule);
		thread->wait_parent = sem;

		if (sem->locked_thread != NULL)
		{
			/* Append "sem->locked_thread" to the right of "thread" */
			listAttach (&sem->locked_thread->list_schedule, &thread->list_schedule);
		}
		else
		{
			/* Head thread wait on this semaphore */
			sem->locked_thread = thread;
		}

		__tt_schedule ();
	}
	else
		--sem->count;
}


/* Available in: thread. */
void tt_sem_down (TT_SEM_T *sem)
{
	tt_syscall ((void *) sem, __tt_sem_down);
}


#if 0
/* Semaphore with timeout, still in development. */
typedef struct
{
	TT_SEM_T *sem;
	INT32 sleep_ticks;
	int result;
} __TIMEOUT_DOWN_T;

void __tt_sem_down_timeout (void *arg)
{
	__TIMEOUT_DOWN_T *sem_args = (__TIMEOUT_DOWN_T *) arg;
	TT_THREAD_T *thread = tt_thread_self ();
	
	sem_args->sem->count--;
	if (sem_args->sem->count < 0)
	{
	
	}
	
	__tt_sem_down ((void *) sem_args->sem);
	__tt_sleep ((void *) &sem_args->sleep_ticks);
	
}


/* Available in: thread. */
int tt_sem_down_timeout (TT_SEM_T *sem, INT32 sleep_ticks)
{
	__TIMEOUT_DOWN_T sem_args;
	sem_args.sem = sem;
	sem_args.sleep_ticks = sleep_ticks;
	
	tt_syscall ((void *) &sem_args, __tt_sem_down_timeout);

	return sem_args.result;
}
#endif



typedef struct
{
	TT_SEM_T	*sem;
	int			result;
} __TRY_DOWN_T;


static void __tt_sem_trydown (void *arg)
{
	__TRY_DOWN_T *try_args = (__TRY_DOWN_T *) arg;
	TT_SEM_T *sem = try_args->sem;
	
	if (sem->count == 0)
		try_args->result = -1;
	else
	{
		--sem->count;
		try_args->result = 0;
	}
}

/* Available in: irq, thread. */
int tt_sem_try_down (TT_SEM_T *sem)
{
	__TRY_DOWN_T try_args;
	try_args.sem = sem;
	
	tt_syscall ((void *) &try_args, __tt_sem_trydown);
	return try_args.result;
}


/* Available in: irq. */
int tt_sem_can_down (TT_SEM_T *sem)
{
	if (sem->count > 0)
		return 0;
	else
		return -1;
}


static void __tt_sem_up (void *arg)
{
	TT_SEM_T *sem = (TT_SEM_T *) arg;

	if (sem->locked_thread == NULL)
		++sem->count;
	else
	{
		/* Wake up a suspended thread. */
		TT_THREAD_T *thread = sem->locked_thread;
		
		/* Set new head thread */
		if (listIsEmpty (&thread->list_schedule) != false)
			sem->locked_thread = NULL;
		else
		{
			LIST_T *locked_thread = thread->list_schedule.pNext;
			sem->locked_thread = GetParentAddr (locked_thread, TT_THREAD_T, list_schedule);
		}

		/* Append the thread to running thread */
		tt_set_thread_running (thread);
	}
}

/* Available in: irq, thread. */
void tt_sem_up (TT_SEM_T *sem)
{
	tt_syscall ((void *) sem, __tt_sem_up);
}

#endif	// TT_SUPPORT_SEMAPHORE

