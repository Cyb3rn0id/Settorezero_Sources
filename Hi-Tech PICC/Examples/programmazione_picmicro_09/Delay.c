// --------------------------------------------------------------------------------------
// DELAY.C
// High level delay routines for Microchip 16F87x and Hi-Tech C
// Original designed by Shane Tolmie, info designrem.com, view www.microchipc.com
// --------------------------------------------------------------------------------------

#ifndef __DELAY_C
#define __DELAY_C

#include <pic.h>
#include "delay.h"

unsigned char delayus_var;

// Delay max 65535 microseconds
void DelayBigUs(unsigned int cnt) {
unsigned char i;

	if (cnt > 0) {
		i = (unsigned char)(cnt>>8);
		while(i>=1) {
			i--;
			DelayUs(253);
			CLRWDT();
		}
		DelayUs((unsigned char)(cnt & 0xFF));
	}
}


// Delay max 255 milliseconds
void DelayMs(unsigned char cnt) {
unsigned char i;

	if (cnt > 0) {
		do {
			i = 4;
			do {
				DelayUs(250);
				CLRWDT();
			} while(--i);
		} while(--cnt);
	}
}


// Delay max 255 milliseconds for interrupt without CLRWDT
void DelayMs_Interrupt(unsigned char cnt) {
unsigned char i;
	
	if (cnt > 0) {
		do {
			i = 4;
			do {
				DelayUs(250);
			} while(--i);
		} while(--cnt);
	}
}


// Delay max 65535 milliseconds
void DelayBigMs(unsigned int cnt) {
unsigned char i;

	if (cnt > 0) {
		do {
			i = 4;
			do {
				DelayUs(250);
				CLRWDT();
			} while(--i);
		} while(--cnt);
	}
}


// Delay n seconds
void DelaySec(unsigned char cnt) {
unsigned char i;

	if (cnt > 0) {
		do {
			i = 4;
			do {
				DelayMs(250);
				CLRWDT();
			} while(--i);
		} while(--cnt);
	}
}


#endif
