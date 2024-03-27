// PIC24FJ128GA010 Configuration Bit Settings
// For more on Configuration Bits,

// consult your device data sheet
// CONFIG2
#pragma config POSCMOD = XT // XT Oscillator mode selected
#pragma config OSCIOFNC = ON // OSC2/CLKO/RC15 as port I/O (RC15)
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor disabled
#pragma config FNOSC = PRI // Primary Oscillator (XT, HS, EC)
#pragma config IESO = ON // Int Ext Switch Over Mode enabled
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

void zadanie1(){
    for( int i = 0; i<= 255; i++){
        return i;
    }
}

void zadanie2(){
    for (int i = 255; i >= 0; i--){
        return i;
    }
}
//W jêzyku C tê operacjê mo¿na zapisaæ nastêpuj¹cym wyra¿eniem: gray = liczba ^ (liczba / 2) lub gray = liczba ^ (liczba >> 1).
int gray(int x){
    return x ^ (x >> 1);
}

void zadanie3(){
    for( int i = 0; i<= 255; i++){
        return gray(i);
    }
}

void zadanie4(){
    for (int i = 255; i >= 0; i--){
        return gray(i);
    }
}

int bcd(int x){
    return (((x / 10) << 4) | (x % 10));
}

void zadanie5(){
    for( int i = 0; i<= 99; i++){
        return bcd(i);
    }
}

void zadanie6(){
    for( int i = 99; i >= 0; i--){
        return bcd(i);
    }
}

void zadanie7(){
    int snake = 0b00000111;
    while(snake != 0b11100000){
        snake <<= 1;
        return snake;
    }
    while(snake !=0b00000011){
        snake >>=1;
    }
}

void zadanie8(){
    int value = 0;
    for (int i = 0; i < 8 ;i++){
        int temp = 1;
        for (int j = i + 1 ; j < 8;j++){
            decimaltobinary( value + temp);
            temp <<=1;
        }
        decimaltobinary( value + temp); //wy?wietla juz zapisane wartosci
        value += temp;
    }
}


void decimaltobinary(int value){
    int arr[8];
    for(int i = 7; i >= 0; i--){
        arr[i] = (value & 1);
        value >>= 1;
    }
    for(int i = 0; i < 8; i++); //printf("%d",arr[i]);
    //printf("\n");
}

// zadanie 1 i 2

//int main(void) {
//unsigned char portValue = 0;
//char currentS6 = 0 ,currentS7 = 0, prevS6 = 0, prevS7 = 0;
//TRISA = 0x0000; // set all port bits to be output
//TRISD = 0xFFFF;
//
//while(1){
//    LATA = portValue; // write to port latch
//    prevS6 = PORTDbits.RD6;
//    prevS7 = PORTDbits.RD7;
//    __delay32(15000);
//    currentS6 = PORTDbits.RD6;
//    currentS7 = PORTDbits.RD7;
//    
//    if(currentS6 - prevS6 == 1){
//        portValue++;
//    }
//    if(currentS7 - prevS7 ==1){
//        portValue--;
//    }
//}
//
//return 0;
//}

//zadanie 3

//int main(void) {
//    unsigned char portValue;
//    unsigned char gray;
//    // Port A access
//    AD1PCFG = 0xFFFF; // set to digital I/O (not analog)
//    TRISA = 0x0000; // set all port bits to be output
//    while(1) {
//        gray = portValue ^ (portValue/2);
//        portValue += 1;
//        LATA = gray; // write to port latch
//        __delay32(5000000); // delay in instruction cycles
//
//    }
//    return -1;
//}

//zadanie 4

//int main(void) {
//    
//    unsigned char portValue = 255;
//    unsigned char gray;
//    AD1PCFG = 0xFFFF;
//    TRISA = 0x0000;
//    while(1) {
//        gray = portValue ^ (portValue/2);
//        portValue--;
//        LATA = gray;
//        __delay32(5000000);
//    }
//    return -1;
//    
//}

//zadanie 5 

//int main(void) {
//    unsigned char portValue;
//    unsigned char bcd;
//    AD1PCFG = 0xFFFF;
//    TRISA = 0x0000;
//    while(1){
//        bcd = (((portValue / 10) << 4) | (portValue % 10));
//        portValue++;
//        LATA = bcd;
//        __delay32(5000000);
//
//    }
//    return -1;
//}

//zadanie 6

//int main(void) {
//    unsigned char portValue = 99;
//    unsigned char bcd;
//    AD1PCFG = 0xFFFF;
//    TRISA = 0x0000;
//    while(1){
//        bcd = (((portValue / 10) << 4) | (portValue % 10));
//        portValue--;
//        LATA = bcd;
//        __delay32(5000000);
//
//    }
//    return -1;
//}


//zadanie 7

int main(void) {
    unsigned char portValue;
    unsigned char snake = 0b00000111;
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    while(1){
        while (snake != 0b11100000){
            LATA = snake;
           snake <<= 1;
           __delay32(1000000);
        }
       while (snake != 0b00000011){
            LATA = snake;
            snake >>= 1;
            __delay32(1000000);
        }
    }
    return -1;
}


//zadanie 8

//int main(void){
//    unsigned char portValue;
//    unsigned char value = 0;
//    AD1PCFG = 0xFFFF;
//    TRISA = 0x0000;
//    for (int i = 0; i < 8 ; i++){
//        unsigned char temp = 1;
//        for (int j = i + 1; j < 8; j++){
//            LATA = value + temp;
//            temp <<=1;
//            __delay32(1000000);
//        }
//        LATA = (value + temp);
//        value += temp;
//        __delay32(10000000);
//    }
//    return -1;
//}
