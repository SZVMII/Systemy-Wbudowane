// CONFIGURATION BITS
#pragma config POSCMOD = NONE             // Primary Oscillator Select (primary oscillator disabled)
#pragma config OSCIOFNC = OFF             // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD             // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRC                // Oscillator Select (Fast RC Oscillator without Postscaler)
#pragma config IESO = OFF                 // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) disabled)
#pragma config WDTPS = PS32768            // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128              // WDT Prescaler (1:128)
#pragma config WINDIS = ON                // Watchdog Timer Window Mode disabled
#pragma config FWDTEN = OFF               // Watchdog Timer disabled
#pragma config ICS = PGx2                 // Emulator/debugger uses EMUC2/EMUD2
#pragma config GWRP = OFF                 // Writes to program memory allowed
#pragma config GCP = OFF                  // Code protection is disabled
#pragma config JTAGEN = OFF               // JTAG port is disabled

#include "xc.h"
#include <libpic30.h>
#include <stdbool.h>
#include <stdio.h>

//makra
#define FCY         4000000UL	//czestotliwosc oscylatora
#define LCD_E       LATDbits.LATD4  
#define LCD_RW      LATDbits.LATD5
#define LCD_RS      LATBbits.LATB15
#define LCD_DATA    LATE
#define LCD_CLEAR       0x01
#define LCD_HOME        0x02
#define LCD_ON          0x0C
#define LCD_OFF         0x08
#define LCD_CONFIG      0x38
#define LCD_CURSOR      0x80
#define LINE1           0x00
#define LINE2           0x40

//WAZNE!!!!!!
//trzeba przytrzymac sekunde guziki zeby zmienic czas

// delaye
void __delayNanoSec(unsigned long nanosec){
    __delay32(nanosec*FCY/1000000);
}

void __delayMiliSec(unsigned long milisec){
    __delay32(milisec*FCY/1000);
}

//wysylanie komend do lcd
void sendComLCD(unsigned char comend){
    LCD_RW = 0;     
    LCD_RS = 0;     
    LCD_E = 1;      
    LCD_DATA = comend;
    __delayNanoSec(50);
    LCD_E = 0;
}

//wysylanie danych do lcd
void sendDataLCD(unsigned char data){
    LCD_RW = 0;
    LCD_RS = 1;     
    LCD_E = 1;
    LCD_DATA = data;
    __delayNanoSec(50);
    LCD_E = 0;
}

//wyswietlanie ciagow znakow
void printLCD(unsigned char* str){
    while(*str){
        sendDataLCD(*str++);
    }
}

//ustawianie kursora
void setCurLCD(unsigned char row, unsigned char column){
    unsigned char address;
    if (row == 1){
        address = LCD_CURSOR + LINE1 + column;
    }
    if(row == 2){
        address = LCD_CURSOR + LINE2 + column;
    }
    sendComLCD(address);
}

void initializeLCD(){
    __delayMiliSec(20);
    sendComLCD(LCD_CONFIG);
    __delayNanoSec(50);
    sendComLCD(LCD_ON);
    __delayNanoSec(50);
    sendComLCD(LCD_CLEAR);
   __delayMiliSec(2);
}

//wyswietlanie czasu w danym formacie
void timeDisp(unsigned int czas, unsigned int cur){
    setCurLCD(cur, 0);
    printLCD("Czas: ");
    sendDataLCD('0' + czas / 60);
    sendDataLCD(':');
    sendDataLCD('0' + (czas % 60) / 10);
    sendDataLCD('0' + czas % 10);
}


int main(void) {
    TRISB = 0x7FFF;     //rejestry kierunkow
    TRISD = 0xFFE7;
    TRISE = 0x0000;
    TRISA = 0x0000;

    initializeLCD();

    unsigned int p1time = 300; // ustawienie czasu
    unsigned int p2time = 300; // ustawienie czasu
    bool p1now = true; //tura gracza 1
    bool p2now = false; //tura gracza 2
    bool game_over = false; // koniec gry
	
    while(1) {
        if(game_over) {
            setCurLCD(1, 0);
            printLCD("Koniec gry");
            while(1); //zatrzymanie programu
        }

        if(PORTDbits.RD6==0){  //guzik gracza nr.1
            p1now = false;
            p2now = true;
            
        }

        if(PORTDbits.RD7==0){  //guzik gracza nr.2
            p2now = false;
            p1now = true;
        }

        if(p1now) {
            __delayMiliSec(1000); //odliczanie
            p1time--;
        }

        if(p2now) {
            __delayMiliSec(1000);
            p2time--;
        }

	//wyswitlanie czasu graczy
        timeDisp(p1time,1);
        timeDisp(p2time,2);
        
        //skonczyl sie czas
        if(p1time == 0) {
            setCurLCD(2, 0);
            printLCD("Gracz 2 wygrywa!");
            game_over = true;
        }

        if(p2time == 0) {
            setCurLCD(2, 0);
            printLCD("Gracz 1 wygrywa!");
            game_over = true;
        }
    }

    return 0;
}
