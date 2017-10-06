/*
 * HD44780SZ
 * Library for alphanumeric LCD based on Hitachi HD44780 controller
 * or compatible (hitachi HD66702, Samsung KS0066, Samsung KS0070,
 * Sanyo LC7985, Epson SED1278, OKI MSM6222, Sunplus SPLC7800)
 * in 4bits mode
 *
 * Module:              hd44780sz.h
 * Version:             1.0
 * Release date:        04/10/2017
 * Dependencies:        none
 * Compiler:            Microchip XC family
 * MCU:                 PICmicro
 * Pins needed:         7 (+1 for backlight)
 * Peripheral needed:   digital I/Os
 * Author:              Giovanni Bernardo
 * Website:             http://www.settorezero.com
 * Email:               settorezero@gmail.com
 *
 * YOU MAY NOT DISTRIBUTE OR SELL THIS CODE OR PART
 * OF IT WITHOUT AUTHOR'S EXPLICIT AUTHORIZATION
 *
 * E' FATTO DIVIETO ASSOLUTO DI RIDISTRIBUIRE  O VENDERE
 * QUESTO CODICE O SUE PARTI SENZA L'AUTORIZZAZIONE
 * ESPLICITA DELL'AUTORE
 *
 * WARRANTY:
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * WE ARE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 * 
 * L'utilizzatore dichiara di aver letto il disclaimer al seguente
 * indirizzo prima di procedere all'utilizzo di questo codice:
 * http://www.settorezero.com/wordpress/info/disclaimer/
 * 
 * Special thanks to:
 * Andrea Bonzini (author of original Poor's LCD Interface)
 * Sergio Fiocco (for distribution of Andrea Bonzini's library)
 * Gabriel Rapetti (for hints about Busy Flag)
 *
 */
#ifndef HD447480SZ_LIB
#define HD44780SZ_LIB

// crystal frequency (Hz)
#define _XTAL_FREQ 20000000

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// display size
#define LCD_ROWS 2
#define LCD_COLS 16

// uncomment this if your lcd uses font 5x10
// (note: majority of lcds uses 5x8 font, so leave this commented)
//#define FONT5x10

// uncomment if lcd backlight pin is I/O-driven
// comment following 3 statements if lcd backlight is hardwired
#define LCD_BACKLIGHT
#define LCD_BL	PORTCbits.RC1
#define LCD_TRIS_BL TRISCbits.TRISC1

// data connections between LCD and PICmicro
#define	LCD_RS	PORTDbits.RD2	// Register select
#define LCD_RW	PORTDbits.RD1	// Read/Write
#define	LCD_EN	PORTDbits.RD3	// Enable
#define	LCD_D7	PORTDbits.RD7	// LCD data 7
#define	LCD_D6	PORTDbits.RD6	// LCD data 6
#define	LCD_D5	PORTDbits.RD5	// LCD data 5
#define	LCD_D4	PORTDbits.RD4	// LCD data 4

// port connections (tristate registers)
#define LCD_TRIS_RS     TRISDbits.TRISD2
#define LCD_TRIS_RW     TRISDbits.TRISD1
#define LCD_TRIS_EN     TRISDbits.TRISD3
#define LCD_TRIS_D7     TRISDbits.TRISD7
#define LCD_TRIS_D6     TRISDbits.TRISD6
#define LCD_TRIS_D5     TRISDbits.TRISD5
#define LCD_TRIS_D4     TRISDbits.TRISD4

// please don't edit nothing below this line ------------------------

// hd44780 command list
#define LCD_CLEAR  	0x01
#define LCD_HOME 	0x02
#define LCD_SCROLL_R	0x1E
#define LCD_SCROLL_L	0x18
#define LCD_MOVE_L	0x10
#define LCD_MOVE_R	0x14
#define LCD_CURSOROFF	0x0C
#define LCD_CURSORON	0x0E
#define LCD_CURSORBLINK	0x0F
#define	LCD_OFF		0x08
#define LCD_ON		0x0C
#define LCD_GOCGRAM	0x40
#ifdef FONT5x10
#define	LCD_4BIT1LINES	0x24 // 4bit, 1 row, font 5x10
#define	LCD_4BIT2LINES	0x2C // 4bit, 2 or more rows, font 5x10
#else
#define	LCD_4BIT1LINES	0x20 // 4 bit, 1 row, font 5x8
#define	LCD_4BIT2LINES	0x28 // 4 bit, 2 or more rows, font 5x8
#endif
#define LCD_ENTRYMODE4	0x04 // shift off, decrement address
#define LCD_ENTRYMODE5	0x05 // shift on, decrement address
#define LCD_ENTRYMODE6	0x06 // shift off, increment address
#define LCD_ENTRYMODE7	0x07 // shift on, increment address

// start-rows definitions
#define LCD_ROW1 0x80 // Row 1, Col 1
#define LCD_ROW2 0xC0 // Row 2, Col 1
#if (LCD_COLS==20)
#define LCD_ROW3 0x94 // Row 3, Col 1 (20 columns lcds)
#define LCD_ROW4 0xD4 // Row 4, Col 1 (20 columns lcds)
#else
#define LCD_ROW3 0x90 // Row 3, Col 1 (16 columns lcds)
#define LCD_ROW4 0xD0 // Row 4, Col 1 (16 columns lcds)
#endif

// others
#ifndef ON
#define ON  1
#endif
#ifndef OFF
#define OFF 0
#endif

// functions prototypes

// high-level functions
extern void LCD_Init(void);
extern void LCD_Goto(int8_t row, int8_t col);
extern void LCD_Clear(void);
extern void LCD_Putchar(char c);
extern void LCD_Writeuns(uint16_t c);
extern void LCD_Writesig(int16_t c);
extern void LCD_Writeflo(float c,uint8_t p);
extern void LCD_Writestr(const char *s);
extern void LCD_Customchar(int8_t pos, const char *b);
#ifdef LCD_BACKLIGHT
    extern void LCD_Backlight(bool backlight);
#endif

// low-level functions - normally not used in your code
extern void _LCD_Strobe(void);
extern void _LCD_Nibble(char nibble);
extern void _LCD_Sendbyte(char b);
extern void _LCD_Command(int16_t cmd);
extern bool _LCD_Isbusy(void);

#endif