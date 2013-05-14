#ifndef	INC_TT_CONFIG_H__
#define	INC_TT_CONFIG_H__


// Rename this file to tt_config.h and put into your project code
//	tt_config_default.h -> tt_config.h

// Support features
#define	TT_SUPPORT_USLEEP			// Supports tt_usleep()
#define	TT_SUPPORT_SEMAPHORE		// Supports tt_sem_*
#define	TT_SUPPORT_MUTEX			// Supports tt_mutex_* tt_rmutex_*
#define	TT_SUPPORT_CONDITION		// Supports tt_cond_*
#define	TT_SUPPORT_MSG				// Supports tt_msg_*
#define	TT_SUPPORT_STACK_CHECK		// Supports check thread's stack overflow and size
//#define	TT_SUPPORT_DUMP_THREAD	// Supports to dump all threads

#define	TT_IRQ_STACK_SIZE		512	// Size for interrupt handler
#define TT_THREAD_PRIORITY_NUM	3	// Max priority level

//SysTick's count per second.
//TinyThread will swtich the tasks(threads) on each ticks.
#define TT_TICKS_PER_SECOND		512	


// Device including file
#include "M051Series.h"	// For M051 series
//#include "NUC1xx.h"			// For NUC100 series (NUC100, NUC102, NUC120, NUC130, NUC140, etc...)
//#include "nano1xx.h"        		// For Nano100 series

// To save RAM size, list in TinyThread use 
//	((TT_RAM_PTRDIFF_T)offset + TT_RAM_START_ADDR) << TT_RAM_PTR_IGNORE_BIT
// as internal pointer.
//
// For 32-bit CPU with RAM size < 256K,
//	#define TT_RAM_PTRDIFF_T		uint16_t
//	#define TT_RAM_PTR_IGNORE_BIT	2
//	#define TT_RAM_START_ADDR		0x20000000
//
// For 32-bit CPU with RAM size < 64K,
//	#define TT_RAM_PTRDIFF_T		uint16_t
//	#define TT_RAM_PTR_IGNORE_BIT	0
//	#define TT_RAM_START_ADDR		0x20000000
//
// For general usage,
//	#define TT_RAM_PTRDIFF_T		ptrdiff_t
//	#define TT_RAM_PTR_IGNORE_BIT	0
//	#define TT_RAM_START_ADDR		0


#endif

