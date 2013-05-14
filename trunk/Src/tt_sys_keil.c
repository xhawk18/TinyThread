#include "../Inc/tt_thread.h"

TT_THREAD_T *g_thread_current;
TT_THREAD_T *g_thread_next;


// Input
//	R0, arg: argument used by on_schedule
//	R1, on_shedule: function call when scheduled
// Return
//	None
#if 0
__asm void tt_syscall (void *arg, void (*on_schedule) ())
{
	SVC		0x18	; Call SVC
	BX lr
}
#endif



// Implement in tt_time.c
//void SysTick_Handler ();

void SVC_Handler (void *arg, void (*on_schedule) ())
{
	on_schedule (arg);
}


__asm void PendSV_Handler ()
{
	PRESERVE8

	IMPORT	g_thread_current
	IMPORT	g_thread_next

switch_start	
	; Backup next thread address to R12
	LDR		R0, =g_thread_current	; Get pointer (current)
	LDR		R1, =g_thread_next		; Get pointer (next)
	
	LDR		R1, [R1]				; Load next thread address
	MOV		R12, R1					; Save to R12

	LDR		R2, [R0]				; Load current thread address
	STR		R1, [R0]				; g_thread_current = g_thread_next
	TST		R2, R2
	BEQ		switch_next

	; Save registers to stack, the registers in stack would be
	;	(H->L)
	;	xPSR, ReturnAddress(), LR(R14), R12, R3, R2, R1, R0
	;	R7, R6, R5, R4,
	;	R11, R10, R9, R8,
	;	Current LR(R14) 
	MRS		R0, PSP
	SUBS	R0, #4 * 9
	STR		R0, [R2]				; Save PSP to thread handler

	MOV		R3, R9
	MOV		R2, R8
	MOV		R1, LR
	STM		R0!, {R1 - R3}
	
	MOV		R3, R11
	MOV		R2, R10 
	STM		R0!, {R2 - R7}

switch_next
	; Load next thread SP
	MOV		R1, R12
	TST		R1, R1
	BEQ		idle_wait
	LDR		R0, [R1]				; 1st element is PSP
				
	; Restore registers from stack
	LDM		R0!, {R3 - R7}
	; R3 is Current LR now
	MOV		R8, R4
	MOV		R9, R5
	MOV		R10, R6
	MOV		R11, R7
	LDM		R0!, {R4 - R7}
		
	MSR		PSP, R0
				
	BX		R3						; R3 is Current LR here

idle_wait
	WFI								; Wait for interrupt
	B		switch_start
	
	ALIGN
}

