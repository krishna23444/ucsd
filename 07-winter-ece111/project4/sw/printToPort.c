/* Copyright Mentor Graphics Corporation 2004

    All Rights Reserved.

 THIS WORK CONTAINS TRADE SECRET
 AND PROPRIETARY INFORMATION WHICH IS THE
 PROPERTY OF MENTOR GRAPHICS
 CORPORATION OR ITS LICENSORS AND IS
 SUBJECT TO LICENSE TERMS. 
*/
void printToPort (int port, char* s)
{
  /* int i; */

  static int x;
  x = 999; /* null line on which to set xray breakpoint */

#if 0
  for (i=0; s[i]!='\n'; i++)
  /* send all the characters to the specified port address */
  {
    *(unsigned char *)port = s[i];
  }
#endif
}


