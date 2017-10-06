/*****************************************************************
 one-wire.c
 Routines per protocollo 1-Wire
 By Bernardo Giovanni 
 http://www.settorezero.com
 versione 1.1
 data rilascio: 20/12/2010
 
 Free per uso personale, non commerciale
 Ridistribuzione vietata senza consenso dell'autore
 
 elenco revisioni in one-wire.h

 NOTA:
 richiede delle routine di delay precise
 in particolare è richiesta unicamente una funzione DelayUs(char)
*****************************************************************/

  
// resetta il bus 1-wire e rileva la presenza di dispositivi
unsigned char OWReset(void)
	{
    unsigned char ow_detect=0; // variabile usata per rilevare la presenza di dispositivi 1wire
    DQ=0; // predispongo uscita bassa
    TRIS_DQ=0; // linea in uscita
    // linea a livello basso per 480uS
	// nota: la linea deve essere tenuta a livello basso
	// minimo 480uS
	DelayUs(240);
    DelayUs(240);
	TRIS_DQ=1;  // linea in alta impedenza
    // dopo che la linea è stata posta in alta impedenza
	// bisogna attendere dai 15 ai 60uS per una risposta
	DelayUs(70); // attendo 70uS per stare tranquillo
    ow_detect=DQ; // rilevo in che stato si trova la linea
	// l'impulso di presenza dura dai 60 ai 240uS
	// e il master deve aspettare in totale 480uS
	// 480-80=400, quindi attendo 410uS 
	// dopo l'impulso di presenza
	DelayUs(205);
    DelayUs(205);
	// restituisco il valore ottenuto:
	// 0 = ci sono dispositivi (OW_PRESENCE)
	// 1 = non ci sono dispositivi (OW_NO_PRESENCE)
	return ow_detect;
    }
  
// leggo un byte dalla linea
unsigned char OWReadByte(void)
	{
    unsigned char i; // counter
    unsigned char value=0; // byte letto
    for(i=0; i<8; i++) 
		{
        value >>= 1; // sposto i bit verso destra
		if(_OWReadBit()) // leggo un bit a 1
			{
			value |= 0x80; // bit a 1 verso sinistra
			// nota: i bit vengono ricevuti dal meno significativo
			// verso il più significativo
			}
		}
    // restituisco il byte letto
	return value;
    }   

// scrivo un byte sulla linea
void OWWriteByte(char val) 
	{
    unsigned char i; // counter
    for( i=0; i<8; i++) // ciclo per gli 8 bit del valore
		{
        _OWWriteBit(val & 0x01); // "scrivo" l'ultimo bit sulla linea
		val >>= 1; // shift a destra del valore
        }
	} 

// legge il codice del SINGOLO dispositivo sul bus
// e lo memorizza nell'array ID passato come argomento
void OWReadRom(unsigned char *ID)
	{
	OWReset(); // resetto la linea one-wire
	OWWriteByte(OW_READ_ROM); // invio il comando readrom, che si può usare solo se sulla linea c'è una sola sonda
	for(unsigned char a=0; a<8; a++)// ciclo per leggere gli 8 bytes restituiti dal dispositivo
		{
		// eseguo la lettura del byte a-esimo:
		ID[a]=OWReadByte();
		}
	}

// Invia RESET + MATCH ROM + ID dispositivo
void OWSendID(char *ID)
	{
	unsigned char i;
	// reset linea
	OWReset();	
	// comunico che voglio comunicare con il dispositivo specifico
	OWWriteByte(OW_MATCH_ROM);
	// invio l'indirizzo del dispositivo
	for(i=0; i<8; i++)
		{
        OWWriteByte(ID[i]);
		}
	}

// funzioni di basso livello

// leggo un singolo bit dalla linea
unsigned char _OWReadBit(void)
	{
    unsigned char retval;
	DQ=0; // predispongo uscita bassa
    TRIS_DQ=0; // pin come uscita
    DelayUs(6);
    TRIS_DQ=1; // pin in alta impedenza
    // Devo aspettare minimo 15uS per poter campionare la linea
	DelayUs(9);
	retval=DQ;
	DelayUs(55);
    return retval; // restituisco il valore al quale si trova la linea
    }

// scrivo un singolo bit sulla linea	
void _OWWriteBit(char bitval)
	{
	DQ=0; // predispongo uscita bassa
	TRIS_DQ=0;		// uscita bassa...
	if (bitval) // bit a 1
		{
		DelayUs(6);		// ...per 6uS
		TRIS_DQ=1; 		// uscita alta
		DelayUs(64); 	// per 64uS
		}
	else // bit a 0
		{
		DelayUs(60);	// ...per 60uS
		TRIS_DQ=1; 		// uscita alta
		DelayUs(10); 	// per 10uS
		}
	// il timeslot dura 70uS
    }
