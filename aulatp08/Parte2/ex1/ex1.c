#include<detpic32.h>

void putc(char byte2send){
    // wait while UTXBF == 1
    while(U2STAbits.UTXBF == 1);
    // Copy byte2send to the UxTXREG register
    U2TXREG = byte2send;
}

void _int_(32) isr_uart2(void){
    putc(U2RXREG);
    // Clear UART1 rx interrupt flag
    IFS1bits.U2RXIF = 0; 
}

void configUart(unsigned int baud, char parity, unsigned int stopbits){
    if(baud > 115200 || baud < 600){
        baud = 115200;
    }
    if(parity != 'N' && parity != 'E' && parity != 'O'){
        parity = 'N';
    }
    if(stopbits != 1 && stopbits != 2){
        stopbits = 1;
    }
    
    // Configure BaudRate Generator
    U2BRG = (20000000 + 8 * baud) / (16 * baud) - 1;
    // Configure number of data bits (8), parity and number of stop bits
    // Enable the trasmitter and receiver modules
    // Enable UART1

    U2MODEbits.BRGH = 0;
    if(parity == 'N'){
        U2MODEbits.PDSEL = 0;
    }
    else if(parity == 'E'){
        U2MODEbits.PDSEL = 1;
    }
    else if(parity == 'O'){
        U2MODEbits.PDSEL = 2;
    }

    U2MODEbits.STSEL = stopbits - 1;
    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;
    U2MODEbits.ON = 1;
} 

int main(void){
    configUart(115200,'N',1);
    EnableInterrupts();
    while(1);
    return 0;
}
