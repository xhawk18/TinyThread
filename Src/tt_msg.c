#include "../Inc/tt_thread.h"


/* Available in: irq, thread. */
void tt_pc_init (TT_PC_T *pc, int max_produce_count)
{
	tt_sem_init (&pc->producer, max_produce_count);
	tt_sem_init (&pc->consumer, 0);
}


/* Available in: irq, thread. */
void tt_pc_produce (TT_PC_T *pc, void (*produce) (void *arg), void *arg)
{
	tt_sem_down (&pc->producer);

	if (produce != NULL)
		(*produce) (arg);
	
	tt_sem_up (&pc->consumer);
}

/* Available in: irq, thread. */
int tt_pc_try_produce (TT_PC_T *pc, void (*produce) (void *arg), void *arg)
{
	if (tt_sem_try_down (&pc->producer) < 0)
		return -1;

	if (produce != NULL)
		(*produce) (arg);
	
	tt_sem_up (&pc->consumer);

	return 0;
}

/* Available in: irq. */
int tt_pc_can_produce (TT_PC_T *pc)
{
	return tt_sem_can_down (&pc->producer);
}


/* Available in: irq, thread. */
void tt_pc_consume (TT_PC_T *pc, void (*consume) (void *arg), void *arg)
{
	tt_sem_down (&pc->consumer);

	if (consume != NULL)
		(*consume) (arg);
	
	tt_sem_up (&pc->producer);
}

/* Available in: irq, thread. */
int tt_pc_try_consume (TT_PC_T *pc, void (*consume) (void *arg), void *arg)
{
	if (tt_sem_try_down (&pc->consumer) < 0)
		return -1;

	if (consume != NULL)
		(*consume) (arg);
	
	tt_sem_up (&pc->producer);

	return 0;
}


/* Available in: irq. */
int tt_pc_can_consume (TT_PC_T *pc)
{
	return tt_sem_can_down (&pc->consumer);
}

/* Available in: irq, thread. */
TT_MSG_QUEUE_T *tt_msg_queue_init (void *msg_buffer, size_t msg_buffer_size)
{
	TT_MSG_QUEUE_T *msg_queue;
	if (msg_buffer == NULL)
		return NULL;
	if (msg_buffer_size <= sizeof (TT_MSG_QUEUE_T))
		return NULL;

	msg_queue = (TT_MSG_QUEUE_T *) msg_buffer;
	msg_buffer = (void *) ((char *) msg_buffer + sizeof (TT_MSG_QUEUE_T));
	msg_buffer_size -= sizeof (TT_MSG_QUEUE_T);
	
	msg_queue->msg_free = memInit (msg_buffer, msg_buffer_size, TT_MSG_T);
	if (msg_queue->msg_free == NULL)
		return NULL;
		
	msg_queue->max_msg_limit = listLength (msg_queue->msg_free);
	if (msg_queue->max_msg_limit <= 0)
		return NULL;
	
	msg_queue->msg_buffer = msg_buffer;
	msg_queue->msg_buffer_size = msg_buffer_size;
	listInit (&msg_queue->msg_used);
	tt_pc_init (&msg_queue->pc_semaphore, msg_queue->max_msg_limit);
	
	return msg_queue;
}


typedef struct
{
	TT_MSG_QUEUE_T *msg_queue;
	FUN_TT_MSG_PROC msg_proc;
	void *msg_data;
} TT_MSG_PRODUCER_T;

/*
   Return:
	NULL, failed
	other, the message pushed.
 */
static __inline TT_MSG_T *tt_msg_push (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC msg_proc, void *msg_data)
{
	LIST_T *msg_node = memNew (msg_queue->msg_buffer);
	if (msg_node == NULL)
		return NULL;
	else
	{
		TT_MSG_T *msg	= GetParentAddr (msg_node, TT_MSG_T, list);
		msg->msg_proc	= msg_proc;
		msg->msg_data	= msg_data;
		listAttach (&msg_queue->msg_used, &msg->list);
		return msg;
	}
}

static __inline void tt_msg_produce (void *arg)
{
	TT_MSG_PRODUCER_T *msg_producer = (TT_MSG_PRODUCER_T *) arg;
	sysDisableIRQ ();
	tt_msg_push (msg_producer->msg_queue, msg_producer->msg_proc, msg_producer->msg_data);
	sysEnableIRQ ();
}



typedef struct
{
	TT_MSG_QUEUE_T *msg_queue;
	FUN_TT_MSG_PROC *msg_proc;
	void **msg_data;
} TT_MSG_CONSUMER_T;

/*
   Return:
	NULL, failed
	other, the message poped.
   Return pointer just indicates if a message was popped. Do not use it for other purpose.
 */
static __inline TT_MSG_T *tt_msg_pop (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC *msg_proc, void **msg_data)
{
	LIST_T *msg_node = msg_queue->msg_used.pNext;
	if (msg_node == &msg_queue->msg_used)
		return NULL;
	else
	{
		TT_MSG_T *msg	= GetParentAddr (msg_node, TT_MSG_T, list);
		listDetach (msg_node);
		
		if (msg_proc != NULL)
			*msg_proc = msg->msg_proc;
		if (msg_data != NULL)
			*msg_data = msg->msg_data;
		
		memDel (msg_queue->msg_buffer, msg_node);
		return msg;
	}
}

static __inline void tt_msg_consume (void *arg)
{
	TT_MSG_CONSUMER_T *msg_consumer = (TT_MSG_CONSUMER_T *) arg;
	sysDisableIRQ ();
	tt_msg_pop (msg_consumer->msg_queue, msg_consumer->msg_proc, msg_consumer->msg_data);
	sysEnableIRQ ();
}



/* Available in: irq, thread. */
void tt_msg_send (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC msg_proc, void *msg_data)
{
	TT_MSG_PRODUCER_T msg_producer;
	if (msg_queue == NULL || (void *) msg_proc == NULL)
		return;
	
	msg_producer.msg_queue	= msg_queue;
	msg_producer.msg_proc	= msg_proc;
	msg_producer.msg_data	= msg_data;

	tt_pc_produce (&msg_queue->pc_semaphore, tt_msg_produce, (void *) &msg_producer);
}


/* Available in: irq, thread. */
int tt_msg_try_send (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC msg_proc, void *msg_data)
{
	TT_MSG_PRODUCER_T msg_producer;
	if (msg_queue == NULL || (void *) msg_proc == NULL)
		return -1;
	
	msg_producer.msg_queue	= msg_queue;
	msg_producer.msg_proc	= msg_proc;
	msg_producer.msg_data	= msg_data;

	return tt_pc_try_produce (&msg_queue->pc_semaphore, tt_msg_produce, (void *) &msg_producer);
}


/* Available in: irq. */
int tt_msg_can_send (TT_MSG_QUEUE_T *msg_queue)
{
	return tt_pc_can_produce (&msg_queue->pc_semaphore);
}


/* Available in: irq, thread. */
void tt_msg_recv (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC *msg_proc, void **msg_data)
{
	TT_MSG_CONSUMER_T msg_consumer;
	if (msg_queue == NULL)
		return;
	
	msg_consumer.msg_queue	= msg_queue;
	msg_consumer.msg_proc	= msg_proc;
	msg_consumer.msg_data	= msg_data;

	tt_pc_consume (&msg_queue->pc_semaphore, tt_msg_consume, (void *) &msg_consumer);	
}


/* Available in: irq, thread. */
int tt_msg_try_recv (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC *msg_proc, void **msg_data)
{
	TT_MSG_CONSUMER_T msg_consumer;
	if (msg_queue == NULL)
		return -1;
	
	msg_consumer.msg_queue	= msg_queue;
	msg_consumer.msg_proc	= msg_proc;
	msg_consumer.msg_data	= msg_data;

	return tt_pc_try_consume (&msg_queue->pc_semaphore, tt_msg_consume, (void *) &msg_consumer);	
}


/* Available in: irq. */
int tt_msg_can_recv (TT_MSG_QUEUE_T *msg_queue)
{
	return tt_pc_can_consume (&msg_queue->pc_semaphore);
}










