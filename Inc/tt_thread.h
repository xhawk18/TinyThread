#ifndef INC__TT_THREAD_H__
#define INC__TT_THREAD_H__


#undef TT_ENABLE_USLEEP
#undef TT_DEBUG_DUMPTHREAD


#define TT_TICKS_PER_SECOND	512


#include "./InInc/tt_private.h"

/* Function for thread operation */
void tt_init (void);
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


#include "./InInc/tt_semaphore.h"
#include "./InInc/tt_mutex.h"
#include "./InInc/tt_recursive_mutex.h"
#include "./InInc/tt_condition.h"
#include "./InInc/tt_msg.h"
#include "./InInc/tt_time.h"



#define TT_THREAD_BUFFER_SIZE(stack_size) (sizeof (TT_THREAD_T) + sizeof(TT_THREAD_PUSH_STACK) + (stack_size))



#endif

