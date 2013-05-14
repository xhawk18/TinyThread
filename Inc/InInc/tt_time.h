#ifndef	INC__TT_TIME_H__
#define	INC__TT_TIME_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	LIST_T		list;
	void		(*on_timer) (void *arg);
	void		*arg;
	uint32_t	wakeup_ticks;
} TT_TIMER_T;

/* Start a timer. 
   The timer function will be called in interrupt mode! */
void tt_timer_start (TT_TIMER_T *timer,
	void (*on_timer) (void *arg),
	void *arg,
	uint32_t msec);
/* Kill the timer */
void tt_timer_kill (TT_TIMER_T *timer);
/* Check if timer is active or not */
TT_INLINE bool tt_timer_is_active (TT_TIMER_T *timer)
{
	return !listIsEmpty (&timer->list);
}
/* Wait the timer event */
void tt_timer_wait (TT_TIMER_T *timer);

/* Convert ticks to milliseconds */
uint32_t tt_ticks_to_msec (uint32_t ticks);
/* Convert milliseconds to ticks */
uint32_t tt_msec_to_ticks (uint32_t msec);

/* Sleep for some seconds */
void tt_sleep (uint32_t sec);
/* Sleep for some milliseconds */
void tt_msleep (uint32_t msec);

#ifdef	TT_SUPPORT_USLEEP
/* Sleep for some micorseconds */
void tt_usleep (uint32_t usec);
#endif

/* Get current tick count */
uint32_t tt_get_ticks (void);
/* Get current second time */
uint64_t tt_get_time (void);
/* set current second time */
uint64_t tt_set_time (uint64_t new_time);


#ifdef __cplusplus
} // extern "C"
#endif

#endif	// INC__TT_TIME_H__
