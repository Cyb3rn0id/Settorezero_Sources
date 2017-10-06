//*************************************************
//
// settings.h
// autore: Bernardo Giovanni (www.settorezero.com)
// data: 1303/10
//
//*************************************************

//LM35 collegata su AN2 (RA2)
#define BL RC1 // backlight LCD
#define BTN_DN RB5 // pulsante decremento
#define BTN_UP RB6 // pulsante incremento
#define BTN_SET RB7 // pulsante impostazioni
#define MEANVALUES 20 // numero dei valori su cui effettuare la media
#define LED RD0 // led segnalazione intervento

bit calcola; // flag per abilitare la lettura dei registri ADRES
bit save_settings; // flag per abilitare il salvataggio dei valori nella eeprom
bit start_contatore; // flag per abilitare il contatore di tempo di inutilizzo
bit started; // per controllare se la prima misura è stata effettuata o meno, mi serve per evitare eventuali segnalazioni alla partenza quando ancora non abbiamo una misura pronta
unsigned int valore=0; // valore in uscita dal convertitore A/D
unsigned int displayvalue=0; // valore di temperatura da mostrare sul display, risultato della media
unsigned char count_lm35=200; // counter per effettuare la lettura dell'lm35 ogni tot ms

/*
ritardo intervento
c'è un contatore di ritardo intervento (intv_counter) preimpostato a 10 secondi che viene decrementato di continuo nell'ISR.
Giunto a zero attiva il controllo intervento (intv_start) attivando una routine di controllo, se lo stato (intv_state) che ha generato
il reset del contatore è rimasto attivo, allora si interviene. 
*/
bit intv_start;
unsigned int intv_counter=0; // contatore intervento
unsigned char intv_state=0; // stato intervento (nessuno, alta o bassa)
#define intv_delay (int)10000 // ritardo intervento in millisecondi
unsigned char hyst=1; // mezzo grado di isteresi (ogni bit vale mezzo grado)

// Valori di temperatura
// utilizzo char perchè il massimo valore di temperatura che vogliamo impostare è 99.5
// che in digitale corrisponde al valore 199
unsigned char valore_low=15; // preset temperatura bassa, valore da 0 a 1023
unsigned char valore_hi=25; // preset temperatura alta, valore da 0 a 1023
unsigned char pre_valore_low; // valore precedente di set basso
unsigned char pre_valore_hi; // valore precedente di set alto
unsigned int contatore_inutilizzo=20000; // tale contatore fa uscire dalla modalità set, senza salvare, qualora non la stessimo utilizzando per 20 secondi
unsigned int valore_appoggio=0; // valore di appoggio usato nel settaggio e poi da assegnare a valore_low o a valore_hi
unsigned int media=0; // contiene la somma dei valori di temperatura letti
unsigned char contavalori=0; // numero di valori di temperatura sommati
unsigned char setmode=0; // modalità settaggio:
#define MODE_NONE 0 // nessun settaggio
#define MODE_SET_LOW 1 // settaggio temperatura bassa
#define MODE_SET_HI 2 // settaggio temperatura alta

// queste due variabili mi servono per far lampeggiare le scritte nella parte destra del display
// ovvero: SET, LOW e HIGH
bit flash;
unsigned int contaflash=500;

extern void main(void);
extern void interrupt ISR(void);

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
		
		TRISB=0b11100000; 
        TRISC=0b00000000;
        TRISD=0b00000000;
		
		// OPTION
		// Dobbiamo abilitare le resistenze di pullup sulla portaB e impostare il prescaler
		// a 1:32 assegnato al timer0
		OPTION=0b01000100;
		// bit 0 -> Prescaler Rate Select bit 0
		// bit 1 -> Prescaler Rate Select bit 0
		// bit 2 -> Prescaler Rate Select bit 0 (1:32)
		// bit 3 -> Prescaler assegnato al Timer0 
		// bit 4 -> Non importa
		// bit 5 -> Clock per Timer0 derivato da ciclo di clock interno
		// bit 6 -> Non importa
		// bit 7 -> Resistenze di pull-up su porta B attivate

		// INTCONT
		INTCON=0b10100000;
		// abilitiamo l'interrupt sul timer0
		// bit 0 -> RBIF - Flag interrupt su porte B
		// bit 1 -> INTF - Flag interrupt su RB0/INT
		// bit 2 -> T0IF - Flag interrupt su Timer0
		// bit 3 -> RBIE, Interrupt su porte B disattivato
		// bit 4 -> INTE, Interrupt su porta RB0/INT disattivato
		// bit 5 -> TMR0IE, Interrupt su Timer0 attivato
		// bit 6 -> PEIE, Interrupt di periferica disattivato
		// bit 7 -> GIE, Gestione Interrupt attiva
		
		// abilitiamo anche l'interrupt sulla fine conversione A/D:
		ADIE=1;
	
		/*
		precaricando timer1 col valore 102, e usando un quarzo da 20MHz, avremo un
		interrupt ogni 1mS, che sfrutteremo per riavviare la conversione. Difatti
		non possiamo mettere un ritardo nell'ISR, sfruttiamo quindi quest'altra tecnica. E'
		importante perchè dobbiamo dare un ritardo per far caricare CHOLD. Avremo potuto fare
		in modo anche da avere l'interrupt ogni microsecondo o ogni 20uS, ma il tempo tra un
		interrupt e l'altro sarebbe stato troppo breve e poteva impedirci di eseguire
		numerose operazioni.
		*/
		TMR0=102;
	
	
		// ADCON0
		ADCON0=0b10010001;
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
		// 						abbiamo preselezionato la porta AN2 (010) in quanto è qui 
		//						che abbiamo collegato la LM35
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