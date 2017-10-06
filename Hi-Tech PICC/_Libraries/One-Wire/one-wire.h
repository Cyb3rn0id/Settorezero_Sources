/*****************************************************************
 one-wire.h
 Routines per protocollo 1-Wire
 (c)2010 Bernardo Giovanni 
 http://www.settorezero.com
 versione 1.1
 data rilascio: 20/12/2010

 Free per uso personale, non commerciale
 Ridistribuzione vietata senza consenso dell'autore
 
 NOTA:
 richiede delle routine di delay precise
 in particolare è richiesta unicamente una funzione DelayUs(char)
 
 ELENCO REVISIONI
 
 1.1 (20/12/10) : -aggiunta funzione OWSendID(char *ID) per inviare
				   RESET+ MATCH_ROM + ID dispositivo.
				  -aggiustati ed eliminati alcuni ritardi.
				
 1.0 (22/09/10) : prima release
*****************************************************************/


// modificare questi due defines per adattarli al pic utilizzato
#define	DQ  	RC5  	// Pin dati sul quale sono collegate le linee DQ dei dispositivi 1-wire
#define	TRIS_DQ	TRISC5 	// Registro TRIStato relativo alla porta usata per DQ


// non modificare nulla al di sotto di questa linea
//-----------------------------------------------------------------

// comandi specifici per il bus 1-wire
#define OW_SEARCH_ROM	0xF0	// avvia la ricerca dei dispositivi sul bus 1-wire
#define	OW_READ_ROM		0x33	// da usare per leggere l'ID quando vi è un unico dispositivo sul bus
#define	OW_MATCH_ROM	0x55	// permette di comunicare con il dispositivo specifico
#define	OW_SKIP_ROM		0xCC	// permette di comunicare con tutti i dispositivi contemporaneamente	

// altro
#define	OW_PRESENCE		0x00	// livello zero se dispositivi 1wire presenti
#define OW_NO_PRESENCE	0x01	// livello 1 se nessun dispositivo 1wire presente

// prototipi funzioni

// funzioni di alto livello
unsigned char OWReset(void); // esegue il reset della linea
unsigned char OWReadByte(void); // legge un byte dalla linea
void OWWriteByte(char val); // scrive un byte sulla linea
void OWReadRom(unsigned char *ID); // memorizza nell'array ID il ROM code del SINGOLO dispositivo sulla linea
void OWSendID(char *ID); // // invia RESET + MATCH ROM + ID dispositivo

// funzioni di basso livello - da non usare nelle applicazioni
unsigned char _OWReadBit(void); // legge un singolo bit dalla linea
void _OWWriteBit(char bitval); // scrive un singolo bit sulla linea

// funzioni
#include "one-wire.c"
