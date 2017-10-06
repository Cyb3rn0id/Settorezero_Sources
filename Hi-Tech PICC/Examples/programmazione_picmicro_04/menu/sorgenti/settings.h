//*************************************************
//
// settings.h
// autore: Bernardo Giovanni (www.settorezero.com)
// data: 21/10/09
//
//*************************************************

// pulsanti su porte B 0-3
// led porte B 4-7
#define BTNUP RB0
#define BTNDOWN RB1
#define BTNOK RB2
//#define BTN4 RB3

#define LED1 RB7
#define LED2 RB6
#define LED3 RB5
#define LED4 RB4

#define PRESSED 0 //  per verifica pulsante premuto
#define ARROW 0X7E // codice del carattere freccia
#define ON 1
#define OFF 0
#define CICALINO RE1

signed char menupos=0; // variabile che tiene conto della posizione della freccia nel menù

void settings(void)
        {
        TRISA=0b00000000;
        TRISB=0b00001111;
        TRISC=0b00000000;
        TRISD=0b00000000;
        TRISE=0b00000000;
        INTCON=0b00000000;
		LED1=OFF;
		LED2=OFF;
		LED3=OFF;
		LED4=OFF;
		CICALINO=OFF;
        }