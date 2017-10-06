//*************************************************
//
// settings.h
// autore: Bernardo Giovanni (www.settorezero.com)
// data: 19/01/10
//
//*************************************************

#define BT4 RB7
#define BT3 RB6
#define BT2 RB5
#define BT1 RB4
#define LED1 RD0
#define LED2 RD1
#define LED3 RD2
#define LED4 RD3
#define BUZZER RC0
#define MODE_OK 0 // per modalità cicalino, suono ok
#define MODE_ERROR 1 // per modalità cicalino, suono per errore
#define PRESSED 0

char input=0; // variabile per contenere il buffer di ricezione seriale
bit ledmode; // flag per capire se siamo in modalità di accensione dei led
bit statusmode; // flag per capire se dobbiamo stampare a video lo stato dei led
bit errormode; // flag per indicare che abbiamo inviato un comando non riconosciuto
bit okmode; // flag per indicare che abbiamo inviato un comando riconosciuto

// prototipi funzione
void settings(void);
void suona(char mode);
void main(void);
void interrupt isr(void);

void settings(void)
	{
	PORTA=0;
	TRISA=0b00000000;
	PORTB=0;
	TRISB=0b11110000; // RB7-RB4 pulsanti
	PORTC=0;
	TRISC=0b11000000; // RC6 e RC7 pin di ricetrasmissione seriale
	PORTD=0;
	TRISD=0b00000000;
	PORTE=0;
	TRISE=0b00000000;
	RBPU=0; // attivo le resistenze di pullup su porta B
	GIE=1; // gestione globale interrupt attiva
	PEIE=1; // interrupt di periferica attivati
	RCIE=1; // interrupt di ricezione su seriale attivato
	}