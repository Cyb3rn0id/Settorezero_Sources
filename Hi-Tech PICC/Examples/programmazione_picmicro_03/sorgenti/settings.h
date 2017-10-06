//*************************************************
// CORSO PROGRAMMAZIONE PICMICRO
// Lezione 6
// www.settorezero.com
//
// modulo: settings.h
// autore: Bernardo Giovanni
// data: 28/09/09
// descrizione: configurazione del picmicro
//
//*************************************************

#define BTN1	RD0	// pulsante 1
#define BTN2	RD1	// pulsante 2
#define LED1	RD2	// led 1
#define LED2	RD3	// led 2

void settings(void)
	{
	TRISA=0;			// Tutte output
	TRISB=0;
	TRISC=0;
	TRISD=0b00000011; 	// Le porte RD0 e RD1 devono essere input (1) perchè vi sono collegati i pulsanti, le altre output (0)
	TRISE=0;
	
	// All'avvio i 2 led devono essere spenti
	LED1=0;
	LED2=0;
	}