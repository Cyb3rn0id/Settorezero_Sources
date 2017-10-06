//*************************************************
// CORSO PROGRAMMAZIONE PICMICRO
// www.settorezero.com
//
// modulo: main.c
// autore: Bernardo Giovanni
// data: 18/08/09
// descrizione: lampeggia un led su RD0
// picmicro: PIC16F877A
// clock: 20MHz
//
//*************************************************

#define  XTAL_FREQ 20MHZ // questo è utilizzato dalle routine di ritardo contenute in Delay.C
#include <pic.h> // contiene i nomi mnemonici di registri e porte

// Fuses di configurazione
__CONFIG (HS & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTEN & DEBUGDIS & UNPROTECT);

#include "delay.c" // routine per ritardi
#define	LED	RD0 // invece di scrivere RD0, scriverò LED, così mi è più facile ricordare

// funzione principale, eseguita all'avvio del picmicro
void main(void)
	{

	// imposto i registri tristato in maniera tale che tutte le porte siano configurate come pin di uscita
	TRISA=0b00000000;
	TRISB=0b00000000;
	TRISC=0b00000000;
	TRISD=0b00000000; 
	TRISE=0b00000000;

	while(1) // eseguo un ciclo finito
        {
		LED=LED^1;
		DelayMs(250);
		}// Fine ciclo continuo
	
	} // Fine main