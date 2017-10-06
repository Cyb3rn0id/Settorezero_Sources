//*************************************************
// CORSO PROGRAMMAZIONE PICMICRO
// Lezione 5
// www.settorezero.com
//
// modulo: settings.h
// autore: Bernardo Giovanni
// data: 26/09/09
// descrizione: configurazione del picmicro
//
//*************************************************

#define	TEMPOLED	250
#define	LED			RD0
#define	CICALINO	RE1

unsigned char TimerLed=0;	//Char è un tipo di dato a 8 bit, quindi può arrivare a contenere valori fino a 255, a noi serve massimo 250, quindi va bene

void settings(void)
	{
	// Tutte le porte come output 
	TRISA=0;
	TRISB=0;
	TRISC=0;
	TRISD=0;
	TRISE=0;
	
	// Impostazione del registro OPTION (pag.55 del datasheet)
	OPTION=0b11000100;
	// bit 0 -> Prescaler Rate Select bit 0
	// bit 1 -> Prescaler Rate Select bit 0
	// bit 2 -> Prescaler Rate Select bit 0 (1:32)
	// bit 3 -> Prescaler assegnato al Timer0 
	// bit 4 -> Non importa
	// bit 5 -> Clock per Timer0 derivato da ciclo di clock interno
	// bit 6 -> Non importa
	// bit 7 -> Resistenze di pull-up su porta B disattivate
	
	// Impostazione Interrupt
	INTCON=0b10100000;
	// bit 0 -> RBIF - Flag interrupt su porte B
	// bit 1 -> INTF - Flag interrupt su RB0/INT
	// bit 2 -> T0IF - Flag interrupt su Timer0
	// bit 3 -> RBIE, Interrupt su porte B disattivato
	// bit 4 -> INTE, Interrupt su porta RB0/INT disattivato
	// bit 5 -> TMR0IE, Interrupt su Timer0 attivato
	// bit 6 -> PEIE, Interrupt di periferica disattivato
	// bit 7 -> GIE, Gestione Interrupt attiva
	
	TMR0=100; // Imposto Timer0 a 100
	}