//*************************************************
// CORSO PROGRAMMAZIONE PICMICRO
// Lezione 9
// http://www.settorezero.com
//
// modulo: main.c
// autore: Bernardo Giovanni
// data:  09/01/10
// descrizione: contatore up/down con display a 7
//              segmenti in multiplex su interrupt
// con memoria e azzeramento
// picmicro: PIC16F877A
// clock: 20MHz
//
//*************************************************

/*
DESCRIZIONE FUNZIONAMENTO
I display vengono comandati in multiplex, in pratica si accendono 
alternativamente ogni 5ms per cui danno l'impressione
di essere accesi contemporaneamente. Il multiplexing è interamente
comandato dall'interrupt service routine che si verifica ogni 
millisecondo. Mentre l'ISR si occupa di visualizzare sui display
il valore del contatore, noi nel main possiamo eseguire tranquillamente
tutti i nostri compiti.
*/

#include <pic.h> // nomi mnemonici di registri e porte
__CONFIG (HS & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTEN & DEBUGDIS & UNPROTECT);
#define XTAL_FREQ 20MHZ // per routine di ritardo in delay.c
#include "delay.c" // routine per ritardi
#include "settings.h" // settaggi del picmicro
#include "eeprom_internal.c" // funzioni eeprom

void main(void)
	{
	settings(); // imposto le porte come da settings.h	
	displayson=1; // attivo i display

	while(1) // Ciclo infinito
		{
		// il controllo della pressione dei pulsanti lo eseguo soltanto se i display sono attivi. 
		// I display vengono disattivati durante le operazioni di lettura/scrittura eeprom. 
		// In questo modo non corro il rischio di falsare il valore salvato/letto se per caso 
		// premo qualche pulsante durante le operazioni di salvataggio/lettura
		if (displayson)
			{
			// pulsante "su" premuto
			if (btnUp==Pressed && btnDn==!Pressed) 
				{
				DelayMs(100);
				if (btnUp==Pressed && btnDn==!Pressed) // antibounce
					{
					counter++; // incremento il contatore
					// se il contatore supera 99, lo azzero
					if (counter>99) 
						{
						counter=0;
						}
					}
				}
			
			// pulsante "giu" premuto
			if (btnDn==Pressed && btnUp==!Pressed)
				{
				DelayMs(100);
				if (btnDn==Pressed && btnUp==!Pressed) // antibounce
					{
					if (counter==0)
						{
						counter=99;
						}
					else
						{
						counter--; // decremento il contatore
						}
					}
				}
			
			// pulsanti su e giu premuti insieme: azzeramento
			if (btnDn==Pressed && btnUp==Pressed)
				{
				DelayMs(250);
				DelayMs(250);
				DelayMs(250);
				DelayMs(250);
				if (btnDn==Pressed && btnUp==Pressed) // antibounce
					{
					counter=0;
					}
				}
		
			} // fine che i display sono in funzione
			
			// pulsante richiamo memoria premuto
			if (btnRc==Pressed)
				{
				DelayMs(100);
				if (btnRc==Pressed)
					{
					recall();
					}
				}
				
			// pulsante memoria premuto
			if (btnMe==Pressed)
				{
				DelayMs(100);
				if (btnMe==Pressed)
					{
					displayson=0; // faccio rimanere spenti i display
					memory();
					}
				}
				
				
		} // fine while
	} // fine main
	
void memory(void)
	{
	dispReg=0;
	displayU=OFF;
	displayD=OFF;
	eei_write_byte(counter,0); // scrivo il valore del counter nella locazione di memoria 0
	DelayMs(250);
	DelayMs(250);
	displayson=1; // riattivo i display
	}
	
void recall(void)
	{
	counter=eei_read_byte(0); // il counter ora ha il valore messo in memoria
	}
	
// INTERRUPT SERVICE ROUTINE
void interrupt isr(void)
	{
	// questo interrupt si verifica ogni millisecondo come da impostazioni
	if (T0IF)
        {
		// Ricarico subito il Timer0
		TMR0=100;

    	if (displayson)
			{

			// **************************************************************
			// MULTIPLEXING DISPLAY
			// **************************************************************

			/*
			Visualizzo sui display il valore attuale di "counter".
			Per fare questo devo accendere alternativamente prima un display 
			e la volta successiva l'altro. L'accensione di uno o dell'altro 
			display è comandata dal flag "actdisplay" il quale viene invertito 
			ogni 5ms (ogni 5 interrupt) tramite il contatore "muxcount".
			*/
	
			muxcount++; // incremento il contatore per switch display
	
			/* Controllo quale display accendere:
			actdisplay=0 -> devo accendere il display delle unità
			actdisplay=1 -> devo accendere il display delle decine
			*/
		
			if (actdisplay==0) // devo mostrare solo le unità
				{
				displayU=ON; // accendo il display delle unità
				// mi calcolo quindi la sola unità da mostrare
				result=counter/10; // ottengo la decina (esempio: 28/10 = 2)
				// nota: in C la divisione tra interi restituisce un intero, quindi privo di decimali
				result=counter-(result*10); // ottengo la sola unità
				// se, ad esempio, il contatore era 28, result ora 
				//qui vale 2, quindi dal contatore sottraggo (2*10) e ottengo 8
				}
			else // devo mostrare solo le decine
				{
				result=counter/10; // ottengo la sola decina
				// accendo la decina solo se il risultato è maggiore di zero
				if (result)
					{
					displayD=ON;
					}
				}
	
		/*
		Imposto il registro sul quale sono collegati i display.
		I segmenti da accendere in base al numero da visualizzare sono
		contenuti nell'array "dispnum".
		*/
	
		dispReg=dispnum[result]; 
	
		// in base al valore raggiunto da muxcount, cambio display
		// ogni display rimane acceso in pratica 5millisecondi
		if (muxcount==5)
			{
			muxcount=0; // azzero il contatore
			actdisplay=actdisplay^1; // cambio il display per il prossimo ciclo
			dispReg=0; // azzero il display attualmente visualizzato 
		           //per prevenire l'effetto "ghosting"
			displayU=OFF; // spengo display unità
			displayD=OFF; // spengo display decine
			}
		} // fine che i display devono funzionare
		T0IF=0;// Reset flag interrupt su timer 0
		} // End T0IF
} // End ISR
// EOF