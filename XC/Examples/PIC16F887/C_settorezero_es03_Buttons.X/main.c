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
// ESEMPIO 3 (LEZIONE 6)
// Gestione pulsanti
// leds on/off su pressione pulsanti.
// Links:
// Lezione 6: http://bit.ly/2dtnxXz
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

/* ATTENZIONE !
 * dato che il PICkit comunica su RB6 e RB7, per questo esempio non è possibile
 * utilizzare il PICkit come sorgente di alimentazione altrimenti interferisce
 * con i pulsanti 3 e 4. In alternativa se avete realizzato il circuito su 
 * millefori, potete collegare i pulsanti su altre porte ma ricordate che c'è 
 * bisogno delle resistenze di pull-up esterne se il PICmicro che avete scelto
 * non le ha sulle porte su cui avete collegato i pulsanti. Il PIC16F887 ha le
 * resistenze di pull-up solo sul banco B. Il 16F18877 le ha su tutti i banchi,
 * su quest'ultimo si possono abilitare agendo sui registri WPUA, WPUB, WPUC,
 * WPUD,WPUE (dopo aver disabilitato la funzione analogica da ANSELA, ANSELB...)
 */

#include "conf_16f887.h"
#define _XTAL_FREQ 20000000
#include <xc.h>

#define BUTTON1	PORTBbits.RB4
#define BUTTON2	PORTBbits.RB5
#define BUTTON3	PORTBbits.RB6
#define BUTTON4	PORTBbits.RB7
#define LED1	PORTDbits.RD0
#define LED2	PORTDbits.RD1
#define LED3	PORTDbits.RD2
#define LED4	PORTDbits.RD3

// prototipi funzione
void settings(void);

int main(void)
    {
    settings();
    
    while(1)
        {
        /*
        Controllo pulsanti.
        Normalmente i pin ai quali sono collegati i pulsanti si trovano
        a livello logico alto tramite le resistenze di pull-up 
        (esterne o integrate, in questo caso abbiamo abilitato le resistenze
        di pull-up integrate portando a zero il bit nRBPU del registro
        OPTION_REG). Quando il pulsante viene premuto, il pin viene
        portato a livello logico basso, per cui mi controllo questo stato.
        
        A differenza del vecchio esempio in cui facevo accendere il led solo
        su pressione del pulsante e quindi spegnerlo al rilascio, qui illustro
        4 comportamenti differenti con 4 routine diverse. Leggi il commento
         al di sopra di ogni routine di gestione pulsante
        */
        
        // pulsante 1 - led 1
        // il led si accende su pressione del pulsante e si spegne
        // non appena si rilascia il pulsante.
        if (!BUTTON1) 
            {
            __delay_ms(100); // ritardo per antirimbalzo
            // se dopo 100ms il pulsante è ancora premuto, allora non si
            // tratta di un rimbalzo
            if (!BUTTON1)
                {
                LED1=1; // accendo il led
                __delay_ms(50);
                }
            }
        else // pulsante non premuto, spengo il led
            {
            LED1=0;
            }

        // pulsante 2 - led 2
        // controllo il pulsante con l'antibounce
        // invertendo lo stato del led ad ogni pressione
        // se tengo premuto, il led lampeggia
        if (!BUTTON2) 
            {
            __delay_ms(100);
            if (!BUTTON2)
                {
                LED2^=1;
                }
            }
       
        // pulsante 3 - led 3
        // inverto lo stato del led dopo pressione del pulsante
        // ma non eseguo ulteriori cambiamenti fino a che il pulsante 
        // non viene rilasciato
        if (!BUTTON3)
            {
            // eseguo comunque un controllo antibounce
             __delay_ms(100);
            if (!BUTTON3)
                {
                LED3^=1;
                // rimango bloccato fino a che non viene rilasciato il pulsante
                while(!BUTTON3);
                // attendo un po' prima di tornare al main per non calcolare
                // i rimbalzi dovuti al rilascio
                __delay_ms(100);
                }
            }
        
        // pulsante 4 - led 4
        // controllo il pulsante senza antibounce
        // invertendo lo stato del led ad ogni pressione
        // potete notare che la gestione del pulsante è impossibile
        // a causa dei rimbalzi
        if (!BUTTON4)
            {
            LED4^=1;
            }
        }// fine ciclo infinito
    }// fine main

void settings(void)
    {
    ANSEL=0; // pins A/D (0-7) come digitali
    ANSELH=0; // pins A/D (8-13) come digitali
    ADCON0bits.ADON=0; // modulo AD spento

    TRISA=0xFF;
    TRISB=0b11111111; // RB7-6-5-4 come ingressi => pulsanti
    TRISC=0xFF;
    TRISD=0b11110000; // RD0-1-2-3 come uscite => leds
    TRISE=0xFF;
    
    // Led spenti all'avvio
    LED1=0;
    LED2=0;
    LED3=0;
    LED4=0;

    OPTION_REGbits.nRBPU=0; // abilito resistenze di pull-up su portB
    }