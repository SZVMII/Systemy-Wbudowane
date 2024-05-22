/*
* File:   source.c
* Author: local
*
* Created on 10 kwietnia 2024, 12:00
*/
 
// CONFIG2
#pragma config POSCMOD = NONE             // Primary Oscillator Select (primary oscillator disabled)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRC              // Oscillator Select (Fast RC Oscillator without Postscaler)
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
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include <libpic30.h>
#define FCY 4000000UL
#define LCD_E LATDbits.LATD4
#define LCD_RW LATDbits.LATD5
#define LCD_RS LATBbits.LATB15
#define LCD_DATA LATE
#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_ON 0x0C
#define LCD_OFF 0x08
#define LCD_CONFIG 0x38
#define LCD_CURSOR 0x80
#define LINE1 0x00
#define LINE2 0x40
#define LCD_CUST_CHAR 0x40
#define LCD_SHIFT_R 0x1D
#define LCD_SHIFT_L 0x1B

void __delay_us(unsigned long us){
    __delay32(us*FCY/1000000);
}
void __delay_ms(unsigned long ms){
    __delay32(ms*FCY/1000);
}
void LCD_sendCommand(unsigned char command){
    LCD_RW = 0;
    LCD_RS = 0;
    LCD_E = 1;
    LCD_DATA = command;
    __delay_us(50);
    LCD_E = 0;
}
void LCD_sendData(unsigned char data){
    LCD_RW = 0;
    LCD_RS = 1;
    LCD_E = 1;
    LCD_DATA = data;
    __delay_us(50);
    LCD_E = 0;
}
void LCD_print(char* string){
    while(*string){
        LCD_sendData(*string++);
    }
}
void LCD_setCursor(unsigned char row, unsigned char col){
    unsigned char adress;
    if(row == 1){
        adress = LCD_CURSOR + LINE1 + col;
    }
    if(row == 2){
        adress = LCD_CURSOR + LINE2 + col;
    }
    LCD_sendCommand(adress);
}
void LCD_saveCustChar(unsigned char slot, unsigned char *array){
    unsigned char i;
    LCD_sendCommand(LCD_CUST_CHAR + (slot*8));
    for (i=0;i<8;i++){
        LCD_sendData(array[i]);
    }
}
void LCD_init(){
    __delay_ms(20);
    LCD_sendCommand(LCD_CONFIG);
    __delay_us(50);
    LCD_sendCommand(LCD_ON);
    __delay_us(50);
    LCD_sendCommand(LCD_CLEAR);
    __delay_ms(2);
}
unsigned char symbol1[8] = {
   0b00000,
   0b01010,
   0b10101,
   0b10001,
   0b01010,
   0b00100,
   0b00000,
   0b00000,
};
int main(void) {
    TRISD = 0x0000;
    TRISB = 0x7FFF;
    TRISE = 0x0000;
    LCD_init();
    LCD_saveCustChar(0, symbol1);
    LCD_setCursor(2,3);
    LCD_print("Opel Omega ");
    LCD_sendData(0);
    __delay_ms(500);
    LCD_sendCommand(LCD_SHIFT_R);
    __delay_ms(500);
    return 0;
}