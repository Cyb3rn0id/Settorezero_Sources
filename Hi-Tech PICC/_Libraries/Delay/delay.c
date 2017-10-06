/*
high level delay routines - see delay.h for more info.

Designed by Shane Tolmie of www.microchipC.com corporation.  Freely distributable.
Questions and comments to webmaster@microchipC.com.
PICuWEB - Program PIC micros with C. Site has FAQ and sample source code.
www.microchipC.com.

For Microchip 12C67x, 16C7x, 16F87x and Hi-Tech C
*/
#include	"delay.h"

void DelayMs(unsigned char cnt)
	{
	unsigned char i;
	do {
		i = 4;
		do {
			DelayUs(250);
			CLRWDT();
			} while(--i);
		} while(--cnt);
	}

void DelayBigMs(unsigned int cnt)
	{
	unsigned char i;
	do {
		i = 4;
		do {
			DelayUs(250);
			CLRWDT();
			} while(--i);
		} while(--cnt);
	}

void DelayS(unsigned char cnt)
	{
	unsigned char i;
	do {
		i = 4;
		do {
			DelayMs(250);
			CLRWDT();
			} while(--i);
		} while(--cnt);
	}
