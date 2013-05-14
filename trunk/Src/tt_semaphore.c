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


static void __tt_sem_up2 (TT_SEM_T *sem, TT_THREAD_T *thread)
{
	if (sem->locked_thread == NULL)
		++sem->count;
	else
	{
		/* Wake up the suspended thread. */
		
		/* Set new head thread */
		if (listIsEmpty (&thread->list_schedule) != false)
			sem->locked_thread = NULL;
		else if(sem->locked_thread == thread)
		{
			LIST_T *locked_thread = listGetNext (&thread->list_schedule);
			sem->locked_thread = GetParentAddr (locked_thread, TT_THREAD_T, list_schedule);
		}

		/* Append the thread to running thread */
		tt_set_thread_running (thread);
	}
}



/* Available in: thread. */
void tt_sem_down (TT_SEM_T *sem)
{
	tt_syscall ((void *)sem, __tt_sem_down);
}


/* Semaphore with timeout. */
typedef struct
{
	TT_TIMER_T	timer;
	TT_THREAD_T	*thread;
	int			result;
	TT_SEM_T	*sem;
} __TIMEOUT_DOWN_T;

static void __tt_sem_down_timeout (void *arg)
{
	__TIMEOUT_DOWN_T *sem_args = (__TIMEOUT_DOWN_T *)arg;
	
	sem_args->result = 0;	//sem_args->result may be overwritten on timeout
	if (sem_args->sem->count == 0)
	{
		if (tt_timer_is_active (&sem_args->timer))
			__tt_sem_down ((void *)sem_args->sem);
		else 
			sem_args->result = -1;
	}
	else
		sem_args->sem->count--;
}

static void __tt_sem_wakeup (void *arg)
{
	__TIMEOUT_DOWN_T *sem_args = (__TIMEOUT_DOWN_T *)arg;
	
	if (sem_args->thread->wait_parent == sem_args->sem)
	{
		/* Wakeup thread */
		sem_args->result = -1;
		__tt_sem_up2 (sem_args->sem, sem_args->thread);
	}
}

/* Available in: thread. */
int tt_sem_down_timeout (TT_SEM_T *sem, uint32_t msec)
{
	__TIMEOUT_DOWN_T sem_args;

	sem_args.sem	= sem;
	sem_args.thread	= tt_thread_self ();
	tt_timer_start (&sem_args.timer, __tt_sem_wakeup, &sem_args, msec);	
	tt_syscall ((void *)&sem_args, __tt_sem_down_timeout);
	tt_timer_kill (&sem_args.timer);

	return sem_args.result;
}


typedef struct
{
	TT_SEM_T	*sem;
	int			result;
} __TRY_DOWN_T;


static void __tt_sem_trydown (void *arg)
{
	__TRY_DOWN_T *try_args = (__TRY_DOWN_T *)arg;
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
	
	tt_syscall ((void *)&try_args, __tt_sem_trydown);
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


static void __tt_sem_up(void *arg)
{
	TT_SEM_T *sem = (TT_SEM_T *)arg;
	__tt_sem_up2 (sem, sem->locked_thread);
}

/* Available in: irq, thread. */
void tt_sem_up (TT_SEM_T *sem)
{
	tt_syscall ((void *)sem, __tt_sem_up);
}

#endif	// TT_SUPPORT_SEMAPHORE

