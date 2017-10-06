/**************************************************************************
DS1307 REAL TIME CLOCK CALENDAR (RTCC) LIBRARY
Version: 		1.0
Release date:	17/01/2010

Author:
(C)2011 Bernardo Giovanni
www.settorezero.com

FileName:     	ds1307.h
Dependencies:	i2c.c
Compiler:  		Hitech-C Compiler >9.81
Company:		www.settorezero.com

YOU MAY NOT DISTRIBUTE OR SELL THIS CODE OR PART OF IT WITHOUT
EXPLICIT AUTHORIZATION

E' FATTO DIVIETO ASSOLUTO DI RIDISTRIBUIRE QUESTO CODICE O SUE
PARTI SENZA L'AUTORIZZAZIONE ESPLICITA DELL'AUTORE

* Warranty
* 
* THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
* WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
* LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
* WE ARE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
* CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
* PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
* BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
* THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
* SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
* (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
*
**************************************************************************/

// this library requires i2c settorezero.com libraries
#define	RTC_SQOUT		0 // output level in absence of square wave on SQW/OUT pin. 0 or 1

// --------------------------------------------------------------------------------------
// DON'T EDIT BELOW THIS LINE!
// --------------------------------------------------------------------------------------

// DS1307 registers
#define	RTC_SECONDS		0x00 // secondi (0-59)
#define	RTC_MINUTES		0x01 // minuti (0-59)
#define RTC_HOURS		0x02 // ore (0-23 o 1-12 + AM/PM). Nella modalità 12 ore assumo che mezzanotte sono le 12:00 AM e mezzogiorno le 12:00 PM
#define	RTC_DAYOFWEEK	0x03 // numero ordinale del giorno: possono essere definiti dall'utente ma devono essere sequenziali. Assumo che 1=lunedì e 7=domenica
#define	RTC_DATE		0x04 // giorno del mese (1-31, 1-30, 1-29, 1-28). Il DS1307 effettua lui stesso la correzione per l'anno bisestile
#define	RTC_MONTH		0x05 // mese (1-12)
#define RTC_YEAR		0x06 // anno (0-99)
#define	RTC_CONTROL		0x07 // controllo onda quadra in uscita dal pin SQW/OUT

// DS1307 I2C addresses
#define RTC_WRITE		0xD0 // identificativo i2c DS1307 per scrittura
#define RTC_READ		0XD1 // identificativo i2c DS1307 per lettura

// flags used for hour format
#define	RTC_MODE_24H	0 // modalità 24 ore
#define	RTC_MODE_12H	1 // modalità 12 ore
#define	RTC_AM			0 // orario antimeridiano (per modalità 12ore)
#define RTC_PM			1 // orario postmeridiano (per modalità 12 ore)

// valori possibili per l'onda quadra in uscita dal pin SQW/OUT
#define	SQW_1HZ			0 // onda quadra ad 1Hz
#define	SQW_4KHZ		1 // onda quadra a 4,096KHz
#define	SQW_8KHZ		2 // onda quadra a 8,192KHz
#define SQW_32KHZ		3 // onda quadra a 32,768KHz
#define DS1307_SETTOREZERO

#ifndef I2C_SETTOREZERO
#error i2c library by settorezero.com not included! Cannot continue
#endif

// prototipi funzioni di alto livello
extern void Ds1307Start(void); // avvia l'oscillatore del DS1307 e setta l'uscita del pin SQW/OUT in assenza di onda quadra
extern void Ds1307Stop(void); // ferma l'oscillatore del DS1307 e setta l'uscita del pin SQW/OUT in assenza di onda quadra
extern char Ds1307Mode(void); // restituisce RTC_MODE24H (0) se ci troviamo in modalità 24 ore o RTC_MODE12H (1) se ci troviamo in modalità 12 ore
extern char Ds1307AmPm(void); // restituisce RTC_AM (0) se siamo in AM o RTC_PM (1) se siamo in PM (vale solo con la modalità 12 ore)
extern void Ds1307SqwOn(char freq); // setta l'onda quadra in uscita dal pin SQW/OUT. Valori possibili: da 0 a 3 come definito alla riga 36 del presente file
extern void Ds1307SqwOff(void); // spegne l'onda quadra in uscita dal pin SQW/OUT
extern char Ds1307ToggleMode(void); // inverte la modalità da 12 a 24 ore e viceversa
extern char Ds1307Read(char rgstr); // restituisce il valore del registro RGSTR in formato decimale
extern char Ds1307Set(char rgstr,char value); // imposta il registro sul valore decimale passato, le ore vanno sempre passate in formato 24H
extern void Ds1307SetAll(const char * strng); // imposta in un unico colpo i valori di data e orario passati come "dDDMMYYHHmmSS" 

// prototipi funzioni di basso livello (da non utilizzare per il proprio programma)
extern char _Ds1307Read(char rgstr); // restituisce il valore del registro RGSTR così come è scritto nel DS1307
extern char _Ds1307Write(char rgstr,char value); // imposta il registro RGSTR sul valore VALUE così come va scritto