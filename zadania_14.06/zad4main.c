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

//makra
#define FCY         4000000UL   //czestotliwosc oscylatora
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

//WAZNE!!!!!!
//trzeba przytrzymac sekunde guzik start/stop by wznowic/zatrzymac prace

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
void LCD_setCursor(unsigned char row, unsigned char col){
    unsigned char address;
    if (row == 1){
        address = LCD_CURSOR + LINE1 + col;
    }
    if(row == 2){
        address = LCD_CURSOR + LINE2 + col;
    }
    sendComLCD(address);
}


void LCD_init(){
    __delayMiliSec(20);
    sendComLCD(LCD_CONFIG);
    __delayNanoSec(50);
    sendComLCD(LCD_ON);
    __delayNanoSec(50);
    sendComLCD(LCD_CLEAR);
   __delayMiliSec(2);
}

//obsluga potencjometru
unsigned int read_ADC(void){
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE);
    return ADC1BUF0;
}

//wyswietlanie mocy w danym formacie
void powerDisp(unsigned int moc){
    LCD_setCursor(1, 0);
    printLCD("Moc(W): ");
    sendDataLCD('0' + moc / 100);
    sendDataLCD('0' + (moc % 100) / 10);
    sendDataLCD('0' + moc % 10);
}

//wyswietlanie czasu w danym formacie
void timeDisp(unsigned int czas){
    LCD_setCursor(2, 0);
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
    TRISA = 0xFFFF;
    
    AD1CON1 = 0x80E4;
    AD1CON2 = 0x0404;
    AD1CON3 = 0x0F00;
    AD1CHS = 0;
    AD1CSSL = 0x0020;
    
    LCD_init();

	//zmienne do mocy i czasu
    unsigned int moc = 0;
    unsigned int czas = 0;
    bool running = false;
    
    unsigned char current6 = 0, prev6 = 0;
    unsigned char current7 = 0, prev7 = 0;
    unsigned char currentA7 = 0, prevA7 = 0;
    
    while(1) {
        moc = read_ADC() / 10; //skala oscylatora
        
        current6 = PORTDbits.RD6;
        current7 = PORTDbits.RD7;
		currentA7 = PORTAbits.RA7;
        
        if(running && czas > 0 && moc > 0){
            __delayMiliSec(1000); //odliczanie w dol
            czas--;
        }
        else
        {
            __delay32(150000); //delay do wykrywania zmian stanow
        }
        
        prev6 = PORTDbits.RD6;      
        prev7 = PORTDbits.RD7;
		prevA7 = PORTAbits.RA7;
        
        if(current6 - prev6 == 1){  //dodawanie czasu
            czas += 10;
        }
       
        if(current7 - prev7 == 1){  //start/stop
            running = !running;
        }
        
        if(currentA7 - prevA7 == 1){  //reset
            moc = 0;
            czas = 0;
            running = false;
        }
        
        //zatrzymuje sie gry moc jest rowna 0
        if(moc == 0)
        {
            running = false;
        }
        
        //zatrzymuje sie gdy skonczy sie czas
        if(czas == 0)
        {
            running = false;
        }
        powerDisp(moc);
        timeDisp(czas);
    }
    
    return 0;
}
