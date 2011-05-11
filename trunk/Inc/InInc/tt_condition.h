#ifndef	INC__TT_CONDITION_H__
#define	INC__TT_CONDITION_H__

#ifdef	TT_SUPPORT_CONDITION

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	/* locked_thread points to list of locked thread */
	TT_THREAD_T		*locked_thread;
	TT_MUTEX_T		*mutex;
	bool			wait_on_recursive_mutex;
} TT_COND_T;


/* Initialize a condition variable */
void tt_cond_init (TT_COND_T *cond);
/* Wait cond on a common mutex */
void tt_cond_wait (TT_COND_T *cond, TT_MUTEX_T *mutex);
/* Wait cond on a recursive mutex */
void tt_cond_wait2 (TT_COND_T *cond, TT_RMUTEX_T *mutex);
/* Unblock at lease one thread waiting on this condition */
void tt_cond_signal (TT_COND_T *cond);
/* Unblock all thread waiting on this condition */
void tt_cond_broadcast (TT_COND_T *cond);


#ifdef __cplusplus
} // extern "C"
#endif

#endif	// TT_SUPPORT_CONDITION
#endif	// INC__TT_CONDITION_H__
