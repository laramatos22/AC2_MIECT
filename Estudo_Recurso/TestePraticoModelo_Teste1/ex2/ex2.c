#include<detpic32.h>

void send2displays(unsigned char value);
void delay(int ms);

int main(void) {

    TRISB = (TRISB & 0X80FF) | 0x000F;  // 0x1000 0000 1111 1111 
                                        // 0x0000 0000 0000 1111
                                        // LOGO: RB[3..0] como entradas
                                        //       RB[14..8] como saidas

    TRISD = TRISD & 0xFF9F;             // 0x1111 1111 1001 1111
                                        // LOGO: RD[6..5] como saidas

    int i;
    //int counter = 0;

    while (1)
    {
        char c = getChar();
        i = 0;

        if (c >= '0' && c <= '3')
        {
            do
            {
                send2displays(c-0x30);
                delay(10);      // T = 10 milisegundos = 10x10⁻3 segundos
                                // f = 1 / T = 1 / (10*10⁻3) = 100 Hz
            } while (++i < 20);
            
        } else
        {
            do
            {
                send2displays(0xFF);
                delay(10);
            } while (++i < 100);
            
        }
        
        LATD = LATD & 0x009F;
        
    }
    
    return 0;

}

// ------------ FUNCAO SEND2DISPLAYS ---------------- //
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
    } else {                                    //  wait -> displayFlag == 0
        LATD = (LATD | 0x0020) & 0xFFBF;        //  activate display low & deactivate display high
        LATB = (LATB & 0x80FF) | dl<< 8;        //  set display low value
    }
    displayFlag = !displayFlag;     // toggle displayFlag value
}

// ------------ FUNCAO DELAY -------------- //
void delay(int ms){
    for(;ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() > 20000);
    }
}
