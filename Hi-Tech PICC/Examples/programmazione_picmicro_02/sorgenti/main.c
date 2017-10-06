//*************************************************
// CORSO PROGRAMMAZIONE PICMICRO
// Lezione 5
// www.settorezero.com
//
// modulo: main.c
// autore: Bernardo Giovanni
// data: 26/09/09
// descrizione: lampeggia un led su RD0 tramite interrupt e suona un cicalino su RE1
// picmicro: PIC16F877A
// clock: 20MHz
//
//*************************************************

#define  XTAL_FREQ 20MHZ // questo è utilizzato dalle routine di ritardo contenute in Delay.C
#include <pic.h> // contiene i nomi mnemonici di registri e porte

// Fuses di configurazione
__CONFIG (HS & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTEN & DEBUGDIS & UNPROTECT);

#include "delay.c" // routine per ritardi
#include "settings.h" // settaggi del picmicro

// funzione principale, eseguita all'avvio del picmicro
void main(void)
	{
	
	settings(); // eseguo la funzione settings contenuta nel file header settings.h, così imposto le porte e i registri
	
	while(1) // eseguo un ciclo finito
        {
		/*
		L'unica cosa che eseguo durante questo ciclo infinito, è l'inversione
		dello stato del cicalino ogni 200microsecondi, in maniera tale da generare
		un'onda quadra di 2,5KHz che, applicata al cicalino, appunto,
		mi permette di farlo suonare facendogli emettere una nota a tale frequenza
		*/
		DelayUs(200);
		CICALINO=1;
		DelayUs(200);
		CICALINO=0;
		}// Fine ciclo continuo
	
	} // Fine main
	
/*
Questa routine, avendo l'attributo "interrupt" prima del nome della routine stessa, viene chiamata in automatico
ogni qualvolta si verifica un interrupt. Essendo le sorgenti di interrupt di vari tipi, in questa routine dobbiamo
capire quale elemento ha generato l'interrupt. Con le impostazioni utilizzate, Timer0 genererà un interrupt ogni
millisecondo.
*/
void interrupt ISR (void)
	{
	if (T0IF) // L'interrupt è stato causato da un overflow del timer0 ?
		{
		TMR0 = 100; // Reimposto Timer0
		TimerLed++; // Incremento il Timer per il lampeggio del led
		if (TimerLed >= TEMPOLED) // Se il tempo è passato
			{
			LED=LED^1; // Inverto lo stato del led per farlo lampeggiare
			TimerLed=0; // Ricarico il timer del led per ricominciare daccapo
			}
		T0IF=0; // Resetto il flag interrupt su timer 0,
		} // fine che interrupt verificatosi su timer0
	} // fine interrupt service routine