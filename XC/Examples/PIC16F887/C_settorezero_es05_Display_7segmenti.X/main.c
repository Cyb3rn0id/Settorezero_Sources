//************************************************************
// CORSO PROGRAMMAZIONE PICMICRO IN C
// (C) Bernardo Giovanni
// http://www.settorezero.com
//
// Tutti i diritti riservati, vietata la
// ridistribuzione e/o riutilizzo anche parziale
// del codice senza consenso scritto dell'autore.
// Al seguente codice si applicano tutte le regole già
// esposte nel disclaimer all'indirizzo:
// http://www.settorezero.com/wordpress/info/disclaimer/
//
//************************************************************
// ESEMPIO 5 (LEZIONE 8)
// Gestione display 7 segmenti
// Links:
// Lezione 8: http://bit.ly/2drCmgv
//************************************************************
//
// modulo:      main.c
// autore:      Bernardo Giovanni
// sito web:    www.settorezero.com
// mcu:         PIC16F887
// clock:       esterno @ 20MHz
// board:       Freedom II
// compilatore: Microchip XC8 (1.44)
// ide:         MPLAB X IDE (4.01)
// programm.:   PICkit 3
//
//************************************************************

#include "conf_16f887.h"
#define _XTAL_FREQ 20000000
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

// porte I/O
#define DISPLAY_REG PORTD // Banco al quale sono collegati i singoli led dei display
#define DISPLAY_M   PORTAbits.RA5 // anodo/catodo comune display migliaia (o decina delle ore)
#define DISPLAY_C   PORTEbits.RE0 // anodo/catodo comune display centinaia (o unità delle ore)
#define DISPLAY_D   PORTEbits.RE1 // anodo/catodo comune display decine (o decina dei minuti)
#define DISPLAY_U   PORTEbits.RE2 // anodo/catodo comune display unità (o unità dei minuti)
#define BUTTON1     PORTBbits.RB4
#define BUTTON2     PORTBbits.RB5

// se il display utilizzato è del tipo a catodo comune,
// commentare la seguente riga
#define COMMON_ANODE

/* se il display è ad anodo comune, i segmenti si accenderanno
 * mandando un livello logico 0 ai singoli catodi e sulla base
 * del transistor PNP che pilota l'anodo comune (che porterà Vdd).
 * Viceversa se il display è a catodo comune, i segmenti si
 * accenderanno mandando un livello logico 1 ai singoli anodi
 * e sulla base del transistor NPN che pilota il catodo comune.
 */
#ifdef COMMON_ANODE
	#define TURN_ON 0
	#define TURN_OFF 1
#else
	#define TURN_ON 1
	#define TURN_OFF 0
#endif

/*
definizione simboli per segmenti
PIN		SEGMENTO
RD0		A
RD1		B
RD2		C
RD3		D
RD4		E
RD5		F
RD6		G
RD7		DOTS

   A
F|---|
 | G |B
  ---
E|   |C
 |---| .DP
   D

Ad ogni segmento (Sn) associo il valore numerico del bit
della porta alla quale il segmento stesso è collegato
(es.: Il segmento A è collegato su RD0, quindi sul bit 0 della porta
 che vale 1, il segmento E è collegato su RD4 => bit 4 => valore 16 ecc
*/

#define	SA	1
#define	SB	2
#define	SC	4
#define	SD	8
#define	SE	16
#define	SF	32
#define	SG	64
#define DOTS    128

// Ogni cifra avrà quindi i suoi segmenti da accendere
// in base al numero (Nn) da comporre, lo faccio sommando il valore
// numerico dei singoli segmenti che compongono la cifra
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
Teniamo conto che i segmenti sono 7 e tutti collegati ad una stessa porta.
L'ottavo bit (bit7) anzichè essere usato per il puntino decimale, lo uso per i due puntini dei secondi.
Definisco i valori in un array in maniera tale da facilitarmi il compito: mi basterà
semplicemente assegnare alla porta l'elemento N-esimo dell'array quando vorrò far apparire il numero N.
Es.: voglio far apparire il numero 7 => PORTD=dispnum[7]
L'array lo definisco come costante (const), dal momento che non varia nell'esecuzione
del programma, inoltre in questo modo viene memorizzato nella memoria programma e non nella ram
*/
const uint8_t dispnum[]={N0,N1,N2,N3,N4,N5,N6,N7,N8,N9};

// Variabili
volatile uint8_t counterinc=0; // counter per incrementare il contatore principale
volatile uint16_t counter=0; // valore del contatore
volatile uint8_t muxcount=0; // contatore multiplex
volatile uint8_t actdisplay=0; // tiene conto del display correntemente visualizzato (0,1,2,3)
volatile bool flash=false; // accendi/spegni i due puntini
volatile bool start=false; // cronometro in cammino/fermo

// prototipi funzione
void settings(void);
void reset(void);
void __interrupt isr(void);

void main(void)
    {
    settings();
    reset();
    ei(); // abilito gli interrupts
    
    while(1)
        {
        // pulsante 1: stop/start
        if (!BUTTON1)
            {
            __delay_ms(50);
            if (!BUTTON1)
                {
                start^=1; 
                // se stop, lascio i due puntini accesi
                if (!start)
                    {
                    flash=true;
                    }
                }
            while(!BUTTON1);
            __delay_ms(50);
            }
        // pulsante 2: counter reset
        if (!BUTTON2)
            {
			__delay_ms(1500);
            if (!BUTTON2)
                {
                reset();
                while(!BUTTON2);
                __delay_ms(50);
                }
            }
        // qui non c'è più nulla. Fa tutto l'interrupt.
        }// while(1)
    }// main

void reset(void)
    {
    counter=0;
    counterinc=0;
    flash=false;
    start=false;
    }

void settings(void)
    {
    ANSEL=0; // pins A/D (0-7) come digitali
    ANSELH=0; // pins A/D (8-13) come digitali
    ADCON0bits.ADON=0; // modulo AD spento
    TRISA=0b11011111; // RA5 come uscita
    TRISB=0xFF; // tutti ingressi
    TRISC=0xFF; // tutti ingressi
    TRISD=0; // tutte uscite (ci sono collegati i segmenti dei display)
    TRISE=0b11111000; // RE0,1,2 come uscite

    // configurazione registro OPTION_REG
    OPTION_REGbits.T0CS=0; // sorgente clock TMR0 da Fosc/4
    OPTION_REGbits.PSA=0; // prescaler assegnato a TMR0
    OPTION_REGbits.PS=0b100; // prescaler 1:32
    OPTION_REGbits.nRBPU=0; // abilito resistenze di pull-up su portB

    // configurazione registro INTCON
    INTCONbits.T0IE=1; // interrupt su overflow TMR0 abilitato
    TMR0=100; // Imposto Timer0 a 100
    }

// interrupt vector
void __interrupt isr(void)
    {
    if (INTCONbits.T0IF) // L'interrupt è stato causato da un overflow del timer0 ?
        {
        // Ricarico subito il Timer0
        TMR0=100;
        if (start)
        {
        /* gestione del counter principale: incremento il counter "counterinc"
         * ogni millisecondo. Arrivato a 10 incremento il counter principale "counter"
         * in modo che la cifra delle unità scatta ogni 10mS (centesimi di secondo)
         * quindi la cifra delle decine scatta ogni 100mS (decimi di secondo) e la cifra
         * delle centinaia ogni secondo.
         */

        counterinc++;
        if (counterinc==10)
            {
            counterinc=0;
            counter++;
            // con 4 display non posso mostrare più di 9999: ritorno a zero
            if (counter>9999)
                {
                counter=0;
                }
            }
        
        /* gestione due punti lampeggianti: incremento il counter "flashdots"
         * ad ogni interrupt, quindi ogni 1mS. Quando il counter è arrivato a 500
         * (ovvero: dopo 500mS) inverto il valore del bit "flash" che mi farà
         * accendere (1) o spegnere (0) i due punti. In questo modo tra i due
         * punti accesi passa 1 secondo.
         */
        }
        
        uint16_t a=0; // variabile temporanea
        uint16_t result=0; // variabile che conterrà la singola cifra da visualizzare
         /*
         * Multiplexing display
         * Visualizzo sui display il valore attuale della variabile "counter".
         * Per fare questo devo accendere alternativamente prima un display
         * e la volta successiva l'altro. L'accensione di uno o dell'altro
         * display è comandata dalla variabile "actdisplay" la quale viene incrementata
         * ogni 5mS (ovvero ogni 5 interrupt tramite il contatore "muxcount").
         * In base al display da visualizzare al momento (unità, decine, centinaia, migliaia)
         * devo estrarre dal contatore soltanto la cifra che mi interessa (solo l'unià, solo la decina ecc)
         * e questo lo faccio con una serie di divisioni (per 1000, 100, 10) e sottrazioni
         * Il valore da mostrare viene immagazzinato nella variabile "result"
         */

        muxcount++; // incremento il contatore per switch display

        switch (actdisplay)
            {
            case 0: // display unità
                DISPLAY_U=TURN_ON; // attivo il display delle unità, da visualizzare sempre anche quando è zero
                // dalla variabile counter devo recuperare la sola unità da mostrare
                a=counter/1000; // ottengo il valore di migliaia (esempio: 2800/1000 = 2)
                // nota: in C la divisione tra interi restituisce un intero, quindi privo di decimali
                result=counter-(a*1000); // ora result contiene il valore di counter senza migliaia
                a=result/100; // ottengo il valore delle centinaia
                result=result-(a*100); // ora result contiene il valore di counter senza migliaia e senza centinaia
                a=result/10; // ottengo il valore delle decine
                result=result-(a*10); // ora result contiene solo il valore delle unità (ho tolto anche le decine)
            break;

            case 1: // display decine
                a=counter/1000;
                result=counter-(a*1000);
                a=result/100;
                result=result-(a*100);
                result=result/10; // ottengo le sole decine
                if (start)
                    {
                    if (result<5) // le decine sono minori di 5: accendo i puntini...
                        {
                        //... ma solo se abbiamo superato i 99 centesimi
                        counter>99?flash=1:flash=0;
                        }
                    else
                        {
                        flash=0;    
                        }
                    }
                
                // accendo il display della decina solo se il valore di counter
                // è superiore a 9, perchè non voglio mostrare uno zero non significativo
                if (counter>9)
                    {
                    DISPLAY_D=TURN_ON;
                    }
            break;

            case 2: // display centinaia
                a=counter/1000;
                result=counter-(a*1000);
                result=result/100; // ottengo le sole centinaia
                if (counter>99)
                    {
                    DISPLAY_C=TURN_ON;
                    }
            break;

            case 3: // display migliaia
                result=counter/1000;
                if (counter>999)
                    {
                    DISPLAY_M=TURN_ON;
                    }
            break;
            }// switch (actdisplay)

	/*
	Ora imposto il registro sul quale sono collegati i display.
	I segmenti da accendere in base al numero da visualizzare sono
	contenuti nell'array "dispnum".
	*/

        #ifdef COMMON_ANODE
        DISPLAY_REG=(unsigned)~(dispnum[result]+(flash*DOTS)); // per l'anodo comune nego il valore
        #else
        DISPLAY_REG=(unsigned)dispnum[result]+(flash*DOTS);
        #endif

        // in base al valore raggiunto da muxcount, cambio display
	// ogni display rimane acceso in pratica 5millisecondi
	if (muxcount==5)
            {
            muxcount=0; // azzero il contatore
            actdisplay++;// cambio il display per il prossimo ciclo
            if (actdisplay==4)
                {
                actdisplay=0;
                }
            #ifdef COMMON_ANODE
            DISPLAY_REG=(unsigned)(0xFF-(flash*DOTS)); // azzero il display attualmente visualizzato per prevenire l'effetto "ghosting"
            #else
            DISPLAY_REG=(unsigned)(flash*DOTS);
            #endif
            // spengo gli altri display
            switch(actdisplay)
                {
                case 0:
                DISPLAY_D=TURN_OFF;
                DISPLAY_C=TURN_OFF;
                DISPLAY_M=TURN_OFF;
                break;

                case 1:
                DISPLAY_U=TURN_OFF;
                DISPLAY_C=TURN_OFF;
                DISPLAY_M=TURN_OFF;
                break;

                case 2:
                DISPLAY_U=TURN_OFF;
                DISPLAY_D=TURN_OFF;
                DISPLAY_M=TURN_OFF;
                break;

                case 3:
                DISPLAY_U=TURN_OFF;
                DISPLAY_D=TURN_OFF;
                DISPLAY_C=TURN_OFF;
                break;
                }
            }// if (muxcount==5)
        INTCONbits.T0IF=0; // Resetto il flag interrupt su timer 0,
        } // fine che interrupt verificatosi su timer0
    }
