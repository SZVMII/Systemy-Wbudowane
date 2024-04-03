// CONFIG2
#pragma config POSCMOD = NONE // XT Oscillator mode selected
#pragma config OSCIOFNC = ON // OSC2/CLKO/RC15 as port I/O (RC15)
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor disabled
#pragma config FNOSC = FRC // Primary Oscillator (XT, HS, EC)
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

unsigned portValue = 0;
char  prevS6=0, currentS6=0, prevS7=0, currentS7, program = 0;

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

int main(void) {
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    T1CON = 0x8020;
    PR1 = 0xFFFF; //maksymalna wartosc licznika
    _T1IE = 1; // wywoluje przerwanie gdy timer dojdzie do konca
    _T1IP = 1; // ważnośc 1-najnizszy
    
    while(1){
        prevS6=PORTDbits.RD6;
        prevS7=PORTDbits.RD7;
        __delay32(1000);
        currentS6=PORTDbits.RD6;
        currentS7=PORTDbits.RD7;;
        
        if(currentS6-prevS6 == -1){
            program = 0;
        }
        
        if(currentS7-prevS7 == -1){
            program = 1;
        }
    }
    return 0;
}
