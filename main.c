/******************************************************************************\
 main.c
DSP2 - Sinus generator
\******************************************************************************/

// arm-elf-objdump >> text.txt

/******************************************************************************\
 Include files
\******************************************************************************/ 
#include "LPC214x.h" 
#include "leds.h"
#include "pll.h" 
#include "vic.h" 

#include "keys.h"
#include "serial.h"
#include "printf.h"

#include "sin.h"

/******************************************************************************\
 Initialize timer 1
\******************************************************************************/

extern  void __enable_interrupts(); 
extern  void __disable_interrupts(); 

int freq_deler = 1;




/******************************************************************************\
 MyTimerInterrupt
\******************************************************************************/
extern  void MyTimerInterrupt()	// function name used as handle in Vicsetup
{
	static unsigned int DAC_Value = 0;
	static unsigned int index = 0;
	
	DACR=(DAC_Value<<7)&0xFFC0;
	index += freq_deler;			// Decimatie
	index = index&0x7FF;
	DAC_Value = sin[index];	


	
	T0IR = 0xFF;					// clear interrupt flags
}
/******************************************************************************\ 
 Initialize timer 1
 (interrupt source 5) to vic slot 0
 Initialize timer 0
 (interrupt source 4) to vic slot 1
 
\******************************************************************************/
void InitTimer()
{
	__disable_interrupts();
	


// Initialise timer0
	T0TCR = 0;						// stop and reset counter and prescaler
	T0PR  = 60;		// prescale counter
	T0MR0 = 50;	// max counter value
	T0MCR = 1<<1 | 1<<0;			// reset and interrupt on match
	T0IR = 0xFF;					// clear interrupt flags
	T0TC = 0;						// clear countvalue!
	VicSetup((unsigned)MyTimerInterrupt,IRQ,4,0);

	T0TCR = 1<<0;					//go timer 0 and reset

	VICVectAddr = 0xFF;				// update priority hardware spooling

	 __enable_interrupts(); 
}

/******************************************************************************\ 
 main
\******************************************************************************/
extern int main()
{ 
	/*   init libs   */
	LED_init();
	PLL_init(); 			// make sure you know to what frequency!!!
	/*  Init timer   */
	
	Keys_Init();
	int keys;
	int i =0;
	
	init_serial0(115200);
	putstring_serial0("init done\r\n");
	printf("printf werkt\r\n\r\n");
	

	printf("DSP1 - Sinus generator - exp branch\r\n\r\n");

		
	PINSEL1 |= 1<<19;
	PINSEL1 &= ~(1<<18);
		
	InitTimer();			// init the timers and GO
	/*
	 * Main loop
	 */
	while(1) 	 
	{

		
		keys = Keys_Read();
		if(keys == key_s)
		{
			keys = 0;
			if (freq_deler < 511) freq_deler++;
			printf("deler: %u \r\n",freq_deler);
		}
		if(keys == key_h)
		{
			keys = 0;
			if (freq_deler > 1) freq_deler--;
			printf("deler: %u \r\n",freq_deler);
		}
			
	}
	 
	while(1);
	return 0;								// don't ever come near this
}



