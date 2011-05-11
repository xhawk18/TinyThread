#ifndef	INC__TT_PRIVATE_H__
#define	INC__TT_PRIVATE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tt_sys.h"
#include "tt_list.h"
#include "tt_memory_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Public functions and structures */
/* Limitation:
	
	1. Not all functions can be called in both irq and thread.
	   Please check the remarks before functions implements.

 */

typedef struct
{
	uint32_t uIRQ_LR;

	uint32_t uR8;
	uint32_t uR9;
	uint32_t uR10;
	uint32_t uR11;

	uint32_t uR4;
	uint32_t uR5;
	uint32_t uR6;
	uint32_t uR7;

	uint32_t uR0;
	uint32_t uR1;
	uint32_t uR2;
	uint32_t uR3;

	uint32_t uR12;
	uint32_t uLR;
	uint32_t uPC;
	uint32_t uPSR;
} TT_THREAD_PUSH_STACK;


typedef struct tagTT_THREAD_T
{
	uint32_t uSP;

	void					*wait_parent;		//Point to the the object that locked me
	LIST_T					list_schedule;		//list node for schedule (including sleep, semaphore, shedule)

#ifdef	TT_SUPPORT_MUTEX
	LIST_T					list_wait_head;		//list node for other thread wait on me
	LIST_T					list_wait_node;		//list node for me to wait on other
	bool					locked_by_mutex;	//Indicates that the thread is locked by a mutex
	unsigned char			fixed_priority;		//the initial priority
#endif	// TT_SUPPORT_MUTEX

	unsigned char			priority;			//inherited priority

#ifdef	TT_SUPPORT_STACK_CHECK
	void					*stack_base;
	void					*stack_max_used;
	void					*stack_limit;
#endif

#ifdef	TT_SUPPORT_DUMP_THREAD
	LIST_T					list_threads;		//list node for the threads list
#endif
	
#ifdef	TT_SUPPORT_SLEEP
	uint32_t				wakeup_time;		//the wakeup time(ticks) for sleeping thread
#endif
	
	char					name[8];
} TT_THREAD_T;


/* Founction for internal use */
extern void tt_syscall(void *arg, void (*on_schedule)(void *));
extern void __tt_schedule (void);
extern void __tt_schedule_yield (void *);
extern void tt_set_thread_running (TT_THREAD_T *thread);
extern void tt_set_priority_inherit (TT_THREAD_T *thread, unsigned char priority);

extern void tt_timer_init (uint32_t systick_frequency);
extern void tt_enable_usleep (void);
extern void __tt_wakeup (void);

#ifdef	TT_SUPPORT_MUTEX
#include "tt_mutex.h"
extern void __tt_mutex_lock (TT_MUTEX_T *mutex, TT_THREAD_T *thread);
extern void __tt_mutex_unlock (TT_MUTEX_T *mutex, TT_THREAD_T *new_owner);
#endif	// TT_SUPPORT_MUTEX

#ifdef __cplusplus
} // extern "C"
#endif

#endif	// INC__TT_PRIVATE_H__
