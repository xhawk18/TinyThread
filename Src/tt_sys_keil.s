;/*---------------------------------------------------------------------------------------------------------*/
;/*                                                                                                         */
;/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
;/*                                                                                                         */
;/*---------------------------------------------------------------------------------------------------------*/

				PRESERVE8
				GBLL	TT_DEBUG_THREAD_SWITCH_TIME
TT_DEBUG_THREAD_SWITCH_TIME	SETL	{FALSE}

				IF TT_DEBUG_THREAD_SWITCH_TIME
				AREA    |.data|, DATA, READWRITE, ALIGN=2
g_ticks_before_schedule		DCD		0     
				ENDIF
				

				AREA    |.text|, CODE, READONLY
				THUMB             
; void tt_syscall(void *arg, void (*on_schedule)(void *));
; Input
;	R0, arg: argument used by on_schedule
;	R1, on_shedule: function call when scheduled
; Return
;	None
tt_syscall		PROC
				EXPORT tt_syscall
				SVC		0x18	; Call SVC
				BX lr
				ENDP

idle_thread		PROC
				WFI
				B		idle_thread
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

				; When enterring interrupt hander
				;    if CurrentMode==Mode_Handler then
				;        LR = 0xFFFFFFF1;
				;    else
				;    if CONTROL<1> == '0' then
				;        LR = 0xFFFFFFF9;
				;    else
				;        LR = 0xFFFFFFFD;
				MOV		R3, LR
				MOVS	R2, #8
				TST		R3, R2
				BEQ		on_schedule_in_handler

save_thread_context
				IMPORT	g_thread_current
				IMPORT	g_thread_next

				LDR		R3, =g_thread_current	; Get pointer
				LDR		R3, [R3]				; Get the thread handler
				TST		R3, R3
				BEQ		restore_MSP;

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
				STR		R2, [R3]			; Save PSP to thread handler
				MOV		R7, R11
				MOV		R6, R10
				MOV		R5, R9
				MOV		R4, R8
				MOV		R3, LR
				STM		R2!, {R3 - R7}

on_schedule
				; Call function on_schedule(arg)
				BLX		R1
				
				; Load next thread's SP
				LDR		R3, =g_thread_current
				LDR		R2, =g_thread_next
				LDR		R2, [R2]
				STR		R2, [R3]				; Set new current thread
				TST		R2, R2
				BEQ		call_idle_thread
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
				
				IF TT_DEBUG_THREAD_SWITCH_TIME
				LDR		R0, =0xE000E018
				LDR		R0,[R0]
				LDR		R1, = g_ticks_before_schedule
				LDR		R1, [R1]
				SUBS	R0, R1, R0		; Now R0 is the thread switch time used
				ENDIF
				
				BX		R3	; R3 is Current LR here

restore_MSP
				; Idle thread share stack(MSP) with irq handler,
				; and in fact it does not need to save any stack content
				; since all things are fixed.
				; So we restore MSP here
				ADD		SP, #0x20
				B		on_schedule 
call_idle_thread
				; Construct stack on MSP for idle thread
				;	xPSR, ReturnAddress(), LR(R14), R12, R3, R2, R1, R0
				SUB		SP, #0x20
				LDR		R0, =idle_thread
				STR		R0, [SP, #0x14]	;LR
				STR		R0, [SP, #0x18]	;PC
				LDR		R0, =0x01000000
				STR		R0, [SP, #0x1C]	;xPSR
				LDR		R0, =0xFFFFFFF9
				BX		R0	; R1 is Current LR here
on_schedule_in_handler
				PUSH	{LR}
				BLX		R1
				POP		{PC}

				ENDP

SysTick_Handler PROC
                EXPORT  SysTick_Handler
				;MOV		R0, LR
				;LDR		R1, =Old_SysTick_Handler
				;BLX		R1
				;MOV		LR, R0

				IF TT_DEBUG_THREAD_SWITCH_TIME
				LDR		R0, =0xE000E018
				LDR		R0,[R0]
				LDR		R1, = g_ticks_before_schedule
				STR		R0, [R1]
				ENDIF

				; Will call __tt_on_timer
				IMPORT	__tt_on_timer
				LDR		R1, =__tt_on_timer

				; Check if the interrupt comes from thread or not
				; If SysTick is from another interrupt handler,
				; it does not save and restore thread context.
				; 
				; SysTick exception from interrupt handler
				;	1. Call void on_schedule(void *)
				; SysTick exception  from thread
				;	1. Save thread context for g_thread_current
				;	2. Call void on_schedule(void *)
				;	3. Restore thread context from g_thread_next

				; When enterring interrupt hander
				;    if CurrentMode==Mode_Handler then
				;        LR = 0xFFFFFFF1;
				;    else
				;    if CONTROL<1> == '0' then
				;        LR = 0xFFFFFFF9;
				;    else
				;        LR = 0xFFFFFFFD;
				MOV		R3, LR
				MOVS	R2, #8
				TST		R3, R2
				BEQ		on_schedule_in_handler

				B		save_thread_context
				ENDP

sysIsInIRQ		PROC
				EXPORT	sysIsInIRQ
				MRS		R0, IPSR
				BX		LR
				ENDP				

				ALIGN
				END
