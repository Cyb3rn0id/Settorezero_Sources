/*
Nome Modulo:		eeprom_external.h

Descrizione:
Header file per utilizzo eeprom esterne I2C

Linguaggio:			HITEC-C (ansi C)
Versione:			1.0
Data Emissione:		31/08/2010
Autore:				Bernardo Giovanni
Sito web:			http://www.settorezero.com
*/

#define EE_CONTROL_CODE 			0b10100000	// indirizzo delle eeprom serie 24 (solo 4 bit alti)

extern void eee_write_byte(const unsigned char deviceid, const unsigned int address, const unsigned char data);
extern unsigned char eee_read_byte(const unsigned char deviceid, const unsigned char address);