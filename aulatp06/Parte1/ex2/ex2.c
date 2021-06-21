#include<detpic32.h>

// Global variable
volatile unsigned char voltage = 0;

//funcao delay()
void delay(int ms)
{
    for(;ms>0;ms--)
    {
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

void send2displays(unsigned char value)
{
    static const char display7Scodes[] = {0x3F,     //0
                                         0x06,      //1
                                         0x5B,      //2
                                         0x4F,      //3
                                         0x66,      //4
                                         0x6D,      //5
                                         0x7D,      //6
                                         0x07,      //7
                                         0x7F,      //8
                                         0x6F,      //9
                                         0x77,      //A
                                         0x7C,      //B
                                         0x39,      //C
                                         0x5E,      //D
                                         0x79,      //E
                                         0x71};     //F

    static char displayFlag = 0;    // static variable: doesn't loose its
                                    // value between calls to function

    // send digit_high (dh) to display_high: dh = value >> 4
    unsigned char digit_high = value >> 4;                          // Get the index of the decimal part
    
    // send digit_low (dl) to display_low: dl = value & 0x0F
    unsigned char digit_low = value & 0x0F;                         // Get the index of the unitary part

    // Get the correct hexadecimal code for the number
    digit_high = display7Scodes[digit_high];
    digit_low = display7Scodes[digit_low];

    // if "displayFlag" is 0 then send digit_low to display_low
    if (displayFlag == 0)
    {
        LATD = (LATD | 0x0040) & 0xFFDF;                                // Display High active and Display Low inactive
        LATB = (LATB & 0x80FF) | (unsigned int)(digit_low) << 8;        // Clean display and set the right value
    }
    // else send digit_high to didplay_high
    else
    {
        LATD = (LATD | 0x0020) & 0xFFBF;                                // Displya High inactive and Display Low active
        LATB = (LATB & 0x80FF) | (unsigned int)(digit_high) << 8;       // Clean display and set the right value
    }      
    // toggle "displayFlag" variable
    displayFlag = !displayFlag;

}

int soma = 0;
/**
// Interruption Handler
void _int_(27) isr_adc(void)        // Replace VECTOR by the A/D vector
                                    // number - see "PIC32 family data
                                    // sheet" (pages 74-76)
{

    // Calculate buffer average (8 samples)
    int *p = (int *) (&ADC1BUF0);
    int j;
    for (j = 0; j < 8; j++){
        soma += p[j*4];
    }

    int average = soma / 8;
    
    // Calculate voltage amplitude
    int volt= (average*33)/1023;
    // Convert voltage amplitude to decimal. Assign it to "voltage"
    int q = volt / 10;
    int r = volt % 10;
    //printf("%d \t %d\n",q,r);

    voltage = (q << 4 | r);
   
    // Reset AD1IF flag
    IFS1bits.AD1IF = 0;
}*/

void _int_(27) isr_adc(void)
{
    int sum = 0;
    int *p = (int *) (&ADC1BUF0);
    for(; p<= (int *)(&ADC1BUFF); p+=4) {
        sum += *p;
    }
    
    double average = (double) sum/8.0;          // Calculate buffer average (8 samples)
    voltage = (char) ((average*33)/1023);       // Calculate voltage amplitude
    // Convert voltage amplitude to decimal. Assign it to "voltage"
    IFS1bits.AD1IF = 0;                         // Reset AD1IF flag
} 


int main(void)
{
    unsigned int cnt = 0;
    // Configure all (digital I/O, analog input, A/D module, interrupts)

    //A/D Configuration
    TRISBbits.TRISB4 = 1;           //RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;          //RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;           //internal counter ends sampling and starts conversion
    AD1CON1bits.CLRASAM = 1;        //Stop conversion when first interruption is generated
    AD1CON3bits.SAMC = 16;          //Sample time is 16 TAD
    AD1CON2bits.SMPI = 8-1;         //1 conversion
    AD1CHSbits.CH0SA = 4;           //channel AN4
    AD1CON1bits.ON = 1;             //A/D converter activated
    
    //Interrupt System configuration
    IPC6bits.AD1IP = 2;             //Interruption priority is 2
    IEC1bits.AD1IE = 1;             //A/D interruptions are enabled
    IFS1bits.AD1IF = 0;             //reset AD1IF flag
    EnableInterrupts();             //Global Interrupt Enable

    AD1CON1bits.ASAM = 1;           //A/D conversion begins

    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0xFF9F;

    LATB = LATB & 0x80FF;
    LATD = LATD & 0xFF9F;

    // Global Interrupt Enable
    while(1)
    {
        if(cnt % 25 == 0)
        // 250 ms (4 samples/second)
        {
            // Start A/D conversion
            AD1CON1bits.ASAM = 1;           //A/D conversion begins
        }
        // Send "voltage" variable to displays
        send2displays(toBcd(voltage & 0xFF));

        cnt++;
        // Wait 10 ms
        delay(10);
    }
    return 0;
}



