//*************************************************
//
// settings.h
// autore: Bernardo Giovanni (www.settorezero.com)
// data: 01/11/09
//
//*************************************************
#define CICALINO RE1

void settings(void)
        {
        TRISA=0b00000000;
        TRISB=0b00001111;
        TRISC=0b00000000;
        TRISD=0b00000000;
        TRISE=0b00000000;
        INTCON=0b00000000;
        }