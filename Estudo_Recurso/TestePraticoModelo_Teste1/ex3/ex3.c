#include<detpic32.h>

void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);
void delay(int ms);

int main(void) {
    
    TRISB = TRISB & 0x80FF;         // 0x1000 0000 1111 1111 -> RB[14..8] como saidas
    TRISD = TRISD & 0xFF9F;         // 0x1111 1111 1001 1111 -> RD[6..5] como saidas

    // --------------------- CONFIGURACAO ADC -------------------- //
    TRISBbits.TRISB4 = 1;           // RBx digital output disconnected
    AD1PCFGbits.PCFG4= 0;           // RBx configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;           // Conversion trigger selection bits: in this
                                    // mode an internal counter ends sampling and
                                    // starts conversion
    AD1CON1bits.CLRASAM = 1;        // Stop conversions when the 1st A/D converter
                                    // interrupt is generated. At the same time,
                                    // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16;          // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 1-1;         // Interrupt is generated after 1 samples
                                    // (replace XX by the desired number of
                                    // consecutive samples)
    AD1CHSbits.CH0SA = 4;           // replace x by the desired input
                                    // analog channel (0 to 15)    
    AD1CON1bits.ON = 1;             // This must the last command of the A/D
                                    // configuration sequence
}


void send2displays(unsigned char value) {
    static char displayFlag = 0;
    char display7Scodes[] = {
            0x3F, //0
            0x06, //1
            0x5B, //2
            0x4F, //3
            0x66, //4
            0x6D, //5
            0x7D, //6
            0x07, //7
            0x7F, //8
            0x6F, //9
            0x77, //A
            0x7C, //b
            0x39, //C
            0x5E, //d
            0x79, //E
            0x71  //F
    };

    // send digit_high(dh) to display_high : dh = value >> 4; get the correct hexadecimal code for the number using display7Scodes[...];
    unsigned char dh = display7Scodes[value >> 4];      // Get the value at 7..4
    // send digit_low(dl) to display_low : dl = value & 0x0F; get the correct hexadecimal code for the number using display7Scodes[...];
    unsigned char dl = display7Scodes[value & 0x0F];    // Get the value at 3..0

    if (displayFlag == 1) {
        LATD = (LATD | 0x0040) & 0xFFDF;        //  activate display high & deactivate display low
        LATB = (LATB & 0x00FF) | dh  << 8;      //  set display high value
    } else {                                    // wait -> displayFlag == 0
        LATD = (LATD | 0x0020) & 0xFFBF;        //  activate display low & deactivate display high
        LATB = (LATB & 0x80FF) | dl<< 8;        //  set display low value
    }
    displayFlag = !displayFlag;     // toggle displayFlag value
}


unsigned char toBcd(unsigned char value) {
    return ((value / 10) << 4) + (value % 10);
}


void delay(int ms) {
    for(;ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() > 20000);
    }
}
