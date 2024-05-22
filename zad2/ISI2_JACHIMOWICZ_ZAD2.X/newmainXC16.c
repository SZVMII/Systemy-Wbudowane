/*
 * File:   main.c
 * Author: local
 *
 * Created on 27 marca 2024, 11:19
 */

// CONFIG2
// CONFIG2
#pragma config POSCMOD = HS             // Primary Oscillator Select (HS Oscillator mode selected)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRIPLL           // Oscillator Select (Primary Oscillator with PLL module (HSPLL, ECPLL))
#pragma config IESO = OFF               // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) disabled)
// CONFIG1
#pragma config WDTPS = PS32768 // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128 // WDT Prescaler (1:128)
#pragma config WINDIS = ON // Watchdog Timer Window Mode disabled
#pragma config FWDTEN = OFF // Watchdog Timer disabled
#pragma config ICS = PGx2 // Emulator/debugger uses EMUC2/EMUD2
#pragma config GWRP = OFF // Writes to program memory allowed
#pragma config GCP = OFF // Code protection is disabled
#pragma config JTAGEN = OFF // JTAG port is disabled
#include "xc.h"
#include <stdbool.h>
unsigned portValue;

int main(void) {
    TRISA = 0x0000;      //TRISA = 0b0000000000000000;
    TRISB = 0xFFFF;      //TRISB = 0b1111111111111111;
    
    AD1CON1 = 0x80E4;   //AD1CON1 = 0b1000000011100100;
    AD1CON2 = 0x0404;   //AD1CON2 = 0b0000010000000100;
    AD1CON3 = 0x0F00;   //AD1CON3 = 0b0000111100000000;
    AD1CHS = 0;
    AD1CSSL = 0x0020;   //AD1CSSL = 0b0000000000100000;
    
    int nastawa = 550;
    bool alarm = false;
    bool alarm_extended = false;
    char current6 = 0, prev6 = 0;
    
    while(1) 
    {
        while(!AD1CON1bits.DONE);
        prev6 = PORTDbits.RD6;
        __delay32(100000);
        current6 = PORTDbits.RD6;
        
        if(alarm == true)
            {
                for(int i=0; i<5; i++)
                {
                    LATA = 1;
                    __delay32(10000000);
                    LATA = 0;
                    __delay32(10000000);
                }
                alarm_extended = true;
                alarm = false;
            }
        
        if(alarm_extended == true)
        {
            LATA = 255;
            alarm = false;
        }
            
        if(ADC1BUF0 > nastawa && alarm_extended == false)
        {
            alarm = true;
        }

        if(ADC1BUF0 < nastawa)
        {
            alarm = false;
            alarm_extended = false;
            LATA = 0;
        }
            
        if(current6 - prev6 == 1)
        {
            alarm = false;
            alarm_extended = false;
            LATA = 0;
            __delay32(100000000);
            // break
        }
    }
    return 0;
}