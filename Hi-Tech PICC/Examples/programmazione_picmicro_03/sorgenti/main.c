//*************************************************
// CORSO PROGRAMMAZIONE PICMICRO
// Lezione 6
// www.settorezero.com
//
// modulo: main.c
// autore: Bernardo Giovanni
// data: 28/09/09
// descrizione: pulsanti e led
// picmicro: PIC16F877A
// clock: 20MHz
//
//*************************************************

#define  XTAL_FREQ 20MHZ // questo è utilizzato dalle routine di ritardo contenute in Delay.C
#include <pic.h> // contiene i nomi mnemonici di registri e porte
__CONFIG (HS & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTEN & DEBUGDIS & UNPROTECT);
#include "delay.c" // routine per ritardi
#include "settings.h" // settaggi del picmicro

void main(void)
	{
	settings(); // eseguo la funzione settings contenuta nel file header settings.h, così imposto le porte e i registri
	
	while(1)
        {
		// controllo pulsante 1
		if (!BTN1) // se pulsante1 premuto (quando è premuto, porta il pin allo stato logico basso)
			{
			DelayMs(100); // ritardo per antirimbalzo
			if (!BTN1) // se dopo 100ms il pulsante è ancora premuto, non si tratta di un rimbalzo
				{
				LED1=1; // accendo led 1
				}
			}
		else
			{
			LED1=0; // se pulsante 1 non premuto, spengo led 1
			}
		
		// controllo pulsante 2
		if (!BTN2) // se pulsante 2 premuto
			{
			DelayMs(100); // antirimbalzo
			if (!BTN2)
				{
				LED2=LED2^1; // inverto lo stato del led
				}
			}
		}// Fine ciclo continuo
	} // Fine main