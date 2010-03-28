;/*---------------------------------------------------------------------------------------------------------*/
;/*                                                                                                         */
;/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
;/*                                                                                                         */
;/*---------------------------------------------------------------------------------------------------------*/

				THUMB

; Vector Table Mapped to Address 0 at Reset
                AREA    |.text|, CODE, READONLY
              
; void tt_syscall(void *arg, void (*on_schedule)(void *));
; Input
;	R0, arg: argument used by on_schedule
;	R1, on_shedule: function call after before scheduled
; Return
;	None
tt_syscall		PROC
				EXPORT tt_syscall
				SVC		0x18	; Call SVC
				BX lr
				ENDP

SVC_Handler     PROC
                EXPORT  SVC_Handler

				; Check if the interrupt comes from thread or not
				; If SVC command is called in another interrupt handler,
				; it does not save and restore thread context.
				; 
				; SVC called by interrupt handler
				;	1. Call void on_schedule(void *)
				; SVC called by thread
				;	1. Save thread context for g_thread_current
				;	2. Call void on_schedule(void *)
				;	3. Restore thread context from g_thread_next
				MOV		R3, LR
				MOVS	R2, #8
				TST		R3, R2
				BNE		Thread_Switch_Call
				PUSH	{LR}
				BLX		R1
				POP		{PC}

Thread_Switch_Call
				; Save registers to stack, the registers in stack would be
				;	(H->L)
				;	xPSR, ReturnAddress(), LR(R14), R12, R3, R2, R1, R0
				;	R7, R6, R5, R4,
				;	R11, R10, R9, R8,
				;	Current LR(R14) 
				MRS		R2, PSP
				SUBS	R2, #4 * 4
				STM		R2!, {R4 - R7}
				SUBS	R2, #4 * 9
				MOV		R7, R11
				MOV		R6, R10
				MOV		R5, R9
				MOV		R4, R8
				MOV		R3, LR
				STM		R2!, {R3 - R7}
				SUBS	R2, #4 * 5

				; Save current PSP to thread control block
				IMPORT	g_thread_current
				IMPORT	g_thread_next
				LDR		R3, =g_thread_current	; Get pointer
				LDR		R3, [R3]				; Get the thread handler
				STR		R2, [R3]				; 1st element is PSP
				
				; Call function on_schedule(arg)
				BLX		R1
				
				; Load next thread's SP
				LDR		R3, =g_thread_current
				LDR		R2, =g_thread_next
				LDR		R2, [R2]
				STR		R2, [R3]				; Set new current thread
				LDR		R2, [R2]				; 1st element is PSP
				
				; Restore registers from stack
				LDM		R2!, {R3 - R7}
				; R3 is Current LR now
				MOV		R8, R4
				MOV		R9, R5
				MOV		R10, R6
				MOV		R11, R7
				LDM		R2!, {R4 - R7}
				
				MSR		PSP, R2					
				
				BX		R3	; R3 is Current LR here
				ENDP

SysTick_Handler PROC
                EXPORT  SysTick_Handler
				;MOV		R0, LR
				;LDR		R1, =Old_SysTick_Handler
				;BLX		R1
				;MOV		LR, R0
				IMPORT	__tt_on_timer
				LDR		R1, =__tt_on_timer
				B		Thread_Switch_Call
				ENDP

                END
