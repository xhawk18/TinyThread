#ifndef INC__TT_THREAD_H__
#define INC__TT_THREAD_H__

#include "tt_config.h"

#ifdef	TT_SUPPORT_CONDITION
#	ifndef	TT_SUPPORT_MUTEX
#		define	TT_SUPPORT_MUTEX	
#		warning	Adds "#define TT_SUPPORT_MUTEX" since TT_SUPPORT_CONDITION is defined.
#	endif
#endif

#ifdef	TT_SUPPORT_MUTEX
#	ifndef	TT_SUPPORT_SEMAPHORE
#		define	TT_SUPPORT_SEMAPHORE
#		warning	Adds "#define TT_SUPPORT_SEMAPHORE" since TT_SUPPORT_MUTEX is defined.
#	endif
#endif

#ifdef	TT_SUPPORT_MSG
#	ifndef	TT_SUPPORT_SEMAPHORE
#		define	TT_SUPPORT_SEMAPHORE
#		warning	Adds "#define TT_SUPPORT_SEMAPHORE" since TT_SUPPORT_MSG is defined.
#	endif
#endif


#include "./InInc/tt_private.h"

/* Function for thread operation */
#ifdef __cplusplus
extern "C" {
#endif


/* Param: systick_frequency: Clock frequency of systick */
void tt_init (uint32_t systick_frequency);

TT_THREAD_T *tt_thread_self (void);
TT_THREAD_T *tt_thread_create (
	const char		*name,
	unsigned char	priority,
	void			*buffer,			/* Global */
	size_t			buffer_size,
	void			(*tt_thread_entry) (void *),
	void			*tt_thread_arg
	);
void tt_thread_exit (void);
void tt_thread_yield (void);

void tt_set_priority (TT_THREAD_T *thread, unsigned char priority);
unsigned char tt_get_priority (TT_THREAD_T *thread);

#define TT_THREAD_BUFFER_SIZE(user_stack_size)							\
	(sizeof (TT_THREAD_T)			/* Thread handler */				\
	+ sizeof(TT_THREAD_PUSH_STACK)	/* Stack for thread's registers */	\
	+ (user_stack_size))			/* Stack that can be safely used by user */


#ifdef __cplusplus
} // extern "C"
#endif


#include "./InInc/tt_semaphore.h"
#include "./InInc/tt_mutex.h"
#include "./InInc/tt_recursive_mutex.h"
#include "./InInc/tt_condition.h"
#include "./InInc/tt_msg.h"
#include "./InInc/tt_time.h"

#endif

