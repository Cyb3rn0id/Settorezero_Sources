//*************************************************
//
// main.c
// autore: Bernardo Giovanni - www.settorezero.com
// data: 01/11/09
// esempio di caratteri custom
//
//*************************************************

#define  XTAL_FREQ 20MHZ
#include <pic.h>
#include <stdio.h>

__CONFIG (HS & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTEN & DEBUGDIS & UNPROTECT);

#include "settings.h" // impostazioni di configurazione
#include "delay.c" // routine per ritardi
#include "lcd.c" // comando display lcd HD44780A

// do dei nomi per ricordarmi piu facilmente i caratteri
#define play 0
#define stop 1
#define cuore 2
#define invader 3
#define sole 4
#define teschio 5
#define smile 6
#define triste 7

void main(void)
        {
        
		settings();
        LCD_INIT(); // ricordarsi sempre di inizializzare l'lcd
		
		// definisco i caratteri personalizzati
		LCD_CUSTOMCHAR(play,16,24,28,30,28,24,16,0);
		LCD_CUSTOMCHAR(stop,0,31,31,31,31,31,0,0);
		LCD_CUSTOMCHAR(cuore,0,10,31,31,14,4,0,0);
		LCD_CUSTOMCHAR(invader,17,10,31,21,31,31,10,27);
		LCD_CUSTOMCHAR(sole,4,17,14,31,14,17,4,0);
		LCD_CUSTOMCHAR(teschio,14,31,21,31,14,0,14,0);
		LCD_CUSTOMCHAR(smile,0,10,10,0,17,14,0,0);
		LCD_CUSTOMCHAR(triste,0,10,10,0,14,17,0,0);
		
		LCD_CLEAR();
		
		// eseguo un ciclo per stampare sul display gli 8 caratteri da 0 a 7
		for (int a=0; a<8; a++)
			{
			LCD_PUTCH(a);
			}
		
		LCD_GOTO(2,1);
		LCD_PUTS("SETTOREZERO.COM");
		
		while(1)
			{
			}
        }// Fine ciclo continuo