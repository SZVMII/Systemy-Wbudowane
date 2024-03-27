// CONFIG2
#pragma config POSCMOD = HS             // Primary Oscillator Select (HS Oscillator mode selected)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRIPLL           // Oscillator Select (Primary Oscillator with PLL module (HSPLL, ECPLL))
#pragma config IESO = OFF               // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) disabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

#include <xc.h>
#include <libpic30.h>

// Definicje pinów
#define BTN_FORWARD PORTBbits.RB0
#define BTN_BACKWARD PORTBbits.RB1
#define LEDS LATC

// Deklaracje funkcji
void program1();
void program2();
void program3();
void program4();
void program5();
void program6();
void program7();
void program8();
void program9();

unsigned char program_switch = 1; // Zmienna do przechowywania aktualnie wybranego programu

void main() {
    // Konfiguracja pinów przycisków jako wejścia
    TRISBbits.TRISB0 = 1; // Przycisk do przodu
    TRISBbits.TRISB1 = 1; // Przycisk do tyłu
    
    // Konfiguracja pinów diod LED jako wyjścia
    TRISC = 0; 
    
    while(1) {
        // Wybór programu na podstawie stanu przycisków
        if(BTN_FORWARD == 0 && BTN_BACKWARD == 1) {
            program_switch++;
            if(program_switch > 9) program_switch = 1;
        } else if(BTN_FORWARD == 1 && BTN_BACKWARD == 0) {
            program_switch--;
            if(program_switch < 1) program_switch = 9;
        }
        
        // Wykonanie odpowiedniego programu
        switch(program_switch) {
            case 1:
                program1();
                break;
            case 2:
                program2();
                break;
            case 3:
                program3();
                break;
            case 4:
                program4();
                break;
            case 5:
                program5();
                break;
            case 6:
                program6();
                break;
            case 7:
                program7();
                break;
            case 8:
                program8();
                break;
            case 9:
                program9();
                break;
        }
    }
}

// Implementacje poszczególnych programów

void program1() {
    // 8 bitowy licznik binarny zliczający w górę (0 do 255)
    unsigned char count = 0;
    while(1) {
        LEDS = count;
        count++;
        if(count == 0) break;
    }
}

void program2() {
    // 8 bitowy licznik binarny zliczający w dół (255 do 0)
    unsigned char count = 255;
    while(1) {
        LEDS = count;
        count--;
        if(count == 255) break;
    }
}

void program3() {
    // 8 bitowy licznik w kodzie graya zliczający w górę (0 do 255)
    unsigned char count = 0;
    while(1) {
        LEDS = count ^ (count >> 1);
        count++;
        if(count == 0) break;
    }
}

void program4() {
    // 8 bitowy licznik w kodzie graya zliczający w dół (255 do 0)
    unsigned char count = 255;
    while(1) {
        LEDS = count ^ (count >> 1);
        count--;
        if(count == 255) break;
    }
}

void program5() {
    // 2x4 bitowy licznik w kodzie BCD zliczający w górę (0 do 99)
    unsigned char tens, units;
    for(tens = 0; tens < 10; tens++) {
        for(units = 0; units < 10; units++) {
            LEDS = (tens << 4) | units;
        }
    }
}

void program6() {
    // 2x4 bitowy licznik w kodzie BCD zliczający w dół (99 do 0)
    unsigned char tens, units;
    for(tens = 9; tens >= 0; tens--) {
        for(units = 9; units >= 0; units--) {
            LEDS = (tens << 4) | units;
        }
    }
}

void program7() {
    // 3 bitowy wezyk poruszający się w lewo-prawo
    unsigned char pattern = 1;
    while(1) {
        LEDS = pattern;
        __delay_ms(1000); // Oczekiwanie 1 sekundy
        pattern = (pattern << 1) | (pattern >> 2);
    }
}

void program8() {
    // Implementacja kolejki
    // Do wykonania
}

void program9() {
    // 6 bitowy generator liczb pseudolosowych oparty o konfigurację 1110011
    unsigned char seed = 0b1110011;
    while(1) {
        LEDS = seed;
        seed = ((seed << 1) | (seed >> 5)) & 0b111111;
    }
}
