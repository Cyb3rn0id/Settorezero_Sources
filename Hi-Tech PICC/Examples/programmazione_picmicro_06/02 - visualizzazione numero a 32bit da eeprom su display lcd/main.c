//*************************************************
//
// main.c
// esempio di memorizzazione su eeprom e visualizzazione
// su LCD di numero a 32bit
// autore: Bernardo Giovanni (www.settorezero.com)
// data: 09/01/2010
//
//*************************************************
#define  XTAL_FREQ 20MHZ
#include <pic.h>

__CONFIG (HS & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTEN & DEBUGDIS & UNPROTECT);

#include "settings.h"
#include "delay.c"
#include "lcd.c"
#include "eeprom_internal.c"

void main(void)
        {
		settings(); // imposto le porte
		LCD_INIT(); // inizializzo l'LCD
		DelayMs(100); // piccolo ritardo iniziale
		LCD_CLEAR(); // ripulisco il display

		// numero di esempio a 32 bit
		unsigned long numero = 3374966197;
		
		/*
		memorizzo il numero a 32 bit nell'eeprom partendo dalla locazione zero. 
		Essendo composto da 4 byte, la prossima locazione libera in cui memorizzare
		altri dati sarà la numero 4, in quanto il numero a 32bit mi occuperà le locazioni
		da 0 a 3
		*/
		eei_write_long(numero,0);
		
		// ora recupero il valore dall'eeprom e lo memorizzo in un'altra variabile
		unsigned long lettura = eei_read_long(0);
		
		// mostro il valore letto sul display
		LCD_PUTUN32(lettura);
	
		while(1)
			{
			}
			
	}