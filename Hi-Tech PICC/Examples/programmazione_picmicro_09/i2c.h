/*
Modulo:			i2c.h
Descrizione:	Header file per funzioni i2c
Compilatore:	Hitec-C
Autore:			Bernardo Giovanni - http://www.settorezero.com
*/

// Impostare la frequenza del clock del modulo MSSP espressa in KHz
#define	CLOCK	100	// (100, 400 o 1000)

// Impostare la frequenza del quarzo espressa in Hertz, lasciare la L dopo il valore
#define FOSC	20000000L

// Impostare i pin usati per la comunicazione I2C
#define SCL_PIN	TRISC3
#define SDA_PIN	TRISC4
/*
PIC						SCL			SDA
PIC16F877A				TRISC3		TRISC4
PIC16F887				""			""
PIC16F1939				""			""
PIC16F1827 (mssp1)		TRISB1		TRISB4
*/

// --------------------------------------------------------------------------------------
// Da questa riga in poi non modificare niente
#define BAUD (char)((FOSC/(4000UL*CLOCK))-1) // SSPADD = (FOSC/(4*Clock))-1
#if CLOCK == 400
#define SLEWRATE 0 // 0 per 400KHz, 1 per 100KHz e 1MHz
#else
#define SLEWRATE 1
#endif

// prototipi funzione
extern void I2cInitMaster (void); // Init MSSP in Master mode
extern void I2cInitSlave (const unsigned char address); // Init MSSP in Slave mode (indirizzo a 7 bit)
extern void I2cWait(); // Attesa flag interrupt su modulo MSSP
extern void I2cWaitForIdle(); // Attesa bus in stato idle
extern void I2cStart(); // Sequenza di start
extern void I2cRepStart(); // Sequenza di start ripetuto
extern void I2cStop(); // Sequenza di stop
extern unsigned char I2cReadMaster(unsigned char ack); // Lettura in modalità Master
extern unsigned char I2cWriteMaster(unsigned char I2cWriteData); // Scrittura in modalità Master
extern unsigned char I2cReadSlave(); // Lettura in modalità Slave
extern void I2cWriteSlave(unsigned char data); // Scrittura in modalità Slave