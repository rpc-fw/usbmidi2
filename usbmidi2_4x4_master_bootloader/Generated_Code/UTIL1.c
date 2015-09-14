/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : UTIL1.c
**     Project     : usbmidi2_4x4_master_bootloader
**     Processor   : MKL26Z64VFM4
**     Component   : Utility
**     Version     : Component 01.109, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-07, 20:11, # CodeGen: 0
**     Abstract    :
**          Contains various utility functions.
**     Settings    :
**          Component name                                 : UTIL1
**     Contents    :
**         IsLeapYear - bool UTIL1_IsLeapYear(uint16_t year);
**
**     License   :  Open Source (LGPL)
**     Copyright : (c) Copyright Erich Styger, 2014, all rights reserved.
**     xatoi(): Copyright (C) 2010, ChaN, all right reserved. (see copyright notice and license at the function implementation).
**     This an open source software implementing utility functions using Processor Expert.
**     This is a free software and is opened for education,  research and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for
**       personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file UTIL1.c
** @version 01.00
** @brief
**          Contains various utility functions.
*/         
/*!
**  @addtogroup UTIL1_module UTIL1 module documentation
**  @{
*/         

/* MODULE UTIL1. */

#include "UTIL1.h"

/* Internal method prototypes */
static void ShiftRightAndFill(uint8_t *dst, uint8_t fill, uint8_t nofFill);

/*
** ===================================================================
**     Method      :  ShiftRightAndFill (component Utility)
**
**     Description :
**         Moves the content of a string to the right and fills the space 
**         on the left side with a pattern
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void ShiftRightAndFill(uint8_t *dst, uint8_t fill, uint8_t nofFill)
{
  signed char i, j;

  j = 0;
  while(dst[j] != '\0') {
    j++;
  }
  i = (signed char)nofFill;
  if (i==j) {
    /* nothing to do, we are done */
  } else if (i>j) {
    while (j>=0) {
      dst[i] = dst[j];
      i--; j--;
    }
    while(i>=0) {
      dst[i] = fill;
      i--;
    }
  } else {
    /* hmmm, not enough space, return what we have, do nothing */
  }
}

/*
** ===================================================================
**     Method      :  UTIL1_IsLeapYear (component Utility)
**     Description :
**         Returns true if a given year is a leap year
**     Parameters  :
**         NAME            - DESCRIPTION
**         year            - Year, in the YYYY format.
**     Returns     :
**         ---             - If the year is a leap year or not.
** ===================================================================
*/
bool UTIL1_IsLeapYear(uint16_t year)
{
  return ((((year%4)==0) && (year%100)!=0) || (year%400)==0);
}

/* END UTIL1. */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.11]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/