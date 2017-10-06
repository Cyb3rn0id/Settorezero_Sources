//*************************************************
//
// settings.h
// autore: Bernardo Giovanni (www.settorezero.com)
// data: 13/03/10
//
//*************************************************

//trimmer collegato su AN1 (RA1)
#define BL RC1 // backlight LCD

unsigned int valore=0; // valore in uscita dal convertitore A/D
unsigned long tensione=0; // valore convertito in tensione

extern void main(void);

void settings(void)
        {
        /*
		le porte da usare come analogiche, vanno settate come ingressi nel relativo registro TRIS
		in questo caso, configurando tutte le porte come analogiche, metterò a 1 i relativi I/O
		Le porte analogiche sono distribuite sul banco A e sul banco E, 
		del banco A, RA4 non ha funzione analogica
		*/
		TRISA=0b00101111; // RA4 non è analogico, RA6 e RA7 non esistono
        TRISE=0b00000111; // le porte RE3,4,5,6,7 non esistono
		
		TRISB=0b00001111; 
        TRISC=0b00000000;
        TRISD=0b00000000;

	// INTERRUPT
	// per questo esempio non utilizzeremo l'interrupt, quindi è tutto off
    INTCON=0b00000000;
    // bit 0 -> RBIF		Flag Interrupt su porte B (variabile RBIF)
    // bit 1 -> INTF		Flag Interrupt su porta RB0/INT (variabile INTF)
    // bit 2 -> TMR0IF		Flag Interrupt su Timer0 (variabile T0IF oppure TMR0IF)
    // bit 3 -> RBIE		Interrupt su porte B 0=disattivato
    // bit 4 -> INTE		Interrupt su porta RB0/INT 0=disattivato
    // bit 5 -> TMR0IE		Interrupt su Timer0 1=attivato
    // bit 6 -> PEIE		Interrupt di periferica 0=disattivatoo
    // bit 7 -> GIE			Gestione Globale Interrupts 1=attivata
		
	// ADCON0
	ADCON0=0b10001001;
	// bit 0 -> ADON		Abilitazione modulo ADC, 1=Acceso , 0=Spento
	// bit 1 -> NON USATO
	// bit 2 -> GO/DONE		Stato di conversione 1=avvia conversione , 0=conversione finita
	// bit 3 -> CHS0		Selezione della porta analogica da leggere
	// bit 4 -> CHS1		Selezione della porta analogica da leggere
	// bit 5 -> CHS2		Selezione della porta analogica da leggere
	//                      CHS2|CHS1|CHS0
	//						000=AN0
	//						001=AN1
	//						010=AN2
	//						011=AN3
	//						100=AN4 
	//						101=AN5
	//						110=AN6
	//						111=AN7
	// 						abbiamo preselezionato la porta AN1 (001) 
	//						in quanto è qui che abbiamo collegato il trimmer
	// bit 6 -> ADCS0		Frequenza di conversione A/D    
	// bit 7 -> ADCS1       Frequenza di conversione A/D
	//                      |ADCON1|   ADCON0  |
	//						|ADCS2 |ADCS1|ADCS0|Frequenza di conversione
	//						|  0   |  0  |  0  |Fosc/2
	//						|  0   |  0  |  1  |Fosc/8
	//						|  0   |  1  |  0  |Fosc/32
	//						|  0   |  1  |  1  |FRC(clock derivato da un circuito RC)
	//						|  1   |  0  |  0  |Fosc/4
	//						|  1   |  0  |  1  |Fosc/16
	//						|  1   |  1  |  0  |Fosc/64
	//						|  1   |  1  |  1  |FRC(clock derivato da un circuito RC)
	// 						abbiamo scelto Fosc/32 in quanto, con il quarzo da 20MHz, 
	//						ci garantisce un TAD di 1,6uS come spiegato nella parte introduttiva 
	//						di questo capitolo del corso
	
	// ADCON1
	ADCON1=0b10001110;
	// bit 0 -> PCFG0		Configurazione delle porte analogiche , quali analogiche o digitali
	// bit 1 -> PCFG1		Configurazione delle porte analogiche , quali analogiche o digitali
	// bit 2 -> PCFG2		Configurazione delle porte analogiche , quali analogiche o digitali
	// bit 3 -> PCFG3		Configurazione delle porte analogiche , quali analogiche o digitali
	// 						abbiamo selezionato tutte le porte come analogiche
	// bit 4 -> NON USATO
	// bit 5 -> NON USATO
	// bit 6 -> ADCS2		unito a ADCS1 e a ADCS0 su ADCON0 per impostare la frequenza di campionamento
	// bit 7 -> ADFM		1: giustificazione a destra, 0: giustificazione a sinistra
	}