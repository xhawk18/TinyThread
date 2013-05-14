#ifndef INC_TT_WAIT_QUEUE_H__
#define INC_TT_WAIT_QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif


/* Wait queue */
typedef struct
{
	LIST_T list;
} TT_WQ_T;


/* Initilize the wait queue */
TT_INLINE void tt_wq_init (TT_WQ_T *wait_queue)
{
	listInit (&wait_queue->list);
}

/* Wait an event on wait queue */
void tt_wq_wait_event (TT_WQ_T *wait_queue);
/* Set an event on wait queue */
void tt_wq_set_event (TT_WQ_T *wait_queue);

	

#ifdef __cplusplus
} // extern "C"
#endif

#endif


