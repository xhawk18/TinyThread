#ifndef	INC__TT_SEMAPHORE_H__
#define	INC__TT_SEMAPHORE_H__

#ifdef	TT_SUPPORT_SEMAPHORE

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	/* locked_thread points to list of locked thread */
	TT_THREAD_T		*locked_thread;
	unsigned int	count;
} TT_SEM_T;	/* Semaphore */


void tt_sem_init (TT_SEM_T *sem, unsigned int count);
void tt_sem_down (TT_SEM_T *sem);
int tt_sem_try_down (TT_SEM_T *sem);
int tt_sem_can_down (TT_SEM_T *sem);
void tt_sem_up (TT_SEM_T *sem);


#ifdef __cplusplus
} // extern "C"
#endif

#endif	// TT_SUPPORT_SEMAPHORE
#endif	// INC__TT_SEMAPHORE_H__
