#include<detpic32.h>

void delay(int ms){
    for(; ms > 0; ms--){
       resetCoreTimer();
       while(readCoreTimer() < 20000);
    }
}

//----------------------
// Procedimento de transmissao - POOLING
void putc(char byte2send){
    // wait while UTXBF == 1
    while(U2STAbits.UTXBF == 1);
    // Copy byte2send to the UxTXREG register
    U2TXREG = byte2send;
}

void puts(char *str){
    // use putc() function to send each charater ('\0' should not be sent)
    int i=0;
    while(str[i] != '\0'){
        putc(str[i]);
        i++;
    } 
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
    //config rb6 as output
    TRISBbits.TRISB6 = 0;
    while(1){
        while (U2STAbits.TRMT == 0);
        LATBbits.LATB6 = 1;
        //wait until trmt==1
        //set rb6
        puts("12345");
        //reset rb6
        LATBbits.LATB6 = 0;
    }
    return 0;
}
