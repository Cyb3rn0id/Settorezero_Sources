//*************************************************
// CORSO PROGRAMMAZIONE PICMICRO
// Lezione 11
// http://www.settorezero.com
//
// modulo: main.c
// autore: Bernardo Giovanni
// data:  13/03/10
// descrizione: esempio di utilizzo convertitore A/D
// mostriamo il valore letto da un trimmer collegato
// su AN1 su una barra a led
// picmicro: PIC16F877A
// clock: 20MHz
//
//*************************************************

#define  XTAL_FREQ 20MHZ
#include <pic.h>

__CONFIG (HS & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTEN & DEBUGDIS & UNPROTECT);

#include "settings.h"
#include "delay.c"

void main(void)
        {
		settings();
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
			
			/*
			adesso, in base al valore, variabile da 0 a 1023, facciamo accendere una serie di led da 1 a 8
			i led si trovano tutti sulla portD.
			
			volendo rappresentare 0-5 volt su una scala di 8 led, ogni led rappresenterà 5/8 = 0,625V = 625mV
			sapendo che la risoluzione del convertitore A/D è 4,88mv, ogni led si dovrà accendere con un
			valore del convertitore A/D pari a 625/4.88=128
			*/
			
			// affianco sono i riportati i valori da dare a PORTD se vogliamo visualizzare
			// una barra o un punto
			//								BARRA		PUNTO
			if (valore<=128)
				{PORTD=1;}				//	1			1
			else if (valore<=256)
				{PORTD=2;}				//	3			2
			else if (valore<=384)
				{PORTD=4;}				//	7			4
			else if (valore<=512)
				{PORTD=8;}				//	15			8
			else if (valore<=640)
				{PORTD=16;}				//	31			16
			else if (valore<=768)
				{PORTD=32;}				//	63			32
			else if (valore<=896)
				{PORTD=64;}			//	127			64
			else 
				{PORTD=128;}			//	255			128
			}
	}