#ifndef	INC__TT_MSG_H__
#define	INC__TT_MSG_H__

#ifdef	TT_SUPPORT_MSG

#ifdef __cplusplus
extern "C" {
#endif


/* Producer-consumer */
typedef struct
{
	TT_SEM_T producer;
	TT_SEM_T consumer;
} TT_PC_T;

void tt_pc_init (TT_PC_T *pc, int max_produce_count);
void tt_pc_produce (TT_PC_T *pc, void (*produce) (void *arg), void *arg);
int tt_pc_produce_timeout (TT_PC_T *pc, void (*produce) (void *arg), void *arg, uint32_t msec);
int tt_pc_try_produce (TT_PC_T *pc, void (*produce) (void *arg), void *arg);
int tt_pc_can_produce (TT_PC_T *pc);
void tt_pc_consume (TT_PC_T *pc, void (*consume) (void *arg), void *arg);
int tt_pc_consume_timeout (TT_PC_T *pc, void (*consume) (void *arg), void *arg, uint32_t msec);
int tt_pc_try_consume (TT_PC_T *pc, void (*consume) (void *arg), void *arg);
int tt_pc_can_consume (TT_PC_T *pc);


/* Message queue */
typedef struct
{
	LIST_T		*msg_free;
	LIST_T		msg_used;
	int			max_msg_limit;
	void		*msg_buffer;
	size_t		msg_buffer_size;
	size_t		each_msg_size;
	TT_PC_T		pc_semaphore;
} TT_MSG_QUEUE_T;

/* Procedure message */
typedef void (*FUN_TT_MSG_PROC) (void *msg_data);

typedef struct tagTT_MSG_T
{
	LIST_T			list;
	FUN_TT_MSG_PROC	msg_proc;
	void			*msg_data;
} TT_MSG_T;


TT_MSG_QUEUE_T *tt_msg_queue_init (void *msg_buffer, size_t msg_buffer_size);
void tt_msg_send (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC msg_proc, void *msg_data);
int tt_msg_send_timeout (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC msg_proc, void *msg_data, uint32_t msec);
int tt_msg_try_send (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC msg_proc, void *msg_data);
int tt_msg_can_send (TT_MSG_QUEUE_T *msg_queue);
void tt_msg_recv (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC *msg_proc, void **msg_data);
int tt_msg_recv_timeout (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC *msg_proc, void **msg_data, uint32_t msec);
int tt_msg_try_recv (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC *msg_proc, void **msg_data);
int tt_msg_can_recv (TT_MSG_QUEUE_T *msg_queue);

#define TT_MSG_BUFFER_SIZE(max_msg_num) \
	(sizeof (TT_MSG_QUEUE_T) + MEMORYPOOL_SIZE (sizeof (TT_MSG_T), max_msg_num))


/* Block message queue */
typedef struct
{
	TT_MSG_QUEUE_T	queue;
	size_t			each_msg_size;
} TT_BMSG_QUEUE_T;

/* Block message */
typedef struct
{
	LIST_T			list;
	int				msg_data[1];	/* size of msg_data is defined by tt_bmsg_queue_init */
} TT_BMSG_T;

TT_BMSG_QUEUE_T *tt_bmsg_queue_init (void *msg_buffer, size_t msg_buffer_size, size_t each_msg_size);
void tt_bmsg_send (TT_BMSG_QUEUE_T *msg_queue, void *msg_data);
int tt_bmsg_send_timeout (TT_BMSG_QUEUE_T *msg_queue, void *msg_data, uint32_t msec);
int tt_bmsg_try_send (TT_BMSG_QUEUE_T *msg_queue, void *msg_data);
int tt_bmsg_can_send (TT_BMSG_QUEUE_T *msg_queue);
void tt_bmsg_recv (TT_BMSG_QUEUE_T *msg_queue, void **msg_data);
int tt_bmsg_recv_timeout (TT_BMSG_QUEUE_T *msg_queue, void **msg_data, uint32_t msec);
int tt_bmsg_try_recv (TT_BMSG_QUEUE_T *msg_queue, void **msg_data);
int tt_bmsg_can_recv (TT_BMSG_QUEUE_T *msg_queue);
void tt_bmsg_recv_free (TT_BMSG_QUEUE_T *msg_queue, void *msg_data);


#define TT_BLOCK_MSG_BUFFER_SIZE(max_msg_num,each_msg_size) \
	( \
		sizeof (TT_BMSG_QUEUE_T) + \
		sizeof (LIST_T) + \
		MEMORYPOOL_SIZE ((GetOffsetSize(TT_BLOCK_MSG_T, msg_data) + (each_msg_size)), (max_msg_num)) \
	)


#ifdef __cplusplus
} // extern "C"
#endif

#endif	// TT_SUPPORT_MSG
#endif	// INC__TT_MSG_H__
