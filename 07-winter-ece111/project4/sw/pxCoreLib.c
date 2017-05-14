/*
 * 
 *    Copyright Mentor Graphics Corporation 2004
 *    All Rights Reserved.
 *    Company Confidential
 *
 */


/* #include "support.h"
   #include "peripherals.h" */
#include "pxCoreLib.h"

unsigned int defaultIRQ = 0;
unsigned int defaultFIQ = 0;

#define MODE_SVC_32 0x13

/* NewState=1 will enable FIQ/IRQ, NewState=0 will disable FIQ/IRQ */
/* ARM core must be in a privileged mode, e.g. supervisor  */
void ChangeInterrupt(unsigned int NewState) {
    int tempCPSR;
    
    __asm {     
        /* get current program status */
        MRS tempCPSR, CPSR                 

        /* transform to supervisor mode (if possible) */
        ORR tempCPSR, tempCPSR, #MODE_SVC_32
        MSR CPSR_c, tempCPSR
        
        /* clear IRQ disable bit in the status register */ 
        AND tempCPSR, tempCPSR, #ENABLE_IRQ 

        /* set IRQ bit with new value */   
        ORR tempCPSR, tempCPSR, NewState, LSL #7   

        /* clear FIQ disable bit in the status register */ 
        AND tempCPSR, tempCPSR, #ENABLE_FIQ    

        /* reset FIQ bit with new value */   
        ORR tempCPSR, tempCPSR, NewState, LSL #6   

        /* store updated program status */    
        MSR CPSR_c, tempCPSR                      

    }
    return;

}


/* Enable all the interrupts in the CPU */
int pxEnableInterrupts () {
    /* 
     * For the ARM processors, we need to set the FIQ disable and IRQ disable
     * bits to 0 in the CPSR in order to enable both of them.
     */
    /* printToPort(0, "Started  changing the interrupt .."); */
    ChangeInterrupt(0);
    /* printToPort(0, "Finished changing the interrupt .."); */
    return 0;
}

/* Enable all the interrupts in the CPU */
int pxSetIsr (void *isr) {
    int status = 0;
    unsigned int  result = (unsigned int) isr;
    unsigned int  opcode = 0xEA000000U;
    unsigned int fiq = result - FIQ_vector;
    unsigned int irq = result - IRQ_vector;
    unsigned int check_fiq;
    unsigned int check_irq;
    fiq -= 8;
    irq -= 8;

    fiq = fiq >> 2;
    irq = irq >> 2;

    fiq += opcode;
    irq += opcode;
    
    defaultIRQ = 0;
    defaultFIQ = 0;
    __asm {
        /* Load the instruction at FIQ_vector into A3 */
        LDR R3, [FIQ_vector]
        LDR R4, [IRQ_vector]

        /* Save the instruction in the defaultISR global variable */
        ORR defaultFIQ, R3, #0x00000000
        ORR defaultIRQ, R4, #0x00000000

        /* Store the new opcode in the proper place */
        STR fiq, [FIQ_vector]
        STR irq, [IRQ_vector]

        /* Compare the stored values */
        LDR check_fiq, [FIQ_vector]
        LDR check_irq, [IRQ_vector]
        
    }
    
    if (check_fiq != fiq || check_irq != irq)
        return 1;

    return status;
}

/* Enable all the interrupts in the CPU */
int pxRestoreDefaultIsr () {
    __asm {
        STR defaultFIQ, [FIQ_vector]
        STR defaultIRQ, [IRQ_vector]
    }
    return 0;
    
}


/* Enable all the interrupts in the CPU */
int pxDisableInterrupts () {
    /* 
     * For the ARM processors, we need to set the FIQ disable and IRQ disable
     * bits to 1 in the CPSR in order to disable both of them.
     */
    /* printToPort(0, "Started  changing the interrupt .."); */
    ChangeInterrupt(1);
    /* printToPort(0, "Finished changing the interrupt .."); */
    return 0;
}


