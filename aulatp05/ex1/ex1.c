#include<detpic32.h>

int main(void)
{
    // configure o porto RB4 como entrada analógica; configure o módulo A/D de acordo com as
    // indicações dadas anteriormente e de modo a que o número de conversões consecutivas
    // seja 1;
    TRISBbits.TRISB4 = 1;       // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;      // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;       // Conversion trigger constant
    
    AD1CON1bits.CLRASAM = 1;    // Stop conversion when the 1st A/D converter intetupr is generated.
                                // At the same time, hardware clears ASAM bit
    AD1CON3bits.SAMC = 16;      // Sample time is 16 TAD (TAD = 100ns)
    AD1CON2bits.SMPI = 0;       // Interrupt is generated after 1 sample -> (1-1)

    AD1CHSbits.CH0SA = 4;       // analog channel input 4
    AD1CON1bits.ON = 1;         // Enable the A/D configuration sequence

    // em ciclo infinito:
    while (1)
    {
        // i) dê ordem de conversão;
        AD1CON1bits.ASAM = 1;               // Start conversion

        // ii) espere pelo fim de conversão;
        while ( IFS1bits.AD1IF == 0 );      // Wait while conversion not done

        // iii) utilizandoo system call printInt(), imprima o resultado da conversão (disponível em ADC1BUF0)
        // em hexadecimal, formatado com 3 dígitos ( printInt(ADC1BUF0, 16 | 3 << 16) ).
        printInt(ADC1BUF0, 16 | 3 << 16);   // Read buffer and print it

        IFS1bits.AD1IF = 0;                 // Reset AD1IF
    }
    
    return 0;
}


