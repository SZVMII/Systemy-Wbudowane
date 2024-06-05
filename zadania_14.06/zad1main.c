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

#include "xc.h"
#include <libpic30.h>

int nrZad = 0; //zmienna do przechodzenia miedzy programami


void zad1() // 8 bitowy licznik binarny zliczający w górę (0…255)
{
    unsigned char portValue1 = 0;
    TRISA = 0x0000; // set all port bits to be output
    TRISD = 0xFFFF;

    while(1) 
    {
        LATA = portValue1;
       __delay32(4000000);
       portValue1 += 1;
       
       // obsluga przyciskow       
       if(PORTDbits.RD6 == 0) 
       {
           nrZad =8;
           break;
       }
        
       if(PORTDbits.RD7 == 0)
       {
           nrZad ++;
           break;
       }
    }
}


void zad2() //8 bitowy licznik binarny zliczający w dół (255…0)
{
    unsigned char portValue2 = 255;
    TRISA = 0x0000;
    TRISD = 0xFFFF;

    while(1) 
   {
        LATA = portValue2;
        __delay32(4000000);
        portValue2 -= 1;
        
               
       if(PORTDbits.RD6 == 0)
       {
          nrZad --;
           break;
       }
        
       if(PORTDbits.RD7 == 0)
       {
          nrZad ++;
           break;
       }
    }
}


void zad3() //8 bitowy licznik w kodzie Graya zliczający w górę (repr. 0…255)
{
    unsigned char portValue3 = 0;
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    
    while(1)
    {
        LATA = (portValue3 >> 1) ^ portValue3;
        __delay32(4000000);
        portValue3 += 1;
        
               
       if(PORTDbits.RD6 == 0)
       {
           nrZad --;
           break;
       }
        
       if(PORTDbits.RD7 == 0)
       {
           nrZad ++;
           break;
       }
    }
}

void zad4() //8 bitowy licznik w kodzie Graya zliczający w dół (repr. 255…0)
{
    unsigned char portValue4 = 0;
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    
    while(1)
    {
        LATA = (portValue4 >> 1) ^ portValue4;
        __delay32(4000000);
        portValue4 -= 1;
        
               
       if(PORTDbits.RD6 == 0)
       {
           nrZad --;
           break;
       }
        
       if(PORTDbits.RD7 == 0)
       {
           nrZad ++;
           break;
       }
    }
    
}

void zad5() //2x4 bitowy licznik w kodzie BCD zliczający w górę (0…99)
{
    unsigned char portValue5 = 0;
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    
    while(1)
    {
        LATA = (((portValue5/10) << 4) | (portValue5 % 10));
        __delay32(4000000);
        portValue5 = (portValue5 + 1) % 100;
        
               
       if(PORTDbits.RD6 == 0)
       {
           nrZad --;
           break;
       }
        
       if(PORTDbits.RD7 == 0)
       {
           nrZad ++;
           break;
       }
        

        
    }
}

void zad6() //2x4 bitowy licznik w kodzie BCD zliczający w dół (99…0)
{
    unsigned char portValue6 = 99;
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    
    while(1)
    {
        LATA = (((portValue6/10) << 4) | (portValue6 % 10));
        __delay32(4000000);
        portValue6 = (portValue6 == 0) ? 99 : portValue6 - 1;
        
       if(PORTDbits.RD6 == 0)
       {
           nrZad --;
           break;
       }
        
       if(PORTDbits.RD7 == 0)
       {
           nrZad ++;
           break;
       }
        
    }
}

void zad7()//3 bitowy wężyk poruszający się lewo-prawo
{
    unsigned char portValue7[10] = {7,14,28,56,112,224,112,56,28,14}; // tablica z wartosciami
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    unsigned int i = 0; //index
    
    while(1)
    {
        LATA = portValue7[i];
        __delay32(4000000);
        i++;
       
       if(i==10)
       {
           i = 0; // powrot na poczatek tablicy
       }
        
               
       if(PORTDbits.RD6 == 0)
       {
           nrZad --;
           break;
       }
        
       if(PORTDbits.RD7 == 0)
       {
           nrZad ++;
           break;
       }
    }
}

void zad8() //Kolejka
{
    unsigned char portValue8 = 0;
    TRISA = 0x0000;
    TRISD = 0xFFFF;
	
    int break_button1 = 0;
    int break_button2 = 0;
    
	while(1)
	{
		for(int i=0; i<8; i++)
		{
			int temp = 1;
			for(int j=i+1; j<8; j++)
			{
				LATA = portValue8 + temp;
				temp = temp << 1;    
				__delay32(4000000);
            
                   
				if(PORTDbits.RD6 == 0)
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
			portValue8 += temp;
			
			if(break_button1 == 1)
			{
				break;
			}
        
			if(break_button2 == 1)
			{
				break;
			}
		}
		
		//wyjscie z glownej petli, zmiana programu
		if(break_button1 == 1)
		{
			nrZad --;
			break;
		}
		
		if(break_button2 == 1)
		{
			nrZad ++;
			break;
		}	
	}   
}

void zad9() //6 bitowy generator liczb pseudolosowych oparty o konfigurację 1110011
{
    unsigned char portValue9;
    unsigned char seed = 0b1010101;
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    
    while(seed != 0)
    {
        portValue9 = (seed >> 0) ^ (seed >> 1) ^ (seed >> 4) ^ (seed >> 5) ^ (seed >> 6);
        seed = seed >> 1 | seed << 6;
        LATA = portValue9;
        __delay32(4000000);
        
               
       if(PORTDbits.RD6 == 0)
       {
           nrZad --;
           break;
       }
        
       if(PORTDbits.RD7 == 0)
       {
           nrZad ++;
           break;
       }
    }
}

int main(void) 
{
    if(nrZad < 0)
    {
        nrZad = 8;
    }
    
    if(nrZad == 0)
    {
        zad1();
    }
    
    if(nrZad == 1)
    {
        zad2();
    }
    
    if(nrZad == 2)
    {
        zad3();
    }
    
    if(nrZad == 3)
    {
        zad4();
    }
    
    if(nrZad == 4)
    {
        zad5();
    }
    
    if(nrZad == 5)
    {
        zad6();
    }
    
    if(nrZad == 6)
    {
       zad7();
    }
    
    if(nrZad == 7)
    {
        zad8();
    }
    
    if(nrZad == 8)
    {
        zad9();
    }
    
    if(nrZad > 8)
    {
        nrZad = 0; 
    }
    
}
