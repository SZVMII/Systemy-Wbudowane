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

#include <xc.h>
#include <libpic30.h>

int number = 0; //implementacja zmiennej do przelaczania miedzy zadaniami

void zad1(){    //8 bitowy licznik binarny w gore
    unsigned char counter = 0; //licznik
    AD1PCFG = 0xFFFF; //wszystkie porty cyfrowe
    TRISA = 0x0000; //port A jako wyjscie
    while(1) {
        LATA = counter; //ustawienie wartosci portu
        __delay32(1000000); // Opoznienie przed inkrementacja
        counter++;
        if (counter == 0) // Sprawdzenie, czy przekroczono maksymalna wartosc
            counter = 0; // Zerowanie licznika, jezeli przekroczono maksymalna wartosc
        
        if(PORTDbits.RD6 == 0)      //zadanie poprzednie
        {
            number --;
            break;
        }
        
        if(PORTDbits.RD7 == 0)      //zadanie kolejne
        {
            number ++;
            break;
        }
    }
}
void zad2(){    //8 bitowy licznik zliczajacy w dol
   unsigned char counter = 0; //licznik
    AD1PCFG = 0xFFFF;   //wszystkie porty jako cyfrowe
    TRISA = 0x0000; //port A jako wyjscie
    while(1) {
        LATA = counter;
        __delay32(1000000); // Opoznienie przed inkrementacja
        counter--;
        if (counter == 0) // Sprawdzenie, czy przekroczono maksymalna wartosc
            counter = 0; // Zerowanie licznika, jesli przekroczono maksymalna wartosc
        
        if(PORTDbits.RD6 == 0)  //zadanie poprzednie
        {
            number --;
            break;
        }
        
        if(PORTDbits.RD7 == 0)  //zadanie kolejne
        {
            number ++;
            break;
        }
    }
}


void zad3(){    //  8 bitowy licznik w kodzie Graya w gore
    unsigned char portValue3 = 0;
    unsigned char gray;
    AD1PCFG = 0xFFFF; 
    TRISA = 0x0000; 
    while(1) {
        gray =(portValue3 >> 1) ^ portValue3; //konwersja na kod Graya
        portValue3++;
        LATA = gray; 
        __delay32(1000000); 
        
        if(PORTDbits.RD6 == 0)  //zadanie poprzednie
        {
            number --;
            break;
        }
        
        if(PORTDbits.RD7 == 0)  //zadanie kolejne
        {
            number ++;
            break;
        }

    }
}

void zad4(){    //8 bitowy licznik w kodzie Graya zliczajacy w dol
    unsigned char portValue4 = 255;
    unsigned char gray;
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    while(1) {
        gray = (portValue4 >> 1) ^ portValue4 ; //konwersja na kod Graya
        portValue4--;
        LATA = gray;
        __delay32(1000000);
        
        if(PORTDbits.RD6 == 0)  //zadanie poprzednie
        {
            number --;
            break;
        }
        
        if(PORTDbits.RD7 == 0)  //zadanie kolejne
        {
            number ++;
            break;
        }
    }
}

void zad5(){ //2x4 bitowy licznik w kodzie BCD w gore
    unsigned char portValue5=0;
    unsigned char bcdUp;
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    while(1){
        bcdUp = (((portValue5 / 10) << 4) | (portValue5 % 10)); //konwersja na BCD
        portValue5++;
        LATA = bcdUp;
        __delay32(500000);
        
        if(PORTDbits.RD6 == 0)  //zadanie poprzednie
        {
            number --;
            break;
        }
        
        if(PORTDbits.RD7 == 0)  //zadanie kolejne
        {
            number ++;
            break;
        }
    }
}

void zad6(){ //2x4 bitowy licznik w kodzie BCD zliczajacy w dol
    unsigned char portValue6=99;
    unsigned char bcdDown;
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    while(1){
        bcdDown = (((portValue6 / 10) << 4) | (portValue6 % 10));   //konwersja na BCD
        portValue6--;
        LATA = bcdDown;
        __delay32(500000);
        
        if(PORTDbits.RD6 == 0)  //zadanie poprzednie
        {
            number --;
            break;
        }
        
        if(PORTDbits.RD7 == 0)  //zadanie kolejne
        {
            number ++;
            break;
        }
    }
}

void zad7(){    //snake
    unsigned char portValue7[10] = {7,14,28,56,112,224,112,56,28,14}; // Warto?ci dla efektu "snake"
    TRISA = 0x0000; //port A jako wyjscie
    TRISD = 0xFFFF; //port D jako wyjscie
    unsigned int i = 0;
    
    while(1)
    {
        LATA = portValue7[i];
        __delay32(1000000);
        i++;
       
       if(i==10)
       {
           i = 0; //resetowanie indeksu na koncu tablicy
       }
        
       if(PORTDbits.RD6 == 0)   //zadanie poprzednie
       {
           number --;
       }
        
       if(PORTDbits.RD7 == 0)   //zadanie kolejne
       {
           number ++;
           break;
       }
    }
}

void zad8(){    //Kolejka
    unsigned char portValue8 = 0;
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    int break_button1 = 0;
    int break_button2 = 0;
    
    for(int i=0; i<8; i++)
    {
        int temp = 1;
        for(int j=i+1; j<8; j++)
        {
            LATA = portValue8 + temp; //ustawienie wartosci na porcie
            temp = temp << 1; //przesuniecie bitowe
            
            __delay32(1000000);
            
            if(PORTDbits.RD6 == 0)  //sprawdzenie czy kliknieto guzik do przelaczenia zadania
            {
                break_button1 = 1;
                break;
            }
        
            if(PORTDbits.RD7 == 0)  
            {
                break_button2 = 1;
                break;
            }
        }
        portValue8 += temp; //aktualizacja portValue8
        
        if(break_button1 == 1)  //zadanie poprzednie
        {
            number --;
            break;
        }
        
        if(break_button2 == 1)  //zadanie kolejne
        {
            number ++;
            break;
        }
    }
}

void zad9(){    //generator liczb
    unsigned char portValue9;
    unsigned char seed = 0b1110011; //inicjalizacja seedu
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    
    while(seed != 0)
    {
        portValue9 = (seed >> 0) ^ (seed >> 1) ^ (seed >> 4) ^ (seed >> 5) ^ (seed >> 6); //generowanie pseudolosowej wartosci
        seed = seed >> 1 | seed << 6; //aktualizacja ziarna
        LATA = portValue9;
        __delay32(1000000);
        
               
       if(PORTDbits.RD6 == 0)   //zadanie poprzednie
       {
           number --;
           break;
       }
        
       if(PORTDbits.RD7 == 0)   //zadanie kolejne
       {
           number ++;
           break;
       }
    }
}

int main(void) {
    if(number < 0)
    {
        number = 0; //gdy mniejsze od 0 -> zad1
    }
    
    if(number == 0)
    {
        zad1(); //8 bitowy licznik w gore
    }
    
    if(number == 1)
    {
        zad2(); //8 bitowy licznik w gore
    }
    
    if(number == 2)
    {
        zad3(); //8 bitowy w kodzie Graya w gore
    }
    
    if(number == 3)
    {
        zad4(); //8 bitowy w kodzie Graya w dol
    }
    
    if(number == 4)
    {
        zad5(); //2x4 bitowy BCD w gore
    }
    
    if(number == 5)
    {
        zad6(); //2x4 bitowy BCD w dol
    }
    
    if(number == 6)
    {
        zad7(); //snake
    }
    
    if(number == 7)
    {
        zad8(); //kolejka
    }
    
    if(number == 8)
    {
        zad9(); //generator
    }
    
    if(number > 8)
    {
        number = 0; //gdy wieksze od 8 -> zad1
    }
}