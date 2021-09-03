#include<detpic32.h>

/** 1. Calcule as constantes relevantes e configure o timer T3, de modo a gerar eventos com uma
frequência de 2 Hz. Em ciclo infinito, faça polling do bit de fim de contagem T3IF
(IFS0<T3IF>) e envie para o ecrâ o caracter '.' sempre que esse bit fique ativo: */

/** 2. Substitua o atendimento por polling por atendimento por interrupção, configurando o timer T3
para gerar interrupções à frequência de 2 Hz.*/

void _int_(12) isr_T3(void)     // Replace VECTOR by the timer T3 vector number
{
    putChar('.');
    // Reset T3 interrupt flag
    IFS0bits.T3IF = 0;

}

int main(void)
{
    // Configure Timer T3 (2 Hz with interrupts disabled)
    T3CONbits.TCKPS = 7;    //Kprescaler = 256, FoutPrescaler = 78125
    PR3 = 78125;            //Fout = 2Hz (PBLCK / (256*(39062+1))
    TMR3 = 0;               //Reset timer T3 count register
    T3CONbits.TON = 1;      //Enable timer T3
    EnableInterrupts();

    while(1)
    {
        while(IFS0bits.T3IF == 0);  //wait while T3IF == 0
        IFS0bits.T3IF = 0;          //reset T3IF
        putChar('.');
    }
    return 0;
    
}
