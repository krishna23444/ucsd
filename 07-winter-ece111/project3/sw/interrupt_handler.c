/* Copyright Mentor Graphics Corporation 2004

    All Rights Reserved.

 THIS WORK CONTAINS TRADE SECRET
 AND PROPRIETARY INFORMATION WHICH IS THE
 PROPERTY OF MENTOR GRAPHICS
 CORPORATION OR ITS LICENSORS AND IS
 SUBJECT TO LICENSE TERMS. 
*/
extern printToPort(unsigned int, char *);

void interrupt_handler(int interrupt_type)
{

   if (interrupt_type == 0) {
      printToPort(0, "FIQ received\n");
   } 

   if (interrupt_type == 1) {
      printToPort(0, "IRQ received\n");
   }

   interrupt_type = -1;
}



