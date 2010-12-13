#ifndef	INC_TT_CONFIG_H__
#define	INC_TT_CONFIG_H__


// Rename this file to tt_config.h and put into your project code
//	tt_config_default.h -> tt_config.h


// Device including file
//#include "M051Series.h"	// For M051 series
#include "NUC1xx.h"			// For NUC100 series (NUC100, NUC102, NUC120, NUC130, NUC140, etc...)


// Support features
#define	TT_SUPPORT_SLEEP			// Supports tt_sleep()
#define	TT_SUPPORT_USLEEP			// Supports tt_usleep()
#define	TT_SUPPORT_SEMAPHORE		// Supports tt_sem_*
#define	TT_SUPPORT_MUTEX			// Supports tt_mutex_* tt_rmutex_*
#define	TT_SUPPORT_CONDITION		// Supports tt_cond_*
#define	TT_SUPPORT_MSG				// Supports tt_msg_*
#define	TT_SUPPORT_STACK_CHECK		// Supports check thread's stack overflow and size
//#define	TT_SUPPORT_DUMP_THREAD		// Supports to dump all threads

#define	TT_IRQ_STACK_SIZE		256		// Size for interrupt handler
#define TT_THREAD_PRIORITY_NUM	3		// Max priority level

//SysTick's count per second.
//TinyThread will swtich the tasks(threads) on each ticks.
#define TT_TICKS_PER_SECOND		512	

#endif

