#include "../Inc/tt_thread.h"

TT_THREAD_T *g_thread_current;
TT_THREAD_T *g_thread_next;

// Input
//	R0, arg: argument used by on_schedule
//	R1, on_shedule: function call when scheduled
// Return
//	None
#if 0
__attribute__ ((noinline)) void tt_syscall  (void *arg, void (*on_schedule) ())
{
	register int r0 __asm__ ("r0") = (int)arg;
	register int r1 __asm__ ("r1") = (int)on_schedule;
	__asm__ volatile (
		"	SVC		0x18					\n"	// Call SVC
		:
		: "r"(r0), "r"(r1)
	);
}
#endif


// Implement in tt_time.c
//void SysTick_Handler ();

void SVC_Handler (void *arg, void (*on_schedule) ())
{
	on_schedule (arg);
}

__attribute__ ((naked)) void PendSV_Handler ()
{
	__asm__ volatile (
		"switch_start:						\n"
		// Backup next thread address to R12
		"	LDR		R0, =g_thread_current	\n"	// Get pointer (current)
		"	LDR		R1, =g_thread_next		\n"	// Get pointer (next)

		"	LDR		R1, [R1]				\n"// Load next thread address
		"	MOV		R12, R1					\n"// Save to R12

		"	LDR		R2, [R0]				\n"// Load current thread address
		"	STR		R1, [R0]				\n"// g_thread_current = g_thread_next
		"	TST		R2, R2					\n"
		"	BEQ		switch_next				\n"

		// Save registers to stack, the registers in stack would be
		//	(H->L)
		//	xPSR, ReturnAddress(), LR(R14), R12, R3, R2, R1, R0
		//	R7, R6, R5, R4,
		//	R11, R10, R9, R8,
		//	Current LR(R14) 
		"	MRS		R0, PSP					\n"
		"	SUB		R0, #4 * 9				\n"
		"	STR		R0, [R2]				\n"// Save PSP to thread handler

		"	MOV		R3, R9					\n"
		"	MOV		R2, R8					\n"
		"	MOV		R1, LR					\n"
		"	STM		R0!, {R1 - R3}			\n"
	
		"	MOV		R3, R11					\n"
		"	MOV		R2, R10 				\n"
		"	STM		R0!, {R2 - R7}			\n"

		"switch_next:						\n"
		// Load next thread SP
		"	MOV		R1, R12					\n"
		"	TST		R1, R1					\n"
		"	BEQ		idle_wait				\n"
		"	LDR		R0, [R1]				\n"// 1st element is PSP

		// Restore registers from stack
		"	LDM		R0!, {R3 - R7}			\n"
		// R3 is Current LR now
		"	MOV		R8, R4					\n"
		"	MOV		R9, R5					\n"
		"	MOV		R10, R6					\n"
		"	MOV		R11, R7					\n"
		"	LDM		R0!, {R4 - R7}			\n"

		"	MSR		PSP, R0					\n"

		"	BX		R3						\n"// R3 is Current LR here

		"idle_wait:							\n"
		"	WFI								\n"// Wait for interrupt
		"	B		switch_start			\n"
	);
}

