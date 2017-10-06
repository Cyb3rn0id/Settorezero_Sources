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

#include "i2c.h"

// ---------------------------------------------------------------------------
// Init MSSP module in master mode
// ---------------------------------------------------------------------------
void I2cInitMaster(void) 
	{ 
	SCL_PIN = 1; // SCL/SDA lines as input
	SDA_PIN = 1; 
	SSP1CON1 = 0x28; // Master mode: Clock=Fosc/(4*(SSPADD+1))
	SSP1CON2 = 0;
	SSP1STATbits.CKE = 0; // SMbus compatible mode off
	SSP1ADD = BAUD; // calculated baudrate in i2c.h
	SSP1STATbits.SMP = SLEWRATE; // slewrate
	PIR1bits.SSP1IF = 0; // reset flag interrupt serial port
	PIR2bits.BCL1IF = 0; // reset flag interrupt bus collision
	}

// ---------------------------------------------------------------------------
// Init MSSP module in slave mode
// address = 7bit address
// ---------------------------------------------------------------------------
void I2cInitSlave(const unsigned char address) 
	{ 
	SCL_PIN = 1; // SCL/SDA lines as inputs
	SDA_PIN = 1; 
	SSP1CON1 = 0x36; // slave mode, 7bit address
	SSP1CON2 = 0;
	SSP1ADD = address; // address, bits A7-A1
	SSP1STATbits.CKE = 0; // SMbus compatible mode off
	SSP1STATbits.SMP=SLEWRATE; // slewrate		
	PIR1bits.SSP1IF = 0; // Reset flag interrupt serial port
	PIR2bits.BCL1IF = 0; // Reset flag interrupt bus collision
	}

// ---------------------------------------------------------------------------
// wait current operation finish
// ---------------------------------------------------------------------------
void I2cWait(void) 
	{ 
	while (!PIR1bits.SSP1IF)
		{				
		continue;
		}
	PIR1bits.SSP1IF=0;
	}

// ---------------------------------------------------------------------------
// wait bus in idle status or no transmission
// ---------------------------------------------------------------------------
void I2cWaitForIdle(void) 
	{
 	while ((SSP1CON2 & 0x1F) | SSP1STATbits.R_nW)
		{
 		continue;				
		}
	}

// ---------------------------------------------------------------------------
// start i2c bus
// ---------------------------------------------------------------------------
void I2cStart(void) 
	{
 	I2cWaitForIdle();
 	PIR1bits.SSP1IF = 0;
 	SSP1CON2bits.SEN = 1; // start sequence
 	I2cWait();
	}

// ---------------------------------------------------------------------------
// repeated start
// ---------------------------------------------------------------------------
void I2cRepStart(void) 
	{
	I2cWaitForIdle();
 	PIR1bits.SSP1IF = 0;
 	SSP1CON2bits.RSEN = 1; // repeated start sequence
 	I2cWait();
	}

// ---------------------------------------------------------------------------
// stop i2c bus
// ---------------------------------------------------------------------------
void I2cStop(void) 
	{
 	I2cWaitForIdle();
 	PIR1bits.SSP1IF = 0;
 	SSP1CON2bits.PEN = 1; // stop sequence
 	SSP1CON2bits.ACKEN = 1; // acknowledge sequence
 	I2cWait();
	}

// ---------------------------------------------------------------------------
// slave writes
// ---------------------------------------------------------------------------
void I2cWriteSlave(unsigned char data)
	{
	SSP1CON1bits.CKP = 0; // clock lock
	SSP1BUF = data; // transfer data in the buffer
	PIR1bits.SSP1IF = 0; // interrupt flag reset
	SSP1CON1bits.CKP = 1; // clock unlock
	I2cWait();
	}

// ---------------------------------------------------------------------------
// slave reads
// ---------------------------------------------------------------------------
unsigned char I2cReadSlave(void) 
	{ 
	char temp;
	I2cWait();
	temp = SSP1BUF;
	return temp; 
	}

// ---------------------------------------------------------------------------
// master writes
// returns true when slave send ACK
// ---------------------------------------------------------------------------
unsigned char I2cWriteMaster(unsigned char data) 
	{
 	I2cWaitForIdle();
 	SSP1BUF = data;
 	I2cWait();
 	return !SSP1CON2bits.ACKSTAT;
	}

// ---------------------------------------------------------------------------
// master reads
// returns the byte. if ack=1, sends acknowledge
// ---------------------------------------------------------------------------
unsigned char I2cReadMaster(unsigned char ack) 
	{
	unsigned char data;
 	I2cWaitForIdle();
 	SSP1CON2bits.RCEN = 1;
 	PIR1bits.SSP1IF = 0;
 	I2cWait();
 	data = SSP1BUF;
 	if (ack) 
		{
  		SSP1CON2bits.ACKDT = 0; // sends ACK
		}
 	else 
		{
  		SSP1CON2bits.ACKDT = 1; // no ACK
		}
  	SSP1CON2bits.ACKEN = 1; // starts ACK routine
 	I2cWait();
 	return data;
	}
	
// ---------------------------------------------------------------------------
// master performs a bulk read
// readed bytes are stored in *buffer
// devAddress must be a 7bit address
// ---------------------------------------------------------------------------	
void I2cBulkRead(unsigned char devAddress, unsigned char regAddress, unsigned char numReads, unsigned char *buffer)
	{
	I2cStart();
	I2cWriteMaster(devAddress<<1); // slave address for write (bit0 = 0)
	I2cWriteMaster(regAddress);	// send first register address to read
	I2cRepStart(); // repeated start
	I2cWriteMaster((devAddress<<1) | 1); // slave address for read (bit0 = 1)
	unsigned char i=0;
    while (i<numReads)
		{
		if (i==(numReads-1))
			{
			buffer[i]=I2cReadMaster(0); // last reading, no ACK
			}
		else
			{
			buffer[i]=I2cReadMaster(1); // continue to read, ACK
			}
		i++;
		}
	I2cStop();
	}
	
#undef	CLOCK
#undef	FOSC
#undef	BAUD
#undef	SLEWRATE
