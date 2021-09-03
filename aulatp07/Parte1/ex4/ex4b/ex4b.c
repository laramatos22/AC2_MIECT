#include<detpic32.h>

/** 1. Calcule as constantes relevantes e configure o timer T3, de modo a gerar eventos com uma
frequência de 2 Hz. Em ciclo infinito, faça polling do bit de fim de contagem T3IF
(IFS0<T3IF>) e envie para o ecrâ o caracter '.' sempre que esse bit fique ativo: */

/** 2. Substitua o atendimento por polling por atendimento por interrupção, configurando o timer T3
para gerar interrupções à frequência de 2 Hz.*/

void _int_(12) isr_T3(void)     // Replace VECTOR by the timer T3 vector number
{
    putChar('3');
    // Reset T3 interrupt flag
    IFS0bits.T3IF = 0;
}


void _int_(4) isr_T1(void)     // Replace VECTOR by the timer T3 vector number
{
    putChar('1');
    // Reset T3 interrupt flag
    IFS0bits.T1IF = 0;
}

int main(void)
{
    // Configure Timer T3 (2 Hz with interrupts disabled)
    T1CONbits.TCKPS = 8;    //Kprescaler = 256, FoutPrescaler = 78125
    PR1 = 39062;            //Fout = 2Hz (PBLCK / (256*(39062+1))
    TMR1 = 0;               //Reset timer T3 count register
    T1CONbits.TON = 1;      //Enable timer T3

    // Configure Timer T1 (2 Hz with interrupts disabled)
    T3CONbits.TCKPS = 7;    //Kprescaler = 256, FoutPrescaler = 78125
    PR3 = 62499;            //Fout = 2Hz (PBLCK / (256*(39062+1))
    TMR3 = 0;               //Reset timer T3 count register
    T3CONbits.TON = 1;      //Enable timer T3    

    IFS0bits.T3IF =0;     //wait while T3IF == 0
    IEC0bits.T3IE = 1;          //reset T3IF

    IFS0bits.T1IF =0;     //wait while T3IF == 0
    IEC0bits.T1IE = 1;          //reset T3IF

    IPC3bits.T3IP = 2;  //PRIORIDADE DO TIMER

    IPC1bits.T1IP = 2;  //PRIORIDADE DO TIMER

    EnableInterrupts();


    while(1);

    return 0;
}
