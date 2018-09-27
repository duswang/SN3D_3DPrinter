/*
 ============================================================================
 Name        : Nextion_Project
 Author      : bato
 Version     : 0.1
 Copyright   :
 Description :
 ============================================================================
 */
/*
 * main.c
 *
 *  Created on: Sep 18, 2018
 *      Author: bato
 */

#include <stdio.h>
#include <MODUELS.h>

int main(void)
{
  /* Init */
  MODUELS_DISPLAY_Serial_Init();
  MODUELS_3D_PRINTER_Serial_Init();



  while(1)
  {
      //Working
  }
}
