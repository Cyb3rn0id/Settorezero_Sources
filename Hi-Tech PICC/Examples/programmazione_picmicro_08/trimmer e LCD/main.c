//*************************************************
// CORSO PROGRAMMAZIONE PICMICRO
// Lezione 11
// http://www.settorezero.com
//
// modulo: main.c
// autore: Bernardo Giovanni
// data:  06/03/10
// descrizione: esempio di utilizzo convertitore A/D
// mostriamo il valore letto da un trimmer collegato
// su AN1 su un display LCD
// picmicro: PIC16F877A
// clock: 20MHz
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
		settings();
		BL=1; // abilito retroilluminazione LCD
		LCD_INIT(); // inizializzo l'LCD
		DelayMs(100); // piccolo ritardo iniziale
		LCD_CLEAR(); // ripulisco il display
		
		LCD_GOTO(1,1); // posiziono il cursore sull'LCD: riga1, colonna1
		LCD_PUTS("Valore AD:");
		LCD_GOTO(2,1); // posiziono il cursore sull'LCD: riga1, colonna1
		LCD_PUTS("Tensione :");
		while(1)
			{
			// do un piccolo ritardo per permettere a CHOLD di caricarsi correttamente, 
			// come spiegato nella parte introduttiva di questo capitolo
			DelayUs(20); 

			ADGO=1; // avvio la conversione
			
			// ciclo di continuo fino a che ADGO non diventa zero, segnalandomi la fine conversione
			while(ADGO)
				{continue;}
				
			// mi calcolo il valore numerico restituito dal convertitore
			// tenendo conto che ho scelto la giustificazione a sinistra
			valore=ADRESL + (ADRESH<<8);
			
			// mi posiziono per scrivere il valore numerico
			LCD_GOTO(1,11); // 
			LCD_PUTUN(valore);
			// cancello parte restante, questo per evitare che mi trascino caratteri sull'LCD
			// quando da un numero a più cifre passo a un numero con meno cifre
			LCD_PUTS("   "); 
			
			tensione=valore*5; // valore in millivolt, ricordiamo che ogni bit vale circa 5mV
			LCD_GOTO(2,11);
			LCD_PUTUN(tensione/1000);// parte intera dei volts
			LCD_PUTS("."); 
			LCD_PUTUN(tensione%1000);// parte decimale
			LCD_PUTS("V   "); 
			}
	}