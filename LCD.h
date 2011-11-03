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

#define LCD_MSB 					0xF0
#define LCD_LSB 					0x0F
#define LCD_CLEARLINES				(0x3F<<9)
#define LCD_PIN_CLOCK				(1<<13)

#define LCD_COMMAND					(0<<14)
#define LCD_CHARACTER				(1<<14)

#define LCD_FAST					0
#define LCD_SLOW					1

//LCD Entry Mode Set
#define LCD_POS_INC					1
#define LCD_POS_DEC					0
#define LCD_POS_SHIFT_ON			0
#define LCD_POS_SHIFT_OFF			1

//LCD Display On Off
#define LCD_ON						1
#define LCD_OFF						0
#define LCD_CURSOR_ON				1
#define LCD_CURSOR_OFF				0
#define LCD_BLINK_ON				1
#define LCD_BLINK_OFF				0

//LCD FunctionSet
#define LCD_8BIT_OP					1
#define LCD_4BIT_OP					0
#define LCD_2LINE					1
#define LCD_1LINE					0
#define LCD_FNT_5X10				1
#define LCD_FNT_5X8					0

//LCD CursorOrDisplay Shift
#define LCD_DISPLAY_S				1
#define LCD_CURSOR_S				0
#define	LCD_SHIFT_RIGHT				1
#define	LCD_SHIFT_LEFT				0

#define LCD_VAL_CLEAR				(0x01)
#define LCD_VAL_HOME				(0x02)
#define LCD_VAL_ENTRYMODESET		(0x04)
#define LCD_VAL_DISPLAYONOFF		(0x08)
#define LCD_VAL_CURSORDISPLAY		(0x10)
#define LCD_VAL_FUNCTIONSET			(0x20)
#define LCD_VAL_SETCGRAM			(0x40)
#define LCD_VAL_SETDDRAM			(0x80)

void LCD_Send(int LCD_SEND_VALUE, char LCD_SEND_MODE, int LCD_COMMANDTYPE);

void LCD_Init(void);
void LCD_PrintS(char *LCD_STRING, char LCD_X, char LCD_Y);
void LCD_PrintJ(char *LCD_STRING, char LCD_X, char LCD_Y);
void LCD_Print(char LCD_STRING, char LCD_X, char LCD_Y);
void LCD_SetPos(char LCD_Y, char LCD_X);

void LCD_Clear();
void LCD_Home(void);

void LCD_EntryModeSet(char LCD_DIRECTION, char LCD_SHIFT_ENABLE);
void LCD_DisplayOnOff(char LCD_ON_OFF, char LCD_CURSOR, char LCD_BLINK);
void LCD_CursorOrDisplayShift(char LCD_CURSORORDISPLAY, char LCD_SHIFT_LEFTRIGHT);
void LCD_FunctionSet(char LCD_DATA_LENGTH, char LCD_NUM_LINES, char LCD_FNT);
void LCD_SetCGRAM(char LCD_CGRAMADR);
void LCD_SetDDRAM(char LCD_DDRAMADR);



