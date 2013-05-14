
#if defined __CC_ARM
#	include "tt_sys_keil.c"
#elif defined __ICCARM__
#	include "tt_sys_iar.c"
#elif defined __GNUC__
#	include "tt_sys_gcc.c"
#endif


#include "tt_time.c"
#include "tt_thread.c"
#include "tt_condition.c"
#include "tt_wait_queue.c"
#include "tt_msg.c"
#include "tt_mutex.c"
#include "tt_recursive_mutex.c"
#include "tt_semaphore.c"
#include "tt_sys.c"
#include "tt_list.c"

