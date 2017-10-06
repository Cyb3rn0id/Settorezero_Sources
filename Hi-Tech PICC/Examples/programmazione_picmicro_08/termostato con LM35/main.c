//*************************************************
// CORSO PROGRAMMAZIONE PICMICRO
// Lezione 11
// http://www.settorezero.com
//
// modulo: main.c
// autore: Bernardo Giovanni
// data:  13/03/10
// descrizione: termostato con LM35
// picmicro: PIC16F877A
// clock: 20MHz
//
//*************************************************

#define  XTAL_FREQ 20MHZ
#include <pic.h>

__CONFIG (HS & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTEN & DEBUGDIS & UNPROTECT);

// scrittura eeprom a compilazione
// nella locazione 0 c'è il valore di temperatura set bassa: 0x1E (30) equivale a 15°C
// nella locazione 1 c'è il valore di temperatura set alta: 0x32 (50) equivale a 25°C
__EEPROM_DATA(0x1E,0x24,0x00,0x00,0x00,0x00,0x00,0x00); 

#include "settings.h"
#include "delay.c"
#include "lcd.c"
#include "eeprom_internal.c"

void main(void)
        {
		settings();
		LCD_INIT(); // inizializzo l'LCD
		DelayMs(100); // piccolo ritardo iniziale
		LCD_CLEAR(); // ripulisco il display
		
		// preparo le scritte sul display
		LCD_GOTO(1,1);
		LCD_PUTS("Temp:--.-");
		LCD_GOTO(1,10);
		LCD_PUTCH(0b11011111); // simbolo del grado
		LCD_PUTS("C");
		LCD_GOTO(2,1);
		LCD_PUTS("TL:");
		LCD_GOTO(2,9);
		LCD_PUTS("TH:");
		
		// resetto tutti i flag di stato
		calcola=0;
		save_settings=0;
		start_contatore=0;
		started=0;
		flash=0;
		intv_counter=intv_delay;
		intv_state=MODE_NONE;
	
		LED=0;
		BL=1; // backlight LCD ON
		
		// leggo le impostazioni dall'eeprom
		valore_low=eei_read_byte(0); // locazione 0 => temperatura bassa
		valore_hi=eei_read_byte(1); // locazione 1 => temperatura alta
		
		// valori precedenti per tornare indietro in caso di modifiche errate
		pre_valore_hi=valore_hi;
		pre_valore_low=valore_low;
		
		while(1)
			{
			// l'interrupt sulla fine della conversione mi abilita questa routine
			// che mi permette di aggiornare il valore di temperatura
			if (calcola)
				{
				valore=ADRESL + (ADRESH<<8);
				/*
				l'LM35 fornisce un valore di 10mV per grado, avendo una risoluzione di
				circa 5mV sul convertitore A/D, possiamo quindi apprezzare massimo il 
				mezzo grado. Dobbiamo quindi trovare un fattore di conversione per sapere 
				la temperatura in °C. E' abbastanza semplice: sappiamo che, per noi 1bit = 5mV
				ma sulla sonda 10mV = 1°C
				
				quindi:
				valore*5 = tensione (espressa in mV)
				tensione/10 = gradi
				*/
							
				media+=valore; // sommo il valore
				contavalori++; // incremento il numero di valori letti
				
				// abbiamo raggiunto il numero di letture impostate?
				if (contavalori==(MEANVALUES-1))
					{
					displayvalue=(media/MEANVALUES);
					media=0; // azzero la media
					contavalori=0; // azzero il contatore dei valori sommati
					
					// fatti i calcoli posso mostrare il valore medio di temperatura
					// mi posiziono per scrivere il valore di temperatura misurata
					LCD_GOTO(1,6);
					LCD_PUTUN((displayvalue*5)/10); // parte intera
					LCD_PUTS(".");
					LCD_PUTUN((displayvalue*5)%10); // parte decimale. Varrà sempre 0 o 5 per quanto detto sopra
					
					// se il valore mostrato è minore di 10°, devo cancellare un carattere rimasto dalla
					// cifra precedente
					if (displayvalue<20) // 20 bit equivale a 10°
						{
						LCD_GOTO(1,9);
						LCD_PUTS(" ");
						}

					started=1; // segnalo che la prima misura è pronta
					}
				calcola=0; // resetto il flag
				} // fine routine di calcolo
			
			// Controllo pressione tasto set
			if (!BTN_SET && started)
				{
				DelayMs(70);
				if (!BTN_SET && started)
					{
					while (!BTN_SET) {} // per evitare di ciclare di continuo se si tiene premuto il tasto
					setmode++; // cambio la modalità
					if (setmode==3) // le modalità settaggio vanno da 0 a 2
						{
						setmode=0;
						}
					}
				}

				
			// imposto il comportamento del programma a seconda della modalità set selezionata
			switch (setmode)
				{
				case MODE_NONE:// uscita dal menù
				
				// elimino le parentesi e ripristino i caratteri sul display
				LCD_GOTO(2,3);
				LCD_PUTS(":");
				LCD_GOTO(2,8);
				LCD_PUTS(" ");
				LCD_GOTO(2,11);
				LCD_PUTS(":");
				LCD_GOTO(2,16);
				LCD_PUTS(" ");
				
				start_contatore=0; // fermo il contatore di inutilizzo
				contatore_inutilizzo=20000; // resetto il contatore di inutilizzo
				
				if (save_settings) // devo salvare i dati all'uscita dal menù?
					{
					// prima di salvare mi controllo che i dati siano coerenti
					// ovvero che la temperatura alta sia maggiore di quella bassa
					// e che siano comunque due valori diversi
					if (valore_hi<=valore_low) // valori assurdi
						{
						// ripristino i due valori precedenti
						// e non salvo sull'eeprom
						valore_hi=pre_valore_hi;
						valore_low=pre_valore_low;
						}
					else // i valori sono coerenti
						{
						// se i valori impostati ora sono diversi dai precedenti, li salvo nell'eeprom
						// altrimenti mi risparmio una scrittura
						if (valore_low!=pre_valore_low)
							{
							eei_write_byte(valore_low,0);
							}
						if (valore_hi!=pre_valore_hi)
							{
							eei_write_byte(valore_hi,1);
							}
						// infine valori precedenti diventano quelli attuali
						pre_valore_hi=valore_hi;
						pre_valore_low=valore_low;
						}
					
					save_settings=0; // resetto il flag
					} // fine che devo salvare i dati
				break;
				
				case MODE_SET_LOW: // settaggio temperatura bassa
				
				// evidenzio la selezione della temperatura bassa con le parentesi quadre
				LCD_GOTO(2,3);
				LCD_PUTS("[");
				LCD_GOTO(2,8);
				LCD_PUTS("]");
				
				valore_appoggio=valore_low; // valore su cui operare con i pulsanti
				save_settings=1; // abilito il salvataggio delle impostazioni
				start_contatore=1; // abilito il conteggio di tempo di inutilizzo
				break;
				
				case MODE_SET_HI: // settaggio temperatura alta
				
				// tolgo le parentesi dalla temperatura alta
				LCD_GOTO(2,3);
				LCD_PUTS(":");
				LCD_GOTO(2,8);
				LCD_PUTS(" ");
				
				// evidenzio la selezione della temperatura alta con le parentesi quadre
				LCD_GOTO(2,11);
				LCD_PUTS("[");
				LCD_GOTO(2,16);
				LCD_PUTS("]");

				valore_appoggio=valore_hi; // valore su cui operare con i pulsanti
				save_settings=1; // abilito il salvataggio delle impostazioni
				start_contatore=1; // abilito il conteggio di tempo di inutilizzo
				break;
				} // fine controllo modalità (switch)


			// controllo pulsanti UP/DOWN, da effettuarsi soltanto se siamo in una modalità di settaggio
			// se siamo in una modalità set, devo abilitare l'incremento/decremento del valore tramite i pulsanti
			if (setmode) // se è impostata una qualsiasi modalità settaggio
				{
				
				// mostro la scritta SET sul display
				LCD_GOTO(1,13);
				LCD_PUTS("SET ");
				
				// controllo la pressione del tasto di incremento
				if (!BTN_UP)
					{
					DelayMs(70);
					if (!BTN_UP)
						{
						contatore_inutilizzo=20000; // resetto il contatore di inutilizzo
						valore_appoggio++;
						if (valore_appoggio>(199-hyst)) // 199 corrisponde a 99.5°C
								// il valore non deve superare il massimo meno l'isteresi
							{
							valore_appoggio=(199-hyst);
							}
						}
					}
				
				// controllo pressione del tasto di decremento
				if (!BTN_DN)
					{
					DelayMs(70);
					if (!BTN_DN)
						{
						contatore_inutilizzo=20000; // resetto il contatore di inutilizzo
						valore_appoggio--;
						if (valore_appoggio<4) // 4 corrisponde a 2 gradi, che è il minimo che l'LM35 può
									// leggere in questa configurazione
							{
							valore_appoggio=4;
							}
						}
					}
					
				// una volta controllata la pressione dei pulsanti e aggiornato il valore di appoggio
				// devo pure aggiornare il valore reale che stiamo modificando
				if (setmode==MODE_SET_LOW)
					{
					valore_low=valore_appoggio;
					}
				else
					{
					valore_hi=valore_appoggio;
					}
				
				} // fine controllo pulsanti/modalità settaggio
				
			
			/* Posso quindi visualizzare sul display i valori di set temperatura alta e bassa
			per fare in modo che la parte decimale mi appaia sempre allo stesso punto, anche se la parte intera
			è formata da un solo carattere, mi controllo la parte intera, se il valore è inferiore a 10 vuol dire
			che è costituita da un solo carattere, quindi stampo uno spazio prima, sia per cancellare un eventuale
			carattere precedente, sia per spostare il valore di una posizione a destra sul display. */
			
			valore_appoggio=(valore_low*5)/10;
			LCD_GOTO(2,4);
			if (valore_appoggio<10) {LCD_PUTS(" ");}
			LCD_PUTUN(valore_appoggio); // parte intera
			LCD_PUTS(".");
			LCD_PUTUN((valore_low*5)%10); // parte decimale
			
			valore_appoggio=(valore_hi*5)/10;
			LCD_GOTO(2,12);
			if (valore_appoggio<10) {LCD_PUTS(" ");}
			LCD_PUTUN(valore_appoggio); // parte intera
			LCD_PUTS(".");
			LCD_PUTUN((valore_hi*5)%10); // parte decimale
			
			// quindi devo controllare se la temperatura misurata rientra nel range
			// effettuo una segnalazione se la temperatura è maggiore della massima
			// o se è minore della minima, lo faccio soltanto se non stiamo in modalità
			// impostazione e se la prima misura è stata effettuata
			
			if (!setmode && started)
				{
				if (displayvalue>valore_hi)
					{
					
					// segnalazione temperatura maggiore della massima
					if (!flash)
						{
						LCD_GOTO(1,13);
						LCD_PUTS("HIGH");
						}

					// se lo stato precedente è diverso da questo, 
					// azzero il flag di intervento start e resetto il contatore
					if (intv_state!=MODE_SET_HI)
						{
						intv_counter=intv_delay;
						intv_start=0;
						}
				
					intv_state=MODE_SET_HI; // setto la modalità di intervento
					}
				else if (displayvalue<valore_low)
					{
					// segnalazione temperatura minore della minima
					if (!flash)
						{
						LCD_GOTO(1,13);
						LCD_PUTS("LOW "); // cancello eventuale scritta
						}
					
					// se lo stato precedente è diverso da questo, 
					// azzero il flag di intervento start e resetto il contatore
					if (intv_state!=MODE_SET_LOW)
						{
						intv_counter=intv_delay;
						intv_start=0;
						}
					
					intv_state=MODE_SET_LOW;
					
					}
				else
					{
					// condizione normale
					LCD_GOTO(1,13);
					LCD_PUTS("    "); // cancello eventuale scritta precedente
					
					// se lo stato precedente è diverso da questo, 
					// azzero il flag di intervento start e resetto il contatore
					if (intv_state!=MODE_NONE)
						{
						intv_counter=intv_delay;
						intv_start=0;
						}
					intv_state=MODE_NONE;
					
					}
				} // fine che non siamo in modalità set e che abbiamo pronta la prima misura
			
			
			if (intv_start) // il contatore è arrivato a zero, 
			// controlliamo che stato è settato dalle routine precedenti e 
			// vediamo se lo stato è rimasto così nel tempo
				{
				switch (intv_state)
					{
					case MODE_NONE:
					if ((displayvalue>=valore_low) && (displayvalue<=valore_hi)) // il valore è rimasto nel range
						{
						LED=0; // resetto l'intervento
						}
					break;
					
					case MODE_SET_LOW:
					if (displayvalue<(valore_low-hyst)) // il valore è rimasto basso
						{
						LED=1; // intervengo
						}
					break;
					
					case MODE_SET_HI:
					if (displayvalue>(valore_hi+hyst)) // il valore è rimasto alto
						{
						LED=1; // intervengo
						}
					break;
					}
				intv_start=0; // resetto il flag di intervento
				}
			
			// infine, se il bit flash è settato a 1, cancello l'eventuale scritta nella parte destra del display
			// la scritta sarà poi reimpostata dalle altre funzioni, reimpostandola e cancellandola, lampeggia
			if (flash && !setmode)
				{
				LCD_GOTO(1,13);
				LCD_PUTS("    ");
				}
			}  // fine ciclo infinito
		} // fine main

void interrupt ISR(void)
	{
		
	if (T0IF) // interrupt ogni millisecondo
		{
		TMR0=102; // ricarico il timer0

		// controllo avvio conversione A/D
		count_lm35--; // counter per avvio lettura LM35
		if (count_lm35==0) // avvio la lettura se il counter è arrivato a zero
			{
			ADGO=1;
			count_lm35=200; // resetto il contatore
			}
		
		// controllo tempo inutilizzo menù
		if (start_contatore) // devo conteggiare il tempo di inutilizzo?
			{
			contatore_inutilizzo--;
			if (contatore_inutilizzo==0) // son passati X secondi senza che il menù non sia stato usato
				{
				setmode=MODE_NONE; // esco dalla modalità settaggio
				save_settings=0; // senza salvare i settaggi
				// riporto i valori di set a quelli che erano prima delle eventuali modifiche
				valore_hi=pre_valore_hi;
				valore_low=pre_valore_low;
				// il flag e il contatore saranno ripristinati nel main
				}
			}
			
		// lampeggio allarme
		contaflash--;
		if (contaflash==0)
			{
			contaflash=500;
			flash^=1; // inverto lo stato di flash
			}

		// controllo ritardo intervento
		intv_counter--;
		if (intv_counter==0)
			{
			intv_start=1;
			}

		T0IF=0; // resetto il flag di interrupt su timer0
		}
	
	if (ADIF)
		{
		calcola=1; // abilito, nel main, l'aggiornamento del valore di temperatura
		ADIF=0; // resetto il flag di fine conversione A/D
		}
	}