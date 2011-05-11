#ifndef	INC__TT_MUTEX_H__
#define	INC__TT_MUTEX_H__

#ifdef	TT_SUPPORT_MUTEX

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	/* locked_thread points to list of locked thread */
	TT_THREAD_T	*locked_thread;
	/* "owner" points to the thread that locked this semaphore */
	TT_THREAD_T	*owner_thread;
} TT_MUTEX_T;	/* Lock */


void tt_mutex_init (TT_MUTEX_T *mutex);
void tt_mutex_lock (TT_MUTEX_T *mutex);
int tt_mutex_try_lock (TT_MUTEX_T *mutex);
int tt_mutex_can_lock (TT_MUTEX_T *mutex);
void tt_mutex_unlock (TT_MUTEX_T *mutex);


#ifdef __cplusplus
} // extern "C"
#endif

#endif	// TT_SUPPORT_MUTEX

#endif	// INC__TT_MUTEX_H__
