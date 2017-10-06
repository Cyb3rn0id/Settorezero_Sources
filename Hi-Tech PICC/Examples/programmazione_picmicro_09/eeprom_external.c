/*
Nome Modulo:		eeprom_external.c

Descrizione:
Funzioni per utilizzo eeprom esterne I2C
richiede le librerie I2C

Linguaggio:			HITEC-C (ansi C)
Versione:			1.0
Data Emissione:		31/08/2010
Autore:				Bernardo Giovanni
Sito web:			http://www.settorezero.com
*/

#include "eeprom_external.h"

void eee_write_byte(unsigned char deviceid, const unsigned int address, const unsigned char data)
	{
	// aggiungo all'indirizzo "standard" delle eeprom24, il deviceid che si ottiene
	// settando i pin A0,A1,A2. Il device id devo spostarlo di un bit in quanto il
	// bit 0 è quello che determina la lettura/scrittura
	deviceid = EE_CONTROL_CODE | (deviceid <<1);
	I2cStart();	 // avvio la comunicazione I2C
	I2cWriteMaster(deviceid); // invio l'indirizzo della memoria sulla quale voglio scrivere
	// il bit zero è già posto a zero per specificare l'operazione di scrittura
	I2cWriteMaster((char)(address >> 8)); // invio la parte alta dell'indirizzo in cui scrivere
	I2cWriteMaster((char)(address & 0xFF)); // invio la parte bassa dell'indirizzo in cui scrivere
	I2cWriteMaster(data); // Invio il byte
	I2cStop(); // termino la comunicazione										
	}
	

unsigned char eee_read_byte(unsigned char deviceid, const unsigned char address)
	{
	unsigned char data;
	deviceid = EE_CONTROL_CODE | (deviceid <<1);
	I2cStart();
	I2cWriteMaster(deviceid);  // invio l'indirizzo della memoria dalla quale voglio leggere
	I2cWriteMaster((char)(address >> 8)); // invio la parte alta dell'indirizzo da cui leggere
	I2cWriteMaster((char)(address & 0xFF)); // invio la parte bassa dell'indirizzo da cui leggere
	I2cRepStart(); // start ripetuto
	I2cWriteMaster(deviceid | 1); // invio l'indirizzo della memoria dalla quale voglio leggere, in modalità lettura
	data = I2cReadMaster(0); // leggo e non invio l'acknowledge
	I2cStop(); // termino la comunicazione
	return data; // restituisco il valore letto
	}