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

#include <xc.h>
#include <libpic30.h>

// Definicje makr dla ró?nych parametrów i polece? LCD
#define FCY 4000000UL        // Cz?stotliwo?? zegara (4 MHz)
#define LCD_E LATDbits.LATD4 // E - Enable
#define LCD_RW LATDbits.LATD5 // RW - Read/Write
#define LCD_RS LATBbits.LATB15 // RS - Register Select
#define LCD_DATA LATE        // Port danych LCD
#define LCD_CLEAR 0x01       // Komenda czyszczenia ekranu LCD
#define LCD_HOME 0x02        // Komenda ustawienia kursora na pocz?tku
#define LCD_ON 0x0C          // Komenda w??czenia LCD
#define LCD_OFF 0x08         // Komenda wy??czenia LCD
#define LCD_CONFIG 0x38      // Konfiguracja LCD (tryb 8-bitowy, 2 linie)
#define LCD_CURSOR 0x80      // Komenda ustawienia kursora
#define LINE1 0x00           // Adres pocz?tku pierwszej linii
#define LINE2 0x40           // Adres pocz?tku drugiej linii
#define LCD_CUST_CHAR 0x40   // Komenda definicji niestandardowego znaku
#define LCD_SHIFT_R 0x1D     // Komenda przesuni?cia ekranu w prawo
#define LCD_SHIFT_L 0x1B     // Komenda przesuni?cia ekranu w lewo

// Funkcje opó?nienia
void __delay_us(unsigned long us) {
    __delay32(us * FCY / 1000000); // Opó?nienie w mikrosekundach
}

void __delay_ms(unsigned long ms) {
    __delay32(ms * FCY / 1000); // Opó?nienie w milisekundach
}

// Funkcja wysy?ania komendy do LCD
void LCD_sendCommand(unsigned char command) {
    LCD_RW = 0;  // Ustawienie RW na zapis
    LCD_RS = 0;  // Ustawienie RS na komend?
    LCD_E = 1;   // W??czenie E
    LCD_DATA = command; // Wys?anie komendy
    __delay_us(50);     // Krótkie opó?nienie
    LCD_E = 0;          // Wy??czenie E
}

// Funkcja wysy?ania danych do LCD
void LCD_sendData(unsigned char data) {
    LCD_RW = 0;  // Ustawienie RW na zapis
    LCD_RS = 1;  // Ustawienie RS na dane
    LCD_E = 1;   // W??czenie E
    LCD_DATA = data; // Wys?anie danych
    __delay_us(50);  // Krótkie opó?nienie
    LCD_E = 0;       // Wy??czenie E
}

// Funkcja drukowania ?a?cucha znaków na LCD
void LCD_print(char* string) {
    while (*string) {
        LCD_sendData(*string++); // Wysy?anie kolejnych znaków ?a?cucha
    }
}

// Funkcja ustawiania kursora na LCD
void LCD_setCursor(unsigned char row, unsigned char col) {
    unsigned char adress;
    if (row == 1) {
        adress = LCD_CURSOR + LINE1 + col; // Adres pierwszej linii
    }
    if (row == 2) {
        adress = LCD_CURSOR + LINE2 + col; // Adres drugiej linii
    }
    LCD_sendCommand(adress); // Wys?anie komendy ustawienia kursora
}

// Funkcja zapisywania niestandardowego znaku do pami?ci LCD
void LCD_saveCustChar(unsigned char slot, unsigned char *array) {
    unsigned char i;
    LCD_sendCommand(LCD_CUST_CHAR + (slot * 8)); // Wybór slota na niestandardowy znak
    for (i = 0; i < 8; i++) {
        LCD_sendData(array[i]); // Zapisywanie bajtów znaku
    }
}

// Funkcja inicjalizacji LCD
void LCD_init() {
    __delay_ms(20);          // Opó?nienie pocz?tkowe
    LCD_sendCommand(LCD_CONFIG); // Wys?anie komendy konfiguracji
    __delay_us(50);          // Krótkie opó?nienie
    LCD_sendCommand(LCD_ON); // W??czenie LCD
    __delay_us(50);          // Krótkie opó?nienie
    LCD_sendCommand(LCD_CLEAR); // Wyczy?? ekran LCD
    __delay_ms(2);           // D?u?sze opó?nienie
}

// Definicja niestandardowego znaku (serce)
unsigned char heart[8] = {
   0b00000,
   0b01010,
   0b10101,
   0b10001,
   0b01010,
   0b00100,
   0b00000,
   0b00000,
};

// Definicja niestandardowego znaku (auto)
unsigned char car1[8] = {
   0b00111,
   0b01000,
   0b01000,
   0b10000,
   0b10000,
   0b11111,
   0b01100,
   0b01100,
};

unsigned char car2[8] = {
   0b10000,
   0b01000,
   0b00111,
   0b00001,
   0b00001,
   0b11111,
   0b01100,
   0b01100,
};

int main(void) { // G?ówna funkcja
    TRISD = 0x0000; // Ustawienie portu D jako wyj?ciowego
    TRISB = 0x7FFF; // Ustawienie portu B jako wej?ciowego
    TRISE = 0x0000; // Ustawienie portu E jako wyj?ciowego
    LCD_init();     // Inicjalizacja LCD
    LCD_saveCustChar(0, heart); // Zapisanie niestandardowego znaku (serca) do slota 0
    LCD_saveCustChar(1, car1);     // Zapisanie niestandardowego znaku (auta) do slota 1
     LCD_saveCustChar(2, car2);     // Zapisanie niestandardowego znaku (auta) do slota 2
    
    // Wy?wietlenie napisu "skup z?omu" wy?rodkowanego w pierwszej linii
    LCD_setCursor(1, 1); // Ustawienie kursora w wierszu 1, kolumnie 3
    LCD_sendData(1); // Wys?anie niestandardowego znaku (auta)
    LCD_sendData(2);
    LCD_print("skup zlomu"); // Wydrukowanie tekstu "skup z?omu" na LCD
    LCD_sendData(1); // Wys?anie niestandardowego znaku (auta)
    LCD_sendData(2);
    
    // Wy?wietlenie napisu "Opel Omega" z sercami w drugiej linii
    LCD_setCursor(2, 1); // Ustawienie kursora w wierszu 2, kolumnie 1
    LCD_sendData(0); // Wys?anie niestandardowego znaku (serca)
    LCD_print(" Opel Omega "); // Wydrukowanie tekstu "Opel Omega " na LCD
    LCD_sendData(0); // Wys?anie niestandardowego znaku (serca)
    
    
    __delay_ms(500); // Opó?nienie 500 ms
    LCD_sendCommand(LCD_SHIFT_R); // Przesuni?cie ekranu w prawo
    __delay_ms(500); // Opó?nienie 500 ms
    
    return 0;
}
