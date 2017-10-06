//*************************************************
//
// main.c
// autore: Bernardo Giovanni (www.settorezero.com)
// data: 21/10/09
//
//*************************************************

#define  XTAL_FREQ 20MHZ
#include <pic.h>

__CONFIG (HS & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTEN & DEBUGDIS & UNPROTECT);

#include "settings.h"
#include "delay.c"
#include "lcd.c"

// prototipi di funzione
void main(void); // programma principale
void beep(char beeptype); // esegue il beep, usato quando premo i tasti
void posizionafreccia(char posizione); // posiziona la freccia nel menù
void cambiastato(char posizione); // inverte lo stato del led
void scrivistato(char posizione,char stato); // scrive lo stato del led sul display

void main(void)
        {
		settings();
		// avvio l'LCD e scrivo lo stato di default dei led		
		LCD_INIT();
		DelayMs(100);
		LCD_CLEAR();
		LCD_GOTO(1,2);
		LCD_PUTS("L1:OFF");
		LCD_GOTO(2,2);
		LCD_PUTS("L2:OFF");
		LCD_GOTO(1,11);
		LCD_PUTS("L3:OFF");
		LCD_GOTO(2,11);
		LCD_PUTS("L4:OFF");
		LCD_GOTO(1,1);
		LCD_PUTCH(ARROW); // freccia
		
		while(1)
			{
			// pressione tasto SU
			// decremento il contatore che mi posiziona la freccia
			if (BTNUP==PRESSED)
				{
				DelayMs(100);
				if (BTNUP==PRESSED)
					{
					beep(0); // suono
					menupos--; // decremento la posizione
					// se in posizione 0 ho premuto su
					// vado in posizione 3
					if (menupos<0)
						{
						menupos=3;
						}
					// posiziono quindi la freccia
					posizionafreccia(menupos);
					}
				} // fine pressione tasto su
				
			// pressione tasto GIU
			// incremento il contatore che mi posiziona la freccia
			if (BTNDOWN==PRESSED)
				{
				DelayMs(100);
				if (BTNDOWN==PRESSED)
					{
					beep(0); // suono
					menupos++; // incremento la posizione
					// se in posizione 3 ho premuto giù
					// vado in posizione 0
					if (menupos>3)
						{
						menupos=0;
						}
					// posiziono quindi la freccia
					posizionafreccia(menupos);
					}
				} // fine pressione tasto su
				
			// pressione tasto OK
			if (BTNOK==PRESSED)
				{
				DelayMs(100);
				if (BTNOK==PRESSED)
					{
					beep(1); // suono
					// chiamo la routine cambiastato
					// passandogli la posizione attuale della freccia
					cambiastato(menupos);
					}
				}
				
		}// Fine ciclo continuo
	} // fine main
		

void beep(char beeptype)
	{
	char F;
	switch (beeptype)
		{
		case 0:
		F=70;
		break;
		
		case 1:
		F=90;
		break;
		
		case 2:
		F=110;
		break;
		}
		
	for (char a=40; a--; a!=0)
		{
		DelayUs(F);
		CICALINO=ON;
		DelayUs(F);
		CICALINO=OFF;
		}
	}

void posizionafreccia(char posizione)
	{
	// cancello dapprima tutte le altre frecce presenti
	// in pratica scrivo uno spazio nelle 4 locazioni
	// dove la freccia può apparire
	LCD_GOTO(1,1);
	LCD_PUTCH(32); // 32=codice dello spazio
	LCD_GOTO(2,1);
	LCD_PUTCH(32);
	LCD_GOTO(1,10);
	LCD_PUTCH(32);
	LCD_GOTO(2,10);
	LCD_PUTCH(32);
	
	// in base al valore "posizione" passato alla funzione
	// piazzo il cursore del display nella posizione giusta
	// in cui scrivere la freccia
	switch (posizione)
		{
		case 0:
		LCD_GOTO(1,1);
		break;
		
		case 1:
		LCD_GOTO(2,1);
		break;
		
		case 2:
		LCD_GOTO(1,10);
		break;
		
		case 3:
		LCD_GOTO(2,10);
		break;
		}
	
	// infine scrivo il carattere della freccia
	LCD_PUTCH(ARROW);
	}

void cambiastato(char posizione)
	{
	// inverto lo stato del led in base alla posizione
	// della freccia sul menù.
	// dopo aver invertito lo stato, richiamo la funzione
	// scrivistato, che mi scriverà sul display se il led
	// selezionato è acceso o spento
	
	// nella variabile STATO_LED memorizzo lo stato attuale
	// del led dopo il cambio stato, che utilizzerò per passarlo
	// alla funzione "scrivistato"
	char STATO_LED;
	
	// (Nota: in realtà STATO_LED, contiene un unico bit
	// per cui avrei dovuto dichiararla come bit, ma il compilatore
	// non mi accetta una variabile bit come argomento di una 
	// funzione).
	
	switch (posizione)
		{
		case 0:
		LED1=LED1^1;
		STATO_LED=LED1;
		break;
		
		case 1:
		LED2=LED2^1;
		STATO_LED=LED2;
		break;
		
		case 2:
		LED3=LED3^1;
		STATO_LED=LED3;
		break;
		
		case 3:
		LED4=LED4^1;
		STATO_LED=LED4;
		break;
		}
	// richiamo la funzione per aggiornare il display
	scrivistato(posizione,STATO_LED);
	}
	
void scrivistato(char posizione,char stato)
	{
	// questa funzione aggiorna lo stato del led sul display
	// metto il cursore sulla posizione adatta
	switch(posizione)
		{
		case 0:
		LCD_GOTO(1,5);
		break;
		
		case 1:
		LCD_GOTO(2,5);
		break;
		
		case 2:
		LCD_GOTO(1,14);
		break;
		
		case 3:
		LCD_GOTO(2,14);
		break;
		}
		
		// dopodichè scrivo ON o OFF
		// in base al valore del parametro
		// stato passato alla funzione
		if (stato==ON)
			{
			LCD_PUTS("ON "); // da notare che ho aggiunto uno spazio alla fine, per cancellare l'ultima F di OFF
			}
		else
			{
			LCD_PUTS("OFF");
			}
	}