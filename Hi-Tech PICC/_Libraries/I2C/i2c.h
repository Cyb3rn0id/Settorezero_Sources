/**************************************************************************
I2C HARDWARE ROUTINES FOR MICROCHIP PICMICRO WITH MSSP MODULE
SSP MODULE NOT SUPPORTED

Version: 		1.2
Release date:	18/01/2012

see changelog.txt for revisions list

Author:
(C)2011 Bernardo Giovanni
www.settorezero.com

basato su un lavoro precente di:
Michael Alon - michael@elina-micro.co.il
Littleman - little_man@libero.it
Nicola Sellitto	- snico_one@yahoo.it

FileName:     	i2c.c
Dependencies:	none
Compiler:  		Hitech-C Compiler > 9.81
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

// mssp module clock frequency, expressed as KHz
#define	CLOCK	400	// (100, 400 or 1000)

// Note:
// pullup resistors values are in function of clock frequency

// Fosc frequency as Hz
#define FOSC	20000000UL

// I2C pins
#define SCL_PIN	TRISC3
#define SDA_PIN	TRISC4
/*
PIC						SCL			SDA
PIC16F877A				TRISC3		TRISC4
PIC16F887				""			""
PIC16F1939				""			""
PIC16F1827 (mssp1)		TRISB1		TRISB4
PIC12F1822				TRISA1		TRISA2
*/

// --------------------------------------------------------------------------------------
// DON'T EDIT BELOW THIS LINE!
// --------------------------------------------------------------------------------------

// baud/slewrate calculation
#define BAUD (char)((FOSC/(4000UL*CLOCK))-1) // SSPADD = (FOSC/(4*Clock))-1
#if CLOCK == 400
#define SLEWRATE 0 // 0 for 400KHz, 1 for 100KHz and 1MHz
#else
#define SLEWRATE 1
#endif
// compatibility with older mcus
#ifndef SSPCON
#define SSP1CON1		SSPCON
#define SSP1CON1bits	SSPCONbits
#define SSP1CON2		SSPCON2
#define SSP1CON2bits	SSPCON2bits
#define SSP1STAT		SSPSTAT
#define SSP1STATbits	SSPSTATbits
#define SSP1BUF			SSPBUF
#define SSP1ADD			SSPADD
#define SSP1IF			SSPIF
#define BCL1IF			BCLIF
#end if

#define I2C_SETTOREZERO

// function prototypes
extern void I2cInitMaster (void); // Init MSSP in Master mode
extern void I2cInitSlave (const unsigned char address); // Init MSSP in Slave mode (7 bit address)
extern void I2cWait(void); // waiting interrupt flag
extern void I2cWaitForIdle(void); // waiting bus in idle
extern void I2cStart(void); // start sequence
extern void I2cRepStart(void); // repeated start sequence
extern void I2cStop(void); // stop sequence
extern unsigned char I2cReadMaster(unsigned char ack); // master reads
extern unsigned char I2cWriteMaster(unsigned char I2cWriteData); // master writes
extern unsigned char I2cReadSlave(); // slave reads
extern void I2cWriteSlave(unsigned char data); // slave writes
extern void I2cBulkRead(unsigned char devAddress, unsigned char regAddress, unsigned char numReads, unsigned char *buffer); // master bulk read
