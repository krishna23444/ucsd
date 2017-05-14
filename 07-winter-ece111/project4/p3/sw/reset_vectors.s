	AREA	reset_vectors, CODE, READONLY

	ENTRY
Boot
	IMPORT	Reset_Handler
	IMPORT	Undefined_Handler
	IMPORT	SWI_Handler
	IMPORT	Prefetch_Handler
	IMPORT	Abort_Handler
;	IMPORT	FIQ_Handler
;	IMPORT	IRQ_Handler
	EXPORT	Boot

	IMPORT interrupt_handler
;IMPORT |Image$$RO$$Base| ; Start of ROM 

	B	Reset_Handler
	B	Undefined_Handler
	B	SWI_Handler
	B	Prefetch_Handler
	B	Abort_Data_Handler
	NOP			; Reserved vector
	B	IRQ_Handler
	B	FIQ_Handler

	ALIGN

		;; FIQ_Stack		EQU	0x30008800 ;; These values are defined on the assembler
		;; IRQ_Stack		EQU	0x30008c00 ;; command line in Platform Express

FIQ_Handler
	LDR	R13,	=FIQ_Stack
	STMDB	R13!,{R0-R12,R14}
        MOV  	R0,#0
	B	FIQ_IRQ
	
IRQ_Handler
	LDR	R13,	=IRQ_Stack
	STMDB	R13!,{R0-R12,R14}

        MOV    	R0,#1

FIQ_IRQ
;       LDR    R2,   =|Image$$RO$$Base| ; Get pointer to ROM
        MOV    R2,   #&0
        LDR    R1,   =%100
	ORR    R2, R1, R2               ; fix ROM address   
        MOV    PC,R2			; set PC to ROM area
100
	BL	interrupt_handler	; run C subprogram
	LDMIA	R13,{R0-R12,R14}
	SUBS    PC,R14,#4	        ; return from Interrupt
	
Abort_Data_Handler
	SUBS PC, R14, #4             ; return from interrupt


        
	AREA    irq_vectors2, DATA, READWRITE
        %       1024                      ;1024 words for Base stack
FIQ_Stack
        %       1024                      ;1024 words for Base stack
IRQ_Stack
        %       1024                      ;1024 words for Base stack
ABORT_Stack


	END


