#include <detpic32.h>

//-----------
// 1. Escreva, as macros de ativação e desativação das interrupções de receção e de
// transmissão da UART2:
#define DisableUart2RxInterrupt() IEC1bits.U2RXIE = 0
#define EnableUart2RxInterrupt() IEC1bits.U2RXIE = 1
#define DisableUart2TxInterrupt() IEC1bits.U2RXIE = 0
#define EnableUart2TxInterrupt() IEC1bits.U2RXIE = 1

//--------------
// 2. Declare a estrutura que implementa um buffer circular e crie duas instâncias dessa
// estrutura: rxb e txb (veja a introdução para mais detalhes). Defina a constante BUF_SIZE
// com o valor 8.
#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE - 1)

// estrutura que implementa um buffer circular
typedef struct
{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
} circularBuffer;

// crie duas instâncias dessa estrutura: rxb e txb
volatile circularBuffer txb;    // Transmission buffer
volatile circularBuffer rxb;    // Reception buffer

//---------------
// 3. Escreva as funções comDrv_flushRx() e comDrv_flushTx() que inicializam os buffers
// circulares de transmissão e de receção:

void comDrv_flushRx(void)
{
    // Initialize variables of the reception buffer-> volatile circularBuffer rxb;
    rxb.count=0;
    rxb.head=0;
    rxb.tail=0;
}

void comDrv_flushTx(void)
{
    // Initialize variables of the transmission buffer -> volatile circularBuffer txb;
    txb.count=0;
    txb.head=0;
    txb.tail=0;

}

//----------------
// 4.Escreva a função comDrv_putc() que escreve um carater no buffer de transmissão e
// atualiza as variáveis tail e count. Esta função deverá ainda esperar que haja espaço
// livre no buffer antes de copiar um novo carater:

// write a char in the transmission buffer
void comDrv_putc(char ch)
{
    while(txb.count == BUF_SIZE){}  // Wait while buffer is full

    txb.data[txb.tail] = ch;        // Copy character to the transmission
                                    // buffer at position "tail"
                                    
    txb.tail = (txb.tail + 1) & INDEX_MASK; // Increment "tail" index
                                            // (mod. BUF_SIZE)

    DisableUart2TxInterrupt();      // Begin of critical section

    txb.count++;                    // Increment "count" variable

    EnableUart2TxInterrupt();       // End of critical section
}

//--------------------
// 5. Escreva a função comDrv_puts() que evoca a função escrita no exercício anterior para
// enviar para a linha série uma string (terminada com o carater '\0'):

// write a string in the transmission buffer
void comDrv_puts(char *s)
{
    // (...)
    while (*s != '\0')      // enquanto o caracter for diferente de '\0'
    {
        comDrv_putc(*s);    // write a char in the transmission buffer
        s++;
    }
    
}

//----------------------
// 6. Escreva a rotina de serviço à interrupção de transmissão da UART2.

// UART2 RSI
/*
void _int_(32) ist_uart2(void) 
{
    if (IFS1bits.U2TXIF == 1)   // if U2TXIF is set
    {

        if (txb.count > 0) // if "count" variable (transmission buffer, txb) is greater than 0
        {
            U2TXREG =  txb.data[txb.head];          // Copy character pointed by "head" to U2TXREG register
            txb.head = (txb.head+1) & INDEX_MASK;   // Increment "head" variable (mod BUF_SIZE)
            txb.count--;                            // Decrement "count" variable
        }

        if (txb.count == 0) // if "count" variable is 0 then
            DisableUart2TxInterrupt();

        IFS1bits.U2TXIF = 0;       // Reset UART2 TX interrupt flag
    }
}*/

//----------------------
// 7. Escreva a função main() para testar as funções que escreveu nos pontos anteriores. Para
// a função comDrv_config() pode reaproveitar, com as devidas adaptações, o código
// escrito no guião prático anterior para a função configUart().

void comDrv_config (unsigned int baud, char parity, unsigned int stopbits)
{
    if (baud < 600 || baud > 115200)
        baud = 115200;
    if(parity!='N' || parity!='E' || parity!='O')
        parity = 'N';
    if(stopbits!=1 || stopbits!=2)
        stopbits = 1;
    
    U2BRG = ((PBCLK+8*baud)/(16*baud))-1;       //configure baudrate generator
    U2MODEbits.BRGH = 0;                        //16x baud clk enabled

    //config parity,  nºbits = 8
    switch(parity){
        case 'N':
            U2MODEbits.PDSEL = 0;               //8 bits, no parity
            break;
        case 'E':
            U2MODEbits.PDSEL = 1;               //8 bits, even parity
            break;
        case 'O':
            U2MODEbits.PDSEL = 2;               //8 bits, odd parity
            break;
    }

    //config stopbits
    switch(stopbits){
        case 2:
            U2MODEbits.STSEL = 1;               //2 stop bits
            break;
        case 1:
            U2MODEbits.STSEL = 0;               //1 stop bit
            break;
    }

    U2STAbits.URXEN = 1;                        //UART1 receiver is enabled
    U2STAbits.UTXEN = 1;                        //UART1 transmitter is enabled

    IPC8bits.U2IP = 2;                          //interrupts priority is 2
    IEC1bits.U2RXIE = 1;                        //Rx interrupts enabled
    IEC1bits.U2TXIE = 1;                        //Tx interrupts enabled
    IFS1bits.U2RXIF = 0;                        //reset Rx interrupt flag
    IFS1bits.U2TXIF = 0;                        //reset Tx interrupt flag

    U2MODEbits.ON = 1;                          //UART1 enabled
}

/*
int main (void)
{
    comDrv_config(115200, 'N', 1);
    comDrv_flushRx();
    comDrv_flushTx();
    EnableInterrupts();

    while (1)
    {
        comDrv_puts("Teste do bloco de transmissao do device driver!...\n");
    }
    
}*/

//------------------------
// 8. Escreva a função comDrv_getc() que lê um carater do buffer de receção. A função
// devolve o booleano FALSE se o número de carateres no buffer for zero e TRUE no caso
// contrário. O carater lido do buffer de receção deve ser passado através do ponteiro pchar:

// read a char from the reception buffer
char comDrv_getc(char *pchar)
{
    // Test "count" variable (reception buffer) and return FALSE
    if(rxb.count == 0) return 0;            // if it is zero
    DisableUart2RxInterrupt();              // Begin of critical section
    *pchar = rxb.data[rxb.head];            // Copy character pointed by "head" to *pchar
    rxb.count--;                            // Decrement "count" variable
    rxb.head = (rxb.head + 1) & INDEX_MASK; // Increment "head" variable (mod BUF_SIZE)
    EnableUart2RxInterrupt();               // End of critical section
    return 1;
}

//------------------------
// 9. Escreva a rotina de serviço à interrupção de receção da UART2.

// UART2 RSI
void _int_(32) isr_uart2(void) 
{
    if (IFS1bits.U2TXIF == 1)   // if U2TXIF is set
    {

        if (txb.count > 0) // if "count" variable (transmission buffer, txb) is greater than 0
        {
            U2TXREG =  txb.data[txb.head];          // Copy character pointed by "head" to U2TXREG register
            txb.head = (txb.head+1) & INDEX_MASK;   // Increment "head" variable (mod BUF_SIZE)
            txb.count--;                            // Decrement "count" variable
        }

        if (txb.count == 0)                         // if "count" variable is 0 then
            DisableUart2TxInterrupt();

        IFS1bits.U2TXIF = 0;                        // Reset UART2 TX interrupt flag
    }

    if(IFS1bits.U2TXIF == 1)                        // If U1RXIF is set
    {
        rxb.data[rxb.tail] = U2RXREG;                   // Read character from UART and
        rxb.tail = (rxb.tail + 1) & INDEX_MASK;         // write it to the "tail" position
                                                        // of the reception buffer
                                                        // Increment "tail" variable (mod BUF_SIZE)
        if(rxb.count < BUF_SIZE)                    // If reception buffer is not full (e.g. count < BUF_SIZE) then
            rxb.count++;                            // increment "count" variable
        else                                        // Else
            rxb.head = (rxb.head + 1) & INDEX_MASK; // increment "head" variable (discard oldest character)
        IFS1bits.U2RXIF = 0;                        // reset UART2 RX interrupt flag
    }
    
}

//-------------------------------
// 10. Re-escreva a função main() que escreveu no exercício 7 de modo a fazer o eco dos
// carateres recebidos.

int main(void)
{
    comDrv_config(115200,'N',1);    // default "pterm" parameters
                                    // with RX interrupts enabled and TX
                                    // interrupts disabled
    // (...)
    char c;
    comDrv_flushRx();
    comDrv_flushTx();
    EnableInterrupts();

    comDrv_puts("PIC32 UART Device-driver\n");

    while(1)
    {
        // Read character from reception buffer
        if(comDrv_getc(&c))
            // Send character to the transmission buffer
            comDrv_puts(c);
    }

    return 0;

}



