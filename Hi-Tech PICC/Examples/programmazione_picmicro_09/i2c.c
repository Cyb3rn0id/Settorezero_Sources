/*
Modulo:			i2c.c
Descrizione:	Funzioni i2c, modalità master e slave, pic serie16 con unico modulo MSSP
				Non sono supportati i picmicro con modulo SSP
Compilatore:	Hitec-C
Autori:
				Lavoro originale a cura di:	
				Michael Alon - michael@elina-micro.co.il
				Littleman - little_man@libero.it
				
				Riadattamenti curati da:
				Nicola Sellitto	- snico_one@yahoo.it
				Bernardo Giovanni - http://www.settorezero.com
*/

#include "i2c.h"

// ---------------------------------------------------------------------------
// Inizializzazione modulo MSSP in modalità MASTER
// ---------------------------------------------------------------------------
void I2cInitMaster(void) 
	{ 
	SCL_PIN = 1;			// Impostazione linee SCL/SDA come input
	SDA_PIN = 1; 
	SSPCON = 0x28;			// Modalita' Master, Clock=Fosc/(4*(SSPADD+1))
	SSPCON2 = 0;
	CKE = 0;				// Modalità compatibile SMBus disattivata
	SSPADD = BAUD;
	SMP = SLEWRATE;
	SSPIF = 0;				// Reset flag interrupt serial port
	}

// ---------------------------------------------------------------------------
// Inizializzazione modulo MSSP in modalità SLAVE con indirizzo a 7 bit
// L'indirizzo da dare al picmicro va passato all'argomento address
// ---------------------------------------------------------------------------
void I2cInitSlave(const unsigned char address) 
	{ 
	SCL_PIN = 1;			// Impostazione linee SCL/SDA come input
	SDA_PIN = 1;
	SSPCON = 0x36;			// Modalità Slave 7 bit address
	SSPCON2 = 0;
	SSPADD = address;		// Indirizzo della periferica: bits A7-A1
	CKE = 0;				// Modalità compatibile SMBus disattivata
	SMP = SLEWRATE;			
	SSPIF = 0;				// Reset flag interrupt serial port
	}

// ---------------------------------------------------------------------------
// Attende che sia conclusa l'operazione in corso
// ---------------------------------------------------------------------------
void I2cWait() 
	{ 
	while (!SSPIF)			// Attende l'impostazione del flag di interrupt
		{				
		continue;
		}
	SSPIF=0;
	}

// ---------------------------------------------------------------------------
// Attende che il bus sia in IDLE
// ---------------------------------------------------------------------------
void I2cWaitForIdle() 
	{
 	while ((SSPCON2 & 0x1F) | RW) // Attende idle o trasmissione non in corso
		{
 		continue;				
		}
	}

// ---------------------------------------------------------------------------
// Start della trasmissione
// ---------------------------------------------------------------------------
void I2cStart() 
	{
 	I2cWaitForIdle();
 	SSPIF = 0;
 	SEN = 1;					// Invio sequenza di start
 	I2cWait(); 				    // Attende che sia conclusa l'operazione
	}

// ---------------------------------------------------------------------------
// Start ripetuto della trasmissione
// ---------------------------------------------------------------------------
void I2cRepStart() 
	{
	I2cWaitForIdle();
 	SSPIF = 0;
 	RSEN = 1;					// Invio sequenza di start ripetuta
 	I2cWait();
	}

// ---------------------------------------------------------------------------
// Stop della trasmissione
// ---------------------------------------------------------------------------
void I2cStop() 
	{
 	I2cWaitForIdle();			// Attende che il bus sia in idle
 	SSPIF = 0;
 	PEN = 1; 					// Invio sequenza di stop
 	ACKEN = 1;					// Invio sequenza di acknowledge
 	I2cWait(); 					// Attende che sia conclusa l'operazione
	}

// ---------------------------------------------------------------------------
// Scrittura in Slave mode
// ---------------------------------------------------------------------------
void I2cWriteSlave(unsigned char data)
	{
	CKP = 0;      				// Tiene bloccato il clock
	SSPBUF = data;				// Carica i dati nel buffer 
	SSPIF = 0;    				// Azzera il bit di interrupt	
	CKP = 1;	    			// Sblocca il clock
	I2cWait(); 					// Attende che la trasmissione sia terminata
	}

// ---------------------------------------------------------------------------
// Lettura in Slave mode, restituisce il byte letto
// ---------------------------------------------------------------------------
unsigned char I2cReadSlave() 
	{ 
	char temp;
	I2cWait();	    			// Attende che il buffer sia pieno e l'ACK inviato
	temp = SSPBUF;        		// Legge SSPBUF per azzerare il bit BF
	return temp; 
	}

// ---------------------------------------------------------------------------
// Invia il byte data e restituisce TRUE se lo slave ha inviato l'ACK
// ---------------------------------------------------------------------------
unsigned char I2cWriteMaster(unsigned char data) 
	{
 	I2cWaitForIdle();			// Attende l'IDLE
 	SSPBUF = data;				// Carica il buffer con il dato
 	I2cWait();					// Attende la fine della trasmissione
 	return !ACKSTAT; 			// Restituisce 1 se lo slave ha inviato l'ACK
	}

// ---------------------------------------------------------------------------
// Lettura in Master mode
// restituisce il byte letto e invia (ack=1)/non invia(ack=0) l'acknowledge
// ---------------------------------------------------------------------------
unsigned char I2cReadMaster(unsigned char ack) 
	{
	unsigned char data;
 	I2cWaitForIdle();			// Attende l'IDLE
 	RCEN = 1;					// Si mette in ricezione
 	SSPIF = 0;					// Azzera l'interrupt bit
 	I2cWait();					// Attende la fine della trasmissione
 	data = SSPBUF; 				// Legge il buffer e azzera il bit BF
 	if (ack) 
		{
  		ACKDT = 0; 				// Invia l'ACK
		}
 	else 
		{
  		ACKDT = 1; 				// Non invia l'ACK
		}
  	ACKEN = 1;               	// Avvia la routine di ACK
 	I2cWait();					// Attende che l'ACK sia completo
 	return data;	  			// Restituisce il byte ricevuto
	}
	
#undef	CLOCK
#undef	FOSC
#undef	BAUD
#undef	SLEWRATE