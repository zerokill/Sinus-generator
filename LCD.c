/******************************************************************************\
 	LCD.c
 	Code:				Maurice Daverveldt
 	Met behulp van:		Joost Thijssen
 	
 	Description:
 	LCD Library om HD44780 te besturen. LCD library werkt met elke HD44780 en
 	kan geconfigureert worden in de LCD_Init functie.
 	
 	Belangrijkste functies:
 	LCD_PrintS:			Print een complete string uit een pointer
 	LCD_PrintJ:			Print een complete string in het Japans als er 
 						compatible HD44780 aanwezig is
 	LCD_Print:			Print een bepaald CGRAM adress
 	
 	last update:		5 Juni 2008			
 	
\******************************************************************************/

/******************************************************************************\
 Application Includes
\******************************************************************************/

#include "LPC214x.h"
#include "LCD.h"

/******************************************************************************\
 LCD Send Functie zet alle instructies op de uitgangen
\******************************************************************************/

void LCD_Send(int LCD_SEND_VALUE, char LCD_SEND_MODE, int LCD_COMMANDTYPE)
{
	IO0CLR = LCD_CLEARLINES;						// maak alle lijnen laag
	IO0SET = (LCD_SEND_VALUE & LCD_MSB)<<5 | 		// zet MSB op uitgang
				LCD_PIN_CLOCK | LCD_COMMANDTYPE;	// geef clock en type commando
	IO0SET=0<<9;
	IO0SET=0<<9;
	IO0CLR=LCD_PIN_CLOCK;							
	IO0SET=0<<9;
	IO0SET=0<<9;
	IO0CLR = LCD_CLEARLINES;						// maak alle lijnen laag
	IO0SET = (LCD_SEND_VALUE & LCD_LSB)<<9 | 		// zet LSB op uitgang
				LCD_PIN_CLOCK | LCD_COMMANDTYPE;	// geef clock en type commando
	IO0SET=0<<9;
	IO0SET=0<<9;
	IO0CLR=LCD_PIN_CLOCK;
	IO0SET=0<<9;
	IO0SET=0<<9;	

	int LCD_Wait_Counter=2400;						// wacht functie als commando
	if(LCD_SEND_MODE)LCD_Wait_Counter=98400;		// slow is
	while(LCD_Wait_Counter>0)LCD_Wait_Counter--;			
}

/******************************************************************************\
 Init instructies
\******************************************************************************/

void LCD_Init(void)
{
	PINSEL0&=~0x3FFC0000;				// Deffineer LCD pinnen als GPIO
	IO0DIR|=0x7E00;						// Deffineer LCD pinnen als uitgang
	
	LCD_FunctionSet(LCD_4BIT_OP, LCD_2LINE, LCD_FNT_5X8);
	LCD_FunctionSet(LCD_4BIT_OP, LCD_2LINE, LCD_FNT_5X8);
	LCD_DisplayOnOff(LCD_ON, LCD_CURSOR_OFF, LCD_BLINK_OFF);
	LCD_EntryModeSet(LCD_POS_INC, LCD_POS_SHIFT_ON);
	LCD_Clear();
	
}

/******************************************************************************\
 Char en String print instructies
\******************************************************************************/

void LCD_PrintS(char *LCD_STRING, char LCD_Y, char LCD_X)
{
	LCD_SetPos(LCD_Y, LCD_X);							// zet cursor op juiste plek
	while(*LCD_STRING!='\0')
	{
		LCD_Send(*LCD_STRING, LCD_FAST, LCD_CHARACTER);	// verstuur char
		LCD_STRING++;
	}
}

void LCD_PrintJ(char *LCD_STRING, char LCD_Y, char LCD_X)
{
	LCD_SetPos(LCD_Y, LCD_X);							// zet cursor op juiste plek
	while(*LCD_STRING!='\0')
	{													// zorg dat chars in japanse adressen zitten
		LCD_Send(*LCD_STRING + 0x70, LCD_FAST, LCD_CHARACTER); 
		LCD_STRING++;
	}
}

void LCD_Print(char LCD_STRING, char LCD_Y, char LCD_X)
{
	LCD_SetPos(LCD_Y, LCD_X);							// zet cursor op juiste plek
	LCD_Send(LCD_STRING, LCD_FAST, LCD_CHARACTER);		// verstuur char
}

void LCD_SetPos(char LCD_X, char LCD_Y)
{
	if(LCD_Y==1)LCD_X+=0x40;
	if(LCD_Y==2)LCD_X+=0x14;
	if(LCD_Y==3)LCD_X+=0x54;
	LCD_SetDDRAM(LCD_X);
}

/******************************************************************************\
 Display instructies
\******************************************************************************/

void LCD_Clear(void)
{
	LCD_Send(LCD_VAL_CLEAR, LCD_SLOW, LCD_COMMAND);		// LCD Clear commando
}

void LCD_Home(void)
{
	LCD_Send(LCD_VAL_HOME, LCD_SLOW, LCD_COMMAND);		// LCD home commando
}

/******************************************************************************\
 Config instructie
 	zie HD44780 datasheet voor uitleg van iedere instructie
\******************************************************************************/

void LCD_EntryModeSet(char LCD_DIRECTION, char LCD_SHIFT_ENABLE)
{
	LCD_Send(LCD_VAL_ENTRYMODESET + LCD_DIRECTION*2 + LCD_SHIFT_ENABLE, LCD_FAST, LCD_COMMAND);
}

void LCD_DisplayOnOff(char LCD_ON_OFF, char LCD_CURSOR, char LCD_BLINK)
{
	LCD_Send(LCD_VAL_DISPLAYONOFF + LCD_ON_OFF*4 + LCD_CURSOR*2 + LCD_BLINK, LCD_FAST, LCD_COMMAND);
}

void LCD_CursorOrDisplayShift(char LCD_CURSORORDISPLAY, char LCD_SHIFT_LEFTRIGHT)
{
	LCD_Send(LCD_VAL_CURSORDISPLAY + LCD_CURSORORDISPLAY*8 + LCD_SHIFT_LEFTRIGHT*4, LCD_FAST, LCD_COMMAND);
}

void LCD_FunctionSet(char LCD_DATA_LENGTH, char LCD_NUM_LINES, char LCD_FNT)
{
	LCD_Send(LCD_VAL_FUNCTIONSET + LCD_DATA_LENGTH*16 + LCD_NUM_LINES*8 + LCD_FNT*4, LCD_FAST, LCD_COMMAND);
}

void LCD_SetCGRAM(char LCD_CGRAMADR)
{
	LCD_Send(LCD_VAL_SETCGRAM + LCD_CGRAMADR, LCD_FAST, LCD_COMMAND);
}

void LCD_SetDDRAM(char LCD_DDRAMADR)
{
	LCD_Send(LCD_VAL_SETDDRAM + LCD_DDRAMADR, LCD_FAST, LCD_COMMAND);
}










