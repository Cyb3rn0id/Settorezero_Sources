/*
 * PIC16F1825 Analog + FVR example
 * (c)2020 Giovanni Bernardo (CyB3rn0id)
 * https://www.settorezero.com
 * 
 * Analog input on RA4 (analog channel 3, pin 3)
 * Used FVR as positive reference
 * FVR set to 2x gain (=>2048mv)
 * ADC will read 0 for 0mv and 1024 for 2048mV
 * So: mV=ADC read*2
 * Serial output on RC4 (pin 6) at 9600-N-8-1
 * STDIO redirected to UART, so you can use printf
 */

#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#pragma warning disable 520 // suppress annoying "function not used" warnings

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    // start first conversion
    ADC_StartConversion();
    
    while (1)
    {
        if (ADC_IsConversionDone())
        {
        static uint16_t res=0; // adc result
        static uint8_t convs=0; // number of conversions
            
        res += ADC_GetConversionResult();
        convs++;
            
        if (convs==32)
            {
            convs=0;
            res>>=5; // division by 32
            printf("ADC Reading: %d\n",res);
            // voltage on AN pin is about res*2
            // battery voltage is Vout/0.44
            // so float Vbat=(res*2)/0.44;
            // battery is good from:
            // res=703 => battery 0%
            // to
            // res=923 => battery 100%
            // so, percentage is:
            // ((res-703)/(923-703))*100
            // =>((res-703)*100)/220
            uint16_t percent=((res-703)*100)/220; // only integer part
            if (percent>100) percent=100;
            printf("Battery percent: %d\n\n",percent);
            
            // 2 seconds delay
            uint8_t i=4;
            while(i--) __delay_ms(500);
            res=0;
            }
        
        // Not needed since in the example I'm using only AN3 and is selected
        // by default, but if you enable more than one input as analog, you
        // know where you can find!
        // ADC_SelectChannel(adc_channel_t.channel_AN3);
        ADC_StartConversion();
        }
    }
}