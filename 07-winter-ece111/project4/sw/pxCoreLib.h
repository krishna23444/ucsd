/* Copyright Mentor Graphics Corporation 2004

    All Rights Reserved.

 THIS WORK CONTAINS TRADE SECRET
 AND PROPRIETARY INFORMATION WHICH IS THE
 PROPERTY OF MENTOR GRAPHICS
 CORPORATION OR ITS LICENSORS AND IS
 SUBJECT TO LICENSE TERMS. 
*/
#ifndef __pxCoreLib_h
#define __pxCoreLib_h

#define RESET_vector      0x00000000U
#define UNDEFINED_vector  0x00000004U
#define SOFTWARE_vector   0x00000008U
#define PREFETCH_vector   0x0000000CU
#define ABORT_vector      0x00000010U
#define RESERVED_vector   0x00000014U
#define IRQ_vector        0x00000018U
#define FIQ_vector        0x0000001CU


#define DISABLE_FIQ       0x00000040U
#define ENABLE_FIQ        0xFFFFFFBFU
#define DISABLE_IRQ       0x00000080U
#define ENABLE_IRQ        0xFFFFFF7FU

/* Function prototypes */
extern int pxEnableInterrupts(void);
extern int pxDisableInterrupts(void);
extern int pxSetIsr(void*);
extern int pxRestoreDefaultIsr(void);

#endif /* defined (__pxCoreLib_h) */

