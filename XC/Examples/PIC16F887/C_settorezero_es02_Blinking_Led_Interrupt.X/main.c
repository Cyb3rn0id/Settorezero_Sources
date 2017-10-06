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
// ESEMPIO 2 (LEZIONE 5)
// Lampeggio led su interrupt Timer0, esempio programma
// multitasking (cicalino che suona mentre led lampeggia)
// Links:
// Lezione 4: http://bit.ly/2dK9i3I
// Lezione 5: http://bit.ly/2dvcSv5
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

// stdint contiene le definizioni universagli per i numeri interi,
// utilizzate da tutti i compilatori
#include <stdint.h>

// Led su RD0
#define LED PORTDbits.RD0
// ogni 250mS viene invertito lo stato del led
// (lampeggio a 2Hz)
#define	TEMPOLED 250

// Cicalino
// NOTA: il cicalino, che deve essere del tipo *passivo*
// sulla Freedom II va abilitato tramite l'apposito jumper (JP6)
// spostandolo sulla sinistra (verso il connettore DB9)
#define CICALINO PORTCbits.RC0

// contatore per eseguire lampeggio del led
// uint8_t è un numero intero unsigned ad 8 bit
// il tipo uint8_t è definito in stdint.h
uint8_t TimerLed=0;

// prototipi funzione
void settings(void); // impostazioni registri
void beep(void); // fa emettere un beep al cicalino
void __interrupt isr(void); // Interrupt Service Routine

int main(void)
    {
    settings(); // richiamo la funzione settings, definita più in basso
    ei(); // abilitiamo gli interrupts (INTCONbits.GIE=1;)

    while(1)
        {
        beep();
        __delay_ms(1000);
        }
    }// fine main

void beep(void)
    {
    /* l'inversione dello stato del cicalino ogni 200microsecondi, 
     * genera una nota a 2,5KHz. Per questo ci vuole un cicalino passivo
     * (che si comporta come un altoparlante). I cicalini attivi emettono una 
     * nota fissa e per farli suonare si da tensione fissa.
     */
    static uint8_t t=200;
    while (t--)
        {
        __delay_us(200);
        CICALINO=1;
        __delay_us(200);
        CICALINO=0;
        }
    }
    
void settings(void)
    {
    ANSEL=0; // pins A/D (0-7) come digitali
    ANSELH=0; // pins A/D (8-13) come digitali
    ADCON0bits.ADON=0; // modulo AD spento

    TRISA=0xFF;
    TRISB=0xFF;
    TRISC=0b11111110; // RC0 come uscita => cicalino
    TRISD=0b11111110; // RD0 come uscita => led
    TRISE=0xFF;

    // configurazione registro OPTION_REG
    OPTION_REGbits.T0CS=0; // sorgente clock TMR0 da Fosc/4
    OPTION_REGbits.PSA=0; // prescaler assegnato a TMR0
    OPTION_REGbits.PS=0b100; // prescaler 1:32
    // nota: PS raggruppa tutti e 3 i bit PS2,PS1 e PS0.
    // volendo è possibile definire anche i 3 bits separatamente

    // configurazione registro 
	INTCONbits.T0IE=1; // interrupt su overflow TMR0 abilitato
    
	// periodo interrupt=
	// 1/(5MHz/32/156)=998.4mS
    // 5MHz è Fosc/4 = 20MHz/4
    TMR0=100; // Imposto Timer0 a 100
    }

// interrupt vector
void __interrupt isr(void)
{
if (INTCONbits.T0IF) // L'interrupt è stato causato da un overflow del timer0 ?
    {
    TMR0=100; // Reimposto Timer0
    TimerLed++; // Incremento il Timer per il lampeggio del led
    if (TimerLed >= TEMPOLED) // Se il tempo è passato
        {
        LED^=1; // Inverto lo stato del led per farlo lampeggiare
        TimerLed=0; // Ricarico il timer del led per ricominciare daccapo
        }
    INTCONbits.T0IF=0; // Resetto il flag interrupt su timer 0,
    } // fine che interrupt verificatosi su timer0
}