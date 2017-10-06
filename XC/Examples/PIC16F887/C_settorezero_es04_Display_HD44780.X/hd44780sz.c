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
#include "hd44780sz.h"

// initialize hd44780 controller using 4-bit interface
void LCD_Init(void)
    {
    // make tristate registers as outputs
    LCD_TRIS_RS=0;
    LCD_TRIS_RW=0;
    LCD_TRIS_EN=0;
    LCD_TRIS_D7=0;
    LCD_TRIS_D6=0;
    LCD_TRIS_D5=0;
    LCD_TRIS_D4=0;
    #ifdef LCD_BACKLIGHT
    LCD_TRIS_BL=0;
    #endif
    // control lines at startup
    LCD_RS=0;
    LCD_RW=0;
    LCD_EN=0;
    __delay_ms(60); // power-up delay
    _LCD_Nibble(0b0011); // function set
    __delay_ms(10); // wait more than 4.1mS
    _LCD_Strobe();
    __delay_us(150); // wait more than 100uS
    _LCD_Strobe();
    __delay_us(5);
    _LCD_Nibble(0b0010); // interface set to 4bit
    // from this moment we can check the busy flag
    while (_LCD_Isbusy());
    // set number of display lines and font
    #if (LCD_ROWS==1)
    _LCDCommand(LCD_4BIT1LINES);
    #else
    _LCD_Command(LCD_4BIT2LINES);
    #endif
    _LCD_Command(LCD_OFF);
    _LCD_Command(LCD_CLEAR);
    _LCD_Command(LCD_ENTRYMODE6);
    _LCD_Command(LCD_ON);
    }// _LCD_Init

// set cursor to row and column specified (1-based index!)
void LCD_Goto(int8_t row, int8_t col)
    {
    if (row>LCD_ROWS)
        {
        row=LCD_ROWS;
        }
    if (col>LCD_COLS)
        {
        col=LCD_COLS;
        }

    switch(row)
        {
        case 1:
        _LCD_Command((LCD_ROW1-1)+col);
        break;

        case 2:
        _LCD_Command((LCD_ROW2-1)+col);
        break;

        case 3:
        _LCD_Command((LCD_ROW3-1)+col);
        break;

        case 4:
        _LCD_Command((LCD_ROW4-1)+col);
        break;
        }
    }// LCD_Goto

// clear display
void LCD_Clear(void)
    {
    _LCD_Command(LCD_CLEAR);
    }// LCD_Clear

// turns on/off backlight if backlight pin is I/O-driven
#ifdef LCD_BACKLIGHT
void LCD_Backlight(bool backlight)
    {
    if (backlight)
        {
        LCD_BL=1;
        }
    else
        {
        LCD_BL=0;
        }
    }// LCD_Backlight
#endif

// write a single char on the display
void LCD_Putchar(char c)
    {
    LCD_RS=1;
    _LCD_Sendbyte(c);
    }// LCD_Putchar

// write an unsigned integer
void LCD_Writeuns(uint16_t c)
    {
    uint8_t t1,i;
    bool wrote;
    uint16_t k;
    wrote=false;
    for (i=4;i>=1;i--)
        {
        switch(i)
            {
            case 4:
            k=10000;
            break;

            case 3:
            k=1000;
            break;

            case 2:
            k=100;
            break;

            case 1:
            k=10;
            break;
            }
        t1=c/k;
        if((wrote)||(t1!=0))
            {
            LCD_Putchar(t1+'0');
            wrote=true;
            }

        c-=(t1*k);
        }// end for
    LCD_Putchar(c+'0');
    }//LCD_Writeuns

// write a signed integer
void LCD_Writesig(int16_t c)
    {
    if(c<0)
        {
        LCD_Putchar('-');
        c*=(-1);
        }
    LCD_Writeuns((unsigned)c);
    }//LCD_Writesig

// write a string
void LCD_Writestr(const char *s)
    {
    while(*s)
        {
        LCD_Putchar(*s++);
        }
    }// LCD_Writestr

// write a float number
void LCD_Writeflo(float c,uint8_t p)
    {
    char buf[20];
    if (p>4) p=4;
    if (p<1) p=1;
    switch(p)
        {
        case 1:
            sprintf(buf,"%.1f",c);
            break;
        case 2:
            sprintf(buf,"%.2f",c);
            break;
        case 3:
            sprintf(buf,"%.3f",c);
            break;
        case 4:
            sprintf(buf,"%.4f",c);
            break;
        }
    LCD_Writestr(buf);
    }// LCD_Writeflo

// set a custom char in the ram position
void LCD_Customchar(int8_t pos, const char *b)
    {
    if (pos>7) {return;}
    int8_t u;
    _LCD_Command(LCD_OFF);
    _LCD_Command(LCD_GOCGRAM+(pos*8));
    for (u=0; u<8; u++)
        {
        LCD_Putchar(b[u]);
        }
    _LCD_Command(LCD_CLEAR);
    _LCD_Command(LCD_ON);
    }// LCD_Customchar

// low-level functions

// pulse on the enable line
void _LCD_Strobe(void)
    {
    LCD_EN=1;
    __delay_us(2);
    LCD_EN=0;
    }// _LCD_STrobe

// send 4-bit to lcd
void _LCD_Nibble (char nibble)
    {
    // check bit 7 (or bit 3)
    if (nibble & 0b10000000)
        {
        LCD_D7=1;
        }
    else
        {
        LCD_D7=0;
        }
    // check bit 6 (or bit 2)
    if (nibble & 0b01000000)
        {
        LCD_D6=1;
        }
    else
        {
        LCD_D6=0;
        }
    // check bit 5 (or bit 1)
    if (nibble & 0b00100000)
    	{
        LCD_D5=1;
        }
    else
        {
        LCD_D5=0;
        }
    //check bit 4 (or bit 0)
    if (nibble & 0b00010000)
        {
        LCD_D4=1;
        }
    else
        {
        LCD_D4=0;
        }
    __delay_us(1);
    _LCD_Strobe();
    }// _LCD_Nibble

// send 1 byte to lcd
void _LCD_Sendbyte (char b)
    {
    while (_LCD_Isbusy());
    _LCD_Nibble(b);
    b <<= 4;
    _LCD_Nibble(b);
    }// _LCD_Sendbyte

// send command to lcd (1 byte with RS at low logic level)
void _LCD_Command (int16_t cmd)
    {
    LCD_RS=0;
    _LCD_Sendbyte((char)cmd);
    } // _LCDCommand

// return the busy flag
bool _LCD_Isbusy(void)
    {
    static bool busyFlag;
    static bool tempRS;
    // set data lines as input
    LCD_TRIS_D7=1;
    LCD_TRIS_D6=1;
    LCD_TRIS_D5=1;
    LCD_TRIS_D4=1;
    tempRS = LCD_RS; // actual register select value
    LCD_RW=1; // read mode
    LCD_RS=0; // command mode
    LCD_EN=1; // read bits 4-7 (bit7 = busy status)
    __delay_us(1);
    busyFlag=LCD_D7; // read busy flag
    LCD_EN=0;
    __delay_us(1);
    _LCD_Strobe();
    // set data lines as output
    LCD_TRIS_D7=0;
    LCD_TRIS_D6=0;
    LCD_TRIS_D5=0;
    LCD_TRIS_D4=0;
    LCD_RW=0; // back to write mode
    LCD_RS=tempRS; // restore previous register select value
    return (busyFlag);
    }// _LCD_Isbusy