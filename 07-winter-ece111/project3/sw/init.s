Mode_Bits               EQU    0x1F
Mode_FIQ                EQU    0x11
Mode_IRQ                EQU    0x12
Mode_SVC                EQU    0x13
Mode_USR                EQU    0x10

I_Bit                   EQU    0x80
F_Bit                   EQU    0x40


   IMPORT  pxEnableInterrupts
   IMPORT  pxDisableInterrupts
   IMPORT  pxSetIsr
;   IMPORT  pxRestoreDefaultIsr

	AREA	Init2, CODE, READONLY

	EXPORT	Reset_Handler
	EXPORT	Undefined_Handler
	EXPORT	SWI_Handler
	EXPORT	Prefetch_Handler
	EXPORT	Abort_Handler

	EXPORT	Error


Undefined_Handler
	B	Undefined_Handler

SWI_Handler
	B	SWI_Handler

Prefetch_Handler
	B	Prefetch_Handler

Abort_Handler
	B	Abort_Handler

Error
	B	Error


Reset_Handler ROUT
	        
        ; enable interrupts (note, interrupt controller not yet
        ; enabled, so interrupts still cannot occur)
        ; and enter SVC Mode

        MRS    R0,   CPSR
        BIC    R0,   R0,   #(I_Bit + F_Bit)
        BIC    R0,   R0,   #Mode_Bits
        ORR    R0,   R0,   #Mode_SVC
        MSR    CPSR_c, R0

        ; initialise memory required by c code
;  IMPORT |Image$$RO$$Limit| ; End of ROM code (= start of ROM data) 
;  IMPORT |Image$$RO$$Base| ; End of ROM code (= start of ROM data) 
;       IMPORT |Image$$RW$$Base|	; Base of RAM to initalise
;       IMPORT |Image$$ZI$$Base|	; End of data and beginning of zeros
;       IMPORT |Image$$ZI$$Limit| ; End of area to zero initalise

;       LDR    R0,   =|Image$$RO$$Limit| ; Get pointer to ROM data
;       LDR    R1,   =|Image$$RW$$Base| ; and RAM copy
;       LDR    R3,   =|Image$$ZI$$Base| ; top of initalised data

	   
;	copy Exception vectors and IRQ/FIQ handlers from ROM to RAM ( 0x00000000 )
;	
;       LDR    R0,   =|Image$$RO$$Base| ; Get pointer to ROM data
;       LDR    R0,   Reset_Handler
	MOV    R0, #&0			; start adrress distination
	MOV    R1, #&0			; start adrress distination
	MOV    R3, #&60			; nuber coping words
	CMP    R0,   R1			; if RAM data start and end are = then no data
        BEQ    %1
0       CMP    R1,   R3			; Copy ROM data to RAM
	LDMCCIA R0!, {R4-R13}		; accelerate the process
	STMCCIA R1!, {R4-R13}		; accelerate the process
        BCC    %0

1
; Setup the TCM interface
; TCM Base address setup externally in Platform Express

; glp: temporarily remove TCM setup until rev2 comes out for the arm966E_S

;	LDR r1, tmp1
;	MRC	p15, 0, r0, c9, c1, 0			; Read dTCM dRegion reg
;	ORR	r0, r0, r1; 					dTCM base addr. - defined in PX
;	MCR	p15, 0, r0, c9, c1, 0			; Write dTCM dRegion


;	LDR r1, tmp2
;	MRC	p15, 0, r0, c9, c1, 1			; Read iTCM iRegion reg
;	ORR	r0, r0, r1						; iTCM base addr. - defined in PX
;	MCR	p15, 0, r0, c9, c1, 1			; Write iTCM iRegion

        IMPORT  TestMain

|__TestMain|

        ;Enter IRQ Mode and set up its stack
        MRS     r0, CPSR                        ;get CPSR
        BIC     r0, r0, #Mode_Bits
        ORR     r1, r0, #Mode_IRQ
        MSR     CPSR_c, r1                      ;Enter IRQ Mode
        MOV     r2, #Mode_IRQ
        MSR     SPSR_c, r2                      ;Set SPSR to User Mode
        ldr     sp, =IRQStackBase               ;Setup IRQ stack pointer

        ;Enter FIQ Mode and set up its stack
        MRS     r0, CPSR                        ;get CPSR
        BIC     r0, r0, #Mode_Bits
        ORR     r1, r0, #Mode_FIQ
        MSR     CPSR_c, r1                      ;Enter IRQ Mode
        MOV     r2, #Mode_FIQ
        MSR     SPSR_c, r2                      ;Set SPSR to User Mode
        ldr     sp, =FIQStackBase               ;Setup IRQ stack pointer
        

        ;Enter SVC Mode and set up its stack
        MRS     r0, CPSR                        ;get CPSR
        BIC     r0, r0, #Mode_Bits
        ORR     r1, r0, #Mode_SVC
        MSR     CPSR_c, r1                      ;Enter IRQ Mode
        MOV     r2, #Mode_SVC
        MSR     SPSR_c, r2                      ;Set SPSR to User Mode
        ldr     sp, =SVCStackBase               ;Setup IRQ stack pointer

        BL      pxDisableInterrupts

        BL      pxEnableInterrupts

;        LDR     sp, =BaseStack             ; Initial stack pointer. - defined in PX
;        MOV     fp, #0                     ; No previous frame, so fp=0
	
        LDR     sp, =USRStackBase             ; Initial stack pointer. 

        MOV     a1, #0                     ; and argc to NUL
        MOV     a2, #0                     ; and argv to NUL
        BL      TestMain                   ; Call main, falling through to
                                           ; exit on return.


    IMPORT Boot
        B       Boot

        AREA    Stacks, DATA, READWRITE
        %       128                      ;64 words for IRQ stack
SVCStackBase
        %       128                      ;64 words for IRQ stack
IRQStackBase
        %       128                      ;64 words for IRQ stack
FIQStackBase
        %       128                      ;64 words for USR stack
USRStackBase

; 	AREA    Stacks, DATA, READWRITE 
;        %       64                      ;64 words for Base stack
;BaseStack

        END


