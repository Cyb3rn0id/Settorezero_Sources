//*************************************************
// CORSO PROGRAMMAZIONE PICMICRO
// Lezione 10
// http://www.settorezero.com
//
// modulo: main.c
// autore: Bernardo Giovanni
// data:  19/01/10
// descrizione: esempio di comunicazione RS232
// picmicro: PIC16F877A
// clock: 20MHz
//
//*************************************************

#define  XTAL_FREQ 20MHZ
#include <pic.h>

__CONFIG (HS & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTEN & DEBUGDIS & UNPROTECT);

#include "settings.h"
#include "delay.c"
#include "usart.c"

void main(void)
	{
	ledmode=0;
	statusmode=0;
	settings(); // imposto le porte come da settings.h	
	init_comms();	// set up the USART - settings defined in usart.h

	printf("Programma di esempio comunicazione RS232\n\r");
	printf("By Bernardo Giovanni\n\r");
	printf("www.settorezero.com\n\r\n\r");
	
	while(1)
		{
		
		// controllo pulsanti
		if (BT4==PRESSED)
			{
			DelayMs(100);
			if (BT4==PRESSED)
				{
				suona(MODE_OK);
				printf("Hai premuto il pulsante 4\n\r");
				}
			} // bt4 pressed
			
		if (BT3==PRESSED)
			{
			DelayMs(100);
			if (BT3==PRESSED)
				{
				suona(MODE_OK);
				printf("Hai premuto il pulsante 3\n\r");
				}
			} // bt3 pressed
			
		if (BT2==PRESSED)
			{
			DelayMs(100);
			if (BT2==PRESSED)
				{
				suona(MODE_OK);
				printf("Hai premuto il pulsante 2\n\r");
				}
			} // bt2 pressed
			
		if (BT1==PRESSED)
			{
			DelayMs(100);
			if (BT1==PRESSED)
				{
				suona(MODE_OK);
				printf("Hai premuto il pulsante 1\n\r");
				}
			} // bt1pressed
		
		// se siamo in errormode, suono il cicalino in un modo
		if (errormode)
			{
			errormode=0; // resetto il flag
			suona(MODE_ERROR);
			}
		
		// se il comando è stato riconosciuto, suono il cicalino in un altro modo
		if (okmode)
			{
			okmode=0; // resetto il flag;
			suona(MODE_OK);
			}
			
		// se è stato settato il flag statusmode
		// stampo a video lo stato dei led
		if (statusmode)
			{
			statusmode=0; // resetto il flag
			printf("LED 1: ");
			if (LED1==1)
				{printf(" ON");}
			else
				{printf(" OFF");}
			printf("\n\rLED 2: ");
			if (LED2==1)
				{printf(" ON");}
			else
				{printf(" OFF");}
			printf("\n\rLED 3: ");
			if (LED3==1)
				{printf(" ON");}
			else
				{printf(" OFF");}
			printf("\n\rLED 4: ");
			if (LED4==1)
				{printf(" ON");}
			else
				{printf(" OFF");}
			printf("\n\r\n\r");
			} // statusmode
		} // while
	} // main

void suona(char mode)
	{
	char f=0; // cambio il ritardo a seconda del valore di mode passato alla funzione
	if (mode==MODE_OK)
		{
		f=250;
		}
	else
		{
		f=100;
		}
	for (char a=0; a<20; a++)
		{
		BUZZER=1;
		DelayUs(f);
		BUZZER=0;
		DelayUs(f);
		}
	}
	
// INTERRUPT SERVICE ROUTINE
void interrupt isr(void)
	{
	if (RCIF) // abbiamo ricevuto un byte sulla seriale?
		{
		input = getch(); // recupero carattere ricevuto
		
		// se mi trovo in modalità ledmode, devo identificare altri caratteri
		// vuol dire che prima è stata inviata la lettera L, quindi adesso
		// devo aspettarmi di avere un numero tra 0,1,2,3 e 4
		if (ledmode) 
			{
			switch(input)
				{
				// se ho premuto 1 dopo la L
				case '1': // da notare che 1 l'ho messo tra apici perchè devo rilevare il codice ascii
				LED1^=1;
				okmode=1; // comando riconosciuto
				printf("ok\n\r");
				break;
				
				case '2':
				LED2^=1;
				okmode=1;
				printf("ok\n\r");
				break;
				
				case '3':
				LED3^=1;
				okmode=1;
				printf("ok\n\r");
				break;
				
				case '4':
				LED4^=1;
				okmode=1;
				printf("ok\n\r");
				break;
				
				case '0': // li spengo tutti
				LED1=0;
				LED2=0;
				LED3=0;
				LED4=0;
				okmode=1;
				printf("ok\n\r");
				break;
				
				default:
				errormode=1; // segnalo che non è stato premuto un numero di quelli che mi servivano
				break;
				}
			ledmode=0; // esco dalla modalità ledmode
			} // ledmode=true
		else // altrimenti non mi trovo in modalità ledmode
			{
			switch(input)
				{
				// premuto L maiuscola o minuscola
				case 'L':
				case 'l':
				ledmode=1;
				okmode=1;
				printf("Ora digita il numero del led da invertire\n\r");
				printf("Oppure premi lo zero per spegnerli tutti\n\r");
				break;
			
				// premuto S maiuscola o minuscola
				case 'S':
				case 's':
				statusmode=1;
				okmode=1;
				break;
				
				// premuto un altro carattere che non è né L né S
				default:
				errormode=1; // segnalo che il comando inviato non è  riconosciuto
				break;
				} // switch
			} // ledmode=false
		} // RCIF
	} // isr