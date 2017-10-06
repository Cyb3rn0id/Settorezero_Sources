/***********************************************************************
 POOR's (universal) LCD interface V1.4b                 
 simple and complete LCD routines by Andrea Bonzini
 
 some edits by Bernardo Giovanni (www.settorezero.com) (see changelog.txt)
************************************************************************
 Please report any bug or suggestion at zypkin@inwind.it

************************************************************************
 This code will interface to a standard LCD controller
 like the Hitachi HD44780 or Samsung KS0066
 It has been tested and works correctly with the followings
 LCD types:
 1x8, 2x8, 1x16, 2x16, 2x20, 4x16, 4x20
 
 !!! WARNINGS !!!

 This routines had been written to meet minimum hardware
 requirements...so you can use it even when your main
 application has left a few I/O lines not on the same port :)
 To let this you have the following restriction and benefits:

 1) LCD works only in 4 bit mode.
 2) You can use any Output pin of your MCU, you have only
      to change pin assignments in the define section.
 3) R/W select is not available so you must ground LCD's R/W pin

  !!! NOTE !!! 

 These routines use delay.c

************************************************************************

USER'S ROUTINES DESCRIPTION:

LCD_INIT() -----------> Initialize the LCD.
                        you must call it the first time you use the LCD
                        and before any other LCD routines.

LCD_CLEAR() ----------> Clears and Home LCD.

LCD_CMD('char') ------> Sends a command to the LCD.
                        See LCD datasheet for the complete commands list

LCD_GOTO(line,pos) ---> Set the Cursor to a specified Line and position.
                        Lines available are from 1 to 4. Pos available 
                        starts from 1 to max available on your LCD.

LCD_PUTCH('char') ----> Writes a character on LCD (ASCII representation).

LCD_PUTS("string") ---> Writes a string on LCD.

LCD_PUTUN(number)  ---> Writes an Unsigned Number on LCD.
                        It works both with INT (16bit) and CHAR (8bit).

LCD_PUTSN(number)  ---> Writes a Signed Number on LCD (with Sign if <0).
                        It works both with INT (16bit) and CHAR (8bit).
						
LCD_CUSTOMCHAR(pos,byte1,byte2,byte3,byte4,byte5,byte6,byte7,byte8) -->
                        customizes the character in the CGRAM at position (pos)
		   bytes from 1 to 8 are the pattern of the new character.
		   WARNING: this function can only used if the flag
		   LCD_CANCUSTOMIZE is set to 1

************************************************************************

 !!! ATTENTION !!!
 Follow these simple instructions to configure your LCD module

 1) check your hardware to determine which lines to use
    (you need 6 output lines).
 2) set properly TRIS registers in your main code to configure
    the 6 lines as outputs.
 3) In the next step use the defines to set the 6 lines as your
     hardware requires.
 4) Set LCD Rows and Columns number using the define as shown 
 5) Set LCD cursor blinking and visibility as shown
 6) You are ready...your LCD will work!!! :)

************************************************************************/


/************************************************************************
 Use this includes if these files are not included in your main code 
************************************************************************/
//#include "pic.h"
//#include "delay.c"


/************************************************************************
 Use the following defines to set the lines as your hardware requires 
 You can use ANY output line of the MCU, even on several ports :)
************************************************************************/
#define	LCD_RS	RD2	// Register select
#define	LCD_EN	RD3	// Enable
#define	LCD_D4	RD4	// LCD data 4
#define	LCD_D5	RD5	// LCD data 5
#define	LCD_D6	RD6	// LCD data 6
#define	LCD_D7	RD7	// LCD data 7

/************************************************************************
 Now you have only to write LCD Rows and Columns number
 and the cursor behaviour
/************************************************************************
 !!! NOTE !!!
 Some 1x16 LCD works as 2x8!!! ...be sure how to configure
 yours, see its datasheet!!!
************************************************************************/

#define LCD_ROWS 2 // valid numbers are: 1,2 (set to 2 for 2 or more rows)
#define LCD_COLS 16 // valid numbers are: 8,16,20
#define LCD_CURSOR_VISIBLE 1 // 1:cursor on 0:cursor off
#define LCD_CURSOR_BLINK 1 // 1:cursor blinks 0:cursor doesn't blinks
#define LCD_TYPEWRITE 1 // 1:delay between characters
#define LCD_TW_DELAY 50 // milliseconds between characters 

/************************************************************************
 YOUR LCD IS NOW READY TO WORK!!! :)
 YOU CAN IGNORE THE FOLLOWING CODE
 ENJOY !!!
************************************************************************/

// define the right control value to set the cursor behavior
#if LCD_CURSOR_VISIBLE==1
	#if LCD_CURSOR_BLINK==1
	#define LCD_CONTROL 0b00001111
	#else
	#define LCD_CONTROL 0b00001110
	#endif
#else
	#define LCD_CONTROL 0b00001100
#endif

/************************************************************************
 Use the following defines to send fast command to the LCD
 Ex.: LCD_CMD(LCD_line2); will set the cursor on line 2
 You can add fast command of your own!!!
*************************************************************************
 !!! NOTE !!!
 DON'T CHANGE THE DEFINES WITHIN #if-#endif
*************************************************************************/

#define LCD_CLR  0x01 // Clears Display
#define LCD_HOME 0x02 // Cursor to Home position

#define LCD_line1 0x80 // Cursor to Line 1 position 1
#define LCD_line2 0xC0 // Cursor to Line 2 position 1

#if (LCD_COLS==20)
	#define LCD_line3 0x94 // Cursror to Line 3 position 1 (20 char LCD)
	#define LCD_line4 0xD4 // Cursor to Line 4 position 1 (20 char LCD)
#else
	#define LCD_line3 0x90 // Cursor to Line 3 position 1 (16 char LCD)
	#define LCD_line4 0xD0 // Cursor to Line 4 position 1 (16 char LCD)
#endif

/************************************************************************/

/****************************************
 Enable LCD to read data
*****************************************/
void LCD_STROBE (void)
	{
	LCD_EN = 1;
	DelayUs(1);
	LCD_EN=0;
	}	

/****************************************
 Write a nibble to the LCD
****************************************/
void LCD_NIBBLE_OUT (unsigned char c)
	{
	if (c & 0b10000000) 
		LCD_D7=1;
	else LCD_D7=0;
	
	if (c & 0b01000000)
		LCD_D6=1;
	else LCD_D6=0;

	if (c & 0b00100000)
		LCD_D5=1;
	else LCD_D5=0;

	if (c & 0b00010000)
		LCD_D4=1;
	else LCD_D4=0;

	LCD_STROBE();
	}

/****************************************
 Write a byte to the LCD (4 bit mode) 
****************************************/
void LCD_WRITE (unsigned char c)
	{
	LCD_NIBBLE_OUT(c);
	c <<= 4;
	LCD_NIBBLE_OUT(c);
	DelayUs(50);
	#if LCD_TYPEWRITE==1
	if (LCD_RS)
		{
		DelayMs(LCD_TW_DELAY); // Delay between characters
		}
	#endif
	}

/****************************************
 Sends a command to the LCD
****************************************/
void LCD_CMD (char c)
	{
	LCD_RS = 0; // writes command
	LCD_WRITE(c);
	}

/****************************************
 GoTo specified line and position
****************************************/
void LCD_GOTO (char line,char pos)
	{
	switch(line)
		{
		case 1: LCD_CMD((LCD_line1-1)+pos);
		break;
		
		case 2: LCD_CMD((LCD_line2-1)+pos);
		break;
		
		case 3: LCD_CMD((LCD_line3-1)+pos);
		break;
		
		case 4: LCD_CMD((LCD_line4-1)+pos);
		}
	}

/****************************************
 Clears and Home LCD
*****************************************/
void LCD_CLEAR (void)
	{
	LCD_CMD(LCD_CLR);
	DelayMs(3);
	}

/****************************************
 Writes one character (ASCII) to LCD 
****************************************/
void LCD_PUTCH (char c)
	{
	LCD_RS = 1; // write characters
	LCD_WRITE(c);
	}

/****************************************
  Writes unsigned numbers to the LCD
*****************************************/
void LCD_PUTUN (unsigned int c)
	{
	unsigned char t1,i,wrote;
	unsigned int k;
	wrote=0;
	for (i=4;i>=1;i--)
		{
		switch(i)
			{
			case 4: k=10000;
			break;
	
			case 3: k=1000;
			break;
			
			case 2: k=100;
			break;
	
			case 1: k=10;
			}
		
		t1=c/k;
		
		if((wrote)||(t1!=0))
			{
			LCD_PUTCH(t1+'0');
			wrote=1;
			}
	
		c-=(t1*k);
		}

	LCD_PUTCH(c+'0');
	}

/****************************************
  Writes signed numbers to the LCD
*****************************************/
void LCD_PUTSN (signed int c)
	{
	if(c<0)
		{ 
		LCD_PUTCH('-');
		c*=(-1);
		}
	
	LCD_PUTUN(c);
	}

/****************************************
 Writes a string to the LCD
****************************************/
void LCD_PUTS (const char * s)
	{
	LCD_RS = 1; // write characters
	while(*s)
		LCD_WRITE(*s++);
	}

/****************************************
 Customizes the CGRAM characters (0-7)
 ****************************************/
void LCD_CUSTOMCHAR(unsigned char pos,unsigned char byte0,unsigned char byte1,unsigned char byte2,unsigned char byte3,unsigned char byte4,unsigned char byte5,unsigned char byte6,unsigned char byte7)
	{
	LCD_CMD(0b00001000); // display off
	LCD_CLEAR();
	LCD_CMD(64+(pos*8)); // goto the right CGRAM location
	LCD_PUTCH(byte0);
	LCD_PUTCH(byte1);
	LCD_PUTCH(byte2);
	LCD_PUTCH(byte3);
	LCD_PUTCH(byte4);
	LCD_PUTCH(byte5);
	LCD_PUTCH(byte6);
	LCD_PUTCH(byte7);
	LCD_CMD(LCD_CONTROL); // display on with preferred settings
	}

/****************************************
 Initializes LCD
****************************************/	
void LCD_INIT (void)
	{
	
	LCD_RS = 0; // write control bytes
	DelayMs(50); // power on delay

	LCD_D4=1;
	LCD_D5=1;
	LCD_D6=0;
	LCD_D7=0;

	LCD_STROBE();
	DelayMs(6);

	LCD_STROBE();
	DelayUs(150);

	LCD_STROBE();
	DelayMs(6);

	LCD_D4=0; // set 4 bit mode
	LCD_STROBE();
	
	DelayUs(100);
	
	#if (LCD_ROWS==1)
		LCD_WRITE(0b00100000);	// 4 bit mode, 1 line, 5x8 font
	#else
		LCD_WRITE(0b00101000);	// 4 bit mode, 2 or more lines, 5x8 font
	#endif
	
	LCD_WRITE(0b00001000); // display off
	LCD_WRITE(0b00000000); // clear display
	LCD_WRITE(LCD_CONTROL); // display on, set the cursor behavior as specified
	LCD_WRITE(0b00000110); // entry mode, display not shifted, cursor increments
	}

/************************************************************************/
#undef LCD_ROWS
#undef LCD_COLS
#undef LCD_CURSOR_BLINK
#undef LCD_CURSOR_VISIBLE
/************************************************************************
 !!! END !!!
 THANKS FOR EXAMINING MY CODE ;)
/************************************************************************
 Please report any bug or suggestion at zypkin@inwind.it
************************************************************************/