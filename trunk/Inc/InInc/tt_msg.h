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
int tt_pc_try_produce (TT_PC_T *pc, void (*produce) (void *arg), void *arg);
int tt_pc_can_produce (TT_PC_T *pc);
void tt_pc_consume (TT_PC_T *pc, void (*consume) (void *arg), void *arg);
int tt_pc_try_consume (TT_PC_T *pc, void (*consume) (void *arg), void *arg);
int tt_pc_can_consume (TT_PC_T *pc);




/* Procedure message */
typedef void (*FUN_TT_MSG_PROC) (void *msg_data);

typedef struct tagTT_MSG_T
{
	LIST_T			list;
	FUN_TT_MSG_PROC	msg_proc;
	void			*msg_data;
} TT_MSG_T;

typedef struct
{
	LIST_T		*msg_free;
	LIST_T		msg_used;
	int			max_msg_limit;
	void		*msg_buffer;
	size_t		msg_buffer_size;
	TT_PC_T		pc_semaphore;
} TT_MSG_QUEUE_T;


TT_MSG_QUEUE_T *tt_msg_queue_init (void *msg_buffer, size_t msg_buffer_size);
void tt_msg_send (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC msg_proc, void *msg_data);
int tt_msg_try_send (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC msg_proc, void *msg_data);
int tt_msg_can_send (TT_MSG_QUEUE_T *msg_queue);
void tt_msg_recv (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC *msg_proc, void **msg_data);
int tt_msg_try_recv (TT_MSG_QUEUE_T *msg_queue, FUN_TT_MSG_PROC *msg_proc, void **msg_data);
int tt_msg_can_recv (TT_MSG_QUEUE_T *msg_queue);

#define TT_MSG_BUFFER_SIZE(max_msg_num) \
	(sizeof (TT_MSG_QUEUE_T) + MEMORYPOOL_SIZE (sizeof (TT_MSG_T), max_msg_num))



/* Block message */
typedef struct
{
	LIST_T			list;
	size_t			block_msg_size;
	int				block_msg_data[1];
} TT_BLOCK_MSG_T;

TT_MSG_QUEUE_T *tt_bmsg_queue_init (void *msg_buffer, size_t msg_buffer_size, size_t block_msg_size);
void tt_bmsg_send (TT_MSG_QUEUE_T *msg_queue, void *block_msg_data, size_t block_msg_size);
int tt_bmsg_try_send (TT_MSG_QUEUE_T *msg_queue, void *block_msg_data, size_t block_msg_size);
void tt_bmsg_recv (TT_MSG_QUEUE_T *msg_queue, void **block_msg_data, size_t *block_msg_size);
int tt_bmsg_try_recv (TT_MSG_QUEUE_T *msg_queue, void **block_msg_data, size_t *block_msg_size);
void tt_bmsg_recv_ok (TT_MSG_QUEUE_T *msg_queue);


#define TT_BLOCK_MSG_BUFFER_SIZE(max_msg_num,block_msg_size) \
	( \
		sizeof (TT_MSG_QUEUE_T) + \
		sizeof (LIST_T) + \
		MEMORYPOOL_SIZE ((block_msg_size) + GetOffsetSize (TT_BLOCK_MSG_T, block_msg_data)), max_msg_num) \
	)


#ifdef __cplusplus
} // extern "C"
#endif

#endif	// TT_SUPPORT_MSG
#endif	// INC__TT_MSG_H__
