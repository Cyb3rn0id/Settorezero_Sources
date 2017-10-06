/*
Nome Modulo:		eeprom_internal.c

Descrizione:
Funzioni per scrivere valori ad 8, 16 e 32 bit nell'eeprom
interna del PICMicro

Linguaggio:			HITEC-C (ansi C)
Versione:			1.0
Data Emissione:		08/01/2010
Autore:			Bernardo Giovanni
Sito web:			http://www.settorezero.com
*/

// legge un BYTE (8bit)  dall'indirizzo specificato in "address"
unsigned char eei_read_byte(unsigned char address)
	{
	EEADR=address; // indirizzo da cui leggere
	EEPGD=0; // accesso alla memoria eeprom
	RD=1; // avvio la lettura
	// il valore contenuto in EEDATA è disponibile dal prossimo ciclo
	// se effettuo un return EEDATA, non ottengo nulla
	char EEVALUE;
	EEVALUE=EEDATA;
	return EEVALUE;
	}

// scrive un BYTE (8 bit) (valore "data") nell'indirizzo specificato in "address"
void eei_write_byte(unsigned char data, unsigned char address)
	{
	EEADR=address; // indirizzo in cui scrivere
	EEDATA=data; // dati da scrivere
	EEPGD=0; // accesso alla memoria eeprom
	WREN=1; // abilito la scrittura                            
	// Controllo se sono attivi gli interrupt e mi memorizzo la condizione
	static bit PREGIE=0;	
	if (GIE) 
		{
		PREGIE=1;	
		GIE=0; // disabilito gli interrupt
		}
	// Queste due istruzioni che seguono sono richieste (vedi datasheet) per avviare la scrittura
	EECON2=0x55;
	EECON2=0xAA;
	WR=1; // avvio un ciclo di scrittura
	while(!EEIF); // attendo che la scrittura sia completa
	WREN=0; // disabilito la scrittura sull'eeprom
	EEIF=0; // resetto il flag di scrittura completa
	 // reimposto gli interrupt allo stato in cui si trovavano
	if (PREGIE)
		{
		GIE=1;
		}
	}
	
// legge un valore INT (16bit) dall'indirizzo specificato in "address"
unsigned int eei_read_int(unsigned char address)
	{
	// il byte basso è contenuto nell'indirizzo address
	// il byte alto è contenuto nell'indirizzo address+1
	unsigned int BH; // byte alto (8 bit più a sinistra del valore INT)
	unsigned int BL; // byte basso (8 bit più a destra del valore INT)
	unsigned int EEVALUE; // valore di ritorno
	BL=eei_read_byte(address);
	BH=eei_read_byte(address+1);
	EEVALUE=BL | (BH<<8); // sommo (con OR) il byte basso e il byte alto spostato di 8 bit verso sinistra
	return EEVALUE;
	}
	
// scrive un valore INT (16bit) (valore "data") nell'indirizzo specificato in "address"
void eei_write_int(unsigned int data, unsigned char address)
	{
	// il byte basso è scritto nell'indirizzo address
	// il byte alto è scritto nell'indirizzo address+1
	unsigned int BH; // byte alto (8 bit più a sinistra del valore INT)
	unsigned int BL; // byte basso (8 bit più a destra del valore INT)
	BL = data & 0xFF; // metto a zero gli 8 bit più a sinistra
	BH = data >> 8; // sposto il valore di 8 bit a destra
	eei_write_byte(BL,address);
	eei_write_byte(BH,address+1);
	}
	
// legge un valore LONG (32bit) dall'indirizzo specificato in "address"
unsigned long eei_read_long(unsigned char address)
	{
	// il byte basso è contenuto nell'indirizzo address
	// il byte centrale basso è contenuto nell'indirizzo address+1
	// il byte centrale alto è contenuto nell'indirizzo address+2
	// il byte alto è contenuto nell'indirizzo address+3
	unsigned long BH; // byte alto (8 bit più a sinistra del valore LONG)
	unsigned long BCH; // byte centrale alto
	unsigned long BCL; // byte centrale basso
	unsigned long BL; // byte basso (8 bit più a destra del valore LONG)
	unsigned long EEVALUE; // valore di ritorno
	BL=eei_read_byte(address);
	BCL=eei_read_byte(address+1);
	BCH=eei_read_byte(address+2);
	BH=eei_read_byte(address+3);
	EEVALUE=BL | (BCL<<8) | (BCH<<16) | (BH<<24);
	return EEVALUE;
	}
	
// scrive un valore LONG (32bit) (valore "data") nell'indirizzo specificato in "address"
void eei_write_long(unsigned long data, unsigned char address)
	{
	// il byte basso è scritto nell'indirizzo address
	// il byte centrale basso è scritto nell'indirizzo address+1
	// il byte centrale alto è scritto nell'indirizzo address+2
	// il byte basso è scritto nell'indirizzo address+3
	unsigned long BH; // byte alto (8 bit più a sinistra del valore LONG)
	unsigned long BCH; // byte centrale alto
	unsigned long BCL; // byte centrale basso
	unsigned long BL; // byte basso (8 bit più a destra del valore LONG)
	BL = data & 0xFF; // metto a zero i 24 bit più a sinistra
	BCL = (data >> 8) & 0xFF; // sposto il valore di 8 bit a destra e metto a 0 i 16 bit di sinistra
	BCH = (data >> 16) & 0xFF; // sposto il valore di 8 bit a destra e metto a 0 gli 8 bit di sinistra
	BH = data >> 24; // sposto il valore di 24 bit a destra
	eei_write_byte(BL,address);
	eei_write_byte(BCL,address+1);
	eei_write_byte(BCH,address+2);
	eei_write_byte(BH,address+3);
	}