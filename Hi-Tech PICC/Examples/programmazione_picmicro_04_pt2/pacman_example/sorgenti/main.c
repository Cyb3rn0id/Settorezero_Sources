//*************************************************
//
// main.c
// autore: Bernardo Giovanni (www.settorezero.com)
// data: 01/11/09
// esempio di caratteri custom su LCD 2x16
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
#define pacman 0
#define dot 1
#define ghost 2

void main(void)
        {
        
		settings();
        LCD_INIT(); // ricordarsi sempre di inizializzare l'lcd
		
		// definisco i caratteri personalizzati
		LCD_CUSTOMCHAR(pacman,0,14,29,31,24,31,14,0);
		LCD_CUSTOMCHAR(dot,0,0,0,6,6,0,0,0);
		LCD_CUSTOMCHAR(ghost,14,31,21,31,31,31,21,0);
		
		LCD_CLEAR();
		
		LCD_PUTCH(pacman); // posiziono il pacman
		
		for (int a=0; a<15; a++) // stampo i puntini sulla prima riga
			{
			LCD_PUTCH(dot);
			}
		
		LCD_GOTO(2,1);
		for (int a=0; a<15; a++) // stampo i puntini sulla seconda riga
			{
			LCD_PUTCH(dot);
			}
		
		LCD_PUTCH(ghost);
		
		while(1)
			{
			}
        }// Fine ciclo continuo