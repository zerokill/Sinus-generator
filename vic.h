/******************************************************************************
* Filename		: 	Vic.c
* Description	: 	This file hold the header information used by the vectored 
*					interrupt controller
******************************************************************************/

#ifndef  __VIC_H__                                                                       
#define  __VIC_H__

//#include <config.h>


#define FIQ		1
#define IRQ		0



//prototypes

char VicSetup(unsigned int intHandler ,char intType, char intSource, char intVector);

void EnableVectorInt(char Vector);
void DisableVectorInt(char Vector);



#endif
