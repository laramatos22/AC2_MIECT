#include<detpic32.h>

int voltagemConversao(int VAL_AD)
{
    return (VAL_AD * 33 + 511) / 1023;
}

int main(void)
{
    // configure o porto RB4 como entrada analógica; configure o módulo A/D de acordo com as
    // indicações dadas anteriormente e de modo a que o número de conversões consecutivas
    // seja 1;
    // Potenciometro -> RB4
    TRISBbits.TRISB4 = 1;       // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;      // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;       // Conversion trigger constant

    AD1CON1bits.CLRASAM = 1;    // Stop conversion when the 1st A/D converter intetupr is generated.
                                // At the same time, hardware clears ASAM bit

    // 2. imprimir as 16 posições do buffer do módulo A/D
    // (valores impressos em decimal, formatados com 4 dígitos, separados por 1 espaço).
    // Relembre que os endereços dos 16 registos do buffer de armazenamento dos resultados
    // são múltiplos de 16 (0xBF809070, 0xBF809080, ..., 0xBF809160)
    AD1CON3bits.SAMC = 16;      // Sample time is 16 TAD (TAD = 100ns)

    // 3. Altere a configuração do módulo A/D de modo a que o conversor efetue 4 conversões
    // consecutivas. Observe o resultado.
    AD1CON2bits.SMPI = 3;       // Interrupt is generated after 4 samples -> (4-1)

    AD1CHSbits.CH0SA = 4;       // analog channel input 4
    AD1CON1bits.ON = 1;         // Enable the A/D configuration sequence

    // em ciclo infinito:
    while (1)
    {
        // i) dê ordem de conversão;
        AD1CON1bits.ASAM = 1;               // Start conversion

        // ii) espere pelo fim de conversão;
        while ( IFS1bits.AD1IF == 0 );      // Wait while conversion not done

        int *p = (int *)(&ADC1BUF0);
        int i;
        int voltagem;

        for( i = 0; i < 16; i++ )
        {
            voltagem = voltagemConversao(p[i*4]);
            printInt(voltagem , 10 | 4 << 16);
            putChar(' ');
        }

        IFS1bits.AD1IF = 0;                 // Reset AD1IF
    }
    
    return 0;
}

// ------------- OU -------------
// int *p = (int *)(&ADC1BUF0);
// for(; p <= (int *)(&ADC1BUFF); p+=4 )
// {
//    printInt( *p, ... );
//    ...
// }


