//*************************************************
//
// main.c
// autore: Bernardo Giovanni (www.settorezero.com)
// data: 21/10/09
//
//*************************************************
#define  XTAL_FREQ 20MHZ
#include <pic.h>

__CONFIG (HS & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTEN & DEBUGDIS & UNPROTECT);

#include "settings.h"
#include "delay.c"
#include "lcd.c"

void main(void)
        {
		settings(); // imposto le porte
		LCD_INIT(); // inizializzo l'LCD
		DelayMs(100); // piccolo ritardo iniziale
		LCD_CLEAR(); // ripulisco il display
		LCD_GOTO(1,1); // posiziono il cursore sull'LCD: riga1, colonna1
		LCD_PUTS("HELLO");
		LCD_GOTO(2,1); // riga2, colonna1
		LCD_PUTS("WORLD !");
		while(1)
			{
			}
	}