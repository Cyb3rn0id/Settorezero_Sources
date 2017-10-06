/**************************************************************************
IO EXPANDER ROUTINES
ONLY PCF8574 IS SUPPORTED.
REQUIRES SETTOREZERO I2C ROUTINES

Version: 		1.0
Release date:	18/01/2012

see changelog.txt for revisions list

Author:
(C)2012 Bernardo Giovanni
www.settorezero.com

FileName:     	ioexp.c
Dependencies:	I2C Libraries
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

#include "pcf8574.h"

#ifdef PCF8574
void ioexp_write(unsigned char address, unsigned char value)
	{
	I2cStart();
	I2cWriteMaster(address<<1);
	I2cWriteMaster(value);
	I2cStop();							
	}

unsigned char ioexp_read(unsigned char address)
	{
	unsigned char b;
	I2cStart();	
	I2cWriteMaster(address<<1);
	I2cRepStart();
	I2cWriteMaster((address<<1) | 1);
	b=I2cReadMaster(0);
	I2cStop();
	return b;
	}
#endif	
