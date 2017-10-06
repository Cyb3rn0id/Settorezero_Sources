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
// ESEMPIO 1 (LEZIONE 3)
// Lampeggia un led utilizzando i ritardi
// Links alle lezioni:
// Lezione 1: http://bit.ly/2e1JSh6
// Lezione 2: http://bit.ly/2cSlCNW
// Lezione 3: http://bit.ly/2dtk7US
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

// nel seguente file esterno ho incluso la configurazione del PICmicro
// generata tramite il tool presente in MPLAB X e accessibile da
// Window->Pic Memory Views->Configuration Bits
// ora scrivere la config word è molto più facile!
#include "conf_16f887.h"

// utilizzato quarzo da 20MHz, la seguente define è utilizzata
// unicamente dalle routine di ritardo incluse nell'XC8
#define _XTAL_FREQ 20000000

// mediante questo file l'IDE estrae le caratteristiche del PIC
// selezionato nel progetto (nomi dei registri, aree di
// memoria ecc. - ha la stessa funzione di htc.h/pic.h quando
// utilizzavamo l'Hi-Tech PICC)
#include <xc.h>

// LED su porta RD0 (LED1 su Freedom II)
// I nomi dei bit singoli presi da soli (es.: RD0) non sono più supportati
// quando si vuole fare riferimento ad un bit singolo si usa
// nome del registro in maiuscolo a cui si aggiunge "bits", quindi punto
// e nome del bit in maiuscolo
#define LED PORTDbits.RD0

void main(void)
    {
    // disattivo le funzioni associate al convertitore A/D
    // dal momento che all'avvio sono abilitate e alcuni pin
    // che crediamo possano funzionare come IO digitali, si comportano
    // invece da ingressi analogici.
    ANSEL=0; // pins A/D (0-7) con funzioni di I/O digitale
    ANSELH=0; // pins A/D (8-13) con funzioni di I/O digitale
    ADCON0bits.ADON=0; // modulo A/D spento

    // imposto i registri tristato in maniera tale che tutte le porte
    // siano configurate come pin di ingresso diminuendo il rischio
    // di eventuali cortocircuiti accidentali sui pin non utilizzati
    TRISA=0xFF;
    TRISB=0xFF;
    TRISC=0xFF;
    TRISD=0b11111110; // solo RD0 come uscita: qui è collegato il led
    // potevo anche scrivere: TRISDbits.TRISD0=0;
    TRISE=0xFF;
    
    // ciclo infinito
    while(1)
        {
        // inverto lo stato della porta a cui è collegato il LED
        LED^=1; // LED = LED XOR 1

        // la seguente funzione di ritardo è inclusa nel compilatore XC
        // e accetta unicamente costanti, non utilizzarla con variabili!
        __delay_ms(500); //500mS di attesa
        }
    }// fine main