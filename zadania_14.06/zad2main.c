// CONFIG2
#pragma config POSCMOD = HS             // Primary Oscillator Select (HS Oscillator mode selected)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRIPLL           // Oscillator Select (Primary Oscillator with PLL module (HSPLL, ECPLL))
#pragma config IESO = OFF               // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) disabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window Mode disabled
#pragma config FWDTEN = OFF             // Watchdog Timer disabled
#pragma config ICS = PGx2               // Emulator/debugger uses EMUC2/EMUD2
#pragma config GWRP = OFF               // Writes to program memory allowed
#pragma config GCP = OFF                // Code protection is disabled
#pragma config JTAGEN = OFF             // JTAG port is disabled

#include "xc.h"
#include <stdbool.h>

unsigned portValue;

int main(void) {
    // Konfiguracja portów wej?ciowych i wyj?ciowych
    TRISA = 0x0000;      // Port A jako wyj?ciowy
    TRISB = 0xFFFF;      // Port B jako wej?ciowy
    
    // Konfiguracja przetwornika A/C (ADC)
    AD1CON1 = 0x80E4;   // Konfiguracja trybu pracy ADC
    AD1CON2 = 0x0404;   // Konfiguracja odniesienia i skanowania
    AD1CON3 = 0x0F00;   // Konfiguracja czasu próbkowania i zegara konwersji
    AD1CHS = 0;         // Wybór kana?u wej?ciowego
    AD1CSSL = 0x0020;   // W??czenie skanowania wej??

    int set = 550;           // Warto?? progowa dla alarmu
    bool alarm = false;          // Flaga alarmu
    bool alarm5s = false; // Flaga rozszerzonego alarmu
    char current6 = 0;
    char prev6 = 0; // Zmienne do ?ledzenia stanu przycisku
    
    while(1) 
    {
        // Czekanie na zako?czenie konwersji A/C
        while(!AD1CON1bits.DONE);
        prev6 = PORTDbits.RD6; // Odczyt poprzedniego stanu przycisku
        __delay32(100000);      // Krótkie opó?nienie
        current6 = PORTDbits.RD6; // Odczyt aktualnego stanu przycisku
        
        if(alarm == true)
        {
            for(int i=0; i<5; i++)
            {
                LATA = 1;               // W??czenie diody
                __delay32(10000000);    // Opó?nienie
                LATA = 0;               // Wy??czenie diody
                __delay32(10000000);    // Opó?nienie
            }
            alarm5s = true; // Ustawienie rozszerzonego alarmu
            alarm = false;         // Wy??czenie bie??cego alarmu
        }
        
        if(alarm5s == true)
        {
            LATA = 255;  // W??czenie wszystkich diod
            alarm = false; // Wy??czenie bie??cego alarmu
        }
            
        if(ADC1BUF0 > set && alarm5s == false)
        {
            alarm = true; // W??czenie alarmu je?li warto?? ADC przekracza próg
        }

        if(ADC1BUF0 < set)
        {
            alarm = false;          // Wy??czenie bie??cego alarmu
            alarm5s = false; // Wy??czenie rozszerzonego alarmu
            LATA = 0;               // Wy??czenie wszystkich diod
        }
            
        if(current6 - prev6 == 1)
        {
            alarm = false;          // Wy??czenie bie??cego alarmu
            alarm5s = false; // Wy??czenie rozszerzonego alarmu
            LATA = 0;               // Wy??czenie wszystkich diod
            __delay32(100000000);   // D?ugie opó?nienie
        }
    }
    return 0;
}
