#include<detpic32.h>

/**
 * 3. Escreva e teste uma função para enviar para a porta série uma string (array de carateres
terminado com o carater 0). Utilize a função putc() para enviar cada um dos carateres da
string.
*/

void delay(int ms)
{
    for(;ms>0;ms--)
    {
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

void putc(char byte2send)
{
    // wait while UTXBF == 1
    while (U2STAbits.UTXBF == 1);    
    // Copy byte2send to the UxTXREG register
    U2TXREG = byte2send;
}

void puts(char *str)
{
    // use putc() function to send each charater ('\0' should not
    // be sent)
    while (*str != '\0')
    {
        putc(*str);
        str++;
    }
    
}

void configUart(unsigned int baud, char parity, unsigned int stopbits)
{
    if (baud < 600 || baud > 115200)
    {
        baud = 115200;
    }

    if (parity != 'N' || parity != 'E' || parity != 'O')
    {
        parity = 'N';
    }

    if (stopbits != 1 || stopbits != 2)
    {
        stopbits = 1;
    }
    
    U2BRG = ((PBCLK + 8*baud) / (16*baud)) - 1;     // Configure BaudRate Generator
    U2MODEbits.BRGH = 0;                            // 16x baud clk enabled (fator de divisao do relogio de 16)
    
    // Configure number of data bits (8), parity and number of stop bits
    switch (parity)
    {
    case 'N':
        U2MODEbits.PDSEL = 0;       // 8 bits, no parity
        break;
    
    case 'E':
        U2MODEbits.PDSEL = 1;       // 8 bits, even parity
        break;

    case 'O':
        U2MODEbits.PDSEL = 2;       // 8 bits, odd parity
        break;

    }

    switch (stopbits)
    {
    case 1:
        U2MODEbits.STSEL = 0;       // 1 stop bit
        break;
    
    case 2:
        U2MODEbits.STSEL = 1;       // 2 stop bit
        break;
 
    }

    U2STAbits.URXEN = 1;            // Enable the trasmitter and receiver modules
    U2STAbits.UTXEN = 1; 
    U2MODEbits.ON = 1;              // Enable UART2
}

int main(void)
{
    /**Configure UART2:
    U2BRG = ((PBCLK + 8*115200)/(16*115200))-1; // 1 - Configure BaudRate Generator
    U2MODEbits.BRGH = 0;                        // 2 – Configure number of data bits, parity and number of stop bits
    U2MODEbits.PDSEL = 0;                       //     (see U1MODE register)
    U2MODEbits.STSEL = 1;                       
    U2STAbits.UTXEN = 1;                        // 3 – Enable the trasmitter and receiver modules (see register U1STA)            
    U2STAbits.URXEN = 1;
    U2MODEbits.ON = 1;*/                       // 4 – Enable UART2 (see register U1MODE)

    //configUart(600, 'N', 1);
    //configUart(1200,'O',2);
    //configUart(9600,'E',1);
    //configUart(19200,'N',2);
    configUart(115200,'O',1);

// EXERCICIO 3
    while (1)
    {
        puts("String de teste\n");
        delay(1000);
    }

    return 0;    

}
