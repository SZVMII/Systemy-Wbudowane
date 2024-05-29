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

// DEFINICJE MAKRO
#define FCY         4000000UL   // Cz?stotliwo?? zegara
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
#define LCD_CUST_CHAR   0x40
#define LCD_SHIFT_R     0x1D

// Funkcja opó?nienia w nanosekundach
void __delay_nanoSec(unsigned long us){
    __delay32(us * FCY / 1000000);
}

// Funkcja opó?nienia w milisekundach
void __delay_miliSec(unsigned long ms){
    __delay32(ms * FCY / 1000);
}

// Funkcja wysy?ania komendy do LCD
void LCD_sendCommand(unsigned char command){
    LCD_RW = 0;     // Ustaw RW w tryb zapisu
    LCD_RS = 0;     // Ustaw RS w tryb komendy
    LCD_E = 1;      // W??cz LCD
    LCD_DATA = command; // Wy?lij komend?
    __delay_nanoSec(50); // Czekaj na przetworzenie komendy
    LCD_E = 0;      // Wy??cz LCD
}

// Funkcja wysy?ania danych do LCD
void LCD_sendData(unsigned char data){
    LCD_RW = 0;     // Ustaw RW w tryb zapisu
    LCD_RS = 1;     // Ustaw RS w tryb danych
    LCD_E = 1;      // W??cz LCD
    LCD_DATA = data; // Wy?lij dane
    __delay_nanoSec(50); // Czekaj na przetworzenie danych
    LCD_E = 0;      // Wy??cz LCD
}

// Funkcja drukowania stringa na LCD
void LCD_print(unsigned char* string){
    while(*string){
        LCD_sendData(*string++); // Wy?lij znaki jeden po drugim
    }
}

// Funkcja ustawiania kursora na LCD
void LCD_setCursor(unsigned char row, unsigned char col){
    unsigned char address;
    if (row == 1){
        address = LCD_CURSOR + LINE1 + col; // Oblicz adres dla wiersza 1
    }
    if (row == 2){
        address = LCD_CURSOR + LINE2 + col; // Oblicz adres dla wiersza 2
    }
    LCD_sendCommand(address); // Wy?lij komend? ustawiaj?c? kursor
}

// Funkcja inicjalizacji LCD
void LCD_init(){
    __delay_miliSec(20);        // Czekaj na uruchomienie LCD
    LCD_sendCommand(LCD_CONFIG); // Wy?lij komend? konfiguracyjn?
    __delay_nanoSec(50);        // Czekaj na przetworzenie komendy
    LCD_sendCommand(LCD_ON);    // W??cz LCD
    __delay_nanoSec(50);        // Czekaj na przetworzenie komendy
    LCD_sendCommand(LCD_CLEAR); // Wyczy?? LCD
    __delay_miliSec(2);         // Czekaj na przetworzenie komendy czyszczenia
}

// Funkcja odczytu ADC
unsigned int read_ADC(void){
    AD1CON1bits.SAMP = 1;       // Rozpocznij próbkowanie
    while(!AD1CON1bits.DONE);   // Czekaj na zako?czenie konwersji
    return ADC1BUF0;            // Zwró? warto?? ADC
}

// Funkcja wy?wietlania mocy na LCD
void display_power(unsigned int power){
    LCD_setCursor(1, 0);        // Ustaw kursor na pierwszym wierszu, pierwsza kolumna
    LCD_print("Power: ");       // Wy?wietl "Power: "
    LCD_sendData('0' + power / 100); // Wy?wietl cyfr? setek
    LCD_sendData('0' + (power % 100) / 10); // Wy?wietl cyfr? dziesi?tek
    LCD_sendData('0' + power % 10); // Wy?wietl cyfr? jednostek
}

// Funkcja wy?wietlania czasu na LCD
void display_time(unsigned int time){
    LCD_setCursor(2, 0);        // Ustaw kursor na drugim wierszu, pierwsza kolumna
    LCD_print("Time: ");        // Wy?wietl "Time: "
    LCD_sendData('0' + time / 60); // Wy?wietl minuty
    LCD_sendData(':');          // Wy?wietl dwukropek
    LCD_sendData('0' + (time % 60) / 10); // Wy?wietl dziesi?tki sekund
    LCD_sendData('0' + time % 10); // Wy?wietl sekundy
}

int main(void) {
    TRISB = 0x7FFF;     // Ustawienie rejestrów kierunku
    TRISD = 0xFFE7;
    TRISE = 0x0000;
    
    AD1CON1 = 0x80E4;   // Ustawienia ADC
    AD1CON2 = 0x0404;
    AD1CON3 = 0x0F00;
    AD1CHS = 0;
    AD1CSSL = 0x0020;
    
    LCD_init();         // Inicjalizacja wy?wietlacza
    
    unsigned int power = 0;    // Zmienna do przechowywania warto?ci mocy
    unsigned int time = 0;     // Zmienna do przechowywania warto?ci czasu
    bool running = false;      // Flaga stanu dzia?ania
    bool reset = false;        // Flaga resetowania
    
    char current6 = 0;         // Aktualny stan przycisku RD6
    char prev6 = 0;            // Poprzedni stan przycisku RD6
    char current7 = 0;         // Aktualny stan przycisku RD7
    char prev7 = 0;            // Poprzedni stan przycisku RD7
    char current8 = 0;         // Aktualny stan przycisku RD8
    char prev8 = 0;            // Poprzedni stan przycisku RD8
    
    while(1) {
        power = read_ADC() / 10; // Skalowanie warto?ci ADC do zakresu 0-102
        
        prev6 = PORTDbits.RD6;      // Skanowanie zmiany stanu przycisków
        prev7 = PORTDbits.RD7;
        prev8 = PORTDbits.RD8;
        __delay32(150000);
        current6 = PORTDbits.RD6;
        current7 = PORTDbits.RD7;
        current8 = PORTDbits.RD8;
        
        if(current6 - prev6 == 1){  // Przycisk dodawania czasu
            time += 10;
        }
       
        if(current7 - prev7 == 1){  // Przycisk start/stop
            running = !running;
        }
        
        if(current8 - prev8 == 1){  // Przycisk reset
            power = 0;
            time = 0;
            running = false;
        }
        
        if(running && time > 0){
            __delay_miliSec(1000); // Odliczanie w dó? co sekund?
            time--;
        }
        
        if(time == 0)
        {
            running = false;
        }
        display_power(power);  // Wy?wietlanie mocy na LCD
        display_time(time);    // Wy?wietlanie czasu na LCD
    }
    
    return 0;
}