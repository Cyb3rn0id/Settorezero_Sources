//*************************************************
//
// main.c
// autore: Bernardo Giovanni (www.settorezero.com)
// data: 01/11/09
// esempio animazione Pacman
//
//*************************************************

#define  XTAL_FREQ 20MHZ
#include <pic.h>
#include <stdio.h>

__CONFIG (HS & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTEN & DEBUGDIS & UNPROTECT);

#include "settings.h" // impostazioni di configurazione
#include "delay.c" // routine per ritardi
#include "lcd.c" // comando display lcd HD44780A

#define pacmanR 0
#define pacmanL 1
#define dot 2
#define ghost 3

void beep(void); // prototipo di funzione
void main(void);

void main(void)
        {
        
		settings();
        LCD_INIT();
				
		char posU=1; // contatore per il pacman superiore
		char posD=16; // contatore per il pacman inferiore
						
		LCD_CUSTOMCHAR(pacmanR,0,14,29,31,24,31,14,0); // pacman con bocca aperta verso destra
		LCD_CUSTOMCHAR(pacmanL,0,14,23,31,3,31,14,0);  // pacman con bocca aperta verso sinistra
		LCD_CUSTOMCHAR(dot,0,0,0,6,6,0,0,0);  // puntino da mangiare
		LCD_CUSTOMCHAR(ghost,14,31,21,31,31,31,21,0);  // fantasmino
		
		LCD_CLEAR(); // pulisco il display
		
		while(1)
			{
			// i due pacman partono in questa posizione:
			// O...............
			// ...............O
			
			// Ciclo principale: 16 iterazioni
			for (char c=0; c<16; c++)
				{
				
				LCD_CLEAR(); // ad ogni iterazione pulisco il display
				
				// posiziono il pacman superiore (riga 1)
				LCD_GOTO(1,posU);
				LCD_PUTCH(pacmanR);
				
				// disegno i puntini dalla posizione pacman+1 fino alla 16
				for (char b=posU+1; b<17; b++)
					{
					LCD_PUTCH(dot);
					}
					
				// sulla seconda riga disegno i puntini dalla posizione 1 fino alla posizione pacman-1
				LCD_GOTO(2,1);
				for (char b=1; b<posD; b++)
					{
					LCD_PUTCH(dot);
					}
					
				// posiziono il pacman inferiore
				LCD_PUTCH(pacmanL);
				
				posU++; // incremento il contatore posizione del pacman superiore
				posD--; // decremento il contatore posizione del pacman inferiore
				
				beep(); // suono
				DelayMs(150); // ritardo tra uno "gnam!" e l'altro
				} // fine 16 iterazioni
				
			// resetto le variabili per posizionare il pacman
			posU=1;
			posD=16;
			
			LCD_CLEAR();
			DelayMs(100);
						
			LCD_PUTS("SETTOREZERO.COM");
			
			DelayMs(200);
			DelayMs(200);
			DelayMs(200);
			DelayMs(200);
			
			LCD_GOTO(2,1);			
			
			// qui uso un'altra  tecnica: invece di cancellare tutto il display
			// per far rimanere la scritta superiore
			// disegno gli spazi vuoti sul carattere da cancellare
			for (char a=0; a<17; a++) // mettendo 17 il fantasmino esce di scena
				{
				LCD_GOTO(2,a);
				LCD_PUTCH(32);//spazio sulla posizione precedente
				LCD_GOTO(2,a+1);
				LCD_PUTCH(ghost); // fantasmino
				DelayMs(100);
				}
			
			DelayMs(200);
			DelayMs(200);
			DelayMs(200);
			DelayMs(200);
			DelayMs(200);
			DelayMs(200);
			}
        }// Fine ciclo continuo
	
void beep()
	{
	for (char a=20; a--; a!=0)
		{
		DelayUs(80);
		CICALINO=1;
		DelayUs(80);
		CICALINO=0;
		}
	}