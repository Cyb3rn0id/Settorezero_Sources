//*************************************************
// CORSO PROGRAMMAZIONE PICMICRO
// Lezione 8
// http://www.settorezero.com
//
// modulo: settings.h
// autore: Bernardo Giovanni
// data:  06/12/09
// descrizione: settaggi per contatore up/down 
//              con display a 7 segmenti in 
//              multiplex
// picmicro: PIC16F877A
// clock: 20MHz
//
//*************************************************

// porte I/O
#define dispReg PORTB // Banco al quale sono collegati i display
#define Pressed 0 // Bottone premuto
#define ON 1
#define OFF 0
#define displayU RD7 // attivazione catodo display unità
#define displayD RD6 // attivazione catodo display decine
#define btnUp RD5 // pulsante incremento contatore
#define btnDn RD4 // pulsante decremento contatore
#define btnMe RD3 // pulsante memoria
#define btnRc RD2 // pulsante richiamo memoria

/*
definizione simboli per segmenti
PIN		SEGMENTO
RB6		A
RB5		B
RB4		C
RB3		D
RB2		E
RB1		F
RB0		G
   A
F|---|
 | G |B
  ---
E|   |C
 |---|
   D
ad ogni segmento (Sn) associo proprio il valore del bit
al quale il segmento è collegato (es.: Segmento A, 
collegato su RB6 => valore del bit 6 = 64 ecc)
*/
#define	SA	64
#define	SB	32
#define	SC	16
#define	SD	8
#define	SE	4
#define	SF	2
#define	SG	1

// Segmenti da accendere in base al numero (Nn) da comporre
#define N0 SA+SB+SC+SD+SE+SF
#define N1 SB+SC
#define N2 SA+SB+SG+SE+SD
#define N3 SA+SB+SG+SC+SD
#define N4 SF+SG+SB+SC
#define N5 SA+SF+SG+SC+SD
#define N6 SA+SF+SG+SE+SC+SD
#define N7 SA+SB+SC
#define N8 SA+SB+SF+SG+SC+SD+SE
#define N9 SA+SB+SF+SG+SC+SD

/*
Seguono i valori da assegnare al registro per far accendere i segmenti giusti per far apparire il numero.
Teniamo conto che i segmenti sono 7 e tutti collegati ad una stessa porta. L'ottavo bit (bit7)
non verrà utilizzato (si potrebbe usare per pilotare il punto). 
Definisco i valori in un array in maniera tale da facilitarmi il compito: mi basterà
semplicemente assegnare alla porta l'elemento N-esimo dell'array quando vorrò far apparire il numero N.
Es.: voglio far apparire il numero 7 => PORTB=dispnum[7]

L'array lo definisco come costante (const), dal momento che non varia nell'esecuzione
del programma, inoltre in questo modo viene memorizzato nella memoria programma e non nella ram
*/
const unsigned char dispnum[]={N0,N1,N2,N3,N4,N5,N6,N7,N8,N9};

// Variabili
unsigned char counter=0; // valore del contatore
unsigned char result=0; // variabile di appoggio per calcoli
unsigned char muxcount=0; // contatore multiplex
bit displayson; // per spegnere display
bit actdisplay; // flag che tiene conto del display correntemente visualizzato

// prototipi funzioni
void main(void);
void settings(void);
void memory(void);
void recall(void);
void interrupt isr(void);

void settings(void)
	{
    // 1 = input
    // 0 = output
    TRISA=0; // Porte A tutte uscita
    TRISB=0; // Porte B tutte uscita
	TRISC=0; // Porte C tutte uscita
	TRISD=0b00111100; // Porte D tutte uscita tranne 2,3,4 e 5 dove sono collegati i pulsanti
	TRISE=0; // Porte E tutte uscita
    	
	// Impostazione del registro OPTION (Pag.16)
	// per ottenere un interrupt ogni millisecondo con un quarzo da 20MHz
	// bisogna impostare il prescaler a 31 e inizializzare timer0 a 100
    OPTION=0b11000100;
	// bit 0 -> Prescaler Rate Select bit 0
	// bit 1 -> Prescaler Rate Select bit 0
	// bit 2 -> Prescaler Rate Select bit 0 (1:32)
	// bit 3 -> Prescaler assegnato al Timer0 
	// bit 4 -> Non importa
	// bit 5 -> Clock per Timer0 derivato da ciclo di clock interno
	// bit 6 -> Non importa
	// bit 7 -> Resistenze di pull-up su porta B disattivate
	
	// Impostazione Interrupt (pag.17), attivo soltanto l'interrupt sul timer0, la GIE la attivo dal main dopo
    INTCON=0b10100000;
    // bit 0 -> RBIF		Flag Interrupt su porte B (variabile RBIF)
    // bit 1 -> INTF		Flag Interrupt su porta RB0/INT (variabile INTF)
    // bit 2 -> TMR0IF		Flag Interrupt su Timer0 (variabile T0IF oppure TMR0IF)
    // bit 3 -> RBIE		Interrupt su porte B 0=disattivato
    // bit 4 -> INTE		Interrupt su porta RB0/INT 0=disattivato
    // bit 5 -> TMR0IE		Interrupt su Timer0 1=attivato
    // bit 6 -> PEIE		Interrupt di periferica 0=disattivatoo
    // bit 7 -> GIE			Gestione Globale Interrupts 1=attivata
	TMR0=100; // il timer0 deve partire da 100
	}