#include<detpic32.h>

/** Calcule as constantes relevantes e configure o timer T3, de modo a gerar eventos com uma
frequência de 2 Hz. Em ciclo infinito, faça polling do bit de fim de contagem T3IF
(IFS0<T3IF>) e envie para o ecrâ o caracter '.' sempre que esse bit fique ativo: */

int main(void)
{
    // Configure Timer T3 (2 Hz with interrupts disabled)
    T3CONbits.TCKPS = 7;    //Kprescaler = 256, FoutPrescaler = 78125 -> CHECK
    PR3 = 39062;            //Fout = 2Hz (PBLCK / (256*(39062+1)) -> 20M/Kpresc = 20M/256
    TMR3 = 0;               //Reset timer T3 count register
    T3CONbits.TON = 1;      //Enable timer T3

    while(1)
    {
        while(IFS0bits.T3IF == 0);  //wait while T3IF == 0
        IFS0bits.T3IF = 0;          //reset T3IF
        putChar('.');
    }
    return 0;
    
}
