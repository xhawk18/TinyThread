#ifndef	INC__TT_RECURSIVE_MUTEX_H__
#define	INC__TT_RECURSIVE_MUTEX_H__

#ifdef	TT_SUPPORT_MUTEX

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	TT_MUTEX_T mutex;
	int lock_count;
	TT_THREAD_T *thread;
} TT_RMUTEX_T;	/* Recursive lock */


void tt_rmutex_init (TT_RMUTEX_T *rmutex);
void tt_rmutex_lock (TT_RMUTEX_T *rmutex);
int tt_rmutex_lock_timeout (TT_RMUTEX_T *rmutex, uint32_t msec);
int tt_rmutex_try_lock (TT_RMUTEX_T *rmutex);
int tt_rmutex_can_lock (TT_RMUTEX_T *rmutex);
void tt_rmutex_unlock (TT_RMUTEX_T *rmutex);


#ifdef __cplusplus
} // extern "C"
#endif

#endif	// TT_SUPPORT_MUTEX
#endif	// INC__TT_RECURSIVE_MUTEX_H__
