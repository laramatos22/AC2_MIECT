// Escreva um programa em linguagem C que execute em ciclo infinito, e que, em funcao da tecla premida no PC, tenha 
// o seguinte comportamento:
// -> Se a tecla premida estiver na gama 0 a 3, o valor da tecla deve aparecer nos displays de 7 segmentos (por exemplo, 02);
// -> Se for premida outra qualquer tecla, deve aparecer o valor FF durante 1 segundo e a seguir os displays devem apagar-se 
//    (i.e., todos os segmentos devem ser desligados);
// A frequencia de atualizacao dos displays devem ser 100Hz (utilize o core timer do PIC32 para gerar as temporizacoes).

#include<detpic32.h>

void delay(int ms)
{
    for(;ms>0;ms--)
    {
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

void send2displays(int valor0, int valor1)
{
    static char displayFlag = 0;

    if (displayFlag == 0)
    {
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | valor0;
    }

    else 
    {
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | valor1;
    }
    displayFlag = !displayFlag;
}

int main(void)
{

    char firstChar = 'F', lastChar = 'F';

    int valueToSend0 = 0x7100;
    int valueToSend1 = 0x7100;
    
    // Configurar PORTS
    TRISB = (TRISB & 0x80FF);           // Configure RB14 to RB8 as outputs
    TRISD = (TRISD & 0xFF9F);           // Configure RD6 and RD5 as outputs

    printStr("\nCarregar numa tecla: ");

    while (1)
    {
        firstChar = getChar();

        if (firstChar == '0')
        {
            valueToSend0 = 0x3F00;      // 0 from Display value 00
            valueToSend1 = 0x3F00;      // 0 from Display value 00
            lastChar = '0';
        }

        else if (firstChar == '1')
        {
            valueToSend0 = 0x0600;      // 1 from Display value 01
            valueToSend1 = 0x3F00;      // 0 from Display value 01
            lastChar = '1';
        }
        
        else if (firstChar == '2')
        {
            valueToSend0 = 0x5B00;      // 2 from Display value 02
            valueToSend1 = 0x3F00;      // 0 from Display value 02
            lastChar = '2';
        }

        else if (firstChar == '3')
        {
            valueToSend0 = 0x4F00;      // 3 from Display value 03
            valueToSend1 = 0x3F00;      // 0 from Display value 03
            lastChar = '3';
        }

        else if (firstChar == '4')
        {
            valueToSend0 = 0x6600;      // 2 from Display value 04
            valueToSend1 = 0x3F00;      // 0 from Display value 04
            lastChar = '4';
        }

        else if (firstChar != '\0')
        {
            valueToSend0 = 0x7100;      // 2 from Display value FF
            valueToSend1 = 0x7100;      // 0 from Display value FF
            lastChar = firstChar;
        }
        
        else
        {
            firstChar = lastChar;
        }
        
        send2displays(valueToSend0, valueToSend1);
        delay(10);                      // 10 ms -> f = 100 Hz

    }

    return 0;

}


