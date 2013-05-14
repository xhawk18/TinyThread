#include "../Inc/tt_thread.h"

#ifdef	TT_SUPPORT_MSG

/* Available in: irq, thread. */
void tt_pc_init (TT_PC_T *pc, int max_produce_count)
{
	tt_sem_init (&pc->producer, max_produce_count);
	tt_sem_init (&pc->consumer, 0);
}


/* Available in: thread. */
void tt_pc_produce (TT_PC_T *pc, void (*produce) (void *arg), void *arg)
{
	tt_sem_down (&pc->producer);

	if (produce != NULL)
		(*produce) (arg);
	
	tt_sem_up (&pc->consumer);
}


/* Available in: thread. */
int tt_pc_produce_timeout (TT_PC_T *pc, void (*produce) (void *arg), void *arg, uint32_t msec)
{
	int rt = tt_sem_down_timeout (&pc->producer, msec);

	if (rt == 0)
	{
		if (produce != NULL)
			(*produce) (arg);
	
		tt_sem_up (&pc->consumer);
	}

	return rt;
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


/* Available in: thread. */
void tt_pc_consume (TT_PC_T *pc, void (*consume) (void *arg), void *arg)
{
	tt_sem_down (&pc->consumer);

	if (consume != NULL)
		(*consume) (arg);
	
	tt_sem_up (&pc->producer);
}


/* Available in: thread. */
int tt_pc_consume_timeout (TT_PC_T *pc, void (*consume) (void *arg), void *arg, uint32_t msec)
{
	int rt = tt_sem_down_timeout (&pc->consumer, msec);

	if (rt == 0)
	{
		if (consume != NULL)
			(*consume) (arg);
		tt_sem_up (&pc->producer);
	}
	
	return rt;
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
	msg_buffer = (void *)((char *) msg_buffer + sizeof (TT_MSG_QUEUE_T));
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
static TT_MSG_T *tt_msg_push (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC msg_proc, void *msg_data)
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

static void tt_msg_produce (void *arg)
{
	TT_MSG_PRODUCER_T *msg_producer = (TT_MSG_PRODUCER_T *) arg;
	tt_disable_irq ();
	tt_msg_push (msg_producer->msg_queue, msg_producer->msg_proc, msg_producer->msg_data);
	tt_enable_irq ();
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
static TT_MSG_T *tt_msg_pop (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC *msg_proc, void **msg_data)
{
	LIST_T *msg_node = listGetNext (&msg_queue->msg_used);
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

static void tt_msg_consume (void *arg)
{
	TT_MSG_CONSUMER_T *msg_consumer = (TT_MSG_CONSUMER_T *) arg;
	tt_disable_irq ();
	tt_msg_pop (msg_consumer->msg_queue, msg_consumer->msg_proc, msg_consumer->msg_data);
	tt_enable_irq ();
}


/* Available in: thread. */
void tt_msg_send (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC msg_proc, void *msg_data)
{
	TT_MSG_PRODUCER_T msg_producer;
	if (msg_queue == NULL || (void *)msg_proc == NULL)
		return;
	
	msg_producer.msg_queue	= msg_queue;
	msg_producer.msg_proc	= msg_proc;
	msg_producer.msg_data	= msg_data;

	tt_pc_produce (&msg_queue->pc_semaphore, tt_msg_produce, (void *)&msg_producer);
}


/* Available in: thread. */
int tt_msg_send_timeout (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC msg_proc, void *msg_data, uint32_t msec)
{
	TT_MSG_PRODUCER_T msg_producer;
	if (msg_queue == NULL || (void *)msg_proc == NULL)
		return -1;
	
	msg_producer.msg_queue	= msg_queue;
	msg_producer.msg_proc	= msg_proc;
	msg_producer.msg_data	= msg_data;

	return tt_pc_produce_timeout (&msg_queue->pc_semaphore, tt_msg_produce, (void *)&msg_producer, msec);	
}


/* Available in: irq, thread. */
int tt_msg_try_send (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC msg_proc, void *msg_data)
{
	TT_MSG_PRODUCER_T msg_producer;
	if (msg_queue == NULL || (void *)msg_proc == NULL)
		return -1;
	
	msg_producer.msg_queue	= msg_queue;
	msg_producer.msg_proc	= msg_proc;
	msg_producer.msg_data	= msg_data;

	return tt_pc_try_produce (&msg_queue->pc_semaphore, tt_msg_produce, (void *)&msg_producer);
}


/* Available in: irq. */
int tt_msg_can_send (TT_MSG_QUEUE_T *msg_queue)
{
	return tt_pc_can_produce (&msg_queue->pc_semaphore);
}


/* Available in: thread. */
void tt_msg_recv (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC *msg_proc, void **msg_data)
{
	TT_MSG_CONSUMER_T msg_consumer;
	if (msg_queue == NULL)
		return;
	
	msg_consumer.msg_queue	= msg_queue;
	msg_consumer.msg_proc	= msg_proc;
	msg_consumer.msg_data	= msg_data;

	tt_pc_consume (&msg_queue->pc_semaphore, tt_msg_consume, (void *)&msg_consumer);	
}


/* Available in: thread. */
int tt_msg_recv_timeout (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC *msg_proc, void **msg_data, uint32_t msec)
{
	TT_MSG_CONSUMER_T msg_consumer;
	if (msg_queue == NULL)
		return -1;
	
	msg_consumer.msg_queue	= msg_queue;
	msg_consumer.msg_proc	= msg_proc;
	msg_consumer.msg_data	= msg_data;

	return tt_pc_consume_timeout (&msg_queue->pc_semaphore, tt_msg_consume, (void *)&msg_consumer, msec);	
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

	return tt_pc_try_consume (&msg_queue->pc_semaphore, tt_msg_consume, (void *)&msg_consumer);	
}


/* Available in: irq. */
int tt_msg_can_recv (TT_MSG_QUEUE_T *msg_queue)
{
	return tt_pc_can_consume (&msg_queue->pc_semaphore);
}



/* Available in: irq, thread. */
TT_BMSG_QUEUE_T *tt_bmsg_queue_init (void *msg_buffer, size_t msg_buffer_size, size_t each_msg_size)
{
	TT_BMSG_QUEUE_T *msg_queue;
	if (msg_buffer == NULL)
		return NULL;
	if (msg_buffer_size <= sizeof (TT_BMSG_QUEUE_T))
		return NULL;

	msg_queue = (TT_BMSG_QUEUE_T *) msg_buffer;
	msg_buffer = (void *)((char *) msg_buffer + sizeof (TT_BMSG_QUEUE_T));
	msg_buffer_size -= sizeof (TT_BMSG_QUEUE_T);
	each_msg_size = (each_msg_size + sizeof (int) - 1) / sizeof (int) * sizeof (int);
	
	msg_queue->queue.msg_free = __memInit (
		msg_buffer,
		msg_buffer_size,
		GetOffsetSize(TT_BMSG_T, msg_data) + each_msg_size,	
		(size_t) &((TT_BMSG_T *) 0)->list);
	if (msg_queue->queue.msg_free == NULL)
		return NULL;
		
	msg_queue->queue.max_msg_limit = listLength (msg_queue->queue.msg_free);
	if (msg_queue->queue.max_msg_limit <= 0)
		return NULL;
	
	msg_queue->queue.msg_buffer = msg_buffer;
	msg_queue->queue.msg_buffer_size = msg_buffer_size;
	listInit (&msg_queue->queue.msg_used);
	tt_pc_init (&msg_queue->queue.pc_semaphore, msg_queue->queue.max_msg_limit);
	
	msg_queue->each_msg_size = each_msg_size;
	return msg_queue;
}


typedef struct
{
	TT_BMSG_QUEUE_T *msg_queue;
	void *msg_data;
} TT_BMSG_PRODUCER_T;

/*
   Return:
	NULL, failed
	other, the message pushed.
 */
static TT_BMSG_T *tt_bmsg_push (TT_BMSG_QUEUE_T *msg_queue, void *msg_data)
{
	LIST_T *msg_node = memNew (msg_queue->queue.msg_buffer);
	if (msg_node == NULL)
		return NULL;
	else
	{
		TT_BMSG_T *msg	= GetParentAddr (msg_node, TT_BMSG_T, list);
		memcpy(&msg->msg_data[0], msg_data, msg_queue->each_msg_size);
		listAttach (&msg_queue->queue.msg_used, &msg->list);
		return msg;
	}
}

static void tt_bmsg_produce (void *arg)
{
	TT_BMSG_PRODUCER_T *msg_producer = (TT_BMSG_PRODUCER_T *) arg;
	tt_disable_irq ();
	tt_bmsg_push (msg_producer->msg_queue, msg_producer->msg_data);
	tt_enable_irq ();
}



typedef struct
{
	TT_BMSG_QUEUE_T *msg_queue;
	void **msg_data;
} TT_BMSG_CONSUMER_T;

/*
   Return:
	NULL, failed
	other, the message poped.
   Return pointer just indicates if a message was popped. Do not use it for other purpose.
 */
static TT_BMSG_T *tt_bmsg_pop (TT_BMSG_QUEUE_T *msg_queue, void **msg_data)
{
	LIST_T *msg_node = listGetNext (&msg_queue->queue.msg_used);
	if (msg_node == &msg_queue->queue.msg_used)
		return NULL;
	else
	{
		TT_BMSG_T *msg	= GetParentAddr (msg_node, TT_BMSG_T, list);
		listDetach (msg_node);
		
		if (msg_data != NULL)
			*msg_data = msg->msg_data;

		return msg;
	}
}

static void tt_bmsg_consume (void *arg)
{
	TT_BMSG_CONSUMER_T *msg_consumer = (TT_BMSG_CONSUMER_T *) arg;
	tt_disable_irq ();
	tt_bmsg_pop (msg_consumer->msg_queue, msg_consumer->msg_data);
	tt_enable_irq ();
}



/* Available in: thread. */
void tt_bmsg_send (TT_BMSG_QUEUE_T *msg_queue, void *msg_data)
{
	TT_BMSG_PRODUCER_T msg_producer;
	if (msg_queue == NULL || msg_data == NULL)
		return;
	
	msg_producer.msg_queue	= msg_queue;
	msg_producer.msg_data	= msg_data;

	tt_pc_produce (&msg_queue->queue.pc_semaphore, tt_bmsg_produce, (void *)&msg_producer);
}


/* Available in: thread. */
int tt_bmsg_send_timeout (TT_BMSG_QUEUE_T *msg_queue, void *msg_data, uint32_t msec)
{
	TT_BMSG_PRODUCER_T msg_producer;
	if (msg_queue == NULL || msg_data == NULL)
		return -1;
	
	msg_producer.msg_queue	= msg_queue;
	msg_producer.msg_data	= msg_data;

	return tt_pc_produce_timeout (&msg_queue->queue.pc_semaphore, tt_bmsg_produce, (void *)&msg_producer, msec);
}


/* Available in: irq, thread. */
int tt_bmsg_try_send (TT_BMSG_QUEUE_T *msg_queue, void *msg_data)
{
	TT_BMSG_PRODUCER_T msg_producer;
	if (msg_queue == NULL || msg_data == NULL)
		return -1;
	
	msg_producer.msg_queue	= msg_queue;
	msg_producer.msg_data	= msg_data;

	return tt_pc_try_produce (&msg_queue->queue.pc_semaphore, tt_bmsg_produce, (void *)&msg_producer);
}


/* Available in: irq. */
int tt_bmsg_can_send (TT_BMSG_QUEUE_T *msg_queue)
{
	return tt_pc_can_produce (&msg_queue->queue.pc_semaphore);
}


/* Available in: thread. */
void tt_bmsg_recv (TT_BMSG_QUEUE_T *msg_queue, void **msg_data)
{
	TT_BMSG_CONSUMER_T msg_consumer;
	if (msg_queue == NULL)
		return;
	
	msg_consumer.msg_queue	= msg_queue;
	msg_consumer.msg_data	= msg_data;

	tt_pc_consume (&msg_queue->queue.pc_semaphore, tt_bmsg_consume, (void *)&msg_consumer);	
}


/* Available in: thread. */
int tt_bmsg_recv_timeout (TT_BMSG_QUEUE_T *msg_queue, void **msg_data, uint32_t msec)
{
	TT_BMSG_CONSUMER_T msg_consumer;
	if (msg_queue == NULL)
		return -1;
	
	msg_consumer.msg_queue	= msg_queue;
	msg_consumer.msg_data	= msg_data;

	return tt_pc_consume_timeout (&msg_queue->queue.pc_semaphore, tt_bmsg_consume, (void *)&msg_consumer, msec);	
}


/* Available in: irq, thread. */
int tt_bmsg_try_recv (TT_BMSG_QUEUE_T *msg_queue, void **msg_data)
{
	TT_BMSG_CONSUMER_T msg_consumer;
	if (msg_queue == NULL)
		return -1;
	
	msg_consumer.msg_queue	= msg_queue;
	msg_consumer.msg_data	= msg_data;

	return tt_pc_try_consume (&msg_queue->queue.pc_semaphore, tt_bmsg_consume, (void *)&msg_consumer);	
}


/* Available in: irq. */
int tt_bmsg_can_recv (TT_BMSG_QUEUE_T *msg_queue)
{
	return tt_pc_can_consume (&msg_queue->queue.pc_semaphore);
}

/* Available in: irq, thread */
void tt_bmsg_recv_free (TT_BMSG_QUEUE_T *msg_queue, void *msg_data)
{
	TT_BMSG_T *msg	= GetParentAddr (msg_data, TT_BMSG_T, msg_data);
	memDel (msg_queue->queue.msg_buffer, &msg->list);	
}


#endif	// TT_SUPPORT_MSG









