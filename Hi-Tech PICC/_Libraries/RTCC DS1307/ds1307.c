/**************************************************************************
DS1307 REAL TIME CLOCK CALENDAR (RTCC) LIBRARY
Version: 		1.0
Release date:	17/01/2010

Author:
(C)2011 Bernardo Giovanni
www.settorezero.com

FileName:     	ds1307.c
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

#include "ds1307.h"

// ---------------------------------------------------------------------------
// Avvia l'oscillatore interno del DS1307 e setta l'uscita SQW/OUT in assenza di onda quadra in uscita.
// L'orologio al primo avvio e dopo avvio senza batteria tampone, presenta il bit7 del registro secondi al valore 1.
// In tale condizione l'oscillatore è fermo e l'orologio è quindi bloccato. Per farlo partire bisogna portare a zero
// tale bit. Questa operazione viene fatta in questa funzione rimanendo invariato il valore dei secondi.
// ---------------------------------------------------------------------------
void Ds1307Start(void)
	{
	// il bit 7 del registro 0x00 (secondi) è il Clock Halt: quando tale bit vale 1, l'oscillatore è disattivato, 
	// per cui l'orologio rimane bloccato. Per attivare l'orologio bisogna mettere a 0 tale bit
	char value=0;
	I2cStart(); // avvio la comunicazione i2c
	I2cWriteMaster(RTC_WRITE); // comunico che voglio scambiare dati col DS1307 in modalità scrittura
	I2cWriteMaster(RTC_SECONDS); // punto il registro dei secondi
	I2cRepStart(); // ripeto lo start
	I2cWriteMaster(RTC_READ); // comunico che voglio scambiare dati col DS1307 in modalità lettura
	value=I2cReadMaster(0); // recupero il valore dei secondi e fermo la comunicazione
	I2cStop(); // fermo la comunicazione
	value=value & 0b01111111; // metto a zero il bit 7 dei secondi per poter avviare l'oscillatore
	I2cStart(); // avvio la comunicazione i2c
	I2cWriteMaster(RTC_WRITE); // comunico che voglio scambiare dati col DS1307 in modalità scrittura
	I2cWriteMaster(RTC_SECONDS); // punto il registro dei secondi
	I2cWriteMaster(value); // scrivo il nuovo valore dei secondi
	I2cStop(); // fermo la comunicazione
	Ds1307SqwOff(); // setto il livello di uscita del pin SQW/OUT
	}

// ---------------------------------------------------------------------------
// Ferma l'oscillatore e blocca l'orologio portando a 1 il bit 7 del registro secondi.
// ---------------------------------------------------------------------------
void Ds1307Stop(void)	
	{
	// il bit 7 del registro 0x00 (secondi) è il Clock Halt: quando tale bit vale 0, l'oscillatore è attivo, 
	// per cui l'orologio cammina. Per bloccare l'orologio bisogna mettere a 1 tale bit rimanendo invariato
	// il valore attuale dei secondi
	char value=0;
	I2cStart(); // avvio la comunicazione i2c
	I2cWriteMaster(RTC_WRITE); // comunico che voglio scambiare dati col DS1307 in modalità scrittura
	I2cWriteMaster(RTC_SECONDS); // punto il registro dei secondi
	I2cRepStart(); // ripeto lo start
	I2cWriteMaster(RTC_READ); // comunico che voglio scambiare dati col DS1307 in modalità lettura
	value=I2cReadMaster(0); // recupero il valore dei secondi e fermo la comunicazione
	I2cStop(); // fermo la comunicazione
	value=value | 0b10000000; // metto a uno il bit 7 dei secondi per poter fermare l'oscillatore
	I2cStart(); // avvio la comunicazione i2c
	I2cWriteMaster(RTC_WRITE); // comunico che voglio scambiare dati col DS1307 in modalità scrittura
	I2cWriteMaster(RTC_SECONDS); // punto il registro dei secondi
	I2cWriteMaster(value); // scrivo il nuovo valore dei secondi
	I2cStop(); // fermo la comunicazione
	Ds1307SqwOff(); // setto il livello di uscita del pin SQW/OUT
	}
	
// ---------------------------------------------------------------------------
// Restituisce il formato ora corrente (24 ore o 12 ore).
// Valori restituiti: RTC_MODE24H (0) o RTC_MODE12H (1)
// ---------------------------------------------------------------------------
char Ds1307Mode(void)
	{
	char value=0;
	value=_Ds1307Read(RTC_HOURS); // valore registro ore
	return (value & 0b01000000);
	}
	
// ---------------------------------------------------------------------------
// restituisce AM/PM (il valore restituito è valido solo se ci troviamo in modalità 12 ore).
// Valori restituiti: RTC_AM (0) o RTC_PM (1)
// ---------------------------------------------------------------------------
char Ds1307AmPm(void)
	{
	char value=0;
	value=_Ds1307Read(RTC_HOURS); // valore registro ore
	return (value & 0b00100000);
	}

// ---------------------------------------------------------------------------
// Setta l'onda quadra in uscita dal pin SQW/OUT.
// Valori possibili per freq: da 0 a 3, vedi ds1307.h per maggiori informazioni
// ---------------------------------------------------------------------------
void Ds1307SqwOn(char freq)
	{
	char temp=0;
	temp=0b00010000 | freq | (RTC_SQOUT <<8);
	_Ds1307Write(RTC_CONTROL,temp);
	}

// ---------------------------------------------------------------------------
// Spegne l'onda quadra in uscita dal pin SQW/OUT e setta il livello di uscita
// ---------------------------------------------------------------------------
void Ds1307SqwOff(void)
	{
	char temp=0;
	temp=(RTC_SQOUT << 8);
	_Ds1307Write(RTC_CONTROL,temp);
	}
	
// ---------------------------------------------------------------------------
// Inverte la modalità di visualizzazione dell'orologio:
// da 12H a 24H e viceversa.
// Assumo che mezzanotte siano le 12:00 AM e mezzogiorno le 12:00 PM.
// ---------------------------------------------------------------------------
char Ds1307ToggleMode(void)
	{	
	char htemp=0;
	if (Ds1307Mode()==RTC_MODE_12H) // siamo in modalità 12 ore e devo portare in formato 24ore
		{
		htemp=Ds1307Read(RTC_HOURS); // leggo le ore
		htemp &= 0b00111111; // resetto il bit 6 che mi indica la modalità 12 ore
		if (Ds1307AmPm()==RTC_PM) // se siamo in AM non devo fare niente a meno che non siano le 00:00
			{
			if (htemp==12) // se sono le 12:00 AM devo portare l'orario a 00
				{
				htemp=0;
				}
			}
		else // siamo in PM, devo aggiungere 12 a meno che non siano proprio le 12:00, caso in cui non faccio nulla
			{
			if (htemp<12)
				{
				htemp+=12;
				}
			}
		} // rtc_mode_12h
	else // siamo in modalità 24 ore e devo portare in formato 12 ore
		{
		if (htemp>12) // orario PM
			{
			htemp-=12; // le 23 diventano 11 ecc ecc
			htemp |= 0b01100000; // bit 6=1: modalità 12 ore, bit 5=1: PM
			}
		else if (htemp==0) // mezzanotte => 12 AM
			{
			htemp=12 | 0b01000000; // bit 6=1: modalità 12 ore, bit 5=0: AM
			}
		else if (htemp==12) // mezzogiorno => 12PM
			{
			htemp=12 | 0b01100000; // bit 6=1: modalità 12 ore, bit 5=1: PM
			}
		else
			{
			htemp|=0b01000000; // bit 6=1: modalità 12 ore, bit 5=0: AM
			}
		}
	return Ds1307Set(RTC_HOURS,htemp);
	}

// ---------------------------------------------------------------------------
/*
eseguo la lettura del registro puntato e restituisco il valore in formato "user-friendly" (decimale)

i numeri sono memorizzati del DS1307 in formato BCD (decimale codificato in binario)
per la maggior parte dei valori, l'unità si trova nei 4 bit inferiori e la decina nei 4 bit superiori
fare riferimento a pag.8 del datasheet distribuito con la libreria per capire il modo
in cui i dati sono memorizzati

secondi
bit 7 = Clock Halt (valore da rimuovere)
bit 6,5,4 = decina
bit 3,2,1,0=unità

minuti (questo sistema vale pure per il numero del giorno del mese, per il mese e per l'anno)
bit 7 = sempre 0
bit 6,5,4 = decina
bit 3,2,1,0=unità

ore (questo è il registro più "affollato")
bit 7 : sempre 0
bit 6 : se 1 => modalità 12 ore (le ore andranno da 1 a 12)
          se0 => modalità 24 ore (le ore andranno da 0 a 23)
bit 5 : il suo significato varia in funzione della modalità
         se modalità 12 ore e se bit 5=1 => PM
         se modalità 12 ore e se bit 5=0 => AM
         se modalità 24 ore => secondo bit della decina (serve in pratica per ottenere il 2)
bit 4 : decina (unico bit della decina se siamo in modalità 12 ore)
bit 3,2,10 : unità
*/
// ---------------------------------------------------------------------------
char Ds1307Read(char rgstr)
	{
	char value=0;
	char temp=0;
	value=_Ds1307Read(rgstr); // eseguo la lettura del valore così come si trova nel DS1307
	// NOTA: il caso dayofweek non rientra perchè il valore non necessita di essere elaborato
	switch (rgstr)
		{
		case RTC_SECONDS:
			
			temp=value>>4; // ottengo il valore della decina che si trova nei 4 bit più alti
			temp=temp & 0b00000111; // rimuovo il bit 7, che rappresenta il flag Clock Halt
			temp=temp*10; // moltiplico per dieci per poter fare la somma dopo
			value=value & 0b00001111; // rimuovo il valore della decina dal byte letto
			value=value+temp; // valore completo della decina
			break;
		
		case RTC_MINUTES:
		case RTC_DATE:
		case RTC_MONTH:
		case RTC_YEAR:
		
			temp=value>>4; // ottengo il valore della decina che si trova nei 4 bit più alti
			temp=temp*10; // moltiplico per dieci per poter fare la somma dopo
			value=value & 0b00001111; // rimuovo il valore della decina dal byte letto
			value=value+temp; // valore completo della decina
			break;
			
		case RTC_HOURS:
		
			temp=value>>4; // ottengo il valore della decina che si trova nei 4 bit più alti
			if ((value & 0b00100000)==RTC_MODE_12H)// controllo se mi trovo in modalità 12 o 24 ore testando il bit 5
				{
				temp=temp & 0b00000001; // elimino i primi 3 bit che non mi servono in modalità 12 ore (mi serve solo il primo bit)
				}
			temp=temp*10; // moltiplico per dieci per poter fare la somma dopo
			value=value & 0b00001111; // rimuovo il valore della decina dal byte letto
			value=value+temp; // valore completo della decina
			// ovviamente dovendo visualizzare tale orario su un display o altro, sarà l'utente a far apparire AM o PM se siamo in modalità 12 ore
			// utilizzando le apposite funzioni fornite
			break;
		}
	return value;
	}
	
// ---------------------------------------------------------------------------
// Setto il registro al valore char inviato, il valore va inviato in formato decimale,
// nel caso delle ore, il valore va sempre inviato nel formato 24 ore (0-23) anche se
// siamo in modalità 12 ore, provvederà la funzione a scriverlo nel formato corretto
// ---------------------------------------------------------------------------
char Ds1307Set(char rgstr,char value)
	{
	char temp=0; // variabile di appoggio
	char temp2=0; // altra variabile di appoggio
	
	switch(rgstr)
		{
		case RTC_SECONDS: // nel caso dei secondi devo recuperare il valore del bit7 che mi indica se l'orologio è fermo o no, e quindi lasciarlo inalterato
		
			I2cStart(); // avvio la comunicazione i2c
			I2cWriteMaster(RTC_WRITE); // comunico che voglio scambiare dati col DS1307 in modalità scrittura
			I2cWriteMaster(RTC_SECONDS); // punto il registro dei secondi
			I2cRepStart(); // ripeto lo start
			I2cWriteMaster(RTC_READ); // comunico che voglio scambiare dati col DS1307 in modalità lettura
			temp2=I2cReadMaster(0); // recupero il valore dei secondi e fermo la comunicazione
			I2cStop(); // fermo la comunicazione
			temp2=(temp2 & 0b10000000); // mi interessa solo il bit 7
			break;
		
		case RTC_HOURS:
			// il valore da scrivere cambia in funzione del fatto che siamo in modalità 24 ore o 12 ore
			// alla funzione però verrà sempre passato il valore in formato 24 ore (valori: 0-23) !
			// assumo che mezzanotte sono le 12:00 AM e mezzogiorno le 12:00 PM
			if (Ds1307Mode()==RTC_MODE_12H)
				{
				if (value>12) // orario PM
					{
					value-=12; // le 23 diventano 11 ecc ecc
					temp2=0b01100000; // bit 6=1: modalità 12 ore, bit 5=1: PM
					}
				else if (value==0) // mezzanotte => 12 AM
					{
					value=12;
					temp2=0b01000000; // bit 6=1: modalità 12 ore, bit 5=0: AM
					}
				else if (value==12) // mezzogiorno => 12PM
					{
					value=12;
					temp2=0b01100000; // bit 6=1: modalità 12 ore, bit 5=1: PM
					}
				else
					{
					temp2=0b01000000; // bit 6=1: modalità 12 ore, bit 5=0: AM
					}
				}
			break;
		}
		
		temp=value/10; // valore delle decine
		value=value-(temp*10); // valore delle unità
		temp=(temp<<4)+value; // sposto le decine nei 4 bit alti e aggiungo le unità nei 4 bit bassi
		temp=temp | temp2; // aggiungo il valore degli altri bit per i registri "strani", altrimenti temf vale 0
		return (_Ds1307Write(rgstr,temp)) ; // scrivo il valore ottenuto nel registro
	}
	
// ---------------------------------------------------------------------------
// Imposta tutta la data in un sol colpo.
// Il parametro passato è un'array nel formato "dDDMMYYHHmmSS" dove d è il numero ordinale del giorno e DD è la data.
// L'orario va passato in modalità 24 ore
// ---------------------------------------------------------------------------
void Ds1307SetAll(const char * strng)
		{
		char temp=0;
		// 0 corrisponde al carattere 0x30, per cui per ottenere il numero, devo sottrarre 0x30 dal codice ascii
		temp=strng[0]-0x30; // numero ordinale del giorno
		Ds1307Set(RTC_DAYOFWEEK,temp);
		temp=(strng[1]-0x30)*10; // decina della data
		temp=(strng[2]-0x30)+temp; // valore di data completo di unità
		Ds1307Set(RTC_DATE,temp);
		temp=(strng[3]-0x30)*10; // decina del mese
		temp=(strng[4]-0x30)+temp; // valore di mese completo di unità
		Ds1307Set(RTC_MONTH,temp);
		temp=(strng[5]-0x30)*10; // decina dell'anno
		temp=(strng[6]-0x30)+temp; // valore di anno completo di unità
		Ds1307Set(RTC_YEAR,temp);
		temp=(strng[7]-0x30)*10; // decina dell'ora
		temp=(strng[8]-0x30)+temp; // valore di ora completo di unità
		Ds1307Set(RTC_HOURS,temp);
		temp=(strng[9]-0x30)*10; // decina dei minuti
		temp=(strng[10]-0x30)+temp; // valore di minuti completo di unità
		Ds1307Set(RTC_MINUTES,temp);
		temp=(strng[11]-0x30)*10; // decina dei secondi
		temp=(strng[12]-0x30)+temp; // valore di secondi completo di unità
		Ds1307Set(RTC_SECONDS,temp);
		}

// ---------------------------------------------------------------------------
// eseguo la lettura del valore del registro puntato, restituisco il valore così come scritto nel DS1307
// ---------------------------------------------------------------------------
char _Ds1307Read(char rgstr)
	{
	char value=0;
	I2cStart(); // avvio la comunicazione i2c
	if (I2cWriteMaster(RTC_WRITE)==0) // comunico che voglio scambiare dati col DS1307 in modalità scrittura
		return -1;
	if (I2cWriteMaster(rgstr)==0) // punto il registro specificato come argomento della funzione
		return -1;
	I2cRepStart(); // ripeto lo start
	if (I2cWriteMaster(RTC_READ)==0) // comunico che voglio scambiare dati col DS1307 in modalità lettura
		return -1;
	value=I2cReadMaster(0); // recupero il valore del registro e fermo la comunicazione
	I2cStop(); // fermo la comunicazione
	return value; // restituisco il valore letto
	}

// ---------------------------------------------------------------------------
// scrive un byte (value) nel registro (rgstr) del DS1307
// ---------------------------------------------------------------------------
char _Ds1307Write(char rgstr, char value)
	{
	I2cStart(); // avvio la comunicazione i2c
	if (I2cWriteMaster(RTC_WRITE)==0) // comunico che voglio scambiare dati col DS1307 in modalità scrittura
		return -1;
	if (I2cWriteMaster(rgstr)==0)
		return -1;
	if (I2cWriteMaster(value)==0)
		return -1;
	I2cStop();
	return 1;
	}

	