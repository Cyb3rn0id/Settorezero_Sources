// --------------------------------------------------------------------------------------
// DELAY.H
// Low level delay routines for Microchip 16F87x and Hi-Tech C
// Original designed by Shane Tolmie, info designrem.com, view www.microchipc.com
// --------------------------------------------------------------------------------------

#define PIC_FOSC 20 						// Cristal frequency in Mhz

#ifndef __DELAY_H
#define __DELAY_H

extern unsigned char delayus_var;


// Function prototypes
void DelayBigUs(unsigned int cnt);
void DelayMs(unsigned char cnt);
void DelayMs_Interrupt(unsigned char cnt);
void DelayBigMs(unsigned int cnt);
void DelaySec(unsigned char cnt);


// Delay setting
#if   (PIC_FOSC == 4)
	#define Delay1Us asm("nop")
	#define DelayDivisor 4
	#define WaitFor1Us asm("nop")
	#define Jumpback asm("goto $ - 2")
#elif (PIC_FOSC == 8)
	#define Delay1Us asm("nop");asm("nop")
	#define DelayDivisor 2
	#define WaitFor1Us asm("nop")
	#define Jumpback asm("goto $ - 2")
#elif (PIC_FOSC == 16)
	#define Delay1Us asm("nop");asm("nop");asm("nop");asm("nop")
	#define DelayDivisor 1
	#define WaitFor1Us asm("nop")
	#define Jumpback asm("goto $ - 2")
#elif (PIC_FOSC == 20)
	#define Delay1Us asm("nop");asm("nop");asm("nop");asm("nop");asm("nop")
	#define DelayDivisor 1
	#define WaitFor1Us asm("nop"); asm("nop")
	#define Jumpback asm("goto $ - 3")
#elif (PIC_FOSC == 32)
	#define Delay1Us asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop")
	#define DelayDivisor 1
	#define WaitFor1Us asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop")
	#define Jumpback asm("goto $ - 6")
#else
	#error delay.h - please define PIC_FOSC correctly
#endif


// Core delay in microseconds
#define DelayUs(x) { \
			delayus_var = (unsigned char)(x/DelayDivisor); \
			WaitFor1Us;  \
			asm("decfsz _delayus_var, f"); \
			Jumpback; }



// --------------------------------------------------------------------------------------
// TIMEOUT
// --------------------------------------------------------------------------------------
// Indipendentemente dal clock il codice:
//		unsigned char timeout;
//		char var;
//		timeout = 10;
//		var = 0;
// 		while(timeout-- && (VAR == 0));	
// consuma 14 cicli per loop tranne il primo che impiega 24 cicli
// Mentre dichiarando 
//		unsigned int timeout;
// il codice consuma 20 cicli per loop tranne il primo che impiega 36 cicli
// Considerando che il numero di cicli presenti in 1 us è uguale a Fosc/4,
// volendo aspettare un tempo di n us bisogna aspettare n*Fosc4 cicli,
// quindi n*Fosc4/14 (per char) oppure n*Fosc4/20 (per int) è il numero di loop di codice
// da ripete, ovvero il valore da assegnare alla variabile timeout.
// Il massimo tempo di attesa disponibile con timeout=255 risulta:
// 14*255/Fosc4, mentre per timeout=65535 si ottine 20*65535/Fosc4
//
// 		+-------+-------+----------+--------------+----------+----------------+
//      |       |       |Min attesa|  Max attesa  |Min attesa|  Max attesa    |
// 		| Fosc  | Fosc4 | 14/Fosc4 | 14*255/Fosc4 | 20/Fosc4 | 20*65535/Fosc4 |
// 		|-------+-------+----------+--------------+----------+----------------+
// 		| 4 Mhz |  1    |   14 us  |    3570 us   |   20 us  |   1310700 us   |
// 		| 8 Mhz |  2    |    7 us  |    1785 us   |   10 us  |    655350 us   |
// 		| 16Mhz |  4    |    3 us  |     892 us   |    5 us  |    327675 us   |
// 		| 20Mhz |  5    |    2 us  |     714 us   |    4 us  |    262140 us   |
// 		| 32Mhz |  8    |    1 us  |     446 us   |    2 us  |    163837 us   |
// 		+-------+-------+----------+--------------+----------+----------------+

// --------------------------------------------------------------------------------------

#define LOOP_CYCLES_CHAR	14		// How many cycles per loop, optimizations on
#define LOOP_CYCLES_INT		20		// How many cycles per loop, optimizations on

#define TimeoutUs_Char(x)	(long)(((x)/LOOP_CYCLES_CHAR)*(PIC_FOSC/4))
#define TimeoutUs_Int(x)	(long)(((x)/LOOP_CYCLES_INT)*(PIC_FOSC/4))



#endif


