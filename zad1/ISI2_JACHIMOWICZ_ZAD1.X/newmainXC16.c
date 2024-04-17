// CONFIG2
#pragma config POSCMOD = NONE      // Primary Oscillator Select (HS Oscillator mode selected)
#pragma config OSCIOFNC = ON           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRC           // Oscillator Select (Primary Oscillator with PLL module (HSPLL, ECPLL))
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
    
#define _XTAL_FREQ 8000000

#include <xc.h>
#include <libpic30.h>

unsigned portValue = 0;
char  prevS6=0, currentS6=0, prevS7=0, currentS7, program = 0;
int number = 0;

//zad 1 i 2
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void){
    if(program ==0){
        portValue++;
    }
    
    if(program == 1){
        portValue--;
    }
    
    LATA = portValue;
    _T1IF = 0;
}

void zad1(){
    unsigned char counter = 0;
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    while(1) {
        LATA = counter;
        __delay32(1000000); // Opó?nienie przed inkrementacj?
        counter++;
        if (counter == 0) // Sprawdzenie, czy przekroczono maksymaln? warto??
            counter = 0; // Zerowanie licznika, je?li przekroczono maksymaln? warto??
        
        if(PORTDbits.RD6 == 0)
        {
            number -= 1;
            break;
        }
        
        if(PORTDbits.RD7 == 0)
        {
            number += 1;
            break;
        }
    }
}
void zad2(){
   unsigned char counter = 0;
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    while(1) {
        LATA = counter;
        __delay32(1000000); // Opó?nienie przed inkrementacj?
        counter--;
        if (counter == 0) // Sprawdzenie, czy przekroczono maksymaln? warto??
            counter = 0; // Zerowanie licznika, je?li przekroczono maksymaln? warto??
        
        if(PORTDbits.RD6 == 0)
        {
            number -= 1;
            break;
        }
        
        if(PORTDbits.RD7 == 0)
        {
            number += 1;
            break;
        }
    }
}


void zad3(){
    unsigned char portValue3 = 0;
    unsigned char gray;
    AD1PCFG = 0xFFFF; // set to digital I/O (not analog)
    TRISA = 0x0000; // set all port bits to be output
    while(1) {
        gray =(portValue3 >> 1) ^ portValue3;
        portValue3++;
        LATA = gray; // write to port latch
        __delay32(1000000); // delay in instruction cycles
        
        if(PORTDbits.RD6 == 0)
        {
            number -= 1;
            break;
        }
        
        if(PORTDbits.RD7 == 0)
        {
            number += 1;
            break;
        }

    }
}

void zad4(){
    unsigned char portValue4 = 255;
    unsigned char gray;
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    while(1) {
        gray = (portValue4 >> 1) ^ portValue4 ;
        portValue4--;
        LATA = gray;
        __delay32(1000000);
        
        if(PORTDbits.RD6 == 0)
        {
            number -= 1;
            break;
        }
        
        if(PORTDbits.RD7 == 0)
        {
            number += 1;
            break;
        }
    }
}

void zad5(){
    unsigned char portValue5=0;
    unsigned char bcdUp;
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    while(1){
        bcdUp = (((portValue5 / 10) << 4) | (portValue5 % 10));
        portValue5++;
        LATA = bcdUp;
        __delay32(500000);
        
        if(PORTDbits.RD6 == 0)
        {
            number -= 1;
            break;
        }
        
        if(PORTDbits.RD7 == 0)
        {
            number += 1;
            break;
        }
    }
}

void zad6(){
    unsigned char portValue6=99;
    unsigned char bcdDown;
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    while(1){
        bcdDown = (((portValue6 / 10) << 4) | (portValue6 % 10));
        portValue6--;
        LATA = bcdDown;
        __delay32(500000);
        
        if(PORTDbits.RD6 == 0)
        {
            number -= 1;
            break;
        }
        
        if(PORTDbits.RD7 == 0)
        {
            number += 1;
            break;
        }
    }
}

void zad7(){
    unsigned char portValue7;
    unsigned char snake = 0b00000111;
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    while(1){
        while (snake != 0b11100000){
            LATA = snake;
           snake <<= 1;
           __delay32(1000000);
        }
        while (snake != 0b00000111){
            LATA = snake;
            snake >>= 1;
            __delay32(1000000);
        }
        
        if(PORTDbits.RD6 == 0)
        {
            number -= 1;
            break;
        }
        
        if(PORTDbits.RD7 == 0)
        {
            number += 1;
            break;
        }
    }
}

int main(void) {
    if(number < 0)
    {
        number = 6;
    }
    
    if(number == 0)
    {
        zad1();
    }
    
    if(number == 1)
    {
        zad2();
    }
    
    if(number == 2)
    {
        zad3();
    }
    
    if(number == 3)
    {
        zad4();
    }
    
    if(number == 4)
    {
        zad5();
    }
    
    if(number == 5)
    {
        zad6();
    }
    
    if(number == 6)
    {
        zad7();
    }
    
    if(number > 6)
    {
        number = 0;
    }
}