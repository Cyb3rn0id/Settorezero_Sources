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
// ESEMPIO 4 (LEZIONE 7)
// Gestione lcd alfanumerico basato su controller HD44780
// Links:
// Lezione 7 (pt.1): http://bit.ly/2dtnOtU
// Lezione 7 (pt.2): http://bit.ly/2dEqdDm
// Lezione 7 (pt.3): http://bit.ly/2dEpIcg
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

// bisogna includere il file H della libreria, non il C!
// il file C viene incluso automaticamente dato che si trova
// nella cartella "Source Files" del progetto!
#include "hd44780sz.h"

// prototipi funzione
void settings(void);

// questi define li utilizzerò per i caratteri personalizzati:
// do dei nomi per ricordarmeli più facilmente!
#define PLAY    0
#define STOP    1
#define CUORE   2
#define INVADER 3
#define SOLE    4
#define TESCHIO 5
#define SMILE   6
#define SAD     7
// i caratteri personalizzabili possono essere al massimo 8
// se dobbiamo definirne altri, devono sovrascrivere quelli
// precedenti!
#define PACMANR 0
#define PACMANL 1
#define DOT     2
#define GHOST   3

// array con le definizioni dei caratteri personalizzati
// per crearli potete usare il mio software "Custom Char HD44780"
// scaricabile gratuitamente da questo url: http://bit.ly/2yKsyUG
const char play[]={16,24,28,30,28,24,16,0};
const char stop[]={0,31,31,31,31,31,0,0};
const char cuore[]={0,10,31,31,14,4,0,0};
const char invader[]={17,10,31,21,31,31,10,27};
const char sole[]={4,17,14,31,14,17,4,0};
const char teschio[]={14,31,21,31,14,0,14,0};
const char smile[]={0,10,10,0,17,14,0,0};
const char sad[]={0,10,10,0,14,17,0,0};

const char pacmanr[]={0,14,29,31,24,31,14,0};
const char pacmanl[]={0,14,23,31,3,31,14,0};
const char dot[]={0,0,0,6,6,0,0,0};
const char ghost[]={14,31,21,31,31,31,21,0};

int main(void)
    {
    settings();

    // inizializzo il display
    // Ricordo che l'esempio è realizzato per un
    // display da 2 righe e 16 colonne.
    LCD_Init();
    LCD_Backlight(ON);

    while(1)
        {
        // esempio scrittura stringhe
        LCD_Writestr("Hello World!");
        LCD_Goto(2,1);
        LCD_Writestr("settorezero.com");
        __delay_ms(3000);
        LCD_Clear();

        // esempio scrittura numeri interi
        LCD_Writestr("Unsigned: ");
        LCD_Writeuns(1977);
        LCD_Goto(2,1);
        LCD_Writestr("Signed: ");
        LCD_Writesig(-12345);
        __delay_ms(3000);
        LCD_Clear();

        float numeroa=-1.23456;
        float numerob=0.7654;
        float numeroc=45.16;
        LCD_Writestr("Floats: ");
        LCD_Writeflo(numeroa,3);
        LCD_Goto(2,1);
        LCD_Writeflo(numerob,4);
        LCD_Putchar(' ');
        LCD_Writeflo(numeroc,1);
        __delay_ms(3000);
        LCD_Clear();
        
        // esempio caratteri personalizzati
        // carico nella CGRam del display i caratteri personalizzati
        // il primo parametro della funzione è la posizione nella CGRAM (0-7)
        // il secondo parametro è l'array di valori
        LCD_Customchar(PLAY,play);
        LCD_Customchar(STOP,stop);
        LCD_Customchar(CUORE,cuore);
        LCD_Customchar(INVADER,invader);
        LCD_Customchar(SOLE,sole);
        LCD_Customchar(TESCHIO,teschio);
        LCD_Customchar(SMILE,smile);
        LCD_Customchar(SAD,sad);
        // dopo aver definito i caratteri personalizzati
        // bisogna sempre ripulire il display!
        LCD_Clear();
        LCD_Writestr("Caratteri custom");
        LCD_Goto(2,1);
        // ora scrivo i caratteri utilizzando LCD_Putchar e il
        // numero che gli ho assegnato
        LCD_Putchar(PLAY);
        LCD_Putchar(STOP);
        LCD_Putchar(CUORE);
        LCD_Putchar(INVADER);
        LCD_Putchar(SOLE);
        LCD_Putchar(TESCHIO);
        LCD_Putchar(SMILE);
        LCD_Putchar(SAD);
        __delay_ms(3000);
        LCD_Clear();

        // esempio animazione
        uint8_t posU=1; // contatore per il pacman superiore
        uint8_t posD=16; // contatore per il pacman inferiore
        // ridefinisco i caratteri personalizzati
        LCD_Customchar(PACMANR,pacmanr); // pacman con bocca aperta verso destra
        LCD_Customchar(PACMANL,pacmanl);  // pacman con bocca aperta verso sinistra
        LCD_Customchar(DOT,dot);  // puntino da mangiare
        LCD_Customchar(GHOST,ghost);  // fantasmino
        LCD_Clear(); // pulisco il display
        // i due pacman partono in questa posizione:
        // O...............
        // ...............O
        // Ciclo principale: 16 iterazioni
        for (int8_t c=0; c<16; c++)
            {
            LCD_Clear();
            // posiziono il pacman superiore (riga 1)
            LCD_Goto(1,posU);
            LCD_Putchar(PACMANR);
            // disegno i puntini dalla posizione pacman+1 fino alla 16
            for (int8_t b=posU+1; b<17; b++)
                {
                LCD_Putchar(DOT);
                }
            // sulla seconda riga disegno i puntini dalla posizione 1 fino alla posizione pacman-1
            LCD_Goto(2,1);
            for (int8_t b=1; b<posD; b++)
                {
                LCD_Putchar(DOT);
                }
            // posiziono il pacman inferiore
            LCD_Putchar(PACMANL);
            posU++; // incremento il contatore posizione del pacman superiore
            posD--; // decremento il contatore posizione del pacman inferiore
            __delay_ms(150); // ritardo tra uno "gnam!" e l'altro
            } // fine 16 iterazioni
            // resetto le variabili per posizionare il pacman
            posU=1;
            posD=16;
            LCD_Clear();
            __delay_ms(100);
            LCD_Writestr("SETTOREZERO.COM");
            __delay_ms(1000);
            LCD_Goto(2,1);
            // qui uso un'altra  tecnica: invece di cancellare tutto il display
            // per far rimanere la scritta superiore
            // disegno gli spazi vuoti sul carattere da cancellare
            for (uint8_t a=0; a<17; a++) // mettendo 17 il fantasmino esce di scena (ammesso che si stia usando un display da 16 colonne!)
                {
                LCD_Goto(2,a);
                LCD_Putchar(' ');
                LCD_Goto(2,a+1);
                LCD_Putchar(GHOST); // fantasmino
                __delay_ms(140);
                }
            LCD_Clear();
        }// fine ciclo infinito
    }// fine main

void settings(void)
    {
    ANSEL=0; // pins A/D (0-7) come digitali
    ANSELH=0; // pins A/D (8-13) come digitali
    ADCON0bits.ADON=0; // modulo AD spento
    TRISA=0xFF;
    TRISB=0xFF;
    TRISC=0xFF;
    TRISD=0xFF;
    TRISE=0xFF;
    }