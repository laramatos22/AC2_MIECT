#include<detpic32.h>

//funcao delay()
void delay(int ms)
{
    for(;ms>0;ms--)
    {
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

// Interruption Handler

volatile int adc_value;

void _int_(27) isr_adc(void)        // Replace VECTOR by the A/D vector
                                    // number - see "PIC32 family data
                                    // sheet" (pages 74-76)
{
    adc_value = ADC1BUF0;

    AD1CON1bits.ASAM = 1;           //Start the conversion
    IFS1bits.AD1IF = 0;             // Reset AD1IF flag

    LATEbits.LATE0 = 1;
}

int main(void)
{

    TRISEbits.TRISE0 = 0;

    //A/D Configuration
    TRISBbits.TRISB4 = 1;           //RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;          //RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;           //internal counter ends sampling and starts conversion
    AD1CON1bits.CLRASAM = 1;        //Stop conversion when first interruption is generated
    AD1CON3bits.SAMC = 16;          //Sample time is 16 TAD
    AD1CON2bits.SMPI = 1-1;         //1 conversion
    AD1CHSbits.CH0SA = 4;           //channel AN4
    AD1CON1bits.ON = 1;             //A/D converter activated
    
    //Interrupt System configuration
    IPC6bits.AD1IP = 2;             //Interruption priority is 2
    IEC1bits.AD1IE = 1;             //A/D interruptions are enabled
    IFS1bits.AD1IF = 0;             //reset AD1IF flag
    EnableInterrupts();             //Global Interrupt Enable

    AD1CON1bits.ASAM = 1;           //A/D conversion begins

    while(1) {
        LATEbits.LATE0 = 0;
    };
    return 0;
}