#include<detpic32.h>

void send2displays(unsigned char);
unsigned char toBcd(unsigned char);
void delay(int micros);

volatile int voltage;

int main(void) {

    // ------------------- ADC ------------------------ //
    TRISBbits.TRISB4 = 1;           // RBx digital output disconnected
    AD1PCFGbits.PCFG4= 0;           // RBx configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;           // Conversion trigger selection bits: in this
                                    // mode an internal counter ends sampling and
                                    // starts conversion
    AD1CON1bits.CLRASAM = 1;        // Stop conversions when the 1st A/D converter
                                    // interrupt is generated. At the same time,
                                    // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16;          // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 2-1;         // Interrupt is generated after 2 samples
                                    // (replace XX by the desired number of
                                    // consecutive samples)
    AD1CHSbits.CH0SA = 4;           // replace x by the desired input
                                    // analog channel (0 to 15)    
    AD1CON1bits.ON = 1;             // This must the last command of the A/D
                                    // configuration sequenc
    

    // ------ 7 SEGMENTOS ------ //
    TRISDbits.TRISD6 = 0;       // configure RD6 as output
    TRISDbits.TRISD5 = 0;       // configure RD5 as output
    TRISB = TRISB & 0x80FF;     //0x1000 0000 1111 1111 -> configure RB8 - RB14 as outputs
    LATB = LATB & 0X80FF;       // LAT -> usado para escrever valores para ser enviado para as saídas OU
                                //        armazena o valore no registo para ser enviado para as saídas
                                // A escrita no porto é feita no endereço referenciado pelo identificador
                                // LATx; a leitura do porto é feita do endereço referenciado por PORTx.


    // ------- TIMER T2 -------- //
    T2CONbits.TCKPS = 2;    //1:2 prescaler (i.e. fout_presc = 5MHz)
    //Kprescaler = ceil(fPBCLK / ((65535 + 1) * fout))
    //fout = (fPBCLK/Kprescaler) / (PR2 + 1) = 20MHz / (4 * (41665 + 1)) = 120 Hz
    //PR2 = (fout_presc/fout) - 1 = (5MHz / 120Hz) - 1 = 41665
    PR2 = 41665;
    TMR2 = 0;               //Reset timer T2 count register
    T2CONbits.TON = 1;     //Enable timer T2

    IPC2bits.T2IP = 2;      // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1;      // Enable timer T2 interrupts
    IFS0bits.T2IF = 0;      // Reset timer T2 interrupt flag

    //------------
    EnableInterrupts();

    while (1)
    {
        AD1CON1bits.ASAM = 1;               // Start conversion
        while( IFS1bits.AD1IF == 0 );       // Wait while conversion not done

        int soma = 0;
        int* val = (int*)(&ADC1BUF0);
        int i;
        for (i = 0; i < 2; i++)
        {
            soma += 1.36 * ((val[i*4] * 33 + 511) / 1023) + 20.5 ;
            //                  V=(VAL_AD*33+511)/1023     -> valor produzido pelo conversor A/D
            //                                                formula que está no guiao prático 5, pagina 6
        }

        voltage = soma/2;

        delay(100);
        IFS1bits.AD1IF = 0;
        
    }
    
    return 0;

}

void _int_(8) isr_t2(void) {
    send2displays(toBcd(voltage));
    IFS0bits.T2IF = 0;
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

void delay(int ms){
    for(;ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() > 20000);
    }
}


